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
#include <ociutil/interfaces/IOCIACLMap.hpp>

#include "../vendor/catch.hpp"
#include "../vendor/fakeit.hpp"

using namespace util;
using namespace fakeit;

SCENARIO("Mock IOCIACLMap: lookup", "[YSTLCMP-7]") {

    Mock<oci::OCIACLMapInterface> mock;
    When(Method(mock, lookup)).Return();
    When(Method(mock, lookup))
        .AlwaysDo([](const std::string& key) {
        return nullptr;
            });

    oci::OCIACLMapInterface& i = mock.get();
    REQUIRE(i.lookup("test") == nullptr);
    Verify(Method(mock, lookup));
}