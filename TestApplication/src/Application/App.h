#pragma once

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
		uint8_t mReturnCode;
		bool mRunning;
		bool mWarmedUp;
	public:
		App() : mRunning(false), mWarmedUp(false) {}
		virtual ~App() = default;
		void WarmUp();
		void Run();

	protected:
		virtual bool LoadAssets() = 0;
		virtual bool InitializeGUI() = 0;
		virtual bool DeinitializeGUI() = 0;
		virtual bool UpdateGUI() = 0;
		virtual bool UpdateInput() = 0;
		virtual bool UpdateLogic() = 0;
		virtual bool UpdateScreen() = 0;
		virtual bool FinishFrame() = 0;
	};
}