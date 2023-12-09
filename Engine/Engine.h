#ifndef __G_ENGINE__
#define __G_ENGINE__

#define GAME_VER 1
#define UNUSED(x) (void)(x)

#if !defined(OGL_GFX_OPENGL10) && !defined(OGL_GFX_OPENGL33)
#define OGL_GFX_OPENGL33
#endif

#ifdef OGL_GFX_OPENGL33
#include "Renderer/OpenGL33.h"
#endif

#ifdef OGL_GFX_OPENGL10
#include "Renderer/OpenGL10.h"
#endif

#include "Platform/Windows.h"
#include "Platform/Apple.h"
#include "Platform/Linux.h"

namespace OGL {
  enum rcode { FAIL = 0, OK = 1, NO_FILE = -1 };
}

#include "Core/Defines.h"
#include "Platform.h"

namespace OGL {

  class G_Renderer;
  class G_Platform;

  static std::unique_ptr<G_Platform> platform;
  static std::unique_ptr<G_Renderer> renderer;

  class G_Engine {

  }

}

#endif // __G_ENGINE__