/**
 * @file OCIACLMapInterface.hpp
 * @author Perry Anderson (perryand@yorku.ca)
 * @brief OCIACLMapInterface file
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright (C) May 10, 2022 York University
 *
 */

#ifndef _OCIACLMAP_H_INTERFACE
#define _OCIACLMAP_H_INTERFACE

#include <iostream>
#include <extras/interfaces.hpp>
#include <sisutil/ACLRep.hpp>

namespace util {
    namespace oci {


        /**
         * @brief OCIACLMapInterface
         *
         */
        interface OCIACLMapInterface
        {

            /**
             * @brief moves()
             * @return all the chess moves of the given chess game
             */
            virtual ACL* lookup(const std::string& key) const pure;

        };

        /**
         * @brief ChessGame
         *
         */
        concrete class OCIACLMap implements OCIACLMapInterface
        {

            /**
             * @brief moves()
             * @return all the chess moves of the given chess game
             */
            virtual ACL* lookup(const std::string& key) const pure;

        };

        /**
         * @brief ItemNotFoundException
         *
         */

        concrete class ItemNotFoundException
            extends extras::AbstractCustomException {
        public:
            ItemNotFoundException(
                const std::string& msg,
                const extras::WhereAmI& whereAmI)
                : AbstractCustomException(msg.c_str(),
                    whereAmI._file.c_str(),
                    whereAmI._func.c_str(), whereAmI._line) {}
            static void assertion(
                int sizePGN, int sizeFEN,
                const std::string& msg,
                const extras::WhereAmI& ref);
        };

    } // end namespace 
}

#endif // _OCIACLMAP_H_INTERFACE
