#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/platform/renderer/font.h>
#include <mse/systems/application/application.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/utils/math.h> // findNorm()

namespace mse
{
	SDL_Renderer* Renderer::m_Renderer = nullptr;
	Scene* Renderer::m_ActiveScene = nullptr;
	Layer* Renderer::m_ActiveLayer = nullptr;
	Window* Renderer::m_ActiveWindow = nullptr;
	Camera2D* Renderer::m_ActiveCamera = nullptr;
	
	glm::uvec4 Renderer::m_CurrentScreen = glm::uvec4(1.0f);
	glm::vec2 Renderer::m_PixelSize = glm::vec2(1.0f);
	glm::uvec4 Renderer::m_BackgroundColor = glm::uvec4(1.0f);
	glm::vec2 Renderer::m_FrameSize = glm::vec2(1.0f);
	glm::vec2 Renderer::m_FrameScale = glm::vec2(1.0f);
	glm::vec2 Renderer::m_CameraPosition = glm::vec2(1.0f);
	std::vector<std::vector<uint32_t>> Renderer::m_symbols8bitTable = {};
	
	void Renderer::SetActiveRenderer(void* renderer)
	{
		m_Renderer = (SDL_Renderer*)renderer;
	}
	
	void Renderer::SetActiveScene(Scene* scene)
	{
		m_ActiveScene = scene;
		
		int newWidth, newHeight;
		SDL_GetWindowSize((SDL_Window*)GetActiveWindow()->GetNativeWindow(), &newWidth, &newHeight);
		m_PixelSize.x = (float)newWidth / GetActiveWindow()->GetWidth();
		m_PixelSize.y = (float)newHeight / GetActiveWindow()->GetHeight();
	}
	
	void Renderer::SetActiveLayer(Layer* layer)
	{
		m_ActiveLayer = layer;
	}
	
	void Renderer::SetActiveWindow(Window* window)
	{
		m_ActiveWindow = window;
		if (window != nullptr)
		{
			m_CurrentScreen = {
				0,
				0,
				window->GetPrefs().width,
				window->GetPrefs().height
			};
		} else {
			m_CurrentScreen = {0, 0, 0, 0};
		}
	}
	
	void Renderer::SetActiveCamera(Camera2D* camera)
	{
//		m_ActiveCamera = camera;
//		m_CameraPosition = camera->GetPositionNormalized();
		// m_CameraPosition.x = m_CameraPosition.x / m_Scene->GetLayer()->GetWindow()->GetPrefs().width;
		// m_CameraPosition.y = m_CameraPosition.y / m_Scene->GetLayer()->GetWindow()->GetPrefs().height;
	}
	
	void Renderer::SetActiveScreen(const glm::uvec4& screen)
	{ 
		m_CurrentScreen = screen;
		
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		SDL_Rect viewport;
		viewport = {
			windowScale.x * m_CurrentScreen.x,
			windowScale.y * m_CurrentScreen.y,
			windowScale.x * m_CurrentScreen.z,
			windowScale.y * m_CurrentScreen.w
		};
		// SDL_RenderSetViewport(GetActiveRenderer(), &viewport);
		SDL_RenderSetClipRect(GetActiveRenderer(), &viewport);
	}
	
	void Renderer::SetActiveScreenDefault()
	{ 
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		m_CurrentScreen = {
			0,
			0,
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height,
		};
		
		SDL_Rect viewport;
		viewport = {
			windowScale.x * m_CurrentScreen.x,
			windowScale.y * m_CurrentScreen.y,
			windowScale.x * m_CurrentScreen.z,
			windowScale.y * m_CurrentScreen.w
		};
		// SDL_RenderSetViewport(GetActiveRenderer(), &viewport);
		SDL_RenderSetClipRect(GetActiveRenderer(), &viewport);
	}
	
	void Renderer::SetBackgroundColor(const glm::uvec4& color)
	{
		m_BackgroundColor = color;
		SDL_SetRenderDrawColor(GetActiveRenderer(), color.x, color.y, color.z, color.w);
	}
	
