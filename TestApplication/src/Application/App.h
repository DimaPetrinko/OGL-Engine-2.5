#pragma once

#include <iostream>
#include "ExitCodes.h"

#ifdef PLATFORM_WIN64
#define WORKING_DIRECTORY ""
#elif PLATFORM_LINUX64
#define WORKING_DIRECTORY "TestApplication/"
#endif

namespace Application
{
	class App
	{
	protected:
		uint8_t mExitCode;
	public:
		App() : mExitCode(EXIT_CODE_NOT_INITIALIZED) {}
		virtual ~App() = default;
		void WarmUp();
		uint8_t Run();

	protected:
		virtual void LoadAssets() = 0;
		virtual void InitializeGUI() = 0;
		virtual void DeinitializeGUI() = 0;
		virtual void UpdateGUI() = 0;
		virtual void UpdateInput() = 0;
		virtual void UpdateLogic() = 0;
		virtual void UpdateScreen() = 0;
		virtual void FinishFrame() = 0;
	};
}