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
			instance = this;
		}
		void Init()
		{
			this->Create(Petal::WindowClassArgs{ Petal_TStr("My app wnd class"), my_app_proc }.Register().class_atom);
			this->UpdateTitle(Petal_TStr("Hello Visual Studio 2022 Community Preview"));
			this->Show();
			this->UpdateWindow();
			Petal::Debug::println("Hello Visual Studio 2022 Community Preview");
		}
		~AppWindow()
		{
			instance = nullptr;
		}
		static inline AppWindow* instance{ nullptr };
		static Petal_Decl_WndProc(my_app_proc)
		{
			return Petal::CommonWindowProcess(instance, window_handle, message, w_param, l_param);
		}
	};

	int main() try
	{
		AppWindow app{};
		app.Init();
		return Petal::MessageLoop();
	}
	catch (const std::exception& e)
	{
		Petal::Debug::println("[Exception] {}", e.what());
		return -1;
	}
}

Petal_SetMainSpace(App);