// OracleServer.cpp - anchor for Oracle cursors
// $Id: OracleServer.cpp,v 1.6 1999/01/14 22:43:04 ed Exp $

#include <ociutil/OracleServer.hpp>
#include <ociutil/OCIACLMap.hpp>
#include <sisutil/ACL.hpp>
#include <sisutil/YUApp.hpp>				  // for getTraceLevel
#include <sisutil/ErrLog.hpp>
#include <cstdlib>              // for getenv

OracleServer* OracleServer::theOracleServer = NULL;
OCIACLMap* OracleServer::theOCIACLMap = NULL;
// "setenv OCI_DEBUG" to turn on debug output
const char* OracleServer::Verbose_VAR = "OCI_VERBOSE";
const char* OracleServer::Timing_VAR = "OCI_TIME";
const char* OracleServer::Debug_VAR = "OCI_DEBUG";

OracleServer::OracleServer() {
	theOCIACLMap = new OCIACLMap();
	ACL::setRepository(*theOCIACLMap);
}

OracleServer::~OracleServer() {
	delete theOCIACLMap;
}

// user must create a derived class that passes itself to registerServer()
void OracleServer::registerServer(OracleServer& s) {
	theOracleServer = &s;
}

// singleton
OracleServer* OracleServer::theServer() {
	if (!theOracleServer)
		getTheErrLog().putLine("OracleServer", "theServer", "No Server");
	return theOracleServer;
}

// singleton
OCIACLMap* OracleServer::theACLMap() {
	if (!theOCIACLMap)
		getTheErrLog().putLine("OracleServer", "theACLMap", "No ACLMap");
	return theOCIACLMap;
}

int OracleServer::isTraceVerbose() {
	return (YUApp::getTraceLevel() > YUApp::MAXLEVEL - 7) || getenv(Verbose_VAR);
}

int OracleServer::isTraceTiming() {
	return (YUApp::getTraceLevel() > YUApp::MAXLEVEL - 5) || getenv(Timing_VAR);
}

int OracleServer::isTraceDebug() {
	return (YUApp::getTraceLevel() > YUApp::MAXLEVEL - 3) || getenv(Debug_VAR);
}
