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
		bool _running;
	public:
		App() : _running(true) {}
		virtual ~App() = default;
		void Run();

	protected:
		virtual bool UpdateInput() = 0;
		virtual bool UpdateLogic() = 0;
		virtual bool UpdateScreen() = 0;
	};
}