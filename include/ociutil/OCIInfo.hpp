// OCIInfo.hpp  - OCI parameter descriptor
// $Id: OCIInfo.hpp,v 1.2 1995/09/01 18:54:35 ed Exp $

#ifndef _OCIVARIABLEINFO_HPP 
#define _OCIVARIABLEINFO_HPP  1

struct OCIVariableInfo {
   int  startLocation;
   int  length;
   char type;
};

// Name: OCIInfo
// Documentation: OCI parameter descriptor
// used in a call to OCICursor() to define the data sizes being marshalled
#endif
