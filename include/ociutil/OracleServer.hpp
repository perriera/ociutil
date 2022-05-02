// OracleServer.hpp  - anchor for Oracle cursors
// $Id: OracleServer.hpp,v 1.13 1998/11/23 15:07:56 ed Exp $

#ifndef _ORACLESERVER_H
#define _ORACLESERVER_H 1

#include <sisutil/RMServer.hpp>

class OCICursor;
class OCIACLMap;

#include <string>

#include <oci.h>
#include <ociutil/ocihandle.hpp>

class OracleServer : public RMServer {
public:
   OracleServer();
   virtual ~OracleServer();
   // return true if a initialized OCICursor is registered, return
   // random password for Oracle userName in password
   virtual int addRM(const std::string& serverName,
      const std::string& userName,
      std::string& password,
      long serverId = 0) = 0;     // for getDataBaseLDA)
//virtual Lda_Def * getDataBaseLDA(long serverId) const = 0;
   virtual OCIHANDLE* getOciHandle(long serverId) const = 0;
   virtual int           reopen(long serverId) const = 0;
   virtual int           reopenAll() const = 0;
   static OracleServer* theServer(); // singleton
   static OCIACLMap* theACLMap();  // singleton
   static int            isTraceVerbose();
   static int            isTraceTiming();
   static int            isTraceDebug();
   static const char* Verbose_VAR;
   static const char* Timing_VAR;
   static const char* Debug_VAR;

protected:
   static void registerServer(OracleServer&);
private:
   OracleServer(const OracleServer&);              // disable copy ctor
   OracleServer& operator =(const OracleServer&);   // disable op=
   // Singleton, the one and only, inititalized by concrete class
   static OracleServer* theOracleServer;
   static OCIACLMap* theOCIACLMap;
};

// Name: OracleServer
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
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:
