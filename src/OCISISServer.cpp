// OCISISServer.cpp - anchor for Oracle cursors
// $Id: OCISISServer.cpp,v 1.38 2000/06/22 14:44:38 ed Exp $

#include <cstring>
#include <sstream>
#include <map>
#include <OCISISServer.hpp>
#include <OCICursor.hpp>
#include <OCILogin.hpp>
#include <ErrLog.hpp>
#include <GMLock.hpp>
#include <YUAppErr.hpp>
#include <YUError.hpp>
#include <stdlib.h>                         // for getenv

static const std::string className = "OCISISServer";
OCISISServer::RMMap OCISISServer::myRMMap;
static const char *alterDateFormatSql = "alter session set nls_date_format = 'YYYYMMDDHH24MISS'";

OCISISServer::OCISISServer() {
   if (opinit(OCI_EV_TSF)) { // thread safe OCI environment
      getTheErrLog().logErrMsg(className, "OCISISServer", "opinit", "fails");
      // XXX todo throw FatalError
   }
   OracleServer::registerServer(*this);
}

/**
 * Receives login credentials for a database connection, logs in and stores the login context.
 * @param serverName used for error messages
 * @param userName account for database login
 * @param password login credential for database
 * @param a user supplied id for a database - typically 0 or 1, defaults to 0 if not supplied
 * @return true if an initialized OCILogin is registered, 0 for failure
*/
int OCISISServer::addRM(const std::string& serverName,   // for error messages
							   const std::string& userName,
							   std::string& password,
							   long serverId) {
   if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCISISServer::addRM(): server: " << serverName << ", serverId=" << serverId << std::endl;
	OCILogin* theOCILogin = new OCILogin(userName, password);
	OCISISError status = theOCILogin->open();
	if (!status.isSuccess()) {
		getTheErrLog().logErrMsg(className, "addRM", "OCILogin::open", status);
		delete theOCILogin;
		return 0;
	}

   // Map from rpc handle (LDA_BASE in OCI library) to appropriate database.
	// The problem with this approach is that it only supports 2 databases. (OJ)
	// How to split around 3, 4, or more sets of rpc handles?
	// This has to be common for ALL SIS apps.
   // Currently using STAC rpc split (500000+ is STAC, 0->499999 is SIS).
   //int myId = serverId; if (myId/100 >= 5000) myId = 1; else myId = 0;

	const RMMap::iterator pos = myRMMap.find(serverId);
   //const RMMap::iterator pos = myRMMap.find(myId);
	if (pos != myRMMap.end()) {// replacing, first delete existing entry
		delete myRMMap[serverId];
      //delete myRMMap[myId];
		myRMMap.erase(pos);
	}
	myRMMap[serverId] = theOCILogin;
   //myRMMap[myId] = theOCILogin;

   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCISISServer::addRM(): theOCILogin=" << theOCILogin << std::endl;
      std::cerr << "OCISISServer::addRM(): server: " << serverName << ", serverId=" << serverId << std::endl;
   }
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCISISServer::addRM(): myId=" << myId << std::endl;
   return initRM(serverId);
}

/**
 * Finds the handle registered under aOciHandleId
 * in which case log the error.
 * @param aOciHandleId that is associated with the rpc oci handle
 * @return the OCIHANDLE, if any, else NULL
 */
OCIHANDLE* OCISISServer::getOciHandle(long aOciHandleId) const {
	OCILogin* theOCILogin = getDataBaseLogin(aOciHandleId);
	if ((theOCILogin == 0) || (theOCILogin->ociHandle() == 0)) {
	   //strstream tmpStream;
		std::stringstream tmpStream;
		tmpStream << "No OCI login OciHandle for OciHandleId = " << aOciHandleId; // << std::ends;
		getTheErrLog().putStream(className, "getOciHandle", tmpStream);
		return NULL;
	}
	return theOCILogin->ociHandle();
}

OCISISServer::~OCISISServer() {
	for (RMMap::iterator i = myRMMap.begin(); i != myRMMap.end(); ++i) {
		OCILogin* thisLogin = (*i).second;
		delete thisLogin;
	}
	myRMMap.erase(myRMMap.begin(), myRMMap.end());
}

int OCISISServer::reopen(long aOciHandleId) const {
	OCILogin* theOCILogin = getDataBaseLogin(aOciHandleId);
	if ((theOCILogin == 0)) {
		//strstream tmpStream;
	   std::stringstream tmpStream;
		tmpStream << "No OCIlogin for OciHandleId = " << aOciHandleId; // << std::ends;
		getTheErrLog().putStream(className, "getOciHandle", tmpStream);
		return false;
	}
	theOCILogin->invalidate();
   {                           // critical section
      GlobalMutexLock serializeThisScope;
      if (!theOCILogin->isValid()) { // no other thread completed the reopen
         OCISISError status = theOCILogin->open();
         if (!status.isSuccess()) {
            getTheErrLog().logErrMsg(className, "reopen", "OCILogin::open", status);
            return false;
         }
      }
   }                           // end critical section
	return initRM(aOciHandleId);
}

int OCISISServer::reopenAll() const {
	int allReopened = 1;
	for (RMMap::iterator i = myRMMap.begin(); i != myRMMap.end(); ++i) {
		int thisKey = (*i).first;
		allReopened &= reopen(thisKey);
	}
	return allReopened;
}

/**
 * Searches through the list of database contexts to find one that can handle the rpc associated with the handle parameter.
 * @param aOciHandleId is an id unique to an rpc that can be mapped to a database instance id
 * @return an OCILogin object (database login context) or NULL if one cannot be found
 */
OCILogin* OCISISServer::getDataBaseLogin(long aOciHandleId) const {
   if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCISISServer::getDataBaseLogin(): --0001-- entering with aOciHandleId=" << aOciHandleId << std::endl;
	OCILogin* theOCILogin = NULL;

    // Map from rpc handle (LDA_BASE in OCI library) to appropriate database.
	// The problem with this approach is that it only supports 2 databases. (OJ)
    // How to split around 3, 4, or more sets of rpc handles?
    // This has to be common for ALL SIS apps.
	// Currently using STAC rpc split (500000+ is STAC, 0->499999 is SIS).
	// 20000** is special id, will connect to sis too
	long myId = aOciHandleId;
	RMMap::iterator posTemp0 = myRMMap.find(0);
	if( posTemp0 != myRMMap.end() ) {
		if (aOciHandleId > 10) {
			// for 1 DB or 2 DBs
			myId = aOciHandleId / 100;

			if( myId == 20000 )
				myId = 0; // SISDB
			else if (myId >= 5000)
				myId = 1; // STACDB
			else
				myId = 0; // SIDDB
		} else {
			// for aOciHandleId <= 10
			// and for 2 DBs
			RMMap::iterator posTemp1 = myRMMap.find(0);
			if( posTemp1 != myRMMap.end() ) {
				myId = 1; // STACDB
			}
		}
	}

	// for sis_assess
	RMMap::iterator posTemp2 = myRMMap.find(2);
	if( posTemp2 != myRMMap.end()) {
		if( myId == 0)
			myId = 3; // STACDB
		else if( myId < 1000000 )
			myId = 2; // SISDB
		else
			myId = 3; // STACDB
	}

   if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCISISServer::getDataBaseLogin(): myId=" << myId << std::endl;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCISISServer::getDataBaseLogin(): aOciHandleId=" << aOciHandleId << std::endl;

   //RMMap::iterator pos = myRMMap.find(aOciHandleId);
   RMMap::iterator pos = myRMMap.find(myId);
   //if (pos == myRMMap.end() && aOciHandleId) // try again with default of aOciHandleId = 0
	if (pos == myRMMap.end() && myId) // try again with default of aOciHandleId = 0
		pos = myRMMap.find(0);
	if (pos != myRMMap.end()) theOCILogin = (*pos).second;
	if (!theOCILogin) {
		//strstream tmpStream;
		std::stringstream tmpStream;
		tmpStream << "No OCILogin for OciHandleId = " << aOciHandleId; // << std::ends;
		getTheErrLog().putStream(className, "getDataBaseLogin", tmpStream);
	}
   if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCISISServer::getDataBaseLogin(): --0002-- leaving with the OCILogin: " << theOCILogin << std::endl;
	return theOCILogin;
}

YUError OCISISServer::commit(long aOciHandleId) const {
  OCILogin* theOCILogin = getDataBaseLogin(aOciHandleId);
  if (theOCILogin)
    return theOCILogin->Commit();
  return YUAppError(YUAppError::YUAPP_INTERNAL_ERROR, "No OciHandle for commit");
}

void OCISISServer::Commit(long aOciHandleId) const {
  //YUError status = commit();
  YUError status = commit(aOciHandleId);
  if (!status.isSuccess())
    getTheErrLog().logErrMsg(className, "Commit", "OCILogin::commit", status);
}

YUError OCISISServer::rollback(long aOciHandleId) const {
  OCILogin* theOCILogin = getDataBaseLogin(aOciHandleId);
  if (theOCILogin) 
    return theOCILogin->Rollback();
  return YUAppError(YUAppError::YUAPP_INTERNAL_ERROR, "No OciHandle for rollback");
}

void OCISISServer::Rollback(long aOciHandleId) const {
  YUError status = rollback(aOciHandleId);
  if (!status.isSuccess())
    getTheErrLog().logErrMsg(className, "Rollback", "OCILogin::rollback", status);
}

int OCISISServer::initRM(long serverId) const {
	return alterDateFormat(serverId);
}

int OCISISServer::alterDateFormat(long aOciHandleId) const {
	const std::string routine = "alterDateFormat";
   //OCICursor c(0, alterDateFormatSql);
   if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCISISServer::alterDateFormat(): aOciHandleId=" << aOciHandleId << std::endl;
   OCICursor c(aOciHandleId, alterDateFormatSql);
	OCISISError status = c.Open();
	if (!status.isSuccess()) {
		getTheErrLog().logErrMsg(className, routine, "OCICursor::Open", status);
		return false;
	}
	status = c.Parse();
	if (!status.isSuccess()) {
		getTheErrLog().logErrMsg(className, routine, "OCICursor::Parse", status);
		return false;
	}
	status = c.Exec();
	if (!status.isSuccess()) {
		getTheErrLog().logErrMsg(className, routine, "OCICursor::Exec", status);
		return false;
	}
	Commit(aOciHandleId);
	return true;
}
