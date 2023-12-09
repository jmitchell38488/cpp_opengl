#ifndef __G_PLATFORM__
#define __G_PLATFORM__

#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <array>
#include <cstring>

#if defined(_WIN32) || defined(__MINGW32__)
#include "Platform/Windows.h"
#endif // Windows

#if defined(__APPLE__)
#include "Platform/Apple.h"
#endif // Apple

#include "Core/Vector2.h"

namespace OGL {

  class G_Platform {
  public:
    virtual OGL::rcode ApplicationStartUp() = 0;
    virtual OGL::rcode ApplicationCleanUp() = 0;
    virtual OGL::rcode ThreadStartUp() = 0;
    virtual OGL::rcode ThreadCleanUp() = 0;
    virtual OGL::rcode CreateGraphics(bool bFS = false, bool bVsync = false, const Vector2Int vPos, const Vector2Int vSize) = 0;
    virtual OGL::rcode CreateWindowPane(const Vector2Int vPos, Vector2Int vSize, bool bFS = false) = 0;
    virtual OGL::rcode SetWindowTitle(const std::string& s) = 0;
    virtual OGL::rcode StartSystemEventLoop() = 0;
		virtual OGL::rcode HandleSystemEvent() = 0;
  };

}

#endif __G_PLATFORM__