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
			this->Create();
			this->UpdateTitle(Petal_TStr("Hello Visual Studio 2022 Community Preview"));
			this->Show();
			this->UpdateWindow();
			Petal::Debug::println("Hello Visual Studio 2022 Community Preview");
		}
	};

	int main()
	{
		AppWindow app{};
		return Petal::MessageLoop();
	}
}

Petal_SetMainSpace(App);
