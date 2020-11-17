#include <x86intrin.h>

#include <cstddef>

int main() {
    int32_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    __m256i a = _mm256_loadu_si256((const __m256i*) data);
    __m256i b = _mm256_srai_epi32(a, 1);
    return 0;
}
