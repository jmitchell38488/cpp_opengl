#ifndef __G_CORE_DEFINES_
#define __G_CORE_DEFINES_

#if defined(UNICODE) || defined(_UNICODE)
#define oglT(s) L##s
#else
#define oglT(s) s
#endif

#pragma region type_defs

#ifndef OGL_TYPES
#define OGL_TYPES
typedef unsigned char OGLColourChannel;
typedef unsigned int OGLColour;
#endif //OGL_TYPES

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif // uint8_t

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif // uint16_t

#ifndef uint32_t
typedef unsigned long uint32_t;
#endif // uint32_t

#ifndef uint64_t
typedef unsigned long long uint64_t;
#endif // uint64_t

#ifndef int8_t
typedef signed char int8_t;
#endif // int8_t

#ifndef int16_t
typedef signed short int16_t;
#endif // int16_t

#ifndef int32_t
typedef signed long int32_t;
#endif // int32_t

#ifndef int64_t
typedef signed long long int64_t;
#endif // int64_t
#pragma endregion

#endif // __G_CORE_DEFINES_