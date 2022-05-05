// OCICursor.cpp - Oracle select cursor
// $Id:

#include <ociutil/OCICursor.hpp>
#include <cstring>                          // for strlen
#include <sstream>
#include <iomanip>                          // for setw
#include <sys/time.h>                       // for timeval et al
#include <stdlib.h>                         // for getenv
#include <ociutil/OracleServer.hpp>
#include <sisutil/ErrLog.hpp>

// happify eclipse (not otherwise needed)
#include <oci.h>
#include <ociutil/ocihandle.hpp>

// https://docs.oracle.com/cd/B28359_01/appdev.111/b28395/oci03typ.htm#i423877
const int   OCICursor::CHAR_TYPE = 96;      // SQLT_AFC
const int   OCICursor::INT_TYPE = 3;        // SQLT_INT
const int   OCICursor::RAW_TYPE = 23;       // SQLT_BIN
const int   OCICursor::VARCHAR2_TYPE = 1;   // SQLT_CHR
const int   OCICursor::STRING_TYPE = 5;     // SQLT_STR
const short OCICursor::NULLFLAG = -1;
const short OCICursor::NOTNULLFLAG = 0;
short       OCICursor::zeroIndp = 0;

static const char thisClass[] = "OCICursor";

// utility function to dump the contents of a YUFlatArray
void printYUFA(struct YUFlatArray* yf) {
   std::cerr << "YUFlatArray data: size = " << yf->size << std::endl;
   char* data = yf->data;
   for (int k = 0; k < yf->size; k++) {
      char ch = data[k];
      std::cerr << (isprint(ch) ? ch : '.');
   }
   std::cerr << std::endl << std::endl;
   return;
}

// c'tor 1
OCICursor::OCICursor(long aOciHandleId, const std::string& aSQLString, const OCIVariableInfo aBindInfo[], const OCIVariableInfo aDefineInfo[], int nArrayFetch) :
   IsOpen(0), IsParsed(0), mySQL(aSQLString), myOciHandleId(aOciHandleId), myBindInfo(aBindInfo),
   myDefineInfo(aDefineInfo), myArraySize(nArrayFetch), myCurrentOffset(0), myDefArray(NULL),
   myIndicatorVector(NULL), myOutMS(NULL)
{
   //if (getenv("SIS_OCI_DEBUG"))
   //   std::cerr << "OCICursor::OCICursor(): --0001-- entering c'tor 1" << std::endl;

   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::OCICursor(): --0001-- entering c'tor 1" << std::endl;
      int start, length; char type;
      std::cerr << "Binding Info" << std::endl;
      int index = 0;
      do {
         start = aBindInfo[index].startLocation;
         length = aBindInfo[index].length;
         type = aBindInfo[index].type;
         std::cerr << "Start = " << start << ", Length = " << length << ", type = " << type << std::endl;
         index++;
      } while (start != 0 && length != 0 && type != '\0');

      std::cerr << "Define Info" << std::endl;
      index = 0;
      do {
         start = aDefineInfo[index].startLocation;
         length = aDefineInfo[index].length;
         type = aDefineInfo[index].type;
         std::cerr << "Start = " << start << ", Length = " << length << ", type = " << type << std::endl;
         index++;
      } while (start != 0 && length != 0 && type != '\0');
   }

   // ??? myArraySize can not be too big???
   if (myArraySize > 10)
      myArraySize = 11;

   initArray();

   rowsUpdated = 0;
   p_dfn = (OCIDefine*)0;
   p_bnd = (OCIBind*)0;
   firstFetchFlag = 1;
}

// ctor 2 supplies no variable info so Bind, Execute cannot be used,
// and only BindInt, BindRaw, Exec, can
OCICursor::OCICursor(long aOciHandleId, const std::string& aSQLString) :
   IsOpen(0), IsParsed(0), mySQL(aSQLString), myOciHandleId(aOciHandleId), myBindInfo(NULL), myDefineInfo(NULL),
   myArraySize(0), myCurrentOffset(0), myDefArray(NULL), myIndicatorVector(NULL), myOutMS(NULL)
{
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::OCICursor(): --0002-- entering c'tor 2" << std::endl;
   rowsUpdated = 0;
   p_dfn = (OCIDefine*)0;
   p_bnd = (OCIBind*)0;
   firstFetchFlag = 1;
   myNumDefines = 0;
   myOciHandle = NULL;
}

// Array interface not preserved
// OCICursor::OCICursor(const OCICursor& c)
//         : IsOpen(0), IsParsed(0), mySQL(c.sql()), myLdaId(c.ldaId()),
//         	myLda(c.ociHandle()) ,
//         	myArraySize(0), myCurrentOffset(0), myDefArray(NULL),
//         	myIndicatorVector(NULL),
//         	myOutMS(NULL) {}

