#include "Petal~Window.h"
#include "Petal~VSDebugOutput.h"

namespace App
{
	class AppWindow : public Petal::Window // comment
	{
	public:
		AppWindow() : Window()  /* comment */
		{
			window_class.Register();
			Create(window_class.ClassAtom());
			UpdateTitle(Petal_TStr("Hello Visual Studio 2022 Community Preview"));
			Show();
			UpdateWindow();
			Petal::Debug::println("Hello Visual Studio 2022 Community Preview");
		}
		~AppWindow()
		{
			if (this->Valid())
			{
				this->Destroy(); // bind life circle
				Petal::ExitMessageLoop();
			}
		}
	private:
		Petal::WindowClass window_class;
	};

	int main()
	{
		AppWindow app{};
		return Petal::MessageLoop();
	}
}
