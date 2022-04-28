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
#include <filesystem>

 // #
 // # CMakeLists.txt uses a GLOB_RECURSE macro which automatically includes ALL headers 
 // # and sources under include/ and src/. So, to keep two or more main() declarations 
 // # seperate, we place any sources that have a main() in the instances/ folder
 // #

int main(int, const char**)
{
  std::cout << "Hello, world" << std::endl;
  return 0;
}