	void Renderer::ClearScreen()
	{
		SDL_RenderClear(GetActiveRenderer());
	}
	
	
	// low-level methods (draw pixels, primitives, operate with data)
	uint32_t Renderer::GetPixel(Texture* surface, int x, int y)
	{
		/*
		Got this function from here:
		https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface
		*/
		/* - - - Usage example: - - -
		SDL_Color rgb;
		Uint32 data = getPixel(gSurface, 200, 200);
		SDL_GetRGB(data, gSurface->format, &rgb.r, &rgb.g, &rgb.b);
		- - - - - - - - - - - - - - - */
		int bpp = surface->GetSurface()->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to retrieve */
		uint8_t *p = (uint8_t *)surface->GetSurface()->pixels + y * surface->GetSurface()->pitch + x * bpp;
		
		switch (bpp)
		{
		case 1:
			return *p;
			break;
		case 2:
			return *(uint16_t *)p;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				return p[0] << 16 | p[1] << 8 | p[2];
			} else {
				return p[0] | p[1] << 8 | p[2] << 16;
			}
			break;
		case 4:
			return *(uint32_t *)p;
			break;
		default:
			return 0;       /* shouldn't happen, but avoids warnings */
		}
	}
	
	void Renderer::DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
		GeneralDrawTexture(texture, destRect, srcRect, glm::vec2(1.0f), glm::vec4(1.0f));
	}
	
	void Renderer::DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
		GeneralDrawTexture(texture, destRect, srcRect, tilingFactor, glm::vec4(1.0f));
	}
	
	// Important note: if srcRect is NULL, there is a memory leak about 4 KB per second or two,
	// 	               somehow related to SDL_RenderCopy functions in general.
	void Renderer::GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
		SDL_FRect* place = destRect;
		SDL_Rect* source = srcRect;
		
		// TODO: find out why Application::Get()->GetWindows() is not allowed to be accessed from here
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height,
		};
		
		// correct the source rectangle
		if (srcRect != NULL)
		{
			*source = { 
				srcRect->x, 
				srcRect->y, 
				srcRect->w, 
				srcRect->h 
			};
		} else {
			source = NULL;
		}
		
		// correct the destination rectangle
		if (destRect != NULL)
		{
			// CSE_CORE_LOG("Viewport (x; y): ", m_CurrentScreen.x, "; ", m_CurrentScreen.y);
			*place = 
			{ 
				(int)floorf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * destRect->x)), 
				(int)floorf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * destRect->y)), 
				(int)floorf(windowScale.x * m_CurrentScreen.z * destRect->w), 
				(int)floorf(windowScale.y * m_CurrentScreen.w * destRect->h) 
			};
		} else {
			// not making it NULL is important for the next step - tiling
			*place = 
			{ 
				windowScale.x * m_CurrentScreen.x, 
				windowScale.y * m_CurrentScreen.y, 
				windowScale.x * m_CurrentScreen.z, 
				windowScale.y * m_CurrentScreen.w 
			};
		}
		
		// TODO: draw only if it's on screen
