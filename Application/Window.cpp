#include "Window.h"
#include <iostream>
namespace WaxGourd {
Window::Window(uint2 _resolution) : m_window_resolution(_resolution) {
	if (_resolution == uint2(0u, 0u)) {
		//m_window_resolution = uint2(1920u, 1080u);
		SDL_DisplayID primaryDisplayID = SDL_GetPrimaryDisplay();                                                                                                                                                            // Return the primary display.
		// SDL_PropertiesID primaryDisplayProperties = SDL_GetDisplayProperties(primaryDisplayID);
		const SDL_DisplayMode* pPrimaryDisplayMode = SDL_GetCurrentDisplayMode(primaryDisplayID);
		m_window_resolution = uint2(pPrimaryDisplayMode->w, pPrimaryDisplayMode->h);
	}
	initSDL();
	createWindow();
}
Window::~Window() {
	SDL_DestroyWindow(m_sdl_window);
	SDL_Quit();
}
void Window::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) == false) {
		std::cerr << "SDL could not initialize. SDL_Error: " << SDL_GetError() << std::endl;
	}
}
void Window::createWindow() {
	m_sdl_window = SDL_CreateWindow(
		"WaxGourd", 
		m_window_resolution.x, m_window_resolution.y, 
		SDL_WINDOW_RESIZABLE| 
		SDL_WINDOW_INPUT_FOCUS| 
		SDL_WINDOW_MOUSE_FOCUS| 
		SDL_WINDOW_MOUSE_CAPTURE| 
		SDL_WINDOW_MOUSE_RELATIVE_MODE
	);
	if (m_sdl_window == nullptr) {
		std::cerr << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
	}
	m_window_properties = SDL_GetWindowProperties(m_sdl_window);
	m_window_handle = (HWND*)SDL_GetPointerProperty(m_window_properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
}
HWND* Window::GetWindowHandle() const {
	return m_window_handle;
}
} // namepsace