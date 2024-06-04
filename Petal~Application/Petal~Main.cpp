#include "Petal~Main.h"

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~VSDebugOutput.h"

#include <Windows.h>
#include <vector>

namespace
{
	namespace PetalUnnamed
	{
		namespace Abstract
		{
			class EntryPointArguments
			{
			protected:
				constexpr void Enable() noexcept { this->valid = true; }
				constexpr void SetInitialized() noexcept { this->initialized = true; }
				constexpr Petal::boolean Initialized() const noexcept { return this->initialized; }
			public:
				constexpr const Petal::boolean& Valid() const noexcept { return this->valid; }
			private:
				Petal::boolean valid{ false };
				Petal::boolean initialized{ false };
			};
		}

		namespace Main
		{
			class Arguments final : public Abstract::EntryPointArguments
			{
			public:
				constexpr const int& Argc() const noexcept
				{
					return this->argc;
				}
				constexpr const Petal::ptrc<Petal::ptrc<Petal::TChar>>& Argv() const noexcept
				{
					return this->argv;
				}
				constexpr const Petal::ptrc<Petal::ptrc<Petal::TChar>>& Envp() const noexcept
				{
					return this->envp;
				}
			public:
				constexpr Arguments() = default;
			public:
				constexpr void Initialize(int argc, const Petal::ptrc<Petal::TChar> argv[], const Petal::ptrc<Petal::TChar> envp[]) noexcept // 初始化为有效对象
				{
					if (this->Initialized() == true)
					{
						Petal_VSDbgT("[Petal] Invalid call: Petal::Main::Arguments::Initialize\r\n");
						return;
					}
					this->argc = argc;
					this->argv = argv;
					this->envp = envp;
					this->Enable();
					this->SetInitialized();
				}
				constexpr void Initialize() noexcept
				{
					if (this->Initialized() == true)
					{
						Petal_VSDbgT("[Petal] Invalid call: Petal::Main::Arguments::Initialize\r\n");
						return;
					}
					this->SetInitialized();
				}
			private:
				Petal::ptrc<Petal::ptrc<Petal::TChar>> argv{ nullptr };
				Petal::ptrc<Petal::ptrc<Petal::TChar>> envp{ nullptr };
				int argc{ 0 };
			};
		}

		namespace WinMain
		{
			class Arguments final : public Abstract::EntryPointArguments
			{
			public:
				constexpr const Petal::win32hins& HIns() const noexcept
				{
					return this->instance;
				}
				constexpr const Petal::ptrc<Petal::TChar>& CmdLine() const noexcept
				{
					return this->cmd_line;
				}
				constexpr const Petal::win32int& CmdShow() const noexcept
				{
					return this->cmd_show;
				}
			public:
				constexpr Arguments() = default;
			public:
				constexpr void Initialize(Petal::win32hins instance_handle, Petal::ptrc<Petal::TChar> cmd_line, Petal::win32int cmd_show) noexcept // 初始化为有效对象
				{
					if (this->Initialized() == true)
					{
						Petal_VSDbgT("[Petal] Invalid call: Petal::WinMain::Arguments::Initialize\r\n");
						return;
					}
					this->instance = instance_handle;
					this->cmd_line = cmd_line;
					this->cmd_show = cmd_show;
					this->Enable();
					this->SetInitialized();
				}
				constexpr void Initialize() noexcept
				{
					if (this->Initialized() == true)
					{
						Petal_VSDbgT("[Petal] Invalid call: Petal::WinMain::Arguments::Initialize\r\n");
						return;
					}
					this->SetInitialized();
				}
			private:
				Petal::win32hins instance{};
				Petal::ptrc<Petal::TChar> cmd_line{};
				Petal::win32int cmd_show{ 0 };
			};
		}

		namespace Main
		{
			Arguments arguments{};
		}

		namespace WinMain
		{
			Arguments arguments{};
		}
	}
}

namespace Petal::Main
{
	const int& argc{ PetalUnnamed::Main::arguments.Argc() };
	const ptrc<ptrc<TChar>>& argv{ PetalUnnamed::Main::arguments.Argv() };
	const ptrc<ptrc<TChar>>& envp{ PetalUnnamed::Main::arguments.Envp() };
	const boolean& valid{ PetalUnnamed::Main::arguments.Valid() };
}

