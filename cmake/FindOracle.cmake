#
# How to add third party library support using cmake
# 
# https://github.com/cpm-cmake/CPM.cmake/tree/master/examples
# https://github.com/mattcoding4days/cmake-starter
#

set(ORACLE_HOME $ENV{ORACLE_HOME})
set(ORACLE_INCLUDE "${ORACLE_HOME}/include")
set(ORACLE_LIB "${ORACLE_HOME}/lib")
set(ORACLE_BIN "${ORACLE_HOME}/bin")
link_directories(BEFORE "${ORACLE_LIB}" )
