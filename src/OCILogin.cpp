// OCILogin.cpp
// $Id: OCILogin.cpp,v 1.19 2000/06/20 20:14:34 ed Exp $

#include <cstring>
#include "OCILogin.hpp"
#include <ErrLog.hpp>

static void checkerr(OCIError *p_err, sword status);

OCILogin::OCILogin(const std::string& userName, const std::string& password)
   : myUserName(userName), myPassword(password), myIsValid(false) {
   invalidate();
   initialize();
}

OCISISError OCILogin::open() {
   //std::cerr << "OCILogin::open() ==>" << std::endl;
   OCISISError rc;
   size_t found             = myUserName.find("@");
   std::string tempUserName = myUserName.substr(0, found);
   std::string tempSid      = myUserName.substr(found + 1);

   // TODO: OCILogon has been replaced by OCILogon2
   /*
   if ( OCILogon(myOciHandle.phEnv, myOciHandle.phErr, &myOciHandle.phService,
       (unsigned char *)(text*)tempUserName.c_str(), tempUserName.size(),
       (unsigned char *)(text*)myPassword.c_str(), myPassword.size(),
       (unsigned char *)(text*)tempSid.c_str(), tempSid.size()) )
      rc = OCISISError("OCILogin::open", "OCILogin", &myOciHandle);
      */
   if ( OCILogon2(myOciHandle.phEnv,
                  myOciHandle.phErr,
                  &myOciHandle.phService,
                  (unsigned char *)(text*)tempUserName.c_str(),
                  tempUserName.size(),
                  (unsigned char *)(text*)myPassword.c_str(),
                  myPassword.size(),
                  (unsigned char *)(text*)tempSid.c_str(),
                  tempSid.size(),
                  OCI_DEFAULT)
   )
      rc = OCISISError("OCILogin::open", "OCILogin", &myOciHandle);
   else
      myIsValid = true;
   return rc;
}

OCISISError OCILogin::Commit() {
   OCISISError rc;
   if (OCITransCommit(myOciHandle.phService, myOciHandle.phErr, 0))
      rc = OCISISError("OCILogin::Commit", "OCITransCommit", &myOciHandle);
   return rc;
}

OCISISError OCILogin::Rollback() {
   OCISISError rc;
   if (OCITransRollback(myOciHandle.phService, myOciHandle.phErr, OCI_DEFAULT))
      rc = OCISISError("OCILogin::Rollback", "Rollback", &myOciHandle);
   return rc;
}

OCILogin::~OCILogin() {
  (void) Rollback();            // avoid auto-commit
  if (OCILogoff(myOciHandle.phService, myOciHandle.phErr)) {
    OCISISError oe("OCILogin::~OCILogin", "OCILogoff", &myOciHandle);
    if (!oe.isSuccess()) 
      getTheErrLog().putLine("OCILogin::~OCILogin", "OCILogoff", "fails");
  }
  invalidate();
}

void OCILogin::invalidate() {
   // don't do ologof else risk auto-commit
   myIsValid = false;
   // initialize ocihandle to all zeroes
   memset(&myOciHandle, 0, sizeof(OCIHANDLE));
}

void OCILogin::initialize() {
	sword rc;

	// TODO: OCIInitialize and OCIEnvInit is deprecated - needs to be replaced by OCIEnvCreate() or OCIEnvNlsCreate()
	/*
	rc = OCIInitialize((ub4) OCI_DEFAULT, (dvoid *)0,
	          (dvoid * (*)(dvoid *, size_t)) 0,
	          (dvoid * (*)(dvoid *, dvoid *, size_t))0,
	          (void (*)(dvoid *, dvoid *)) 0 );
	checkerr(myOciHandle.phErr, rc);

	rc = OCIEnvInit(&(myOciHandle.phEnv), (ub4)OCI_DEFAULT, (size_t)0, (dvoid **)0);
	checkerr(myOciHandle.phErr, rc);
   */

	rc = OCIEnvCreate(&(myOciHandle.phEnv),  //OCIEnv     **envhpp,
	                  OCI_DEFAULT,           //ub4       mode,
	                  myOciHandle.phService, //const void *ctxp,
	                  NULL,                  //const void *(*malocfp)(void *ctxp, size_t size),
	                  NULL,                  //const void *(*ralocfp)(void *ctxp, void *memptr, size_t newsize),
	                  NULL,                  //const void  (*mfreefp)(void *ctxp, void *memptr),
	                  0,                     //size_t xtramemsz,
	                  NULL);                 //void **usrmempp
   checkerr(myOciHandle.phErr, rc);

   rc = OCIHandleAlloc(myOciHandle.phEnv, (void**)&(myOciHandle.phErr),     OCI_HTYPE_ERROR,  (size_t)0, (dvoid **)0);
   checkerr(myOciHandle.phErr, rc);

	rc = OCIHandleAlloc(myOciHandle.phEnv, (void**)&(myOciHandle.phService), OCI_HTYPE_SVCCTX, (size_t)0, (dvoid **)0);
	checkerr(myOciHandle.phErr, rc);

	rc = OCIHandleAlloc(myOciHandle.phEnv, (void**)&(myOciHandle.phStmt),    OCI_HTYPE_STMT,   (size_t)0, (dvoid **)0);
	checkerr(myOciHandle.phErr, rc);
}

static void checkerr(OCIError *errhp, sword status)
{
  text errbuf[512];
  ub4 errcode;
  switch (status) {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
    printf("Error - OCI_SUCCESS_WITH_INFO\n");
    break;
  case OCI_NEED_DATA:
    printf("Error - OCI_NEED_DATA\n");
    break;
  case OCI_NO_DATA:
    printf("Error - OCI_NO_DATA\n");
    break;
  case OCI_ERROR:
    OCIErrorGet( (dvoid *)errhp, (ub4)1, (text *)NULL, (int *)&errcode, errbuf,
		           (ub4)sizeof(errbuf), (ub4)OCI_HTYPE_ERROR);
    printf("Error - %s\n", errbuf);
    break;
  case OCI_INVALID_HANDLE:
    printf("Error - OCI_INVALID_HANDLE\n");
    break;
  case OCI_STILL_EXECUTING:
    printf("Error - OCI_STILL_EXECUTE\n");
    break;
  case OCI_CONTINUE:
    printf("Error - OCI_CONTINUE\n");
    break;
  default:
    break;
  }
}
