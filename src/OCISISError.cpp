// OCISISError.cpp -  Oracle error messages
// $Id: OCISISError.cpp,v 1.30 1999/07/30 12:05:42 ed Exp $

#include <string>
#include <cstring>
#include <ociutil/OCISISError.hpp>
#include <ociutil/OracleServer.hpp>			// for isTraceDebug()
#include <sisutil/ErrLog.hpp>
#include <cstdlib>

OCISISError::OCISISError(const std::string& who, const  std::string& w, const OCIHANDLE* hOCI)
	: Error((Error::ErrorStatus)-1) {
	int rc = -1;
	unsigned char tmp[1024]; memset((void*)tmp, 0, (unsigned long int)sizeof(tmp));
	ub4 errcode = 0;
	ErrorStatusValues = OK;

	if (hOCI != 0) {
		OCIErrorGet((dvoid*)(hOCI->phErr),
			(ub4)1,
			(unsigned char*)NULL,
			(int*)&errcode,
			tmp,
			(ub4)sizeof(tmp),
			(ub4)OCI_HTYPE_ERROR);
		if (getenv("SIS_OCI_DEBUG")) std::cerr << "OCISISError::OCISISError() errcode = " << errcode << std::endl;
		// get error code
		rc = errcode;

		// ignore 1406
		if (rc == OCISISError::TRUNCATED) {
			rc = OCISISError::OK;
			if (getenv("SIS_OCI_DEBUG"))
				std::cerr << "OCISISError::OCISISError() 'ORA-01406: fetched column value was truncated.'" << std::endl;
			//if (getenv("SIS_OCI_DEBUG"))
			//	std::cerr << "OCISISError::OCISISError: errcode was converted from =" << errcode << " to " << rc << std::endl;
		}
		// ignore 24347
		else if (rc == OCISISError::NULL_IN_AGGREGATE) {
			rc = OCISISError::OK;
			if (getenv("SIS_OCI_DEBUG"))
				std::cerr << "OCISISError::OCISISError() 'ORA-24347: Warning of a NULL column in an aggregate function.'" << std::endl;
		}
	}
	int i = strlen((const char*)tmp);
	if (i && tmp[i - 1] == '\n')
		tmp[i - 1] = '\0';

	if (rc != -1)
		myText = (const char*)tmp;
	else
		myText = who + " - " + w;

	// This line doesn't make much sense: OCISISErrorERROR_BASE is defined as 0 so rc will always be rc ???
	myErrorStatus = rc ? OCISISErrorERROR_BASE + rc : rc; // leave 0 as 0

	if (getenv("SIS_OCI_DEBUG"))
		std::cerr << "OCISISError::OCISISError() myErrorStatus =" << myErrorStatus << std::endl;

	mySeverity = Error::SEV_UNSPECIFIED;
	if (myErrorStatus && ((myErrorStatus != OCISISError::NOT_FOUND) || OracleServer::isTraceDebug()))
		getTheErrLog().putLine("OCISISError", "OCISISError", std::string(" \"" + text() + "\""));
	//std::cerr << "OCISISError::OCISISError: --------" <<  text() << std::endl;
}

// TODO: get rid of this. The dummy parameter is used only to give it a different signature
//       otherwise it's just the first c'tor.
OCISISError::OCISISError(const std::string& who, const std::string& w, const OCIHANDLE*, const OCIHANDLE* hOCI)
	: Error((Error::ErrorStatus)-1) {
	ErrorStatusValues = OK;
	OCISISError(who, w, hOCI);
}

Error* OCISISError::clone()  const { return new OCISISError(*this); }

const std::string OCISISError::text() const { return myText; }
