#include <emmintrin.h>
#include <cstdint>

int main() {
    uint32_t buf[4] = {1, 2, 3, 4};
    const auto vec = _mm_loadu_si128((const __m128i*) buf);
    const auto res = _mm_shuffle_epi32(vec, 5);
    _mm_storeu_si128((__m128i*) buf, res);
    return 0;
}
