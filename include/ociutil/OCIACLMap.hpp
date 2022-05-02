// OCIACLMap.hpp  -*- C++ -*- OCI based DCE Access control list repository
// $Id: OCIACLMap.hpp,v 1.6 2001/05/10 13:59:23 ojones Exp $

#ifndef _OCIACLMAP_H
#define _OCIACLMAP_H 1

#include <sisutil/ACLRep.hpp>

class ACL;
class OCIVariableInfo;

class OCIACLMap : public ACLRep {
public:
	OCIACLMap() {};
	//enum {READARRAYSIZE = 4, RPCNAMEMAXLEN = 128} OCIACLMapCONSTANTS;
	enum { READARRAYSIZE = 100, RPCNAMEMAXLEN = 128 } OCIACLMapCONSTANTS; // OJ
	ACL* lookup(const std::string& key) const;	// caller must delete return value
	static int      lookupByPrincipal(const std::string& principalName, const std::string& rpcName); // OJ
	inline virtual ~OCIACLMap() {}

private:
	OCIACLMap(const OCIACLMap&); 		          // disable copy c'tor
	OCIACLMap& operator = (const OCIACLMap&);  // disable op=
	static OCIVariableInfo lookupMarshInVector[];
	static OCIVariableInfo lookupMarshOutVector[];
	static const char* SQLlookup;
	static OCIVariableInfo lookupACLREF[];
	static OCIVariableInfo lookupACLResult[];
	static const char* SQLLookupACL;
};

// Name: OCIACLMap
// Documentation: DCE Access control list repository.
// This is a read-only access to the Oracle ACL table
// Visibility: exported 
// Cardinality: 1 
// Hierarchy:
//  Superclasses: ACLRep
// Implementation:
//  Uses: OCICursor
// Concurrency:  blocking 
#endif
