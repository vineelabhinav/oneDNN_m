# CMake generated Testfile for 
# Source directory: /home/vineel/tf_onednn/oneDNN/tests/gtests/graph/unit
# Build directory: /home/vineel/tf_onednn/oneDNN/build/tests/gtests/graph/unit
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_graph_unit_cpu "test_graph_unit" "--engine=cpu")
set_tests_properties(test_graph_unit_cpu PROPERTIES  _BACKTRACE_TRIPLES "/home/vineel/tf_onednn/oneDNN/tests/gtests/graph/unit/CMakeLists.txt;69;add_test;/home/vineel/tf_onednn/oneDNN/tests/gtests/graph/unit/CMakeLists.txt;0;")
subdirs("interface")
subdirs("backend")
subdirs("utils")
