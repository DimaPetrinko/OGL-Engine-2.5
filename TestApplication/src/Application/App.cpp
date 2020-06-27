#include "App.h"

namespace Application
{
	void App::WarmUp()
	{
		CallAndReturnCustom(InitializeGUI(), mExitCode, EXIT_CODE_NOT_INITIALIZED);
		CallAndReturnCustom(LoadAssets(), mExitCode, EXIT_CODE_NOT_INITIALIZED);
		mExitCode = EXIT_CODE_RUNNING;
	}

	uint8_t App::Run()
	{
		if (mExitCode == EXIT_CODE_NOT_INITIALIZED) WarmUp();
		if (mExitCode != EXIT_CODE_RUNNING) return mExitCode;

		while (mExitCode == EXIT_CODE_RUNNING)
		{
			CallAndBreak(UpdateInput(), mExitCode);
			CallAndBreak(UpdateLogic(), mExitCode);
			CallAndBreak(UpdateScreen(), mExitCode);
			CallAndBreak(UpdateGUI(), mExitCode);
			CallAndBreak(FinishFrame(), mExitCode);
		}
		return mExitCode;
	}
}