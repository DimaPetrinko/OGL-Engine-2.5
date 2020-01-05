#include "App.h"

namespace Application
{
	void App::Run()
	{
		if (!_running) return;

		while (_running)
		{
			_running = _running && UpdateInput();
			_running = _running && UpdateLogic();
			_running = _running && UpdateScreen();
		}
		return;
	}
}