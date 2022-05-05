// OCILogin.h - Oracle OCI database access handle
// $Id: OCILogin.hpp,v 1.12 2000/06/20 20:14:34 ed Exp $

#ifndef OCILOGIN_H
#define OCILOGIN_H

#include <oci.h>
#include <ociutil/OCISISError.hpp>
#include <string>

class OCILogin {
public:
	// password is stored for possible reopen() 
	OCILogin(const std::string& userName, const std::string& password);
	OCISISError open();
	OCISISError Rollback();
	OCISISError Commit();
	inline OCIHANDLE* ociHandle() { return &myOciHandle; }
	inline operator OCIHANDLE* () { return &myOciHandle; }
	~OCILogin();                          // non-virtual, do not derive
	void        invalidate();
	inline int  isValid() const { return myIsValid; }
	void        initialize();

private:
	OCILogin(const OCILogin&);             // disable copy ctor
	OCILogin& operator=(const OCILogin&);  // disable op=

	std::string myUserName;
	std::string myPassword;
	int         myIsValid;
	OCIHANDLE   myOciHandle;
};

// Name: OCILogin
// Documentation: Oracle OCI database access handle
// Visibility: exported 
// Cardinality: n
// Hierarchy:
//  Superclasses: none
// Public Interface:
//  Uses: list of class names
//  Operations: list of operation declarations
// Concurrency: sequential (single-threaded OCI interface)
#endif

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 3
// tab-stop-list: (3 6 9 12 15 18 21 24 27 30 33 36 39 42 45 48 51 54 57 60)
// fill-column: 70
// End:
