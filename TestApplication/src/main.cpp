#include "Application/TestApp.h"

int main()
{
	Application::App* app = new Application::TestApp(1280.0f, 720.0f);
	uint8_t returnCode = app->Run();
	delete app;
	return returnCode;
}