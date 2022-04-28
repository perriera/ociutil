// OCISISServer.hpp  -*- C++ -*- anchor for Oracle cursors
// $Id: OCISISServer.hpp,v 1.30 2000/06/22 14:44:38 ed Exp $

#ifndef _OCISISServer_H
#define _OCISISServer_H 1

#include <OracleServer.hpp>
#include <map>
#include <ocihandle.h>

class OCILogin;
class YUError;

class OCISISServer : public OracleServer {
public:
	OCISISServer();
	virtual ~OCISISServer();
	virtual int addRM(const std::string& serverName,   // for error messages
							const std::string& userName,
							std::string& password,
							long serverId = 0);

	// return the LDA registered under aOciHandleId, if any, else the default
	// LDA (registered under 0), else 0
	//virtual Lda_Def* getDataBaseLDA(long aLdaId) const;
	virtual OCIHANDLE* getOciHandle(long aOciHandleId) const;
	virtual int        reopen(long aOciHandleId) const;
	virtual int        reopenAll() const;
	void               Commit(long aOciHandleId = 0) const;
	void               Rollback(long aOciHandleId = 0) const;
	YUError            commit(long aOciHandleId = 0) const;
	YUError            rollback(long aOciHandleId = 0) const;

#if ( defined (__xlC__) && (__xlC__ < 0x400) ) \
	|| ( defined (__SUNPRO_CC) && (__SUNPRO_CC <= 0x420 ) )
	typedef map<int, OCILogin*, less<int> > RMMap;
#else
	typedef std::map<int, OCILogin*> RMMap;
#endif

protected:
  // perform any RM session level initialization
  virtual int         initRM(long serverId = 0) const;

private:
	OCISISServer(const OCISISServer&);	            // disable copy ctor
	OCISISServer&      operator=(const OCISISServer&);   // disable op=
	static RMMap       myRMMap;
	OCILogin*          getDataBaseLogin(long aOciHandleId) const;
	int                alterDateFormat(long aOciHandleId) const;
};

// Name: OCISISServer
// Documentation: anchor for Oracle cursors
// Cardinality: 1 
// Hierarchy:
//  Superclasses: list of class names
// Generic parameters: list of parameters
// Public Interface:
//  Uses: list of class names
//  Operations: list of operation declarations
// Protected Interface:
//  Uses: list of class names
//  Fields: list of field declarations
//  Operations: list of operation declarations
// Private Interface:
//  Uses: list of class names
//  Fields: list of field declarations
//  Operations: list of operation declarations
// Implementation:
//  Uses: list of class names
//  Fields: list of field declarations
//  Operations: list of operation declarations
// Finite State Machine: state transition diagram
// Concurrency: sequential | blocking | active
// Space complexity: text
// Persistance: static | dynamic
#endif

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 3
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:
