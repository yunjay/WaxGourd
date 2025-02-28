#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <windows.h>
#include "MathDefines.h"
namespace WaxGourd {
class Window {

public:
Window(uint2 resolution = uint2(0u,0u)); // 0,0 for fullscreen size
~Window();
HWND* GetWindowHandle() const;
private:
void initSDL();
void createWindow();

SDL_Window* m_sdl_window;
uint2 m_window_resolution;
SDL_PropertiesID m_window_properties;
HWND* m_window_handle;
};
}