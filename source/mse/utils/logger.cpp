#include <mse/utils/logger.h>
#include <iostream>

// #include <color-console/color.hpp> // colorizing console output

namespace mse
{
	std::string Logger::m_u8string = "";
	std::u32string Logger::m_u32string = U"";
	std::stringstream Logger::m_u8stream;
	u32stringstream Logger::m_u32stream;
	
	void Logger::Flush(int mode, int color)
	{
		// flush into command line interface
		if (mode == 0)
		{
			switch (color)
			{
				case 0: // engine log
				{
					// yellow
					std::cout << "\033[38;2;255;255;0m" << m_u8string << "\033[0m";
					break;
				}
				
				case 1: // app log
				{
					// white
					std::cout << "\033[38;2;255;255;255m" << m_u8string << "\033[0m";
					break;
				}
				
				case 2: // dark red
				{
					std::cout << "\033[38;2;255;0;0m" <<  m_u8string << "\033[0m";
					break;
				}
				
				case 3: // light red
				{
					std::cout << "\033[38;2;255;255;128m" << m_u8string << "\033[0m";
					break;
				}
				
				default: // no idea who's log
				{
					std::cout << "\033[38;2;164;164;164m" << m_u8string << "\033[0m";
				}
			}
		}
		
		// flush onto screen
		if (mode == 1) {}
		
		Reset();
	}
}
