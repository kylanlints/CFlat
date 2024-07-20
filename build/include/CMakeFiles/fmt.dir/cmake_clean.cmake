file(REMOVE_RECURSE
  "libfmt.a"
  "libfmt.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/fmt.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
