#ifndef MSE_MSE_H
#define MSE_MSE_H

// load core
#include <mse/core.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/windows/window_manager.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer_manager.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/windows/layers/gui/gui.h>
#include <mse/systems/application/application.h>

// Entry point
int main(int argv, char** argc)
{
//	MSE_LOG("Console logger online");
	MSE_CORE_LOG("Engine online");
	
	mse::Application* app = mse::CreateApplication();
	MSE_CORE_LOG("Application constructed");
	
	app->Run();
	delete app;
	
	MSE_CORE_LOG("Application destroyed");
	MSE_CORE_LOG("Engine offline");
	
	return 0;
}

#endif
