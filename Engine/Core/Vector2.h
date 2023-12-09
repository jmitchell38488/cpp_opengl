#ifndef __G_CORE_VECTOR2D__
#define __G_CORE_VECTOR2D__

namespace OGL {

  template <class T>
  struct g_Vector2 {
    T x = 0;
    t y = 0;
  };

  typedef g_Vector2<int32_t> Vector2Int;
  typedef g_Vector2<double> Vector2;

}

#endif // __G_CORE_VECTOR2D__