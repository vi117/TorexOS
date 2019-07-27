
#define __GET_CALLER(x) __builtin_return_address(0)
#define __GET_FRAME(x)  __builtin_frame_address(0)

#define unlikely(x) __builtin_expect((x), 0)
#define likely(x) __builtin_expect((x), 1)

#define force_inline inline __attribute__((always_inline))