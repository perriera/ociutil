/**
 * @brief the "MIT/X Consortium License", (adapted for EXPARX.COM)
 *
 * Copyright (C) November 22, 2021 EXPARX INCORPORATED
 *
 * Permission is hereby  granted,  free of charge,  to  any  person
 * obtaining a copy of this software and  associated  documentation
 * files   (the  "Software"),  to deal  in   the  Software  without
 * restriction, including  without  limitation the rights  to  use,
 * copy,  modify, merge,  publish,  distribute,  sublicense, and/or
 * sell copies of the  Software, and to permit persons  to whom the
 * Software  is  furnished to  do  so,  subject  to  the  following
 * conditions:
 *
 * (See LICENSE.md for complete details)
 *
 */

#include <iostream>
 // #include <oracle/19.14/client64/occi.h>
#include <occi.h>
#include <bits/c++config.h>
#include <extras_oci/game/ChessGame.hpp>
#include <pthread.h>

#include "../vendor/catch.hpp"

using namespace oracle::occi;
using namespace std;
using namespace extras;

class  occidml
{
private:

    Environment* env = nullptr;
    Connection* conn = nullptr;
    Statement* stmt = nullptr;
public:

    occidml(string user, string passwd, string db)
    {
        env = Environment::createEnvironment(Environment::DEFAULT);
        // conn = env->createConnection(user, passwd, db);
    }

    ~occidml()
    {
        // env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }
};

SCENARIO("Verify Oracle Instant Client SDK can be compiled", "[OracleSDK]")
{
    oci::ChessGame game;
    oci::ChessGameInterface& i = game;
    i.moves();
    REQUIRE(true);
}

SCENARIO("Verify occidml", "[OracleSDK]")
{
    string user = "scott";
    string passwd = "tiger";
    string db = "";
    try {
        cout << "occidml - Exhibiting simple insert, delete & update operations"
            << endl;
        occidml demo(user, passwd, db);
        cout << "Displaying all records before any operation" << endl;
    }
    catch (SQLException& ex) {
        cout << ex.getMessage() << endl;
    }
}
