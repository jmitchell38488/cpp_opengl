#ifndef __G_PLATFORM_APPLE__
#define __G_PLATFORM_APPLE__

#if __cplusplus >= 201703L
#undef USE_EXPERIMENTAL_FS
#endif

#if defined(USE_EXPERIMENTAL_FS) || defined(FORCE_EXPERIMENTAL_FS)
	// C++14
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace _gfs = std::experimental::filesystem::v1;
#else
	// C++17
#include <filesystem>
namespace _gfs = std::filesystem;
#endif

#if !defined(OGL_PLATFORM_X11) || !defined(OGL_PLATFORM_GLUT)
#define GL_SILENCE_DEPRECATION
#define OGL_PLATFORM_GLUT
#endif // OGL_PLATFORM_*

#if defined(OGL_PLATFORM_GLUT)
#define OGL_USE_CUSTOM_START
#endif

#if defined(OGL_PLATFORM_GLUT)
#include <GLUT/glut.h>
#include <objc/message.h>
#include <objc/NSObjCRuntime.h>
#endif


#endif // __G_PLATFORM_APPLE__