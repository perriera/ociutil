// OCIACLMap.cpp
// $Id: OCIACLMap.cpp,v 1.9 1999/01/14 22:20:23 ed Exp $

#include <OCIACLMap.hpp>
#include <ACL.hpp>
#include <OCICursor.hpp>
#include <OCIInfo.hpp>
#include <YUString.hpp>
#include <YUUuid.hpp>
#include <ErrLog.hpp>

//OCIACLMap::OCIACLMap() {}

/*  -------------- This section is for HTTP based servers ---------------- */

const char* OCIACLMap::SQLlookup  = "SELECT GROUPUUIDNAME FROM V222.SIS_ACCESS_CONTROL_LIST WHERE RPCNAME = :1";

OCIVariableInfo OCIACLMap::lookupMarshInVector[] =  {
   /* RPCName                   */  {0, 128, 'S'}
   /*                           */ ,{0,   0, }
};

OCIVariableInfo OCIACLMap::lookupMarshOutVector[] = {
	/* GroupUUIDName             */  {0, sizeof(uuid_t), 'R'}
	/*                           */ ,{0,              0, ' '}
};


ACL* OCIACLMap::lookup(const std::string& theRPCName) const {
	ACL*        theACL = new ACL;
	OCIMarshall inMS(OCIACLMap::RPCNAMEMAXLEN + 1);
	OCIMarshall outMS(sizeof(uuid_t));
	YUString    rpcName(theRPCName);

	inMS << rpcName;
	OCICursor theCursor(0L, OCIACLMap::SQLlookup,
						lookupMarshInVector, lookupMarshOutVector,
						(int)READARRAYSIZE);
	OCISISError status = theCursor.Execute(inMS, outMS);
	while (status.isSuccess()) {
		status = theCursor.Fetch();
		if (status.isSuccess()) {
			YUUuid thisGroupYUUuid;
			outMS >> thisGroupYUUuid;
			theACL->addGroup(thisGroupYUUuid);
			outMS.clear();
		}
	}

	if (status.value() == OCISISError::NOT_FOUND)
		status = OCISISError();
	else if (!status.isSuccess()) {
		delete theACL;
		theACL = NULL;
	}
	return theACL;
}

/*  -------------- This section is for HTTP based servers ---------------- */

// This code is used to see if a user has access to a given rpc
// Returns false if the RPC is not in their groups and true if there is at least one group with the rpc.
const char* OCIACLMap::SQLLookupACL = "select case when count(sux.principalname) > 0 then 'Y' when count(sux.principalname) = 0 then 'N' end from v222.sis_principal_group_xref spgx, v222.sis_uuid_xref sux, v222.sis_access_control_list sacl where spgx.uuid = sux.uuid and sux.principalname like :1 and sacl.rpcname = :2 and sacl.groupuuidname=spgx.groupuuid";

OCIVariableInfo OCIACLMap::lookupACLREF[] =  {
  /* PrincipalName             */  { 0,  70, 'S'}
  /* RPCName                   */ ,{71, 128, 'S'}
  /*                           */ ,{ 0,   0, ' '}
};

OCIVariableInfo OCIACLMap::lookupACLResult[] = {
  /* N or Y returned           */  {0, 1, 'S'}  // fixed size
  /*                           */ ,{0, 0, ' '}
};

int OCIACLMap::lookupByPrincipal(const std::string& principalName, const std::string& rpcName) {
   //std::cerr << "OCIACLMap::lookupByPrincipal entered with principalName = " << principalName << ", rpcName = " << rpcName << std::endl;

   OCIMarshall aMS(70 + OCIACLMap::RPCNAMEMAXLEN + 1);
   OCIMarshall aMSRES(1);
   std::string pName = principalName + "%";

   // values in the struct above are max values - they are adjusted here for actual values
   lookupACLREF[0].length = pName.length();
   lookupACLREF[1].startLocation = pName.length() + 1;

   aMS << pName.c_str() << rpcName.c_str();
   OCICursor* pC = new OCICursor(9999, SQLLookupACL, lookupACLREF, lookupACLResult, 0);
   if (!pC) {
	   return false;
   }
	OCISISError myOCIStatus = pC->Execute(aMS, aMSRES);
	if (myOCIStatus.isSuccess()) {
	   //std::cerr << "OCIACLMap::lookupByPrincipal() created the cursor" << ", isSuccess(): " << myOCIStatus.isSuccess() << std::endl;

      myOCIStatus = pC->Fetch();
		//std::cerr << "OCIACLMap::lookupByPrincipal() fetching the data, result: " << myOCIStatus << ", isSuccess(): " << myOCIStatus.isSuccess() << std::endl;
      if (myOCIStatus.isSuccess()) {
	      YUString val = "Y";            // initialize so it can be marshalled with real data
	      aMSRES >> val;                 // initialize with data from the fetch
	      //std::cerr << "OCIACLMap::lookupByPrincipal() fetched the data, val=" << val << std::endl;
         if (val == "N") {              // the user does NOT have permission
			   delete pC;
		      return false;
	      }
      }
   }
   delete pC;
   return true;
}
