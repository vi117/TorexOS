#include<immintrin.h>

inline uint64_t ReadTimeStampCounter(){
    return __rdtsc();
}