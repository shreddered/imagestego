# CMake module for processor extensions detection
# Defines compiler options for CPU extensions listed below:
# x86/x86_64: SSE, SSE2, SSE3, AVX, AVX2, AVX512, etc
# aarch64/arm: NEON
#
# Compiler options: CPU_${opt}_FLAGS
# Feature support: CPU_${opt}_SUPPORTED

message(STATUS "Detecting processor extensions")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/imagestego-cxx-compiler.cmake")

# macro wrapper around try_run()
macro(imagestego_detect_simd_support OPT)
  message(STATUS "Detecting ${OPT} support")
  string(APPEND CMAKE_CXX_FLAGS " ${CPU_${OPT}_FLAGS}")
  try_run(CPU_${OPT}_SUPPORTED CPU_${OPT}_COMPILES
    ${CMAKE_CURRENT_BINARY_DIR}/simd/
    ${CPU_${OPT}_CHECK_FILE}
    COMPILE_OUTPUT_VARIABLE ${OPT}_OUT
  )
  if (NOT CPU_${OPT}_COMPILES OR NOT CPU_${OPT}_SUPPORTED EQUAL 0)
    message(STATUS "${OPT} extension is not supported")
    if (NOT CPU_${OPT}_COMPILES)
      message(STATUS "${OPT} test file cannot be compiled")
      message(STATUS "${${OPT}_OUT}")
    endif()
    set(CPU_${OPT}_SUPPORTED FALSE)
    string(REPLACE "${CPU_${OPT}_FLAGS}" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  else()
    message(STATUS "${OPT} extension is supported")
    string(APPEND CMAKE_C_FLAGS " ${CPU_${OPT}_FLAGS}")
    set(CPU_${OPT}_SUPPORTED TRUE)
  endif()
endmacro()

# TODO: write AVX512 checks
if (X86 OR X86_64)
  set(CPU_SIMD_EXTENSIONS "SSE;SSE2;SSE3;SSE4_1;SSE4_1;SSE4_2;AVX;AVX2;AVX512")

  # SSE checks
  set(CPU_SSE_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_sse.cpp")
  set(CPU_SSE2_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_sse2.cpp")
  set(CPU_SSSE3_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_ssse3.cpp")
  set(CPU_SSE4_1_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_sse4_1.cpp")
  set(CPU_SSE4_2_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_sse4_2.cpp")
  # AVX checks
  set(CPU_AVX_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_avx.cpp")
  set(CPU_AVX2_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_avx2.cpp")

  if (IMAGESTEGO_ICC)
    macro(imagestego_intel_cpu_flags OPT unix_flags msvc_flags)
      if (MSVC)
        set(CPU_${OPT}_FLAGS ${msvc_flags})
      else()
        set(CPU_${OPT}_FLAGS ${unix_flags})
      endif()
    endmacro()

    # SSE flags
    if (NOT X86_64)
      imagestego_intel_cpu_flags(SSE "-msse" "/arch:SSE")
    endif()
    imagestego_intel_cpu_flags(SSE2 "-msse2" "/arch:SSE2")
    imagestego_intel_cpu_flags(SSE3 "-msse3" "/arch:SSE3")
    imagestego_intel_cpu_flags(SSE4_1 "-msse4.1" "/arch:SSE4.1")
    imagestego_intel_cpu_flags(SSE4_2 "-msse4.2" "/arch:SSE4.2")
    # AVX flags
    imagestego_intel_cpu_flags(AVX "-mavx" "/arch:AVX")
    imagestego_intel_cpu_flags(AVX2 "-mavx2" "/arch:AVX2")
  elseif (IMAGESTEGO_GCC OR IMAGESTEGO_CLANG)
    # SSE flags
    set(CPU_SSE_FLAGS "-msse")
    set(CPU_SSE2_FLAGS "-msse2")
    set(CPU_SSSE3_FLAGS "-mssse3")
    set(CPU_SSE4_1_FLAGS "-msse4.1")
    set(CPU_SSE4_2_FLAGS "-msse4.2")
    # AVX flags
    set(CPU_AVX_FLAGS "-mavx")
    set(CPU_AVX2_FLAGS "-mavx2")
  elseif(MSVC)
    set(CPU_SSE2_FLAGS "/arch:SSE2")
    set(CPU_AVX2_FLAGS "/arch:AVX2")
  endif()
  imagestego_detect_simd_support(AVX2)
  if (NOT CPU_AVX2_SUPPORTED)
    imagestego_detect_simd_support(SSE2)
    imagestego_detect_simd_support(SSSE3)
  endif()

elseif (ARM OR AARCH64)
  set(CPU_NEON_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_neon.cpp")
  set(CPU_NEON_FLAGS "-mfpu=neon")
  imagestego_detect_simd_support(NEON)
elseif (PPC64 OR PPC64LE)
  # TODO: implement IBM POWER support check
else()
  message(STATUS "Unsupported architecture: no extensions available")
endif()