//		if(true)
		if ((place->x + place->w > 0) &&
			(place->y + place->h > 0) &&
			(place->x < (windowScale.x * windowSize.x)) &&
			(place->y < (windowScale.y * windowSize.y)))
		{
			// tiling texture across the place rectangle
			// TODO: Adjust tiling to pixel size
			if (((tilingFactor.x > 0.0f) || (tilingFactor.y > 0.0f)) && (source != NULL))
			{
				// now, get subPlaces and RenderCopy there
				int xNum, yNum; // how many whole tiles there are?
				xNum = 1;
				yNum = 1;
				int xMod, yMod; // how big is the partial tile left?
				xMod = 0;
				yMod = 0;
				glm::uvec2 wholeTileSize = {(*source).w, (*source).h};
				glm::uvec2 currentTileSize = wholeTileSize;
				
				glm::vec2 regionSize = {
					windowSize.x * (*destRect).w, 
					windowSize.y * (*destRect).h
				};
				
				if (tilingFactor.x > 0.0f) // first of all, we need to know how many times to multiply the image
				{
					// how many whole tiles there are?
					xNum = (int)floorf((float)regionSize.x / (wholeTileSize.x * tilingFactor.x));
					// and we need to know how big the remainder is
					xMod = (int)floorf(regionSize.x - (xNum * wholeTileSize.x * tilingFactor.x));
					xMod = (int)(floorf)((float)xMod / tilingFactor.x);
					if (xMod > 0)
						xNum++;
				} else { // not only prevent division by zero, but also allow no-tiling at all
					xNum = 1;
					xMod = 0;
				}
				
				if (tilingFactor.y > 0.0f) // first of all, we need to know how many times to multiply the image
				{
					// how many whole tiles there are?
					yNum = (int)floorf((float)regionSize.y / (wholeTileSize.y * tilingFactor.y)); 
					// and we need to know how big the remainder is
					yMod = (int)floorf(regionSize.y - (yNum * wholeTileSize.y * tilingFactor.y));
					yMod = (int)(floorf)((float)yMod / tilingFactor.y);
					if (yMod > 0)
						yNum++;
				} else { // not only prevent division by zero, but also allow no-tiling at all
					yNum = 1;
					yMod = 0;
				}
				
				// CSE_LOG("xNum: ", xNum, "; yNum: ", yNum, "; xMod: ", xMod, "; yMod: ", yMod);
				
				SDL_FRect* newPlace = new SDL_FRect;
				SDL_Rect* newSource = new SDL_Rect;
				
				glm::uvec4 currentTilePlace = {0, 0, 0, 0};
				
				for (int x = 0; x < xNum; x++)
				{
					if ((x == (xNum - 1)) && (xMod > 0))
					{
						currentTileSize.x = xMod; //wholeTileSize.x - xMod;
					} else {
						currentTileSize.x = wholeTileSize.x;
					}
					
					for (int y = 0; y < yNum; y++)
					{
						if ((y == (yNum - 1)) && (yMod > 0))
						{
							currentTileSize.y = yMod; //wholeTileSize.y - yMod;
						} else {
							currentTileSize.y = wholeTileSize.y;
						}
						
						newSource->x = source->x;
						newSource->y = source->y;
						newSource->w = currentTileSize.x;
						newSource->h = currentTileSize.y;
						
//						*newSource = 
//						{ 
//							(*source).x, 
//							(*source).y, 
//							currentTileSize.x, 
//							currentTileSize.y 
//						};
						
						if (tilingFactor.x > 0.0f)
						{
							currentTilePlace.x = (windowSize.x * (*destRect).x + (x * wholeTileSize.x * tilingFactor.x)) * m_CurrentScreen.z / windowSize.x;
							currentTilePlace.z = currentTileSize.x * tilingFactor.x * m_CurrentScreen.z / windowSize.x;
						} else {
							currentTilePlace.x = (m_CurrentScreen.z * (*destRect).x);
							currentTilePlace.z = m_CurrentScreen.z * (*destRect).w;
						}
						
						if (tilingFactor.y > 0.0f)
						{
							currentTilePlace.y = (windowSize.y * (*destRect).y + (y * wholeTileSize.y * tilingFactor.y)) * m_CurrentScreen.z / windowSize.x;
							currentTilePlace.w = currentTileSize.y * tilingFactor.y * m_CurrentScreen.w / windowSize.y;
						} else {
							currentTilePlace.y = (m_CurrentScreen.w * (*destRect).y);
							currentTilePlace.w = m_CurrentScreen.w * (*destRect).h;
						}
						
						newPlace->x = windowScale.x * (m_CurrentScreen.x + currentTilePlace.x);
						newPlace->y = windowScale.y * (m_CurrentScreen.y + currentTilePlace.y);
						newPlace->w = windowScale.x * currentTilePlace.z;
						newPlace->h = windowScale.y * currentTilePlace.w;
						
//						*newPlace = 
//						{
//							windowScale.x * (m_CurrentScreen.x + currentTilePlace.x), 
//							windowScale.y * (m_CurrentScreen.y + currentTilePlace.y),
//							windowScale.x * currentTilePlace.z, 
//							windowScale.y * currentTilePlace.w 
//						};
						
						// CSE_LOG("Whole tile size: ( ", wholeTileSize.x, "; ", wholeTileSize.y, ") >> x = ", x, "; y = ", y);
						// CSE_LOG("Current tile size: ", currentTileSize.x, "; height: ", currentTileSize.y);
						// CSE_LOG("New place (SDL_Rect): (", (*newPlace).x, "; ", (*newPlace).y, "; ", (*newPlace).w, "; ", (*newPlace).h, ")");
						
						SDL_RenderCopyExF(
							GetActiveRenderer(), 
							texture->GetTexture(), 
							newSource, 
							newPlace,
							0,
							NULL,
							SDL_RendererFlip::SDL_FLIP_NONE
							);
					}
				}
				
				delete newPlace;
				delete newSource;
			} else {
				SDL_RenderCopyExF(
					GetActiveRenderer(), 
					texture->GetTexture(), 
					source, 
					place,
					0,
					NULL,
					SDL_RendererFlip::SDL_FLIP_NONE
					);
			}
		}
		
