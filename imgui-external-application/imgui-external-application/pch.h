#ifndef PCH_H
#define PCH_H

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <Windows.h>
#include <string>
#include <dxgi.h>
#include <wrl/client.h>
#include <memory>
#include <stdexcept>
#include <d3d11.h>
#include <tchar.h>
#include <dwmapi.h>
#include <condition_variable>
#include <mutex>

#pragma comment(lib, "d3d11.lib")

#endif // PCH_H