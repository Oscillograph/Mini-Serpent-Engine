#ifndef MSE_SYSTEMS_PLATFORM_RENDERER_CURSOR_H
#define MSE_SYSTEMS_PLATFORM_RENDERER_CURSOR_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/texture.h>

namespace mse
{
    class Cursor
	{
	public:
		Cursor(Resource* texture, Window* user, int hotX, int hotY);
		~Cursor();
        
        inline SDL_Cursor* GetNativeCursor() { return m_Cursor; }
		
	private:
        Window* m_window = nullptr;
		SDL_Cursor* m_Cursor = nullptr;
        Resource* m_texture = nullptr;
	};
}

#endif

