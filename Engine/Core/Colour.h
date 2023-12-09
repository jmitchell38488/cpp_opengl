#ifndef __G_CORE_COLOUR_
#define __G_CORE_COLOUR_

#include "Defines.h"

constexpr OGLColourChannel oglDefAlpha = 0xFF;
constexpr OGLColour oglDefColour = (oglDefAlpha << 24);

namespace OGL {

  struct Colour {
    union {
      OGLColour colour = oglDefColour;
      struct {
        OGLColourChannel r;
        OGLColourChannel g;
        OGLColourChannel b;
        OGLColourChannel a;
      };
    };

    Colour();
    Colour(OGLColourChannel r, OGLColourChannel g, OGLColourChannel b, OGLColourChannel a = oglDefAlpha);
    Colour(OGLColour p);
  };

}

#endif // __G_CORE_COLOUR_