#ifndef _CPP_CONSTRUCTOR_H_
#define _CPP_CONSTRUCTOR_H_

#define NoOptimize __attribute__((optimize("O0")))

typedef void(*func_ptr)(void);

extern func_ptr __init_array_start, __init_array_end;
extern func_ptr __fini_array_start, __fini_array_end;
#pragma GCC push_options
#pragma GCC optimize ("O0")
extern "C"{
void _init(void) 
{
	for ( func_ptr* func = &__init_array_start; func != &__init_array_end; func++ )
		(*func)();
}
 
void _fini(void)
{
	for ( func_ptr* func = &__fini_array_start; func != &__fini_array_end; func++ )
		(*func)();
}
}
#pragma GCC pop_options 
#endif