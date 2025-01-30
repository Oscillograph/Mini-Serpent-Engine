#ifndef MSE_SYSTEMS_PLATFORM_RENDERER_FONT_H
#define MSE_SYSTEMS_PLATFORM_RENDERER_FONT_H

#include <mse/core.h>

namespace mse
{
	// There should be two different types of fonts:
	// - FontBMP for pixel graphics obtained from a .bmp or a different image file
	// - FontTTF for TrueType fonts obtained from a .ttf file
	
	enum class FontType
	{
		None,
		BitMap,
		TrueType
	};
	
	class Font
	{
	public:
		Font();
		virtual ~Font();
		
		std::string filepath = "";
		FontType type = FontType::None;
		Texture* image = nullptr; 
	};
	
	class FontBitmap : public Font
	{
	public:
		FontBitmap(const std::string& path = "", void* renderer = nullptr);
		
		virtual ~FontBitmap();
		
		void LoadBitmap(void* renderer, const std::string& path);
		void FreeBitmap();
		
		void LoadSymbols();
		
		void GetClip(uint32_t id = 0, uint32_t row = 0);
		
		size_t alphabetSize = 95;
		std::u32string alphabetEn;
		std::u32string alphabetRu;
		std::vector<std::u32string> alphabet = {};
		std::vector<std::vector<uint8_t>> symbols8bitTable = {};
		
		glm::uvec4 destRect = {0, 0, 8, 8};
		glm::uvec4 fontClip = {0, 0, 8, 8};
	};
	
	class FontTrueType : public Font
	{
	public:
		FontTrueType();
		virtual ~FontTrueType();
	};
}

#endif
