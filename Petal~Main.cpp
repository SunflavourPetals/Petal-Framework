#include "Petal~Main.h"

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~VSDebugOutput.h"

#include <Windows.h>
#include <vector>

namespace
{
	namespace PetalUnnamed::Abstract
	{
		// 抽象的 入口函数参数 类
		class EntryPointArguments
		{
		protected:
			constexpr void Enable() noexcept { this->valid = true; }
			constexpr void SetFlagInit() noexcept { this->initialized = true; }
			constexpr Petal::boolean Initialized() const noexcept { return this->initialized; }
		public:
			// 返回引用是为了使外部能观测到 valid 属性，这样就不需要暴露整个对象了。
			// 此后派生的 Main::Arguments 的成员属性的获取函数也同样返回 const Ty& 类型。
			constexpr const Petal::boolean& Valid() const noexcept { return this->valid; }
		public:
			virtual ~EntryPointArguments() = default;
		private:
			Petal::boolean valid{ false }; // 标识对象是否可用
			Petal::boolean initialized{ false }; // 标识对象是否已被初始化
		};
	}

	namespace PetalUnnamed::Main
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
			constexpr void Init(int argc, const Petal::ptrc<Petal::TChar> argv[], const Petal::ptrc<Petal::TChar> envp[]) noexcept // 初始化为有效对象
			{
				if (this->Initialized() == true) // 初始化仅能作用于未被初始化的对象。
				{
					Petal_VSDbgT("[Petal] Invalid call: Petal::Main::Arguments::Init\r\n");
					return;
				}
				this->argc = argc;
				this->argv = argv;
				this->envp = envp;
				this->Enable(); // 启用，将 valid 属性设置为 true。
				this->SetFlagInit(); // 完成初始化，将 initialized 属性设置为 true。
			}
			constexpr void InitAsInvalid() noexcept // 初始化为无效对象
			{
				if (this->Initialized() == true) // 初始化仅能作用于未被初始化的对象。
				{
					Petal_VSDbgT("[Petal] Invalid call: Petal::Main::Arguments::InitAsInvalid\r\n");
					return;
				}
				this->SetFlagInit(); // 完成初始化，将 initialized 属性设置为 true。
			}
		private:
			Petal::ptrc<Petal::ptrc<Petal::TChar>> argv{ nullptr };
			Petal::ptrc<Petal::ptrc<Petal::TChar>> envp{ nullptr };
			int argc{ 0 };
		};
	}

	namespace PetalUnnamed::WinMain
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
			constexpr void Init(Petal::win32hins instance_handle, Petal::ptrc<Petal::TChar> cmd_line, Petal::win32int cmd_show) noexcept // 初始化为有效对象
			{
				if (this->Initialized() == true) // 初始化仅能作用于未被初始化的对象。
				{
					Petal_VSDbgT("[Petal] Invalid call: Petal::WinMain::Arguments::Init\r\n");
					return;
				}
				this->instance = instance_handle;
				this->cmd_line = cmd_line;
				this->cmd_show = cmd_show;
				this->Enable(); // 启用，将 valid 属性设置为 true。
				this->SetFlagInit(); // 完成初始化，将 initialized 属性设置为 true。
			}
			constexpr void InitAsInvalid() noexcept // 初始化为无效对象
			{
				if (this->Initialized() == true) // 初始化仅能作用于未被初始化的对象。
				{
					Petal_VSDbgT("[Petal] Invalid call: Petal::WinMain::Arguments::InitAsInvalid\r\n");
					return;
				}
				this->SetFlagInit(); // 完成初始化，将 initialized 属性设置为 true。
			}
		private:
			Petal::win32hins instance{ nullptr };
			Petal::ptrc<Petal::TChar> cmd_line{ nullptr };
			Petal::win32int cmd_show{ 0 };
		};
	}

	namespace PetalUnnamed
	{
		class Protection final // 保护入口函数，使其不被递归调用
		{
		public:
			void Use() { this->pt_called = true; } // 设置程序状态为已进入入口函数
			bool Used() { return this->pt_called; } // 是否已进入入口函数
			void VSDebugOutputWarning() // 输出警告
			{
				Petal_VSDbgT("[Petal] Warning: calling entry function is invalid\r\n");
			}
		private:
			Petal::boolean pt_called{ false }; // 标识是否已进入入口函数
		};
		Protection protection{};
	}

	namespace PetalUnnamed::Main
	{
		Arguments arguments{};
	}

	namespace PetalUnnamed::WinMain
	{
		Arguments arguments{};
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
	extern const fptr<int> pt_user_main;
	extern const ptrc<TChar> pt_user_main_name;
	extern const ptrc<DbgChar> pt_user_main_name_dbgc;
}


