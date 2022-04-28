// OCICursor.hpp - Oracle cursor class
// $Id: OCICursor.hpp,v 1.42 2000/06/13 01:08:28 ed Exp $

#ifndef OCICURSOR_H
#define OCICURSOR_H 1

#include <OCISISError.hpp>
#include <string>
#include <iostream>
#include <OCIInfo.hpp>
#include <OCIMarsh.hpp>

class OCICursor { // TODO: declare as "final" with c++11
public:
  OCICursor(long aOciHandleId, const std::string& sql,
            const OCIVariableInfo bindInfo[],
            const OCIVariableInfo defineInfo[],
            int nArrayFetch = 0);
  // no variable info supplied, use BindInt, Exec, etc.
  OCICursor(long aOciHandleId, const std::string& sql);
  ~OCICursor();                                  // non-virtual: do not derive

  inline const std::string& sql() const {return mySQL;}
  inline OCIHANDLE* ociHandle() const; // {return myOciHandle;}
  inline long       ociHandleId() const {return myOciHandleId;}
  OCISISError       Open();
  inline int        isOpen() const {return IsOpen;}
  OCISISError       Parse();
  inline int        isParsed() const {return IsParsed;}
  OCISISError       Exec();
  OCISISError       Exec(int &);
  OCISISError       Fetch();
  inline int        RowsUpdated() const {return (int) rowsUpdated;}
  OCISISError       Close();

  OCISISError       Execute(OCIMarshall& inMS);
  OCISISError       Execute(OCIMarshall& inMS, OCIMarshall& outMS);

  // to prevent "error: parameter ‘<anonymous>’ includes pointer to array of unknown bound ‘OCIVariableNames'"
  // (legal in C and early versions of C++)
  // But this just introduces a new error: ISO C++ forbids converting a string constant to ‘char*’ [-Wwrite-strings]
  // And why is a typedef used here?
  // TODO: how to make a self resizing array of char pointers?
  //#define MAX_SIZE_FOR_COMPILER 100
  //typedef char* OCIVariableNames[MAX_SIZE_FOR_COMPILER];
  typedef char* OCIVariableNames;

  // passing optional list of variable names triggers obndrv binding
  OCISISError       Bind(OCIMarshall& aMS, const OCIVariableInfo vi[], const OCIVariableNames[] = NULL);
                         //const OCIVariableNames* = (const OCIVariableNames*)0);
  OCISISError       Define(OCIMarshall& aMS, const OCIVariableInfo vi[]);

  //    String
  OCISISError       BindString(int field, const unsigned char* field_location,
                               int field_size, const short* nullflag);
  OCISISError       BindString(const char* field_literal,
                               const unsigned char* field_location,
                               int field_size, const short* nullflag);
  OCISISError       DefineString(int sequence_no, unsigned char* field_location,
                                 int field_size);

  //    Char
  OCISISError       BindChar(int field, const unsigned char* field_location,
                             int field_size, const short* nullflag);
  OCISISError       BindChar(const char* field_literal,
                             const unsigned char* field_location,
                             int field_size, const short*  nf);
  OCISISError       DefineChar(int sequence_no, unsigned char* field_location,
                               int field_size);

  //    RAW
  OCISISError       BindRaw(int field, const unsigned char* field_location,
                            int field_size, const short* nullflag);
  OCISISError       BindRaw(const char* field_literal,
                            const unsigned char* field_location,
                            int field_size, const short*  nf);
  OCISISError       DefineRaw(int sequence_no, unsigned char* field_location,
                              int field_size);

typedef long binding_int;

  //    int
  OCISISError       BindInt(int  field, binding_int* field_location);
  OCISISError       BindInt(const char* field_literal, binding_int* field_location);
  OCISISError       DefineInt(int sequence_no, long* field_location, const short* nullflag);

private:
  OCICursor();                                        // disable default ctor
  OCICursor&             operator=(const OCICursor&); // disable op=
  void                   initArray();
  const static int       CHAR_TYPE;
  const static int       INT_TYPE;
  const static int       RAW_TYPE ;
  const static int       VARCHAR2_TYPE ;
  const static int       STRING_TYPE ;
  const static short     NULLFLAG;
  const static short     NOTNULLFLAG;
        static short     zeroIndp;

  int                    IsOpen;
  int                    IsParsed;
  std::string            mySQL;
  long                   myOciHandleId;
  const OCIVariableInfo* myBindInfo;
  const OCIVariableInfo* myDefineInfo;
  OCIHANDLE*             myOciHandle;
  // transparent support for Array Interface:
  int                    myArraySize;            // number of rows in each fetch [0 if array
                                                 // interface not used]
  int                    myCurrentOffset;        // next row to return from internal table
  int                    myNumDefines;           // number of define variables
  char**                 myDefArray;             // our internal table
  short*                 myIndicatorVector;
  ub4                    rowsUpdated;            // set value in method Exec
  OCIDefine *            p_dfn;
  OCIBind   *            p_bnd;
  int                    firstFetchFlag;
  OCISISError            theLastOCISISError;     // keep the last OCISISError which we got
  OCIMarshall*           myOutMS;
};  

// Name: OCICursor
// Documentation: Oracle cursor
// Visibility: exported 
// Cardinality: n
// Hierarchy:
//  Superclasses: list of class names
// Generic parameters: list of parameters
// Public Interface:
//  Uses: list of class names
//  Operations: list of operation declarations
// Concurrency: sequential
// Persistance: static | dynamic
#endif
