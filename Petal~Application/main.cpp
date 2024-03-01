#include "Petal~Window.h"

#include "Petal~VSDebugOutput.h"

namespace App
{
	class AppWindow : public Petal::Window
	{
		Petal::WindowClass window_class;
	public:
		AppWindow() : Window()
		{
			window_class.Register();
			Create(window_class.ClassAtom());
			UpdateTitle(Petal_TStr("Hello Visual Studio 2022 Community Preview"));
			Show();
			UpdateWindow();
			Petal::Debug::println("Hello Visual Studio 2022 Community Preview");
		}
	};

	int main()
	{
		AppWindow app{};
		return Petal::MessageLoop();
	}
}
