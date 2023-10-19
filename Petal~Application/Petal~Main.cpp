#include "Petal~Main.h"

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~VSDebugOutput.h"

#include <Windows.h>

namespace Petal::Abstract
{
	class EntryPointArguments
	{
	protected:
		inline void Enable() noexcept { this->valid = true; }
		inline void SetFlagInit() noexcept { this->initialized = true; }
		inline boolean Initialized() const noexcept { return this->initialized; }
	public:
		inline const boolean& Valid() const noexcept { return this->valid; }
	public:
		inline virtual ~EntryPointArguments() = default;
	private:
		boolean valid{ false };
		boolean initialized{ false };
	};
}

namespace Petal::Main
{
	class Arguments final : public Abstract::EntryPointArguments
	{
	public:
		inline constexpr const int& Argc() const noexcept
		{
			return this->argc;
		}
		inline constexpr const ptrc<ptrc<TChar>>& Argv() const noexcept
		{
			return this->argv;
		}
		inline constexpr const ptrc<ptrc<TChar>>& Envp() const noexcept
		{
			return this->envp;
		}
	public:
		constexpr Arguments() = default;
	public:
		inline void Init(int argc, const ptrc<TChar> argv[], const ptrc<TChar> envp[]) noexcept
		{
			if (this->Initialized() == true)
			{
				Petal_VSDbgT("[Petal] Invalid call: Petal::Main::Arguments::Init\r\n");
				return;
			}
			this->argc = argc;
			this->argv = argv;
			this->envp = envp;
			this->Enable();
			this->SetFlagInit();
		}
		inline void InitAsInvalid() noexcept
		{
			if (this->Initialized() == true)
			{
				Petal_VSDbgT("[Petal] Invalid call: Petal::Main::Arguments::InitAsInvalid\r\n");
				return;
			}
			this->SetFlagInit();
		}
	private:
		ptrc<ptrc<TChar>> argv{ nullptr };
		ptrc<ptrc<TChar>> envp{ nullptr };
		int argc{ 0 };
	};
}

namespace Petal::WinMain
{
	class Arguments final : public Abstract::EntryPointArguments
	{
	public:
		inline constexpr const win32hins& HIns() const noexcept
		{
			return this->instance;
		}
		inline constexpr const ptrc<TChar>& CmdLine() const noexcept
		{
			return this->cmd_line;
		}
		inline constexpr const win32int& CmdShow() const noexcept
		{
			return this->cmd_show;
		}
	public:
		constexpr Arguments() = default;
	public:
		inline void Init(win32hins instance_handle, ptrc<TChar> cmd_line, win32int cmd_show) noexcept
		{
			if (this->Initialized() == true)
			{
				Petal_VSDbgT("[Petal] Invalid call: Petal::WinMain::Arguments::Init\r\n");
				return;
			}
			this->instance = instance_handle;
			this->cmd_line = cmd_line;
			this->cmd_show = cmd_show;
			this->Enable();
			this->SetFlagInit();
		}
		inline void InitAsInvalid() noexcept
		{
			if (this->Initialized() == true)
			{
				Petal_VSDbgT("[Petal] Invalid call: Petal::WinMain::Arguments::InitAsInvalid\r\n");
				return;
			}
			this->SetFlagInit();
		}
	private:
		win32hins instance{ nullptr };
		ptrc<TChar> cmd_line{ nullptr };
		win32int cmd_show{ 0 };
	};
}

namespace Petal::EntryPointProtection
{
	class Protection final
	{
	public:
		inline void Use() { this->pt_called = true; }
		inline operator bool() { return this->pt_called; }
		inline void VSDebugOutputWarning()
		{
			Petal_VSDbgT("[Petal] Warning: calling occupied entry function is invalid\r\n");
		}
	private:
		boolean pt_called{ false };
	};

	static Protection protection{};
}

namespace Petal::Main
{
	Arguments arguments{};
	const int& argc{ arguments.Argc() };
	const ptrc<ptrc<TChar>>& argv{ arguments.Argv() };
	const ptrc<ptrc<TChar>>& envp{ arguments.Envp() };
	const boolean& valid{ arguments.Valid() };
}

