#include "App.h"

namespace Application
{
	void App::WarmUp()
	{
		_warmedUp = _warmedUp || InitializeGUI();
		_warmedUp = _warmedUp && LoadAssets();
		_running = _warmedUp;
	}

	void App::Run()
	{
		if (!_warmedUp) WarmUp();
		if (!_running) return;

		while (_running)
		{
			_running = _running && UpdateInput();
			_running = _running && UpdateLogic();
			_running = _running && UpdateScreen();
			_running = _running && UpdateGUI();
			_running = _running && FinishFrame();
		}
		return;
	}
}