void OCICursor::initArray() {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::initArray(): --001-- entering" << std::endl;
   // count the defines:
   for (myNumDefines = 0; myDefineInfo[myNumDefines].length != 0; myNumDefines++)
      ;

   // myArraySize is usually 0
   if (myArraySize == 1 || myArraySize < 0)
      myArraySize = 0;

   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::initArray(): myArraySize = " << myArraySize
      << ", myNumDefines = " << myNumDefines << std::endl;

   if (myArraySize) {
      myDefArray = new char* [myNumDefines * sizeof(char*)];
      if (!myDefArray) {          // call new_handler?
         getTheErrLog().putLine(thisClass, "initArray", "new char* fails");
         myArraySize = 0;
      }
      else {
         for (int defVar = 0; defVar < myNumDefines; defVar++) {
            const OCIVariableInfo& thisDefinition = myDefineInfo[defVar];
            const int thisByteLength = myArraySize * (thisDefinition.length + ((thisDefinition.type == 'S') ? 1 : 0));
            myDefArray[defVar] = new char[thisByteLength];
            memset((void*)myDefArray[defVar], 0, thisByteLength);
            if (!myDefArray[defVar]) { // call new_handler?
               getTheErrLog().putLine(thisClass, "initArray", "new char fails");
               myArraySize = 0;
               break;
            }
         }
         if (myArraySize) {
            myIndicatorVector = new short[myArraySize * sizeof(short)];
            if (!myIndicatorVector)
               myArraySize = 0;
         }
      }
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::initArray(): using ArraySize = " << myArraySize
      << ", myNumDefines = " << myNumDefines << std::endl;
}

// if you want to read data from db, you should set isOpen flag to 1
// if you set isOpen to 0, then Fetch() will exit
OCISISError OCICursor::Open() {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Open(): --002a-- entering" << std::endl;
   OCISISError retVal;
   myOciHandle = OracleServer::theServer()->getOciHandle(ociHandleId());
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Open(): --002b-- myOciHandle = " << myOciHandle << std::endl;
   // because there is no oopen() in Oracle 11g, we fake open() here
   if (!IsOpen)
      IsOpen = 1;
   return retVal;
}

OCISISError OCICursor::Parse() {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Parse(): --003-- entering" << std::endl;
   OCISISError retVal;
   if (OracleServer::isTraceDebug() || OracleServer::isTraceTiming()) {
      getTheErrLog().putLine(thisClass, "Parse", std::string(" \"" + sql() + "\""));
   }

   const text* theSQL = (const text*)sql().c_str();

   OCIHANDLE* handle = ociHandle();
   // TODO: OCIStmtPrepare() has been deprecated in 12c - use OCIStmtPrepare2() instead
   if (OCIStmtPrepare(handle->phStmt,      // OCIStmt       *stmtp
      handle->phErr,                    // OCIError      *errhp
      theSQL,                           // const OraText *stmt,
      (ub4)strlen((char*)theSQL),    // ub4            stmt_len
      OCI_NTV_SYNTAX,                   // ub4            language
      OCI_DEFAULT))                     // ub4            mode
   {
      /*
      if (OCIStmtPrepare2(handle->phService,             // OCISvcCtx     *svchp,
                          &handle->phStmt,               // OCIStmt       *stmthp
                          handle->phErr,                 // OCIError      *errhp
                          theSQL,                        // const OraText *stmt,
                          (ub4) strlen((char *) theSQL), // ub4            stmt_len
                          NULL,                          // const OraText *key,
                          0,                             // ub4            keylen,
                          OCI_NTV_SYNTAX,                // ub4            language
                          OCI_DEFAULT))                  // ub4            mode
      {
      */
      retVal = OCISISError("OCICursor::Parse", sql(), handle);
      Close();
   }
   return retVal;
}

/*
 indp: Output Indicator Value and Meaning:
 -2: The length of the item is greater than the length of the output variable;
 the item has been truncated. Additionally, the original length is longer
 than the maximum data length that can be returned in the sb2 indicator variable.
 -1: The selected value is null, and the value of the output variable is unchanged.
 0: Oracle assigned an intact value to the host variable.
 >0: The length of the item is greater than the length of the output variable;
 the item has been truncated. The positive value returned in the indicator variable is
 the actual length before truncation.
 */
OCISISError OCICursor::DefineString(int sequence_no, unsigned char* field_location, int field_size) {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::DefineString(): --004-- entering with sequence_no=" << sequence_no << ", field_location=" << field_location << ", field_size=" << field_size << std::endl;
   OCISISError retVal;
   sb2 indp = 0;
   //short* pInd = myArraySize ? myIndicatorVector: &zeroIndp;

   OCIHANDLE* handle = ociHandle();
   if (OCIDefineByPos(handle->phStmt, &p_dfn, handle->phErr, (ub4)sequence_no, (void*)field_location,
      (sb4)field_size, (ub2)STRING_TYPE, (void*)&indp, (ub2*)0, (ub2*)0, (ub4)OCI_DEFAULT)) {
      retVal = OCISISError("OCICursor::DefineString", "OCIDefineByPos", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::DefineString(): indp = " << indp << std::endl;
   return retVal;
}

OCISISError OCICursor::DefineChar(int sequence_no, unsigned char* field_location, int field_size) {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::DefineChar(): --004-- entering with sequence_no=" << sequence_no << ", field_location=" << field_location << ", field_size=" << field_size << std::endl;
   OCISISError retVal;
   sb2 indp = 0;
   ub2 lname_len; // it is new for Oracle 11G ?, return me whitespaces if it zero
   //short* pInd = myArraySize ? myIndicatorVector: &zeroIndp;
   OCIHANDLE* handle = ociHandle();
   if (OCIDefineByPos(handle->phStmt, &p_dfn, handle->phErr, sequence_no, (dvoid*)field_location,
      (sword)field_size, CHAR_TYPE, &indp, (ub2*)&lname_len, (ub2*)NULL, OCI_DEFAULT)) {
      retVal = OCISISError("OCICursor::DefineChar", "OCIDefineByPos", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::DefineChar(): indp = " << indp << std::endl;
   return retVal;
}

OCISISError OCICursor::DefineRaw(int sequence_no, unsigned char* field_location, int field_size) {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::DefineRaw(): --004-- entering " << std::endl;
   OCISISError retVal;
   sb2 indp = 0;
   //short* pInd = myArraySize ? myIndicatorVector: &zeroIndp;

   OCIHANDLE* handle = ociHandle();
   if (OCIDefineByPos(handle->phStmt, &p_dfn, handle->phErr, sequence_no, (dvoid*)field_location,
      (sword)field_size, RAW_TYPE, &indp, (ub2*)0, (ub2*)0, OCI_DEFAULT)) {
      retVal = OCISISError("OCICursor::DefineRaw", "OCIDefineByPos", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::DefineRaw(): indp = " << indp << std::endl;
   return retVal;
}

OCISISError OCICursor::DefineInt(int sequence_no, long* field_location, const short*) {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::DefineInt(): --004-- entering " << std::endl;
   OCISISError retVal;
   sb2 indp = 0;
   OCIHANDLE* handle = ociHandle();
   if (OCIDefineByPos(handle->phStmt, &p_dfn, handle->phErr, (unsigned int)sequence_no, (dvoid*)field_location,
      sizeof(long), INT_TYPE, &indp, (ub2*)0, (ub2*)0, OCI_DEFAULT)) {
      retVal = OCISISError("OCICursor::DefineRaw", "OCIDefineByPos", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::DefineString(): indp = " << indp << std::endl;
   return retVal;
}

/*
 indp: Input Indicator Value and Action Taken by Oracle
 -1: Oracle assigns a NULL to the column, ignoring the value of the input variable.
 >=0: Oracle assigns the value of the input variable to the column.
 */
OCISISError OCICursor::BindString(int field, const unsigned char* field_location, int field_size, const short*) {
   OCISISError retVal;
   sb2 indp = 0;
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindString(int): --005-- called with field_literal=" << field << ", field_location=" << field_location << ", field_size=" << field_size << std::endl;

   OCIHANDLE* handle = ociHandle();
   if (OCIBindByPos(handle->phStmt, &p_bnd, handle->phErr, field, (dvoid*)field_location, field_size,
      STRING_TYPE, &indp, (ub2*)0, (ub2*)0, (ub4)0, (ub4*)0, OCI_DEFAULT)) {
      retVal = OCISISError("OCICursor::BindString", "OCIBindByPos", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindString(): indp = " << indp << std::endl;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindString(int): --006-- returning" << std::endl;
   return retVal;
}

OCISISError OCICursor::BindString(const char* field_literal, const unsigned char* field_location, int field_size, const short*) {
   OCISISError retVal;
   sb2 indp = 0;
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindString(const char*): --007-- called with field_literal=" << field_literal << ", field_location=" << field_location << ", field_size=" << field_size << std::endl;

   OCIHANDLE* handle = ociHandle();
   sword res = OCIBindByName(handle->phStmt, &p_bnd, handle->phErr, (text*)field_literal, -1,
      (dvoid*)field_location, field_size, STRING_TYPE, &indp, (ub2*)0, (ub2*)0, (ub4)0, (ub4*)0,
      OCI_DEFAULT);
   if (res) {
      retVal = OCISISError("OCICursor::BindString", "OCIBindByName", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindString(): indp = " << indp << std::endl;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindString(const char*): --008-- returning" << std::endl;
   return retVal;
}

/**
 * @param field a numbered field
 * @param field_location a pointer to the char value to be bound
 * @param field_size
 * @param nf
 */
OCISISError OCICursor::BindChar(int field, const unsigned char* field_location, int field_size, const short*) {
   OCISISError retVal;
   sb2 indp = 0;
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindChar(int): --009-- called with field_literal=" << field << ", field_location=" << field_location << ", field_size=" << field_size << std::endl;

   OCIHANDLE* handle = ociHandle();
   if (OCIBindByPos(handle->phStmt, &p_bnd, handle->phErr, field, (dvoid*)field_location, field_size,
      CHAR_TYPE, &indp, (ub2*)0, (ub2*)0, (ub4)0, (ub4*)0, OCI_DEFAULT)) {
      retVal = OCISISError("OCICursor::BindChar", "OCIBindByPos", handle);
      Close();
   }

   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindChar(int): --010-- returning" << std::endl;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindChar(): indp = " << indp << std::endl;
   return retVal;
}

/**
 * @param field_literal a named field (not numbered)
 * @param field_location a pointer to the string value to be bound
 * @param field_size
 * @param nf (not used)
 */
OCISISError OCICursor::BindChar(const char* field_literal, const unsigned char* field_location, int field_size,
   const short*) {
   OCISISError retVal;
   sb2 indp = 0;

   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::BindChar(const char*): --011-- called, field_literal=" << field_literal
         << ", field_location=" << (char*)field_location << ", field_size=" << field_size << std::endl;
      //std::cerr << "OCICursor::BindChar(char*, binding_int*): p_bnd=" << p_bnd << std::endl;
   }

   OCIHANDLE* handle = ociHandle();
   sword res = OCIBindByName(handle->phStmt, &p_bnd, handle->phErr, (text*)field_literal, -1,
      (void*)field_location, field_size, CHAR_TYPE, &indp, (ub2*)0, (ub2*)0, (ub4)0, (ub4*)0,
      OCI_DEFAULT);
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindChar(const char*): res=" << res << std::endl;
   if (res) {
      retVal = OCISISError("OCICursor::BindChar", "OCIBindByName", handle);
      Close();
   }
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindChar(): indp = " << indp << std::endl;
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindChar(const char*): --013--returning" << std::endl;
   return retVal;
}

OCISISError OCICursor::BindRaw(const char* field_literal, const unsigned char* field_location, int field_size, const short*) {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindRaw(char): --014-- called, field_literal=" << field_literal
      << ", field_location=" << (char*)field_location << ", field_size=" << field_size << std::endl;
   OCISISError retVal;
   sb2 indp = 0;
   OCIHANDLE* handle = ociHandle();
   if (OCIBindByName(handle->phStmt, &p_bnd, handle->phErr, (text*)field_literal, -1, (dvoid*)field_location,
      field_size, RAW_TYPE, &indp, (ub2*)0, (ub2*)0, (ub4)0, (ub4*)0, OCI_DEFAULT))
   {
      retVal = OCISISError("OCICursor::BindRaw", "OCIBindByName", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindRaw(char): indp = " << indp << std::endl;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindRaw(const char*): --015-- returning" << std::endl;
   return retVal;
}

OCISISError OCICursor::BindRaw(int field, const unsigned char* field_location, int field_size, const short*) {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindRaw(int): --016-- called, field=" << field
      << ", field_location=" << (char*)field_location << ", field_size=" << field_size << std::endl;
   OCISISError retVal;
   sb2 indp = 0;
   OCIHANDLE* handle = ociHandle();
   if (OCIBindByPos(handle->phStmt, &p_bnd, handle->phErr, field, (dvoid*)field_location, field_size,
      RAW_TYPE, &indp, (ub2*)0, (ub2*)0, (ub4)0, (ub4*)0, OCI_DEFAULT))
   {
      retVal = OCISISError("OCICursor::BindRaw", "OCIBindByPos", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindRaw(int): indp = " << indp << std::endl;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindRaw(int): --017-- returning" << std::endl;
   return retVal;
}

/**
 * @param field a numbered field
 * @param field_location a pointer to the integer value to be bound
 */
OCISISError OCICursor::BindInt(int field, binding_int* field_location) {
   OCISISError retVal;
   sb2 indp = 0;
   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::BindInt(int, binding_int*): --018-- called, field=" << field << ", value="
         << *field_location << std::endl;
      std::cerr << "OCICursor::BindInt(int, binding_int*): p_bnd=" << p_bnd << std::endl;
   }

   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::BindInt(int, binding_int*): --027-- calling OCIBindByPos()" << std::endl;
   }

   OCIHANDLE* handle = ociHandle();
   sword res = OCIBindByPos(handle->phStmt,
      &p_bnd, handle->phErr,
      (unsigned int)field,
      (dvoid*)field_location,
      sizeof(binding_int),
      INT_TYPE,
      &indp,
      (ub2*)0,
      (ub2*)0,
      (ub4)0,
      (ub4*)0,
      OCI_DEFAULT);
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindInt(int, binding_int*): --021-- res=" << res << std::endl;
   if (res) {
      retVal = OCISISError("OCICursor::BindInt", "OCIBindByPos", handle);
      Close();
   }
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindInt(int, binding_int*): indp = " << indp << std::endl;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindInt(int, binding_int*): --019-- returning" << std::endl;
   return retVal;
}

/**
 * @param field_literal a named field (not numbered)
 * @param field_location a pointer to the integer value to be bound
 */
OCISISError OCICursor::BindInt(const char* field_literal, binding_int* field_location) {
   OCISISError retVal;
   sb2 indp = 0;

   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::BindInt(char*, binding_int*): --020-- called; field_literal=" << field_literal
         << ", value=" << *field_location << std::endl;
      //std::cerr << "OCICursor::BindInt(char*, binding_int*): p_bnd=" << p_bnd << std::endl;
      std::cerr << "OCICursor::BindInt(char*, binding_int*): " << "size of field_location: " << sizeof(field_location)
         << ", size of binding_int: " << sizeof(binding_int) << std::endl;
   }

   //binding_int testInt = *field_location;
   char work[50];
   sprintf(work, "%ld", *field_location);
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindInt(char*, binding_int*): --020-- work=" << work << std::endl;
   /*
    BindString(const char* field_literal,
    const unsigned char* field_location,
    int field_size, NULL);
    */

   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::BindInt(char*, binding_int*): --027-- calling OCIBindByName()" << std::endl;
   }

   OCIHANDLE* handle = ociHandle();
   sword res = OCIBindByName(handle->phStmt,    // statement handle to the SQL or PL/SQL statement
      &p_bnd,                 // a pointer to a bind handle which is implicitly allocated by this call
      handle->phErr,          // an error handle to pass to OCIErrorGet() for diagnostic information in the event of an error.
      (text*)field_literal, // a placeholder, specified by its name, which maps to a variable in the statement associated with the statement handle
      -1,
      (void*)field_location, // pointer to the data value
      //(void *)work, // pointer to the data value
      //&testInt,  // pointer to the data value
      sizeof(binding_int),    // the size in bytes of the data value
      //strlen(work), // the size in bytes of the data value
      INT_TYPE,               // the data type of the value being bound
      //SQLT_INT,               // the data type of the value being bound
      &indp,                  // pointer to an indicator variable
      (ub2*)0,              // "alenp" for arrays, not single values
      (ub2*)0,              // for arrays, not single values
      (ub4)0,                // for PL/SQL binds
      (ub4*)0,              // for arrays, not single values
      OCI_DEFAULT);           // recommended setting for mode
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::BindInt(char*, binding_int*): res=" << res << std::endl;
   if (res) {
      retVal = OCISISError("OCICursor::BindInt", "OCIBindByName", handle);
      Close();
   }
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindInt(char*, binding_int*): indp = " << indp << std::endl;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::BindInt(char*, binding_int*): --021-- returning" << std::endl;
   return retVal;
}

OCISISError OCICursor::Exec(int& RowsReturned) {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Exec(int): --040-- " << std::endl;
   OCISISError retVal = Exec();
   RowsReturned = rowsUpdated;
   return retVal;
}

OCISISError OCICursor::Exec() {
   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::Exec(): ==022== called" << std::endl;
      std::cerr << "\t myArraySize: " << myArraySize << std::endl;
   }

   static const char thisRoutine[] = "Exec";
   OCISISError retVal = OCISISError();
   struct timeval startTime = { 0, 0 };
   if (OracleServer::isTraceTiming()) {
      if (gettimeofday(&startTime, NULL) == -1)
         getTheErrLog().logErrMsg(thisClass, thisRoutine, "gettimeofday", "fails");
   }

   OCIHANDLE* handle = ociHandle();
   /*
   if (OracleServer::isTraceDebug()) {
      sb4 found = 0;
      OraText* bvnp[25], *invp[25];
      ub1 bvnl[25];
      ub1 inpl[25], dupl[25];
      OCIBind* bhnds[25];
      sword res = OCIStmtGetBindInfo(
            handle->phStmt,      // OCIStmt *
            handle->phErr,       // OCIError *
            25,                  // ub4 - size
            1,                   // ub4 - startloc
            &found,              // sb4 * (+ive if fewer than size else -ive)
            bvnp,                // OraText * - pointers to bind variable names
            bvnl,                // ub1 - length of each bvnp element
            invp,                // OraText * - pointers to indicator variable names
            inpl,                // ub1 - pointers to the length of each invp element
            dupl,                // ub1 - duplicates? (0 if false, 1 if true)
            bhnds                // OCIBind * - array of bind handles if they exist
            );

      if (res == OCI_NO_DATA) {
         std::cerr << "OCICursor::Exec(): --022-- no bind variables found." << std::endl;
      } else {
         std::cerr << "OCICursor::Exec(): --022-- found " << found << " bind variables." << std::endl;
         std::cerr << "OCICursor::Exec(): --022-- myNumDefines: " << myNumDefines << std::endl;
         found = abs(found);
         for (int i = 0; i < found; i++) {
            OCIBind * pBhnd = bhnds[i];
            OraText * bvn = bvnp[i];
            OraText * inv = invp[i];
            std::cerr << "OCICursor::Exec(): --022-- " << i << "\t" << pBhnd << " is bound to name: "
                      << bvn << " and " << inv << std::endl;
         }
         std::cerr << "OCICursor::Exec(): --022-- end of bound variables" << std::endl;
      }
   }
   */
   //std::cerr << "OCICursor::Exec(): --022-- continuing" << std::endl;

   // multiple rows
   sword ret;
   if (myArraySize) {
      if (getenv("SIS_OCI_DEBUG"))
         std::cerr << "OCICursor::Exec(): --022-- myArraySize > 0" << std::endl;
      // initialize myCurrentOffset to do a new select
      myCurrentOffset = 0;

      // run sql
      if (getenv("SIS_OCI_DEBUG")) {
         std::cerr << "OCICursor::Exec(): --022-- calling OCIStmtExecute() with array" << std::endl;
      }
      ret = OCIStmtExecute(handle->phService, handle->phStmt, handle->phErr, (ub4)1,
         (ub4)0, (CONST OCISnapshot*) NULL, (OCISnapshot*)NULL,
         OCI_DEFAULT);
      if (getenv("SIS_OCI_DEBUG"))
         std::cerr << "OCICursor::Exec(): OCIStmtExecute ret=" << ret << std::endl;
      if (ret) {
         // an error happened, get error from Oracle
         OCISISError actualRetVal = OCISISError("OCICursor::ExecFet", "OCIStmtExecute", handle);
         if (getenv("SIS_OCI_DEBUG"))
            std::cerr << "OCICursor::Exec(): --023-- return OCISISError=" << actualRetVal << std::endl;

         if (actualRetVal.value() == OCISISError::OK) { // We got OK when convert from TRUNCATED, so cannot call Close(), we will read rows
            if (getenv("SIS_OCI_DEBUG"))
               std::cerr
               << "OCICursor::Exec(): --024-- return OK when convert from TRUNCATED, so can not Call Close(), we will read rows"
               << std::endl;
            //retVal = actualRetVal;
         }
         else if (actualRetVal.value() != OCISISError::NOT_FOUND) { // We got a real error, close
            retVal = actualRetVal; 	// return the error
            if (getenv("SIS_OCI_DEBUG"))
               std::cerr << "OCICursor::Exec(): --025-- return real error" << std::endl;
            Close();
         }
         else { // no data
         // get fetched row count
            OCIAttrGet(handle->phStmt, OCI_HTYPE_STMT, &rowsUpdated, 0, OCI_ATTR_ROWS_FETCHED, handle->phErr);
            if (getenv("SIS_OCI_DEBUG"))
               std::cerr << "OCICursor::Exec(): --026-- set value into rowsUpdated. rowsUpdated=" << rowsUpdated
               << std::endl;

            // eat the OCISISError::NOT_FOUND iff some rows were processed
            if (RowsUpdated() == 0) {
               retVal = actualRetVal; // return the error
               Close();
            } // else leave retVal as success
         }
      }
   }
   else {
      if (getenv("SIS_OCI_DEBUG")) {
         std::cerr << "OCICursor::Exec(): --027-- calling OCIStmtExecute()" << std::endl;
      }
      ret = OCIStmtExecute(handle->phService,           // svchp - service context handle
         handle->phStmt,              // stmtp - statement handle
         handle->phErr,               // errhp - error handle
         (ub4)1,                     // iters -
         (ub4)0,                     // rowoff -
         (CONST OCISnapshot*) NULL,  // snap_in - optional
         (OCISnapshot*)NULL,        // snap_out - optional
         OCI_DEFAULT                  // mode
      );
      if (ret) {
         if (getenv("SIS_OCI_DEBUG")) {
            std::cerr << "OCICursor::Exec(): --027--  OCIStmtExecute() returns: " << ret << std::endl;
         }
         // one row
         retVal = OCISISError("OCICursor::Exec", "OCIStmtExecute", handle);
         /*
         if (getenv("SIS_OCI_DEBUG")) {
            OraText errbuf[512];
            //ub4 buflen;
            sb4 errcode;
            OCIErrorGet(ociHandle()->phErr, (ub4) 1, (text *) NULL, &errcode, errbuf, (ub4) sizeof(errbuf), (ub4) OCI_HTYPE_ERROR);
            std::cerr << "OCICursor::Exec(): --028-- Oracle explanation: " << errbuf << std::endl;
            std::cerr << "OCICursor::Exec(): --028-- myArraySize==0 there is an error when we do OCIStmtExecute" << std::endl;
            std::cerr << "OCICursor::Exec(): --028-- return OCI_Error=" << retVal << ", ret: " << ret << std::endl;
         }
         */
         Close();
      }
   }

   if (OracleServer::isTraceTiming()) {
      struct timeval endTime = { 0, 0 };
      if (gettimeofday(&endTime, NULL) == -1)
         getTheErrLog().logErrMsg(thisClass, thisRoutine, "gettimeofday", "fails");
      int uSecExecTime = (endTime.tv_sec - startTime.tv_sec) * 1000 + (endTime.tv_usec - startTime.tv_usec) / 1000;
      //strstream tmpStream;
      std::stringstream tmpStream;
      tmpStream << "[oexec time : " << std::setw(5) << uSecExecTime << " ms]"; // << std::ends;
      getTheErrLog().putStream(thisClass, thisRoutine, tmpStream);
   }

   if (!retVal.isSuccess() && (retVal.value() != OCISISError::NOT_FOUND) && (OracleServer::isTraceVerbose())) {
      getTheErrLog().putLine(thisClass, thisRoutine, std::string("sql: \"" + sql() + "\""));
   }

   // get fetched row count
   OCIAttrGet(ociHandle()->phStmt, OCI_HTYPE_STMT, &rowsUpdated, 0, OCI_ATTR_ROWS_FETCHED, handle->phErr);
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Exec(): --029--set value into rowsUpdated. rowsUpdated=" << rowsUpdated << std::endl;

   firstFetchFlag = 1;

   // keep the last OCISISError
   theLastOCISISError = retVal;

   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Exec(): ==030== returning" << std::endl;

   return retVal;
}

OCISISError OCICursor::Define(OCIMarshall& aMS, const OCIVariableInfo vi[]) {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Define(): entering" << std::endl;
   OCISISError retVal;
   for (int i = 0; ((vi[i].length != 0) && (retVal.isSuccess())); i++) {
      const int thisLength = vi[i].length;
      //unsigned char* thisItem = myArraySize ? (unsigned char*)myDefArray[i]
      //    	: &aMS.myArray->data[vi[i].startLocation];
      unsigned char* thisItem =
         myArraySize ? (unsigned char*)myDefArray[i] : (unsigned char*)(&aMS.myArray->data[vi[i].startLocation]);
      //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::Define(): " << i << ", type: " << vi[i].type << ", thisItem: " << thisItem << ", length: " << thisLength << std::endl;
      if (vi[i].type == 'S') {
         retVal = DefineString(i + 1, thisItem, thisLength + 1);
         //retVal = DefineString(i + 1, (unsigned char*)thisItem, thisLength + 1);
      }
      else if (vi[i].type == 'R') {
         retVal = DefineRaw(i + 1, thisItem, thisLength);
         //retVal = DefineRaw(i + 1, (unsigned char*)thisItem, thisLength);
      }
      else {
         // default to fixed char for Date,DateTime,Time,Char
         retVal = DefineChar(i + 1, thisItem, thisLength);
         //retVal = DefineChar(i + 1, (unsigned char*)thisItem, thisLength);
      }
   }
   return retVal;
}

// N.B. OCIVariableNames support is for string variables only.
/**
 * For bind by parameter, position must be in order,
 * but our c++ code uses parameters not in order
 * so I changed it to use bind by parameter name (pingli)
 */
OCISISError OCICursor::Bind(OCIMarshall& aMS, const OCIVariableInfo vi[], const OCIVariableNames names[]) {
   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::Bind(): --031-- called" << std::endl;
      //std::cerr << "\t aMS = " << &aMS << std::endl;
      printYUFA(aMS.myArray);
      /*
      std::cerr << "\t incoming data: size = " << aMS.myArray->size << std::endl;
      char * data = aMS.myArray->data;
      for (int k = 0; k < aMS.myArray->size; k++) {
         char ch = data[k];
         std::cerr << (isprint(ch) ? ch : '.');
      }
      std::cerr << std::endl << std::endl;
      */
   }

   //std::cerr << "looping through OCIVariableInfo looking for bound variables" << std::endl;
   OCISISError retVal;
   for (int i = 0; ((vi[i].length != 0) && (retVal.isSuccess())); i++) {
      //unsigned char& thisItem = (unsigned char&) aMS.myArray->data[vi[i].startLocation];
      //printYUFA(aMS.myArray);
      const unsigned char& thisItem = (unsigned char&)(aMS.myArray->data[vi[i].startLocation]);
      const int thisLength = vi[i].length;

      /*
      std::cerr << i << ": ";
      for (int k = 0; k < thisLength; k++) {
         char ch = &thisItem[k];
         std::cerr << (isprint(ch) ? ch : '.');
      }
      std::cerr << std::endl << std::endl;
      */
      if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::Bind(): " << i + 1 << "  thisItem: " << thisItem << ", length: " << thisLength << ", type: " << vi[i].type << std::endl;

      short* flag;
      char* name = (char*)NULL;
      if (names) {
         name = names[i];
         //name = (*names)[i];
      }
      // output logs
      if (thisItem) {
         flag = (short*)&NOTNULLFLAG;
         if (OracleServer::isTraceDebug()) {
            //strstream tmpStream;
            std::stringstream tmpStream;
            tmpStream << "Binding(type=\'" << vi[i].type << "\'): " << (void*)&thisItem;
            tmpStream << " \"" << (char*)&thisItem << "\""; // << std::ends;
            getTheErrLog().putStream(thisClass, "Bind", tmpStream);
         }
      }
      else {
         flag = (short*)&NULLFLAG;
      }

      /*if (vi[i].type == 'N') {
           if (name)
              retVal = BindInt(name,  (long*)&thisItem);
           else
              retVal = BindInt(i + 1, (long*)&thisItem);
      } else */
      if (vi[i].type == 'S') {
         if (name) {
            retVal = BindString(name, &thisItem, thisLength + 1, flag);
         }
         else {
            //retVal = BindString(i + 1, &thisItem, thisLength + 1, flag);
            if (vi[i + 1].length == 0 && i == 0) {
               BindString(i + 1, &thisItem, thisLength + 1, flag);
            }
            else {
               char str[15];
               for (int j = 0; j < 15; j++)
                  str[j] = 0;
               snprintf(str, 15, ":%d", i + 1);
               if (getenv("SIS_OCI_DEBUG"))
                  std::cerr << "OCICursor::Bind(): --032-- R, " << i + 1 << "|" << str << "|" << std::endl;
               retVal = BindString(str, &thisItem, thisLength + 1, flag);
            }
         }
      }
      else if (vi[i].type == 'R') {
         if (OracleServer::isTraceDebug()) {
            std::stringstream tmpStream;
            //tmpStream << "Binding(type='R'): " << (void*)&thisItem; // << std::ends; // (void*)& is not really needed to compile
            tmpStream << "Binding(type='R'): " << (char*)&thisItem; // << std::ends; // (void*)& is not really needed to compile

            /*
            tmpStream << "Binding(type='R'): ";
            unsigned char * data = (unsigned char *)&aMS.myArray->data[vi[i].startLocation];
            for (int i = 0; i < thisLength; i++) {
               char buf[100];
               //snprintf(buf, thisLength, "%0X", data);
               snprintf(buf, thisLength, "%s", data);
               buf[thisLength] = 0;
               unsigned char ch = data[i];
               ch += 32;
               tmpStream << ch;
            }
            //tmpStream << buf; // << ends;
            */
            getTheErrLog().putStream(thisClass, "Bind", tmpStream);
         }

         flag = (short*)&NOTNULLFLAG;
         if (name) {
            if (getenv("SIS_OCI_DEBUG"))
               std::cerr << "OCICursor::Bind('R'): --033-- name: " << name << std::endl;
            retVal = BindRaw(name, &thisItem, thisLength, flag);
         }
         else {
            if (getenv("SIS_OCI_DEBUG"))
               std::cerr << "OCICursor::Bind('R'): --033-- name is null" << std::endl;
            //retVal = BindRaw(i + 1, &thisItem, thisLength, flag);
            if (vi[i + 1].length == 0 && i == 0) {
               if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::Bind('R'): by position " << std::endl;
               retVal = BindRaw(i + 1, &thisItem, thisLength, flag);
            }
            else {
               char str[15];
               // clear the buffer (or set str[0] = 0 or don't bother ?)
               for (int j = 0; j < 15; j++)
                  str[j] = 0;
               sprintf(str, ":%d", i + 1); // TODO: use snprintf for safety
               if (getenv("SIS_OCI_DEBUG")) {
                  std::cerr << "OCICursor::Bind('R'): by name " << std::endl;
                  std::cerr << "OCICursor::Bind('R'): --034-- R, " << i + 1 << "|" << str << "|" << std::endl;
               }
               retVal = BindRaw(str, &thisItem, thisLength, flag);
            }
         }
      }
      else { //N
         if (name) {
            //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::Bind(): by name " << std::endl;
            retVal = BindChar(name, &thisItem, strlen((const char*)&thisItem), flag);
         }
         else {
            //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::Bind(N): by position " << std::endl;
            //if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::Bind(N): " << i << "  thisItem: " << thisItem << ", length: " << thisLength << ", type: " << vi[i].type <<std::endl;
            if (vi[i + 1].length == 0 && i == 0) {
               retVal = BindChar(i + 1, &thisItem, strlen((const char*)&thisItem), flag);
            }
            else {
               char str[15];
               for (int j = 0; j < 15; j++)
                  str[j] = 0;
               sprintf(str, ":%d", i + 1);
               if (getenv("SIS_OCI_DEBUG"))
                  std::cerr << "OCICursor::Bind(): --035-- " << i + 1 << "|" << str << "|" << std::endl;
               retVal = BindChar(str, &thisItem, strlen((const char*)&thisItem), flag);
            }
         }
      }
   }
   if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::Bind() returning" << std::endl;
   return retVal;
}

/**
 * Setup the call to Exec() and then call it
 */
OCISISError OCICursor::Execute(OCIMarshall& inMS) {
   OCISISError myStatus;
   myStatus = Open();
   if (!myStatus.isSuccess())
      return (myStatus);
   if (!isParsed())
      myStatus = Parse();
   if (!myStatus.isSuccess())
      return (myStatus);
   myStatus = Bind(inMS, myBindInfo);
   if (!myStatus.isSuccess())
      return (myStatus);

   return Exec();
}

/**
 * Setup the call to Exec() and then call it
 */
OCISISError OCICursor::Execute(OCIMarshall& inMS, OCIMarshall& outMS) {
   if (getenv("SIS_OCI_DEBUG")) {
      std::cerr << "OCICursor::Execute(OCIMarshall&, OCIMarshall&): --036-- called" << std::endl;
      std::cerr << "\t myArraySize = " << myArraySize << std::endl;
      std::cerr << "\t inMS  = " << &inMS << std::endl;
      std::cerr << "\t outMS = " << &outMS << std::endl;
   }
   if (myArraySize)
      myOutMS = &outMS;                        	// N.B. outMS not copied

   OCISISError myStatus;
   myStatus = Open();
   if (!myStatus.isSuccess())
      return (myStatus);
   if (!isParsed())
      myStatus = Parse();
   if (!myStatus.isSuccess())
      return (myStatus);
   myStatus = Bind(inMS, myBindInfo);
   if (!myStatus.isSuccess())
      return (myStatus);
   myStatus = Define(outMS, myDefineInfo);
   if (!myStatus.isSuccess())
      return (myStatus);

   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Execute(): --037-- proceeding to Exec()" << std::endl;

   OCISISError retVal = Exec();

   if (getenv("SIS_OCI_DEBUG")) {
      // the first record is in outMS.myArray
      std::cerr << "OCICursor::Execute(): --037-- after Exec(), outMS.myArray->data:" << outMS.myArray->data << std::endl;
      std::cerr << "OCICursor::Execute(): --037-- after Exec(), outMS.myArray->size:" << outMS.myArray->size << std::endl;
      /*for(int i=0;i<(outMS.myArray->size>0?outMS.myArray->size:150);i++){
             std::cerr << std::setw(2) << std::setfill('0')
               << std::hex <<  "|" << (int)(outMS.myArray->data[i]) << "|" << outMS.myArray->data[i] << "|" << std::endl;
      }*/
   }
   return retVal;
}

/**
 * 1. Exec() will get a row or "no data".
 * 2. Read the row from Exec() in one of two modes: single row mode or multiple rows mode
 * 3. Do next OCIStmtFetch in one of two modes: single row mode or multiple rows mode
 * 4. In single row mode, it is simple, if we got a row, then data will be put in defined variables
 *    In multiple row mode, keep the OCI_Error first, because we may get a "no-data" error,
 *    but we still got some rows, if there isn't OCI_Error, we don't change the value of theLastOCISISError.
 *    If we got some rows, we need to put data into array, but we only put a row at one Fetch() calling
 *    and return, until all rows which we got was finished, then check theLastOCISISError, if there isn't any OCI_Error,
 *    we will do new OCIStmtFetch, but if there is "no-data" OCI_Error, we will return it and end up the Fetch()s calling.
 */
OCISISError OCICursor::Fetch() {
   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Fetch(): ==038== called" << std::endl;
   static const char thisRoutine[] = "Fetch";
   OCISISError retVal = OCISISError();
   OCIHANDLE* myHandle = ociHandle();  // because ociHandle()->... does not always work ???
   struct timeval startTime = { 0, 0 };
   if (OracleServer::isTraceTiming())
      if (gettimeofday(&startTime, NULL) == -1)
         getTheErrLog().logErrMsg(thisClass, thisRoutine, "gettimeofday", "fails");

   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Fetch(): --039-- myArraySize=" << myArraySize << std::endl;

   if (myArraySize) {
      int curTabSize = myArraySize;

      // get the real row number from db
      if (RowsUpdated() && (RowsUpdated() % myArraySize))
         curTabSize = (RowsUpdated() % myArraySize);

      if (getenv("SIS_OCI_DEBUG")) {
         std::cerr << "OCICursor::Fetch() --041-- RowsUpdated=" << RowsUpdated() << std::endl;
         std::cerr << "OCICursor::Fetch()         myArraySize=" << myArraySize << std::endl;
         std::cerr << "OCICursor::Fetch()         myCurrentOffset=" << myCurrentOffset << std::endl;
         std::cerr << "OCICursor::Fetch()         curTabSize=" << curTabSize << std::endl;
      }

      // will do the real fetch, after read all rows or read the row from Exe()
      if (myCurrentOffset >= curTabSize) {
         if (getenv("SIS_OCI_DEBUG"))
            std::cerr << "OCICursor::Fetch(): --042-- myCurrentOffset >= curTabSize" << std::endl;

         // get the last OCI Error,
         //OCISISError lastRetVal = OCISISError("CURSOR::Fetch", "", ociHandle());
         OCISISError lastRetVal = theLastOCISISError;

         // if the last error is NOT_FOUND, we will end all fetch
         if (lastRetVal.value() == OCISISError::NOT_FOUND) {
            if (getenv("SIS_OCI_DEBUG"))
               std::cerr << "OCICursor::Fetch(): --043-- lastRetVal.value() == OCISISError::NOT_FOUND" << std::endl;
            retVal = lastRetVal;
            Close();
         }
         else {
            // start a new fetch
            myCurrentOffset = 0;
            if (getenv("SIS_OCI_DEBUG"))
               std::cerr << "OCICursor::Fetch(): --044-- myCurrentOffset=" << myCurrentOffset << std::endl;

            // clear (zero) the array fetch area:
            for (int defVar = 0; defVar < myNumDefines; defVar++) {
               const OCIVariableInfo& thisDefinition = myDefineInfo[defVar];
               const int thisLength = thisDefinition.length + ((thisDefinition.type == 'S') ? 1 : 0);
               const char* thisItem = myDefArray[defVar];
               memset((void*)thisItem, 0, (myArraySize * thisLength));
            }

            if (getenv("SIS_OCI_DEBUG"))
               std::cerr << "OCICursor::Fetch(): --045-- do OCIStmtFetch()" << std::endl;
            if (OCIStmtFetch(ociHandle()->phStmt, myHandle->phErr, myArraySize, OCI_FETCH_NEXT, OCI_DEFAULT)) {
               //if (OCIStmtFetch2(ociHandle()->phStmt, myHandle->phErr, myArraySize, OCI_DEFAULT, 0, OCI_DEFAULT)) {
                  // get the error
               OCISISError actualRetVal = OCISISError("CURSOR::Fetch", "OCIStmtFetch1", myHandle);
               // keep the last OCISISError
               theLastOCISISError = actualRetVal;

               if (actualRetVal.value() == OCISISError::OK) {
                  // This is TRUNCATED error, not a real error, we already convert it to OK.
                  // so can not Call Close(), we will read rows
                  if (getenv("SIS_OCI_DEBUG"))
                     std::cerr << "OCICursor::Fetch(): --046-- return OK when convert from TRUNCATED, so can not Call Close(), we will read rows"
                     << std::endl;
                  //retVal = actualRetVal;
               }
               else if (actualRetVal.value() != OCISISError::NOT_FOUND) {
                  // no data any more, we will close
                  if (getenv("SIS_OCI_DEBUG"))
                     std::cerr << "OCICursor::Fetch(): --047-- actualRetVal.value() != OCISISError::NOT_FOUND"
                     << std::endl;
                  retVal = actualRetVal;
                  Close();
               }
               else { //got NOT_FOUND
               // get fetched row count
               //std::cerr << "OCICursor::Fetch(): --048a-- myHandle = " << myHandle << std::endl;
               //std::cerr.flush();
                  OCIAttrGet(myHandle->phStmt, OCI_HTYPE_STMT, &rowsUpdated, 0, OCI_ATTR_ROWS_FETCHED, myHandle->phErr);
                  //OCIAttrGet(ociHandle()->phStmt, OCI_HTYPE_STMT, &rowsUpdated, 0, OCI_ATTR_ROWS_FETCHED,
                  //      ociHandle()->phErr);
                  if (getenv("SIS_OCI_DEBUG"))
                     std::cerr << "OCICursor::Fetch(): --048-- set value into rowsUpdated. rowsUpdated=" << rowsUpdated
                     << "   actualRetVal.value()=" << actualRetVal.value() << std::endl;

                  // eat the OCISISError::NOT_FOUND, if the rest rows in DB is less then curTabSize, then return this error,
                  // but there may have rows, if there were no rows, we will close it
                  if (rowsUpdated == 0) {
                     if (getenv("SIS_OCI_DEBUG"))
                        std::cerr << "OCICursor::Fetch(): --049-- if (RowsUpdated()==prevNProc)" << std::endl;
                     retVal = actualRetVal;
                     Close();
                  }
               }
            }
            // get fetched row count
            //OCIAttrGet(ociHandle()->phStmt, OCI_HTYPE_STMT, &rowsUpdated, 0, OCI_ATTR_ROWS_FETCHED, ociHandle()->phErr);
            OCIAttrGet(myHandle->phStmt, OCI_HTYPE_STMT, &rowsUpdated, 0, OCI_ATTR_ROWS_FETCHED, myHandle->phErr);
            if (getenv("SIS_OCI_DEBUG"))
               std::cerr << "OCICursor::Fetch(): --050--  set value into rowsUpdated. rowsUpdated="
               << rowsUpdated << std::endl;
            if (rowsUpdated == 0) {
               if (getenv("SIS_OCI_DEBUG"))
                  std::cerr << "OCICursor::Fetch(): --052-- if (RowsUpdated()==prevNProc)" << std::endl;
               Close();
            }
         }
      }

      // got data from db, save it into myOutMS
      if (IsOpen) { //isOpen is just a flag to control whether you can read data or not
         if (getenv("SIS_OCI_DEBUG"))
            std::cerr << "OCICursor::Fetch(): --053-- IsOpen" << std::endl;

         for (int defVar = 0; defVar < myNumDefines; defVar++) {
            const OCIVariableInfo& thisDefinition = myDefineInfo[defVar];
            const int thisLength = thisDefinition.length;
            const char* thisItem = myDefArray[defVar];
            if (thisDefinition.type == 'S')
               *myOutMS << Setmw(thisLength) << (char*)&thisItem[myCurrentOffset * (thisLength + 1)];
            else if (thisDefinition.type != 'R')
               *myOutMS << Setmw(thisLength) << (char*)&thisItem[myCurrentOffset * thisLength];
            else
               // transfer each Raw byte
               for (int i = 0; i < thisLength; i++)
                  *myOutMS << thisItem[myCurrentOffset * thisLength + i];
         }
         myCurrentOffset++;
      }
   } // end of "if (myArraySize)"
   else {
      if (firstFetchFlag == 1) {
         // because new Oracle OCI is different from old oci.
         // after OCIStmtExecute runs, we get rows or no data found

         if (getenv("SIS_OCI_DEBUG"))
            std::cerr << "OCICursor::Fetch(): --054-- firstFetchFlag == 1" << std::endl;

         //			OCISISError lastRetVal	= OCISISError("CURSOR::Fetch", "firstFetchFlag = 1", ociHandle());
         //			if (lastRetVal.value() == OCISISError::NOT_FOUND) {
         //				if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCICursor::Fetch() firstFetchFlag == 1 NOT_FOUND" << std::endl;
         //					retVal = lastRetVal;
         //					Close();
         // }

         // we can not read error from Oracle again, because we already read it when we did Exe,
         // so we use rowsUpdated to check if we got data, the value of rowsUpdated came from Exe
         if (rowsUpdated == 0)
            Close();

         firstFetchFlag = 0;
      }
      else if (OCIStmtFetch(myHandle->phStmt, myHandle->phErr, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) {
         //} else if (OCIStmtFetch2(myHandle->phStmt, myHandle->phErr, 1, OCI_DEFAULT, 0, OCI_DEFAULT)) {
         retVal = OCISISError("OCICursor::OCIStmtFetch", "", myHandle);
         Close();
      }
   }

   if (OracleServer::isTraceTiming()) {
      struct timeval endTime = { 0, 0 };
      if (gettimeofday(&endTime, NULL) == -1)
         getTheErrLog().logErrMsg(thisClass, thisRoutine, "gettimeofday", "fails");
      int uSecExecTime = (endTime.tv_sec - startTime.tv_sec) * 1000 + (endTime.tv_usec - startTime.tv_usec) / 1000;
      //strstream tmpStream;
      std::stringstream tmpStream;
      tmpStream << "[ofetch time : " << std::setw(5) << uSecExecTime << " ms]"; // << std::ends;
      getTheErrLog().putStream(thisClass, thisRoutine, tmpStream);
   }
   if (!retVal.isSuccess() && (retVal.value() != OCISISError::NOT_FOUND) && (OracleServer::isTraceVerbose()))
      getTheErrLog().putLine(thisClass, thisRoutine, std::string("sql: \"" + sql() + "\""));

   if (getenv("SIS_OCI_DEBUG"))
      std::cerr << "OCICursor::Fetch(): ==055== end." << std::endl;

   return retVal;
}

OCISISError OCICursor::Close() {
   OCISISError retVal;
   if (isOpen()) { // this call seems unnecessary
      IsOpen = 0;
      IsParsed = 0;
   }

   // We need to free the statement after OCIStmtPrepare2() has given it to us.
   // Failure to do so will eventually result in a "ORA-01000: maximum open cursors exceeded"
   // message.
   OCIHANDLE* handle = ociHandle();
   if (handle != NULL && handle->phStmt != NULL) {
      //OCIStmtRelease(handle->phStmt, handle->phErr, NULL, 0, OCI_DEFAULT);  // oracle 12 oci
      //handle->phStmt = NULL; // guard against double "free"
   }
   return retVal;
}

OCICursor::~OCICursor() {
   Close();
   if (myDefArray) {
      for (int defVar = 0; defVar < myNumDefines; defVar++) {
         char* thisItem = myDefArray[defVar];
         delete[] thisItem;
      }
      delete[] myDefArray;
   }
   if (myIndicatorVector) {
      delete[] myIndicatorVector;
   }
}

OCIHANDLE* OCICursor::ociHandle() const {
   //std::cerr << "OCICursor::ociHandle(): returning: " << myOciHandle << std::endl;
   //std::cerr.flush();
   return myOciHandle;
}
