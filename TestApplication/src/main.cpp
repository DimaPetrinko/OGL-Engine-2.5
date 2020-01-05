#include "Application/TestApp.h"

int main()
{
	Application::App* app = new Application::TestApp(480.0f, 320.0f);
	app->Run();
	delete app;
}