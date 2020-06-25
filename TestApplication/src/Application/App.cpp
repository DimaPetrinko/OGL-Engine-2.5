#include "App.h"

namespace Application
{
	void App::WarmUp()
	{
		mWarmedUp = mWarmedUp || InitializeGUI();
		mWarmedUp = mWarmedUp && LoadAssets();
		mRunning = mWarmedUp;
	}

	void App::Run()
	{
		if (!mWarmedUp) WarmUp();
		if (!mRunning) return;

		while (mRunning)
		{
			mRunning = mRunning && UpdateInput();
			mRunning = mRunning && UpdateLogic();
			mRunning = mRunning && UpdateScreen();
			mRunning = mRunning && UpdateGUI();
			mRunning = mRunning && FinishFrame();
		}
		return;
	}
}