#include "Petal~Main.h"
#include "Petal~Window.h"
#include "Petal~VSDebugOutput.h"

#include <format>

class Main
{
public:
	static int main()
	{
		using namespace Petal;
		class MyWindow : public Window
		{
			void CreateEvent(CreateMessage& e) noexcept override
			{
				dout + std::format("Create: {}", e.continue_creation) + ln;
			}
		} w;
		w.Create(WindowClassArgs{ Petal_TStr("my window class") }.Register().class_atom);
		w.Show();
		return MessageLoop();
	}
};

Petal_SetMainClass(Main);
