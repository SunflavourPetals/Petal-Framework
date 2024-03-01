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
		using namespace Petal;
		struct myt : ::std::char_traits<char>{};
		BasicString<char, myt> s = "cnm";

		CStringRef a = ""_csr;

		using namespace Petal;
		auto csr = "string"_csr;
		using csr_type = decltype(csr);
		csr_type::hasher hasher{}; // CStringRefHash<csr_type::value_type>
		auto hash_val = hasher(csr);

		a = s;
		if ("cnm"_csr)
		{
			Petal::Debug::println("cnmmmm");
		}

		AppWindow app{};
		return Petal::MessageLoop();
	}
}
