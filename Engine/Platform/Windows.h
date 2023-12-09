#ifndef __G_PLATFORM_WIN__
#define __G_PLATFORM_WIN__

#define USE_EXPERIMENTAL_FS
#if defined(_WIN32)
#if _MSC_VER >= 1920 && _MSVC_LANG >= 201703L
#undef USE_EXPERIMENTAL_FS
#endif
#endif // USE_EXPERIMENTAL_FS

#if defined(__MINGW32__)
#if __cplusplus >= 201703L
#undef USE_EXPERIMENTAL_FS
#endif
#endif // USE_EXPERIMENTAL_FS

#if defined(USE_EXPERIMENTAL_FS) || defined(FORCE_EXPERIMENTAL_FS)
	// C++14
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace _gfs = std::experimental::filesystem::v1;
#else
	// C++17
#include <filesystem>
namespace _gfs = std::filesystem;
#endif // USE_EXPERIMENTAL_FS

#if !defined(OGL_PLATFORM_WINAPI)
#if defined(_WIN32)
#define OGL_PLATFORM_WINAPI
#endif
#endif // OGL_PLATFORM_WINAPI

#if defined(OGL_PLATFORM_WINAPI)
#define _WINSOCKAPI_
#if !defined(VC_EXTRALEAN)
#define VC_EXTRALEAN
#endif
#if !defined(NOMINMAX)
#define NOMINMAX
#endif

#if !defined(_WIN32_WINNT)
#ifdef HAVE_MSMF
#define _WIN32_WINNT 0x0600 // Windows Vista
#else
#define _WIN32_WINNT 0x0500 // Windows 2000
#endif
#endif

#include <windows.h>
#undef _WINSOCKAPI_
#endif // OGL_PLATFORM_WINAPI

#if defined(OGL_PLATFORM_WINAPI)
#if defined(_WIN32) && !defined(__MINGW32__)
#pragma comment(lib, "user32.lib")		// Visual Studio Only
#pragma comment(lib, "gdi32.lib")		// For other Windows Compilers please add
#pragma comment(lib, "opengl32.lib")	// these libs to your linker input
#endif
#endif // OGL_PLATFORM_WINAPI

#if defined(OGL_PLATFORM_WINAPI)
#include "../Core/Defines.h"

namespace OGL {

  class G_Platform;

  class Windows_Platform : public OGL::G_Platform {
  private:
    HWND ogl_hWnd = nullptr;
    std::wstring wsAppName;

  private:
  		std::wstring ConvertS2W(std::string s)
		{
#ifdef __MINGW32__
			wchar_t* buffer = new wchar_t[s.length() + 1];
			mbstowcs(buffer, s.c_str(), s.length());
			buffer[s.length()] = L'\0';
#else
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
#endif
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}

  public:
    virtual OGL::rcode ApplicationStartUp() override { return OGL::rcode::OK; }
		virtual OGL::rcode ApplicationCleanUp() override { return OGL::rcode::OK; }
    virtual OGL::rcode ThreadStartUp() override { return OGL::rcode::OK; }
    virtual OGL::rcode ThreadCleanUp() override {
			renderer->DestroyDevice();
			PostMessage(ogl_hWnd, WM_DESTROY, 0, 0);
			return OGL::OK;
		}

    virtual OGL::rcode CreateGraphics(bool bFS = false, bool bVsync = false, const Vector2Int vPos, const Vector2Int vSize) override {
			if (renderer->CreateDevice({ ogl_hWnd }, bFS, bVsync) == OGL::rcode::OK) {
				renderer->UpdateViewport(vPos, vSize);
				return OGL::OK;
			}
			else
				return OGL::FAIL;
		}

    virtual OGL::rcode CreateWindowPane(const Vector2Int vPos, Vector2Int vSize, bool bFS = false) override {
      WNDCLASS wc;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.hInstance = GetModuleHandle(nullptr);
			wc.lpfnWndProc = OGL_WindowEvent;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.lpszMenuName = nullptr;
			wc.hbrBackground = nullptr;
			wc.lpszClassName = oglT("OGL_GAME");
			RegisterClass(&wc);

			// Define window furniture
			DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

      Vector2Int vTL = vPos;

			// Handle Fullscreen
			if (bFS)
			{
				dwExStyle = 0;
				dwStyle = WS_VISIBLE | WS_POPUP;
				HMONITOR hmon = MonitorFromWindow(ogl_hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };
				if (!GetMonitorInfo(hmon, &mi)) return OGL::rcode::FAIL;
				vSize = { mi.rcMonitor.right, mi.rcMonitor.bottom };
				vTL.x = 0;
				vTL.y = 0;
			}

			// Keep client size as requested
			RECT rWndRect = { 0, 0, vSize.x, vSize.y };
			AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
			int width = rWndRect.right - rWndRect.left;
			int height = rWndRect.bottom - rWndRect.top;

			ogl_hWnd = CreateWindowEx(dwExStyle, wc.lpszClassName, oglT(""), dwStyle,
				vTL.x, vTL.y, width, height, NULL, NULL, GetModuleHandle(nullptr), this);

      return OGL::OK;
    }

      virtual OGL::rcode SetWindowTitle(const std::string& s) override
		{
#ifdef UNICODE
			SetWindowText(ogl_hWnd, ConvertS2W(s).c_str());
#else
			SetWindowText(ogl_hWnd, s.c_str());
#endif
			return OGL::OK;
		}

    virtual OGL::rcode StartSystemEventLoop() override
		{
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return OGL::OK;
		}

    virtual OGL::rcode HandleSystemEvent() override { return OGL::FAIL; }

    static LRESULT CALLBACK OGL_WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
      switch (uMsg) {
        case WM_SIZE:
        break;

        case WM_CLOSE:
        break;

        case WM_DESTROY:
          PostQuitMessage(0);
          DestroyWindow(hWnd);
          return 0;
        break;
        
        default:
        break;
      }

      return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

  };

}

#endif 
#endif // __G_PLATFORM_WIN__