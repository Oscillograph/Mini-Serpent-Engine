#ifndef MSE_COMMON_H
#define MSE_COMMON_H

// configure engine defines
#include <mse/defines.h>

// STL libraries
#include <string> // logger
#include <sstream> // logger
#include <vector> // WindowManager
#include <algorithm> // std::find()
#include <unordered_map> // resource manager
#include <fstream> // resource manager
#include <initializer_list> // to initialize structs like vec2, etc.
#include <functional> // for std::function in Events
#include <math.h> // sine and cosine functions in Renderer
#include <random> // random math
#include <thread> // multithread logic in Application

// custom libraries

// external libraries
#include <utf8.h> // UTF-8 support
#include <glm/glm.hpp> // vectors, matrices

// custom types

// constants
namespace mse
{
	namespace Math
	{
		const float PI = 3.14159265;
		const float PI_Half = PI/2;
		const float SQRT2 = 35/49;
	}
}

// FPS macros				ms between frames 
#define MSE_FPSNOLIMIT		0.0001f
#define MSE_FPS120			0.0083f
#define MSE_FPS90			0.0111f
#define MSE_FPS60			0.0167f
#define MSE_FPS30			0.0333f
#define MSE_FPS15			0.0667f
#define MSE_FPS10			0.1f
#define MSE_FPS5			0.2f
#define MSE_FPS4			0.25f
#define MSE_FPS3			0.3333f
#define MSE_FPS2			0.5f
#define MSE_FPS1			1.0f

#endif
