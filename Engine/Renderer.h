#ifndef __G_RENDERER__
#define __G_RENDERER__

#if !defined(OGL_GFX_OPENGL10) && !defined(OGL_GFX_OPENGL33)
#define OGL_GFX_OPENGL10
#endif

#ifdef OGL_GFX_OPENGL10
#include "Renderer/OpenGL10.h"
#endif // OGL_GFX_OPENGL10

#ifdef OGL_GFX_OPENGL33
#include "Renderer/OpenGL33.h"
#endif // OGL_GFX_OPENGL33

namespace OGL {

  class G_Renderer {
  public:
    virtual ~G_Renderer() = default;
    virtual void       PrepareDevice() = 0;
		virtual OGL::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) = 0;
		virtual OGL::rcode DestroyDevice() = 0;
		virtual void       DisplayFrame() = 0;
    virtual void       ClearBuffer(OGL::Colour c, bool bDepth) = 0;
  };

}

#endif // __G_RENDERER__