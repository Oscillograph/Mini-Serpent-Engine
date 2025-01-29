#ifndef MSE_SYSTEMS_PLATFORM_RENDERER_TEXTURE_H
#define MSE_SYSTEMS_PLATFORM_RENDERER_TEXTURE_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>

namespace mse
{
	struct ColorKey
	{
		int r;
		int g;
		int b;
	};
	
	class Texture
	{
	public:
		Texture(const std::string& path, void* renderer);
		Texture(const std::string& path, SDL_Renderer* renderer);
		Texture(const std::string& path, void* renderer, const ColorKey& colorKey);
		
		Texture(void* renderer, int w, int h, uint32_t flags=0, int depth=32,
			uint32_t Rmask=0, uint32_t Gmask=0, uint32_t Bmask=0, uint32_t Amask=0);
		
		~Texture();
		
		void Load(const std::string& path);
		
		SDL_Texture* ConvertToSurface(SDL_Surface* srcSurface);
		
		void Update();
		
		int GetWidth();
		int GetHeight();
		inline SDL_Texture* GetTexture() { return m_Texture; }
		inline SDL_Surface* GetSurface() { return m_Surface; }
		
		// shamelessly took from Lazy Foo's tutorials
		uint32_t* GetPixels32();
		int GetPitch32();
		
		void FreeTexture();
		void FreeSurface();
		void Free();
		
		void Clear();
		
	private:
		SDL_Renderer* m_Renderer;
		SDL_Surface* m_Surface;
		SDL_Texture* m_Texture;
	};
}

#endif

