#include <tmmintrin.h>

int main() {
    __m128i a, b;
    auto c = _mm_hadd_epi16(a, b);
    return 0;
}
