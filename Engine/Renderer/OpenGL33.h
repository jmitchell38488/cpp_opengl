#ifndef __G_RENDERER_OPENGL33__
#define __G_RENDERER_OPENGL33__

#ifdef OGL_GFX_OPENGL33
#ifdef OGL_PLATFORM_WINAPI
#include <dwmapi.h>
#if !defined(__MINGW32__)
#pragma comment(lib, "Dwmapi.lib")
#endif

#include "../Renderer.h"

namespace OGL {

  class Renderer_OGL33 : public OGL::G_Renderer {
    #ifdef OGL_PLATFORM_GLUT
    bool b_fullScreen = false;
    #else
    glDeviceContext_t glDeviceContext = 0;
		glRenderContext_t glRenderContext = 0;
    #endif 
  }

}

#endif // __G_RENDERER_OPENGL33__