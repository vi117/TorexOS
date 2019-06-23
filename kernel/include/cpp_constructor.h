#ifndef _CPP_CONSTRUCTOR_H_
#define _CPP_CONSTRUCTOR_H_

typedef void(*func_ptr)(void);

extern func_ptr __init_array_start, __init_array_end;
extern func_ptr __fini_array_start, __fini_array_end;
 
void call_constructor(void)
{
	for ( func_ptr* func = &__init_array_start; func != &__init_array_start; func++ )
		(*func)();
}
 
void call_destructor(void)
{
	for ( func_ptr* func = &__fini_array_end; func != &__fini_array_end; func++ )
		(*func)();
}
#endif