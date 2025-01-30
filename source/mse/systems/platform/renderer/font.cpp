#include <mse/systems/platform/renderer/font.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/platform/renderer/renderer.h>

namespace mse
{
	// General font
	Font::Font()
	{}
	
	Font::~Font()
	{}
	
	
	// Bitmap
	FontBitmap::FontBitmap(const std::string& path, void* renderer) : Font()
	{
		size_t alphabetSize = 95;
		alphabetEn = U"`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>? \n"; //95
		alphabetRu = U"ё1234567890-=йцукенгшщзхъ\\фывапролджэячсмитьбю.Ё!\"№;%:?*()_+ЙЦУКЕНГШЩЗХЪ/ФЫВАПРОЛДЖЭЯЧСМИТЬБЮ, \n"; //95
		alphabet.push_back(alphabetEn);
		alphabet.push_back(alphabetRu);
		
		if ((renderer != nullptr) && (path.size() > 0))
		{
			LoadBitmap(renderer, path);
			fontClip.z = image->GetWidth() / 95;
			fontClip.w = image->GetHeight() / alphabet.size();
			
			symbols8bitTable.clear();
			symbols8bitTable.resize(alphabet.size());
			for (int i = 0; i < symbols8bitTable.size(); ++i)
			{
				symbols8bitTable[i].resize(alphabetSize * fontClip.z * fontClip.w);
				MSE_CORE_LOG("Font: ", i, " symbols table size is ", symbols8bitTable[i].size());
			}
			
			LoadSymbols();
		}
	}
	
	FontBitmap::~FontBitmap()
	{
		FreeBitmap();
	}
	
	void FontBitmap::LoadBitmap(void* renderer, const std::string& path)
	{
		filepath = path;
		FreeBitmap();
		image = new Texture(path, renderer);
	}
	
	void FontBitmap::FreeBitmap()
	{
		delete image;
	}
	
	void FontBitmap::LoadSymbols()
	{
		SDL_Color rgb = {0, 0, 0};
		
		// for each alphabet
		for(int i = 0; i < alphabet.size(); ++i)
		{
			// and each font symbol
			for(int j = 0; j < alphabet[i].size()-1; ++j)
			{
				// and for each y coordinate
				for(int l = 0; l < fontClip.w; ++l)
				{
					// for each x coordinate
					for(int k = 0; k < fontClip.z; ++k)
					{
						// get the pixel color
						SDL_GetRGB(
							Renderer::GetPixel(image, j*fontClip.z + k, i*fontClip.w + l), 
							image->GetSurface()->format,
							&rgb.r, 
							&rgb.g, 
							&rgb.b
							);
						// if it's black, add 1, else add 0
						if (rgb.r + rgb.g + rgb.b == 0)
						{
							symbols8bitTable[i][j*(fontClip.w * fontClip.z) + l*fontClip.z + k] = 1;
						} else {
							symbols8bitTable[i][j*(fontClip.w * fontClip.z) + l*fontClip.z + k] = 0;
						}
					}
				}
			}
		}
	}
	
	void FontBitmap::GetClip(uint32_t id, uint32_t row)
	{
		fontClip.x = id * fontClip.z;
		fontClip.y = row * fontClip.w;
	}
	
	
	// True Type
	FontTrueType::FontTrueType()
	{}
	
	FontTrueType::~FontTrueType()
	{}
}
