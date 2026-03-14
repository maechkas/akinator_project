# CMake generated Testfile for 
# Source directory: /Users/soonserv/CLionProjects/akinator_project
# Build directory: /Users/soonserv/CLionProjects/akinator_project/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[akinator_tests]=] "/Users/soonserv/CLionProjects/akinator_project/build/akinator_tests")
set_tests_properties([=[akinator_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/soonserv/CLionProjects/akinator_project/CMakeLists.txt;54;add_test;/Users/soonserv/CLionProjects/akinator_project/CMakeLists.txt;0;")
subdirs("_deps/json-build")
subdirs("_deps/googletest-build")
