#ifndef __COMMON_H
#define __COMMON_H

// more descriptive identifiers for the static keyword
// _CL_INTERNAL: applied to a function to declare it is local to the translation unit
// _CL_LOCAL_PERSIST: applied to a variable to declare it as a global variable but locally scoped
#ifdef CL_STATIC_TYPEDEFS
	#define _CL_INTERNAL static
	#define _CL_LOCAL_PERSIST static
#else
	#define _CL_INTERNAL
	#define _CL_LOCAL_PERSIST
#endif

// way to avoid compiler warnings for if variables are not used
// in principle this itself shouldn't be used, but when debugging it's helpful
#ifndef UNUSED
	#define UNUSED(x) ((void)(x))
#endif


// i don't like typing capitals for NULL
#ifndef null
	#define null NULL
#endif

#endif // __COMMON_H