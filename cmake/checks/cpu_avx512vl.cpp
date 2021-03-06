#include <cstdint>
#include <immintrin.h>

int main() {
    int16_t arr[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                     0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
    __m256i a = _mm256_maskz_loadu_epi16(32, arr);
    _mm256_storeu_si256((__m256i*) arr, a);
    return 0;
}
