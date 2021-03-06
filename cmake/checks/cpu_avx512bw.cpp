#include <cstdint>
#include <immintrin.h>

int main() {
    int16_t arr[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                     0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
    __m512i a = _mm512_srai_epi16(_mm512_loadu_si512(arr), 1);
    _mm512_storeu_si512(arr, a);
    return 0;
}
