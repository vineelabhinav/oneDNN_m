file(REMOVE_RECURSE
  ".3"
  "libdnnl.pdb"
  "libdnnl.so"
  "libdnnl.so.3"
  "libdnnl.so.3.2"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/dnnl.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