namespace Petal::WinMain
{
	const win32hins& hins{ PetalUnnamed::WinMain::arguments.HIns() };
	const ptrc<TChar>& cmd_line{ PetalUnnamed::WinMain::arguments.CmdLine() };
	const win32int& cmd_show{ PetalUnnamed::WinMain::arguments.CmdShow() };
	const boolean& valid{ PetalUnnamed::WinMain::arguments.Valid() };
	win32hins HIns() noexcept
	{
#ifdef Petal_Enable_Unicode
		return ::GetModuleHandleW(nullptr);
#else
		return ::GetModuleHandleA(nullptr);
#endif
	}
	TCStringRef CmdLine() noexcept
	{
		static ptrc<TChar> cmd_line
		{
#ifdef Petal_Enable_Unicode
				::GetCommandLineW()
#else
				::GetCommandLineA()
#endif
		};
		static tsize length{ ::std::char_traits<TChar>::length(cmd_line) };
		return { cmd_line, length };
	}
}

#ifdef Petal_Enable_PetalMain

#include "Petal~UserEntrance.h"

#endif

namespace Petal::UserEntrance
{
	extern const fptr<int> user_main;
	extern const ptrc<TChar> user_main_name;
	extern const ptrc<DbgChar> user_main_name_dbgc;
}

namespace
{
	namespace PetalUnnamed
	{
		void DebugPrintMainInfo(Petal::ptrc<Petal::DbgChar> entry_point, Petal::ptrc<Petal::DbgChar> main_space) noexcept
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Entry point: {}\r\n"), entry_point).c_str());
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Arguments in Petal::{} is valid now\r\n"), main_space).c_str());
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] User entrance: {}\r\n"), Petal::UserEntrance::user_main_name_dbgc).c_str());
		}

		bool IsEntryFnHasBeenCalled()
		{
			static bool entry_function_has_been_called{ false };
			if (entry_function_has_been_called)
			{
				Petal_VSDbgT("[Petal] Warning: calling entry function is invalid\r\n");
				return true;
			}
			entry_function_has_been_called = true;
			return false;
		}

		int main(
			Petal::win32hins instance,
			Petal::ptrc<Petal::TChar> cmd_line,
			Petal::win32int cmd_show,
			Petal::ptrc<Petal::DbgChar> entry_point,
			Petal::ptrc<Petal::DbgChar> main_space)
		{
			if (IsEntryFnHasBeenCalled()) return -1;
			Main::arguments.Initialize();
			WinMain::arguments.Initialize(instance, cmd_line, cmd_show);
			DebugPrintMainInfo(entry_point, main_space);
			return Petal::UserEntrance::user_main();
		}

		int main(
			int argc,
			Petal::ptr<Petal::TChar> argv[],
			Petal::ptr<Petal::TChar> envp[],
			Petal::ptrc<Petal::DbgChar> entry_point,
			Petal::ptrc<Petal::DbgChar> main_space)
		{
			if (IsEntryFnHasBeenCalled()) return -1;
			Main::arguments.Initialize(argc, argv, envp);
			WinMain::arguments.Initialize();
			DebugPrintMainInfo(entry_point, main_space);
			return Petal::UserEntrance::user_main();
		}
	}
}

#if defined(Petal_Enable_Unicode)

INT WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPWSTR cmd_line, _In_ INT cmd_show)
{
	return PetalUnnamed::main(instance, cmd_line, cmd_show, Petal_DbgStr("wWinMain"), Petal_DbgStr("WinMain"));
}

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	return PetalUnnamed::main(argc, argv, envp, Petal_DbgStr("wmain"), Petal_DbgStr("Main"));
}

#else

INT WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPSTR cmd_line, _In_ INT cmd_show)
{
	return PetalUnnamed::main(instance, cmd_line, cmd_show, Petal_DbgStr("WinMain"), Petal_DbgStr("WinMain"));
}

int main(int argc, char* argv[], char* envp[])
{
	return PetalUnnamed::main(argc, argv, envp, Petal_DbgStr("main"), Petal_DbgStr("Main"));
}

#endif
