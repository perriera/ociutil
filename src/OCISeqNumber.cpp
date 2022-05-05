// OCISeqNumber.cpp - Oracle unique number generation
// $Id: OCISeqNumber.cpp,v 1.4 1998/12/04 02:12:47 ed Exp $

#include <ociutil/OCISeqNumber.hpp>
#include <ociutil/OCICursor.hpp>
#include <ociutil/OCISISError.hpp>
#include <cstdlib>         // for strtol
#include <errno.h>

// TODO: looks like code in nextval() and currval() is almost the same - could reduce size with a helper method

/** A simple method to fetch the next number from a sequence on the database.
 * @param aOciHandleId the id for the desired database to query
 * @param aSeq the name of the sequence number generator in the database (could be an alias)
 * @return the desired sequence number or -1 if there was a failure (sequence numbers are always positive)
 */
YUNumber OCISeqNumber::nextval(const int aOciHandleId, const std::string& aSeq) {
   //OCISeqNumber theSeq(aSeq);
   std::string x = "Select " + aSeq + ".NEXTVAL FROM DUAL";
   OCICursor cur(aOciHandleId, x);
   OCISISError myStatus;
   myStatus = cur.Open();
   if (!myStatus.isSuccess())
      return -1L;
   myStatus = cur.Parse();
   if (!myStatus.isSuccess())
      return -1L;
   unsigned char tmp_value[10] = "";
   myStatus = cur.DefineString(1, tmp_value, sizeof(tmp_value));
   if (!myStatus.isSuccess())
      return -1L;
   myStatus = cur.Exec();
   if (!myStatus.isSuccess())
      return -1L;
   myStatus = cur.Fetch();
   if (!myStatus.isSuccess())
      return -1L;
   YUNumber retval(YUNumProps(8)); // we do have 10 digit sequence numbers, e.g. on stac
   //retval = atoi((char*)tmp_value); // atoi is deprecated
   errno = 0;
   retval = strtol((char*)tmp_value, NULL, 10);
   if ((errno == ERANGE && (retval == LONG_MAX || retval == LONG_MIN)) || (errno != 0 && retval == 0))
      return -1L;
   return retval;
}

/** A simple method to fetch the last number from a sequence on the database.
 * @param aOciHandleId the id for the desired database to query
 * @param aSeq the name of the sequence number generator in the database (could be an alias)
 * @return the desired sequence number or -1 if there was a failure (sequence numbers are always positive)
 */
YUNumber OCISeqNumber::currval(const int aOciHandleId, const std::string& aSeq) {
   std::string x = "Select " + aSeq + ".CURRVAL FROM DUAL";
   OCICursor cur(aOciHandleId, x);
   OCISISError myStatus;
   myStatus = cur.Open();
   if (!myStatus.isSuccess())
      return -1L;
   myStatus = cur.Parse();
   if (!myStatus.isSuccess())
      return -1L;
   unsigned char tmp_value[10] = "";
   myStatus = cur.DefineString(1, tmp_value, sizeof(tmp_value));
   if (!myStatus.isSuccess())
      return -1L;
   myStatus = cur.Exec();
   if (!myStatus.isSuccess())
      return -1L;
   myStatus = cur.Fetch();
   if (!myStatus.isSuccess())
      return -1L;
   YUNumber retval(YUNumProps(8)); // we do have 10 digit sequence numbers, e.g. on stac
   //retval = atoi((char*)tmp_value); // atoi is deprecated
   errno = 0;
   retval = strtol((char*)tmp_value, NULL, 10);
   if ((errno == ERANGE && (retval == LONG_MAX || retval == LONG_MIN)) || (errno != 0 && retval == 0))
      return -1L;
   return retval;
}

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 3
// tab-stop-list: (3 6 9 12 15 18 21 24 27 30 33 36 39 42 45 48 51 54 57 60 63 69)
// fill-column: 120
// End:
