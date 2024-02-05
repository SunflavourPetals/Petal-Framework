#include "Petal~Main.h"
#include "Petal~Window.h"
#include "Petal~VSDebugOutput.h"

namespace App
{
	class AppWindow : public Petal::Window
	{
	public:
		AppWindow() : Window()
		{

			Petal::Debug::println("Hello Visual Studio 2022 Community Preview");
		}
		~AppWindow()
		{
			if (WindowHandle())
				Destroy();
		}
	};
	AppWindow& MyWindow()
	{
		static AppWindow my_window{};
		return my_window;
	}
//	Petal_Decl_CommonWndProcessVRef(my_win_proc, MyWindow, false);
	Petal_Decl_WndProc(my_win_proc_old)
	{
		return Petal::CommonWindowProcess(MyWindow, window_handle, message, w_param, l_param);
	}
	int main() try
	{
		auto& app = MyWindow();

		app.Create(Petal::WindowClassArgs{ Petal_TStr("My app wnd class"), &my_win_proc_old }.Register().class_atom);
		
		app.UpdateTitle(Petal_TStr("Hello Visual Studio 2022 Community Preview"));

		app.Show();

		Petal::MessageLoop();
		
		return 0;
	}
	catch (...)
	{
		Petal::Debug::println("exception!");
		
		return -1;
	}
}

Petal_SetMainSpace(App);