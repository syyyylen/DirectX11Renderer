#include "AppWindow.h"
#include "../GameEngine/InputSystem/InputSystem.h"

int main()
{
	try
	{
		GraphicsEngine::Create();
		InputSystem::Create();
	}
	catch (...) { return -1; }

	try
	{
		AppWindow app;
		while(app.IsRunning());
	}
	catch (...)
	{
		InputSystem::Release();
		GraphicsEngine::Release();
		return -1;
	}

	InputSystem::Release();
	GraphicsEngine::Release();

	return 0;
}
