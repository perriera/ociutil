// OCISeqNumber.hpp  - Oracle unique number generation
// $Id: OCISeqNumber.hpp,v 1.4 2000/01/29 16:47:33 ed Exp $

#ifndef _OCISEQNUMBER_H
#define _OCISEQNUMBER_H 1
#ifndef _YU_YUNUM_H_INCLUDED
#include <YUNum.hpp>
#define _YU_YUNUM_H_INCLUDED
#endif
#include <string>

class OCISeqNumber {
public:
    static YUNumber nextval(const int aOciHandleId, const std::string& aSeq);
    static YUNumber currval(const int aOciHandleId,
	                         const std::string& aSeq); // who uses this?
private:
    OCISeqNumber();                                // disable default ctor
    OCISeqNumber(const OCISeqNumber&);	            // disable copy ctor
    OCISeqNumber& operator=(const OCISeqNumber&);  // disable op=
    inline virtual ~OCISeqNumber() {}
    inline OCISeqNumber(const std::string& aSeq) : mySeq(aSeq) {}
    std::string mySeq;
};

// Name: OCISeqNumber
// Documentation: Oracle unique number generation
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
