/**
* @file version.hpp
* @author Matt Williams (mattltf@protonmail.com)
* @brief Adds version support for Cmake script
* @version 3.2.0
* @date 2021-08-08
*
* @copyright (C) August 8, 2021 Matt Williams
*
*/

#ifndef _EXTRAS_OCI_VERSION_HPP
#define _EXTRAS_OCI_VERSION_HPP

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

#define EXTRAS_OCI_VER_MAJOR 1
#define EXTRAS_OCI_VER_MINOR 0
#define EXTRAS_OCI_VER_PATCH 1

#define EXTRAS_OCI_VERSION \
  (EXTRAS_OCI_VER_MAJOR * 10000 + EXTRAS_OCI_VER_MINOR * 100 + EXTRAS_OCI_VER_PATCH)

#endif// _EXTRAS_OCI_VERSION_HPP
