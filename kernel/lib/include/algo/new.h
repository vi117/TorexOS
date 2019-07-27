#pragma once
#include <stddef.h>

#ifndef _NEW

inline void *operator new([[maybe_unused]] size_t, void *ptr) noexcept { return ptr; }
inline void *operator new[]([[maybe_unused]] size_t, void *ptr) noexcept { return ptr; }

#endif