//		delete place;
//		delete source;
	}
	
	void Renderer::DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
		// float scaleX = m_CurrentScreen.z * m_Scene->GetLayer()->GetWindow()->GetScale().x;
		// float scaleY = m_CurrentScreen.w * m_Scene->GetLayer()->GetWindow()->GetScale().y;
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height
		};
		
		SDL_FRect rect = 
		{
			windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * (center.x - size.x/2)), 
			windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * (center.y - size.y/2)), 
			windowScale.x * m_CurrentScreen.z * size.x, 
			windowScale.y * m_CurrentScreen.w * size.y
		};
		
		SDL_SetRenderDrawColor(GetActiveRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawRectF(GetActiveRenderer(), &rect);
		SetBackgroundColor(m_BackgroundColor);
	}
	
	void Renderer::DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color)
	{
		// float scaleX = m_CurrentScreen.z * m_Scene->GetLayer()->GetWindow()->GetScale().x;
		// float scaleY = m_CurrentScreen.w * m_Scene->GetLayer()->GetWindow()->GetScale().y;
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height
		};
		
		SDL_Point points[5] = {
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p1.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p1.y)),
			}, 
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p2.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p2.y)),
			},  
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p3.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p3.y)),
			}, 
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p4.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p4.y)),
			},
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p1.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p1.y)),
			}
		};
		
		SDL_SetRenderDrawColor(GetActiveRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawLines(GetActiveRenderer(), points, 5);
		SetBackgroundColor(m_BackgroundColor);
	}
	
	void Renderer::DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height
		};
		
		SDL_FRect rect = 
		{
			windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * (center.x - size.x/2)), 
			windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * (center.y - size.y/2)), 
			windowScale.x * m_CurrentScreen.z * size.x, 
			windowScale.y * m_CurrentScreen.w * size.y
		};
		
		SDL_SetRenderDrawColor(GetActiveRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderFillRectF(GetActiveRenderer(), &rect);
		SetBackgroundColor(m_BackgroundColor);
	}
	
	// low-level methods on surfaces (for software rendering)
	void Renderer::SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
