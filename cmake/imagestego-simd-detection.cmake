# CMake module for processor extensions detection
# Defines compiler options for CPU extensions listed below:
# x86/x86_64: SSE, SSE2, SSE3, AVX, AVX2, AVX512, etc
# aarch64/arm: NEON
#
# Compiler options: CPU_${opt}_FLAGS
# Feature support: CPU_${opt}_SUPPORTED

message(STATUS "Detecting processor extensions")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/imagestego-cxx-compiler.cmake")

include(CheckCXXSourceRuns)

# function wrapper around try_run()
macro(imagestego_detect_simd_support OPT)
  file(READ "${CPU_${OPT}_CHECK_FILE}" CHECK_FILE)
  set(CMAKE_REQUIRED_FLAGS "${CPU_${OPT}_FLAGS}")
  check_cxx_source_runs("${CHECK_FILE}" CPU_${OPT}_SUPPORTED)
  if (CPU_${OPT}_SUPPORTED)
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " ${CPU_${OPT}_FLAGS}")
    string(APPEND CMAKE_C_FLAGS_RELEASE " ${CPU_${OPT}_FLAGS}")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " ${CPU_${OPT}_FLAGS}")
    string(APPEND CMAKE_C_FLAGS_DEBUG " ${CPU_${OPT}_FLAGS}")
  endif()
endmacro()

if (X86 OR X86_64)
  set(CPU_SIMD_EXTENSIONS "SSE;SSE2;SSE3;SSE4_1;SSE4_1;SSE4_2;AVX;AVX2;AVX512")

  # SSE checks
  set(CPU_SSE_CHECK_FILE      "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_sse.cpp")
  set(CPU_SSE2_CHECK_FILE     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_sse2.cpp")
  set(CPU_SSSE3_CHECK_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_ssse3.cpp")
  set(CPU_SSE4_1_CHECK_FILE   "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_sse4_1.cpp")
  set(CPU_SSE4_2_CHECK_FILE   "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_sse4_2.cpp")
  # AVX checks
  set(CPU_AVX_CHECK_FILE      "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_avx.cpp")
  set(CPU_AVX2_CHECK_FILE     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_avx2.cpp")
  set(CPU_AVX512VL_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_avx512vl.cpp")
  set(CPU_AVX512BW_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_avx512bw.cpp")

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
      imagestego_intel_cpu_flags(SSE2   "-msse2" "/arch:SSE2")
    endif()
    imagestego_intel_cpu_flags(SSE3   "-msse3" "/arch:SSE3")
    imagestego_intel_cpu_flags(SSE4_1 "-msse4.1" "/arch:SSE4.1")
    imagestego_intel_cpu_flags(SSE4_2 "-msse4.2" "/arch:SSE4.2")
    # AVX flags
    imagestego_intel_cpu_flags(AVX      "-mavx" "/arch:AVX")
    imagestego_intel_cpu_flags(AVX2     "-mavx2" "/arch:AVX2")
    # AVX512 flags
    imagestego_intel_cpu_flags(AVX512BW "-mavx512bw" "/arch:AVX512")
    imagestego_intel_cpu_flags(AVX512VL "-mavx512vl" "/arch:AVX512")
  elseif (IMAGESTEGO_GCC OR IMAGESTEGO_CLANG)
    # SSE flags
    set(CPU_SSE_FLAGS    "-msse")
    set(CPU_SSE2_FLAGS   "-msse2")
    set(CPU_SSSE3_FLAGS  "-mssse3")
    set(CPU_SSE4_1_FLAGS "-msse4.1")
    set(CPU_SSE4_2_FLAGS "-msse4.2")
    # AVX flags
    set(CPU_AVX_FLAGS  "-mavx")
    set(CPU_AVX2_FLAGS "-mavx2")
    # AVX512 flags
    set(CPU_AVX512BW_FLAGS "-mavx512bw")
    set(CPU_AVX512VL_FLAGS "-mavx512vl")
  elseif(MSVC)
    set(CPU_SSE2_FLAGS "/arch:SSE2")
    set(CPU_AVX2_FLAGS "/arch:AVX2")
    set(CPU_AVX512BW_FLAGS "/arch:AVX512")
    set(CPU_AVX512VL_FLAGS "/arch:AVX512")
  endif()
  if (NOT FORCE_SSE)
    imagestego_detect_simd_support(AVX512BW)
    imagestego_detect_simd_support(AVX512VL)
    imagestego_detect_simd_support(AVX2)
  endif()
  if (NOT CPU_AVX2_SUPPORTED)
    imagestego_detect_simd_support(SSE2)
    imagestego_detect_simd_support(SSSE3)
  endif()

elseif (ARM OR AARCH64)
  set(CPU_NEON_CHECK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpu_neon.cpp")
  set(CPU_NEON_FLAGS "")
  imagestego_detect_simd_support(NEON)
elseif (PPC64 OR PPC64LE)
  # TODO: implement IBM POWER support check
else()
  message(STATUS "Unsupported architecture: no extensions available")
endif()
