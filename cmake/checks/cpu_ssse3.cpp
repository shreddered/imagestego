#include <cstdint>
#include <tmmintrin.h>

int main() {
    uint32_t buf[4] = {1, 2, 3, 4};
    const auto vec = _mm_loadu_si128((const __m128i*) buf);
    auto c = _mm_hadd_epi16(vec, vec);
    return 0;
}
