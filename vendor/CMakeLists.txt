add_subdirectory(mem)
add_subdirectory(fmt)

set(TBS_MT OFF)
set(TBS_USE_ARCH_WORD_SIMD ON)
set(TBS_USE_SSE2 ON)
set(TBS_USE_AVX ON)

add_subdirectory(TBS)
