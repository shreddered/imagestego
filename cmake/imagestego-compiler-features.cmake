# CMake module for detecting compiler features

include(CheckIncludeFiles)
include(CheckCXXSourceCompiles)
include(CheckCSourceCompiles)

# intrin.h header
if (MSVC)
  check_include_files("intrin.h" HAVE_INTRIN_H)
endif()

# inline check
if (MSVC)
  set(INLINE_OPTS "__inline;inline")
else()
  set(INLINE_OPTS "__inline__;inline")
endif()
foreach(inline ${INLINE_OPTS})
  check_c_source_compiles("${inline} static int foo() { return 0; } int main() { return foo(); }" HAVE_INLINE)
  if (HAVE_INLINE)
    set(INLINE ${inline})
    break()
  endif()
endforeach()
if (NOT HAVE_INLINE)
  message(WARNING "Can't inline functions")
endif()

# cxx-constexpr
check_cxx_source_compiles("constexpr int foo(int a) { return a + 42; } int main() { return foo(-42); }" HAVE_CXX_CONSTEXPR)
