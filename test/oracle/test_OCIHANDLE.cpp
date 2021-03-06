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
#include <occi.h>
#include <oci.h>
#include <ociutil/ocihandle.hpp>
#include <bits/c++config.h>
#include <ociutil/game/ChessGame.hpp>
#include <pthread.h>

#include "../vendor/catch.hpp"

using namespace oracle::occi;
using namespace std;
using namespace util;


SCENARIO("Verify 2Oracle Instant Client SDK can be compiled", "[OracleSDK]")
{
    oci::ChessGame game;
    oci::ChessGameInterface& i = game;
    i.moves();
    REQUIRE(true);
}

