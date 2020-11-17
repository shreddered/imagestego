#ifndef __ARM_NEON__
#   error "No ARM NEON available"
#endif

#include <arm_neon.h>

int main() {
    int16x8_t a, b;
    auto c = vhaddq_s16(a, b);
    return 0;
}
