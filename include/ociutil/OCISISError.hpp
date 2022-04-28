// OCISISError.hpp - Oracle error messages
// $Id: OCISISError.hpp,v 1.20 2000/11/01 20:24:08 ed Exp $

#ifndef _OCISISError_
#define _OCISISError_ 1

#include <Error.hpp>

#include <oci.h>
#include <ocihandle.h>


class OCISISError : public Error {
public:
    OCISISError(const std::string& who, const std::string& w, const OCIHANDLE* hOCI);
    OCISISError(const std::string& who, const std::string& w, const OCIHANDLE* dummy, const OCIHANDLE* hOCI);
    inline          OCISISError(const OCISISError& rhs) : Error(rhs), myText(rhs.text()) { ErrorStatusValues = OK; };
    inline          OCISISError& operator = (const OCISISError& rhs);
    inline virtual ~OCISISError() {};
    inline          OCISISError() { ErrorStatusValues = OK; };
    virtual const   std::string text() const;
    virtual Error * clone()  const;
    typedef enum {OCISISErrorERROR_BASE = 0} OCISISErrorBase;
    enum            ErrorStatusValuesEnum {
                            OK = OCISISErrorERROR_BASE,
                            NOT_FOUND = OCISISErrorERROR_BASE + 1403,
                            TRUNCATED = OCISISErrorERROR_BASE + 1406, //ORA-01406: fetched column value was truncated
                            NULL_IN_AGGREGATE = OCISISErrorERROR_BASE + 24347 //ORA-24347: Warning of a NULL column in an aggregate function
                    } ErrorStatusValues;

protected:
    std::string     myText;
};

inline OCISISError& OCISISError::operator = (const OCISISError& rhs) {
   if (this != &rhs) {
    Error::operator = (rhs);
    myText = rhs.text();
  }

  return *this;
}

// Name: OCISISError
// Documentation: Oracle OCI error messages
// Cardinality: 1
// Hierarchy:
//  Superclasses: Error
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
