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
			if (instance)
				throw std::exception{ "Instance has already existed" }; // ������ͬʱ���ڶ��ʵ��
			instance = this;
		}
		~AppWindow()
		{
			if (Valid())
				this->Destroy(); // ���쳣���������δ����CloseEventʱ���ֶ�Destroy����
			instance = nullptr;
		}
		void Init()
		{
			this->Create(Petal::WindowClassArgs{ Petal_TStr("My app wnd class"), my_app_proc }.Register().class_atom);
			this->UpdateTitle(Petal_TStr("Hello Visual Studio 2022 Community Preview"));
			this->Show();
			this->UpdateWindow();
			Petal::Debug::println("Hello Visual Studio 2022 Community Preview");
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
		AppWindow double_instance_make_except{}; // throw
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