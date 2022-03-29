file(REMOVE_RECURSE
  "libapla.pdb"
  "libapla.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/apla.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