namespace Petal::WinMain
{
	Arguments arguments{};
	const win32hins& hins{ arguments.HIns() };
	const ptrc<TChar>& cmd_line{ arguments.CmdLine() };
	const win32int& cmd_show{ arguments.CmdShow() };
	const boolean& valid{ arguments.Valid() };
}

#ifndef Petal_Enable_PetalMain

namespace Petal::UserEntrance
{
	extern const fptr<int> pt_user_main;
	extern const ptrc<TChar> pt_user_main_name;
}

#else

int PetalMain();

#endif // !Petal_Enable_PetalMain

namespace Petal::XMain
{
	inline void VSDebugOutput(ptrc<DbgChar> entry_point, ptrc<DbgChar> main_space)
	{
		Petal_VSDbgT("[Petal] Entry point: ");
		Petal_VSDbg(entry_point);
		Petal_VSDbgT("\r\n");

		Petal_VSDbgT("[Petal] Arguments in Petal::");
		Petal_VSDbg(main_space);
		Petal_VSDbgT(" is valid now\r\n");

		Petal_VSDbgT("[Petal] User entrance: ");
#ifndef Petal_Enable_PetalMain
		Petal_VSDebugOutput(UserEntrance::pt_user_main_name);
#else
		Petal_VSDbgT("PetalMain");
#endif
		Petal_VSDbgT("\r\n");
	}
}

#if defined(Petal_Enable_Unicode)
INT WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPWSTR cmd_line, _In_ INT cmd_show)
{
	if (Petal::EntryPointProtection::protection)
	{
		Petal::EntryPointProtection::protection.VSDebugOutputWarning();
		return -1;
	}
	Petal::EntryPointProtection::protection.Use();
	Petal::Main::arguments.InitAsInvalid();
	Petal::WinMain::arguments.Init(instance, cmd_line, cmd_show);
	Petal::XMain::VSDebugOutput(Petal_DbgStr("wWinMain"), Petal_DbgStr("WinMain"));
#ifndef Petal_Enable_PetalMain
	return Petal::UserEntrance::pt_user_main();
#else
	return ::PetalMain();
#endif
}
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	if (Petal::EntryPointProtection::protection)
	{
		Petal::EntryPointProtection::protection.VSDebugOutputWarning();
		return -1;
	}
	Petal::EntryPointProtection::protection.Use();
	Petal::Main::arguments.Init(argc, argv, envp);
	Petal::WinMain::arguments.InitAsInvalid();
	Petal::XMain::VSDebugOutput(Petal_DbgStr("wmain"), Petal_DbgStr("Main"));
#ifndef Petal_Enable_PetalMain
	return Petal::UserEntrance::pt_user_main();
#else
	return ::PetalMain();
#endif
}
#else
INT WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPSTR cmd_line, _In_ INT cmd_show)
{
	if (Petal::EntryPointProtection::protection)
	{
		Petal::EntryPointProtection::protection.VSDebugOutputWarning();
		return -1;
	}
	Petal::EntryPointProtection::protection.Use();
	Petal::Main::arguments.InitAsInvalid();
	Petal::WinMain::arguments.Init(instance, cmd_line, cmd_show);
	Petal::XMain::VSDebugOutput(Petal_DbgStr("WinMain"), Petal_DbgStr("WinMain"));
#ifndef Petal_Enable_PetalMain
	return Petal::UserEntrance::pt_user_main();
#else
	return ::PetalMain();
#endif
}
int main(int argc, char* argv[], char* envp[])
{
	if (Petal::EntryPointProtection::protection)
	{
		Petal::EntryPointProtection::protection.VSDebugOutputWarning();
		return -1;
	}
	Petal::EntryPointProtection::protection.Use();
	Petal::Main::arguments.Init(argc, argv, envp);
	Petal::WinMain::arguments.InitAsInvalid();
	Petal::XMain::VSDebugOutput(Petal_DbgStr("main"), Petal_DbgStr("Main"));
#ifndef Petal_Enable_PetalMain
	return Petal::UserEntrance::pt_user_main();
#else
	return ::PetalMain();
#endif
}
#endif