namespace
{
	namespace PetalUnnamed::XMain
	{
		void VSDebugOutput(Petal::ptrc<Petal::DbgChar> entry_point, Petal::ptrc<Petal::DbgChar> main_space) noexcept
		{
			try
			{
				Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Entry point: {}\r\n"), entry_point).c_str());
				Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Arguments in Petal::{} is valid now\r\n"), main_space).c_str());
				Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] User entrance: {}\r\n"), Petal::UserEntrance::pt_user_main_name_dbgc).c_str());
			}
			catch (const ::std::exception&) {}
		}
	}
}

#if defined(Petal_Enable_Unicode)

INT WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPWSTR cmd_line, _In_ INT cmd_show)
{
	if (PetalUnnamed::protection.Used() == true)
	{
		PetalUnnamed::protection.VSDebugOutputWarning();
		return -1;
	}
	PetalUnnamed::protection.Use();
	PetalUnnamed::Main::arguments.InitAsInvalid();
	PetalUnnamed::WinMain::arguments.Init(instance, cmd_line, cmd_show);
	PetalUnnamed::XMain::VSDebugOutput(Petal_DbgStr("wWinMain"), Petal_DbgStr("WinMain"));
	return Petal::UserEntrance::pt_user_main();
}

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	if (PetalUnnamed::protection.Used() == true)
	{
		PetalUnnamed::protection.VSDebugOutputWarning();
		return -1;
	}
	PetalUnnamed::protection.Use();
	PetalUnnamed::Main::arguments.Init(argc, argv, envp);
	PetalUnnamed::WinMain::arguments.InitAsInvalid();
	PetalUnnamed::XMain::VSDebugOutput(Petal_DbgStr("wmain"), Petal_DbgStr("Main"));
	return Petal::UserEntrance::pt_user_main();
}

#else

INT WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPSTR cmd_line, _In_ INT cmd_show)
{
	if (PetalUnnamed::protection.Used() == true)
	{
		PetalUnnamed::protection.VSDebugOutputWarning();
		return -1;
	}
	PetalUnnamed::protection.Use();
	PetalUnnamed::Main::arguments.InitAsInvalid();
	PetalUnnamed::WinMain::arguments.Init(instance, cmd_line, cmd_show);
	PetalUnnamed::XMain::VSDebugOutput(Petal_DbgStr("WinMain"), Petal_DbgStr("WinMain"));
	return Petal::UserEntrance::pt_user_main();
}

int main(int argc, char* argv[], char* envp[])
{
	if (PetalUnnamed::protection.Used() == true)
	{
		PetalUnnamed::protection.VSDebugOutputWarning();
		return -1;
	}
	PetalUnnamed::protection.Use();
	PetalUnnamed::Main::arguments.Init(argc, argv, envp);
	PetalUnnamed::WinMain::arguments.InitAsInvalid();
	PetalUnnamed::XMain::VSDebugOutput(Petal_DbgStr("main"), Petal_DbgStr("Main"));
	return Petal::UserEntrance::pt_user_main();
}

#endif
