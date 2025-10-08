#ifndef MSE_UTILS_FILEIO_H
#define MSE_UTILS_FILEIO_H

#include <mse/common.h>

namespace mse
{
	class FileIO
    {
    public:
        //static void ConstructFileSystem();
        
        static std::string GetRawText(std::string filename);
        
        static bool WriteRawText(const std::string& filename, const char* text);
        
        static std::string GetName(std::string filepath);
    };
}

#endif