//		MSE_CORE_LOG("Render: pixel color is (", (int)color.r, ", ", (int)color.g, ", ", (int)color.b, ", ", (int)color.a, ")");
		uint32_t pxColor = SDL_MapRGBA(target->GetSurface()->format, color.r, color.g, color.b, color.a);
		int xn, yn = 0;
		uint32_t* pxBuffer = nullptr;
			
		for (int k = 0; k < pxSize; k++)
		{
			xn = center.x + k;
			for (int l = 0; l < pxSize; l++)
			{
				yn = center.y + l;
				if ((xn < target->GetSurface()->w) && (yn < target->GetSurface()->h))
				{
					pxBuffer = (uint32_t*)target->GetSurface()->pixels + yn*target->GetSurface()->pitch/4 + xn;
					*pxBuffer = pxColor;
				}
			}
		}
	}
	
	void Renderer::SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
		if (target->GetSurface() != NULL)
		{
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_LockSurface(target->GetSurface());
			}
			
			SurfaceDrawPixel_unsafe(target, center, pxSize, color);
			
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_UnlockSurface(target->GetSurface());
			}
		}
	}
	
	void Renderer::SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
		float xn, yn = 0;
		
		glm::vec2 dxy = Math::findNorm({(float)(x2 - x1), (float)(y2 - y1)});
		int count = (int)round(Math::length({(float)(x2 - x1), (float)(y2 - y1)}));
		
		for (int i = 0; i <= count; ++i)
		{
			xn = x1 + i*dxy.x;
			yn = y1 + i*dxy.y;
			SurfaceDrawPixel(target, {(int)round(xn), (int)round(yn)}, pxSize, color);
		}
	}
	
	void Renderer::SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
		if (target->GetSurface() != NULL)
		{
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_LockSurface(target->GetSurface());
			}
			
			SurfaceDrawLine_unsafe(target, x1, y1, x2, y2, pxSize, color);
			
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_UnlockSurface(target->GetSurface());
			}
		}
	}
	
	void Renderer::SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
		int x = destRect.x;
		int y = destRect.y;
		int w = destRect.w;
		int h = destRect.h;
		
		if ((x + w) > target->GetSurface()->w)
		{
			int pxWidth = (int)(round(target->GetSurface()->w - x));
		} 
		if ((y + h) > target->GetSurface()->h)
		{
			int pxHeight = (int)(round(target->GetSurface()->h - y));
		}
		
		SurfaceDrawLine(target, x, y, x+w, y, pxSize, color); // draw top line
		SurfaceDrawLine(target, x+w, y, x+w, y+h, pxSize, color); // draw right line
		SurfaceDrawLine(target, x+w, y+h, x, y+h, pxSize, color); // draw bottom line
		SurfaceDrawLine(target, x, y+h, x, y, pxSize, color); // draw left line
	}
	
	void Renderer::SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
		if (target->GetSurface() != NULL)
		{
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_LockSurface(target->GetSurface());
			}
			
			SurfaceDrawRect_unsafe(target, destRect, pxSize, color);
			
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_UnlockSurface(target->GetSurface());
			}
		}
	}
	
	void Renderer::SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
		int pxWidth = destRect.w; 
		int pxHeight = destRect.h;
		if ((destRect.x + destRect.w) > target->GetSurface()->w)
		{
			pxWidth = (int)(round(target->GetSurface()->w - destRect.x));
		} 
		if ((destRect.y + destRect.h) > target->GetSurface()->h)
		{
			pxHeight = (int)(round(target->GetSurface()->h - destRect.y));
		} 
		
		for (int i = 0; i < pxHeight; i++)
		{
			for (int j = 0; j < pxWidth; j++)
			{
				SurfaceDrawPixel_unsafe(
					target, 
					{destRect.x + j, destRect.y + i}, 
					1, 
					color);
			}
		}
	}
	
	void Renderer::SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
		if (target->GetSurface() != NULL)
		{
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_LockSurface(target->GetSurface());
			}
			
			SurfaceDrawRectFilled_unsafe(target, destRect, color);
			
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_UnlockSurface(target->GetSurface());
			}
		}
	}
	
	void Renderer::SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		double circlePerimeter = Math::PI * r * 2; // find the perimeter of a circle
		int pxNumber = (int)round(circlePerimeter / pxSize); // count pixels in the perimeter
		int xn, yn = 0;
		
		double degreeStep = 2 * Math::PI / pxNumber; // find how much degrees in each pixel
		for (int j = 0; j < pxNumber; j++) // draw the pixels
		{ 
			xn = (int)(center.x + round(r*cos(j*degreeStep)));
			yn = (int)(center.y + round(r*sin(j*degreeStep)));
			xn = xn - (xn % pxSize);
			yn = yn - (yn % pxSize);
			
			if ((xn < target->GetSurface()->w) && (yn < target->GetSurface()->h) && (xn >= 0) && (yn >= 0))
			{
				SurfaceDrawPixel_unsafe(target, {xn, yn}, pxSize, color);
			}
		}
	}
	
	void Renderer::SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		if (target->GetSurface() != NULL)
		{
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_LockSurface(target->GetSurface());
			}
			
			SurfaceDrawCircle_unsafe(target, center, r, pxSize, color);
			
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_UnlockSurface(target->GetSurface());
			}
		}
	}
	
	void Renderer::SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		int x = center.x - (center.x % pxSize);
		int y = center.y - (center.y % pxSize);
		int r2 = r*r;
		int xn, yn = 0;
		
		for (int k = -r; k <= r; k = k + pxSize)
		{
			for (int l = -r; l <= r; l = l + pxSize)
			{
				if (k*k + l*l <= r2){
					xn = x + k;
					yn = y + l;
					xn = xn - (xn % pxSize);
					yn = yn - (yn % pxSize);
					
					if ((xn < target->GetSurface()->w) && (yn < target->GetSurface()->h) && (xn >= 0) && (yn >= 0))
					{
						SurfaceDrawPixel_unsafe(target, {xn, yn}, pxSize, color);
					}
				}
			}
		}
	}
	
	void Renderer::SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		if (target->GetSurface() != NULL)
		{
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_LockSurface(target->GetSurface());
			}
			
			SurfaceDrawCircleFilled_unsafe(target, center, r, pxSize, color);
			
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_UnlockSurface(target->GetSurface());
			}
		}
	}
	
	void Renderer::SurfaceDrawText_unsafe(
		Texture* target,
		const glm::uvec4& place,	// where to draw  
		int pxSize, 				// size of the pen
		const std::u32string& text, 		// what to draw
		Resource* textFont,			// how is should look like
		const glm::uvec4& color,	// color
		int interval)				// interval between lines
	{	
		// convert from float Rect to int
		SDL_Rect tempRect = {
			round(place.x),
			round(place.y),
			round(place.z),
			round(place.w)
		};
		
		FontBitmap* font = (FontBitmap*)(textFont->data);
		
		// text
		// cols and rows with no respect to spaces trimming made via correctX variable
		int cols = tempRect.w / (font->fontClip.z*pxSize); // total columns to render
		int rows = tempRect.h / ((font->fontClip.w + interval)*pxSize); // total rows to render
		int currentRow = 0; // the row we render right now
		long long unsigned int symbolId = 0; // the current symbol to render, its Id should correspond to the font alphabet, and its type - to the type of std::u32string::npos
		int correctX = 0;
		int correctY = 0;
		int curX;
		int curY;
		int alphabetId = 0;
		bool foundSymbol = false;
		uint32_t fontColor = SDL_MapRGBA(target->GetSurface()->format,
			color.r, color.g, color.b, color.a);
		
		// for each symbol in text
		int j = 0; // index of a symbol inside a line being typed right now
		
		for(int i = 0; i < text.size(); ++i)
		{
			// load font symbol
			foundSymbol = false;
			while ((!foundSymbol) && (alphabetId < font->alphabet.size()))
			{
				symbolId = font->alphabet[alphabetId].find(text[i]);
				// force a new line if '\n' found
				if (symbolId == font->alphabetSize)
				{
					foundSymbol = true;
					currentRow++;
					j = 0; // reset the index inside a line
					correctX = 0;
					symbolId = 0;
					alphabetId = 0;
				} else {
					if (symbolId != std::u32string::npos)
					{
						// Correct coordinates of the "cursor"
						if (j > cols){
							currentRow++;
							j = 0;
							correctX = 0;
						}
						correctY = currentRow*(font->fontClip.w + interval)*pxSize;
						
						uint32_t blackPixelsCount = 0;
						uint32_t symbolLeftEdge = symbolId*(font->fontClip.w * font->fontClip.z);
						for (int k = 0; k < font->fontClip.z; ++k)
						{
							blackPixelsCount += font->symbols8bitTable[alphabetId][symbolLeftEdge + k * font->fontClip.w];
							blackPixelsCount += font->symbols8bitTable[alphabetId][symbolLeftEdge + k * font->fontClip.w + 1];
						}
						if (blackPixelsCount == 0)
						{
							correctX--;
						}
						
						// position the pencil according to where the cursor is
						curX = tempRect.x + j*font->fontClip.w*pxSize + correctX*pxSize;
						curY = tempRect.y + correctY;
						
						// draw the symbol in its place
						for(int l = 0; l < font->fontClip.z; ++l)
						{
							for(int k = 0; k < font->fontClip.w; ++k)
							{
								if (font->symbols8bitTable[alphabetId][symbolLeftEdge + k + l*font->fontClip.w] == 1)
								{
									SurfaceDrawPixel_unsafe(target, {curX + k*pxSize, curY + l*pxSize}, pxSize, {color.x, color.y, color.z, color.w});
								}
							}
						}
						alphabetId = 0; // reset alphabet
						symbolId = 0; // reset symbol index in that alphabet
						j++; // we move through the line!
						foundSymbol = true;
					} else {
						alphabetId++; // look in an another alphabet
						symbolId = 0; // reset symbol index in that alphabet
					}
				}
			}
		}
	}
	
	void Renderer::SurfaceDrawText(Texture* target, const glm::uvec4& place, int pxSize, const std::u32string& text, Resource* textFont, const glm::uvec4& color, int interval)
	{
		if (target->GetSurface() != NULL)
		{
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_LockSurface(target->GetSurface());
			}
			
			if (textFont != nullptr)
			{
				SurfaceDrawText_unsafe(target, place, pxSize, text, textFont, color, interval);
			} else {
				MSE_CORE_LOG("Renderer: Could not draw text due to textFont is nullptr");
			}
			
			if (SDL_MUSTLOCK(target->GetSurface()))
			{
				SDL_UnlockSurface(target->GetSurface());
			}
		}
	}
	
	// mid-level methods (advanced renderer commands)
	void Renderer::NewFrame()
	{
		ClearScreen();
		
		if (GetActiveCamera() != nullptr)
		{
//			m_CameraPosition = GetActiveCamera()->GetPosition();
		}
	}
	
	void Renderer::ShowFrame()
	{
		SDL_RenderPresent(GetActiveRenderer());
	}
}

