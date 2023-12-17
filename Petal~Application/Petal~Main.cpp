#include "Petal~Main.h"

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~VSDebugOutput.h"

#include <Windows.h>

namespace Petal::Unnamed::Abstract
{
	namespace
	{
		// 抽象的 入口函数参数 类
		class EntryPointArguments
		{
		protected:
			constexpr void Enable() noexcept { this->valid = true; }
			constexpr void SetFlagInit() noexcept { this->initialized = true; }
			constexpr boolean Initialized() const noexcept { return this->initialized; }
		public:
			// 返回引用是为了使外部能观测到 valid 属性，这样就不需要暴露整个对象了。
			// 此后派生的 Main::Arguments 的成员属性的获取函数也同样返回 const Ty& 类型。
			constexpr const boolean& Valid() const noexcept { return this->valid; }
		public:
			virtual ~EntryPointArguments() = default;
		private:
			boolean valid{ false }; // 标识是否可用
			boolean initialized{ false }; // 标识是否已被初始化
		};
	}
}

namespace Petal::Unnamed::Main
{
	namespace
	{
		class Arguments final : public Abstract::EntryPointArguments
		{
		public:
			constexpr const int& Argc() const noexcept
			{
				return this->argc;
			}
			constexpr const ptrc<ptrc<TChar>>& Argv() const noexcept
			{
				return this->argv;
			}
			constexpr const ptrc<ptrc<TChar>>& Envp() const noexcept
			{
				return this->envp;
			}
		public:
			constexpr Arguments() = default;
		public:
			constexpr void Init(int argc, const ptrc<TChar> argv[], const ptrc<TChar> envp[]) noexcept // 初始化
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
			ptrc<ptrc<TChar>> argv{ nullptr };
			ptrc<ptrc<TChar>> envp{ nullptr };
			int argc{ 0 };
		};
	}
}

namespace Petal::Unnamed::WinMain
{
	namespace
	{
		class Arguments final : public Abstract::EntryPointArguments
		{
		public:
			constexpr const win32hins& HIns() const noexcept
			{
				return this->instance;
			}
			constexpr const ptrc<TChar>& CmdLine() const noexcept
			{
				return this->cmd_line;
			}
			constexpr const win32int& CmdShow() const noexcept
			{
				return this->cmd_show;
			}
		public:
			Arguments()
			{
				this->instance = Petal::WinMain::HIns(); // 初始化 instance 属性
			}
		public:
			constexpr void Init(win32hins instance_handle, ptrc<TChar> cmd_line, win32int cmd_show) noexcept // 初始化
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
			win32hins instance{ nullptr }; // 不管是 Init 还是 InitAsInvalid，instance 属性在初始化后始终有效
			ptrc<TChar> cmd_line{ nullptr };
			win32int cmd_show{ 0 };
		};
	}
}

namespace Petal::Unnamed
{
	namespace
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
			boolean pt_called{ false }; // 标识是否已进入入口函数
		};
		Protection protection{};
	}
}

namespace Petal::Unnamed::Main
{
	namespace
	{
		Arguments arguments{};
	}
}

namespace Petal::Main
{
	const int& argc{ Petal::Unnamed::Main::arguments.Argc() };
	const ptrc<ptrc<TChar>>& argv{ Petal::Unnamed::Main::arguments.Argv() };
	const ptrc<ptrc<TChar>>& envp{ Petal::Unnamed::Main::arguments.Envp() };
	const boolean& valid{ Petal::Unnamed::Main::arguments.Valid() };
}

namespace Petal::Unnamed::WinMain
{
	namespace
	{
		Arguments arguments{};
	}
}

namespace Petal::WinMain
{
	const win32hins& hins{ Petal::Unnamed::WinMain::arguments.HIns() };
	const ptrc<TChar>& cmd_line{ Petal::Unnamed::WinMain::arguments.CmdLine() };
	const win32int& cmd_show{ Petal::Unnamed::WinMain::arguments.CmdShow() };
	const boolean& valid{ Petal::Unnamed::WinMain::arguments.Valid() };
	win32hins HIns() noexcept
	{
#ifdef Petal_Enable_Unicode
		return ::GetModuleHandleW(nullptr);
#else
		return ::GetModuleHandleA(nullptr);
#endif
	}
}

#ifndef Petal_Enable_PetalMain

namespace Petal::UserEntrance
{
	extern const fptr<int> pt_user_main;
	extern const ptrc<TChar> pt_user_main_name;
	extern const ptrc<DbgChar> pt_user_main_name_dbgc;
}

#else

int PetalMain();

#endif // !Petal_Enable_PetalMain

namespace Petal::Unnamed::XMain
{
	namespace
	{
		void VSDebugOutput(ptrc<DbgChar> entry_point, ptrc<DbgChar> main_space)
		{
			try
			{
				Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Entry point: {}\r\n"), entry_point).c_str());
				Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Arguments in Petal::{} is valid now\r\n"), main_space).c_str());
#ifndef Petal_Enable_PetalMain
				Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] User entrance: {}\r\n"), UserEntrance::pt_user_main_name_dbgc).c_str());
#else
				Petal_VSDbgT("[Petal] User entrance: PetalMain\r\n");
#endif
			}
			catch (const ::std::exception&) {}
		}
	}
}

#if defined(Petal_Enable_Unicode)
INT WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPWSTR cmd_line, _In_ INT cmd_show)
{
	if (Petal::Unnamed::protection.Used() == true)
	{
		Petal::Unnamed::protection.VSDebugOutputWarning();
		return -1;
	}
	Petal::Unnamed::protection.Use();
	Petal::Unnamed::Main::arguments.InitAsInvalid();
	Petal::Unnamed::WinMain::arguments.Init(instance, cmd_line, cmd_show);
	Petal::Unnamed::XMain::VSDebugOutput(Petal_DbgStr("wWinMain"), Petal_DbgStr("WinMain"));
#ifndef Petal_Enable_PetalMain
	return Petal::UserEntrance::pt_user_main();
#else
	return ::PetalMain();
#endif
}
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	if (Petal::Unnamed::protection.Used() == true)
	{
		Petal::Unnamed::protection.VSDebugOutputWarning();
		return -1;
	}
	Petal::Unnamed::protection.Use();
	Petal::Unnamed::Main::arguments.Init(argc, argv, envp);
	Petal::Unnamed::WinMain::arguments.InitAsInvalid();
	Petal::Unnamed::XMain::VSDebugOutput(Petal_DbgStr("wmain"), Petal_DbgStr("Main"));
#ifndef Petal_Enable_PetalMain
	return Petal::UserEntrance::pt_user_main();
#else
	return ::PetalMain();
#endif
}
#else
INT WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPSTR cmd_line, _In_ INT cmd_show)
{
	if (Petal::Unnamed::protection.Used() == true)
	{
		Petal::Unnamed::protection.VSDebugOutputWarning();
		return -1;
	}
	Petal::Unnamed::protection.Use();
	Petal::Unnamed::Main::arguments.InitAsInvalid();
	Petal::Unnamed::WinMain::arguments.Init(instance, cmd_line, cmd_show);
	Petal::Unnamed::XMain::VSDebugOutput(Petal_DbgStr("WinMain"), Petal_DbgStr("WinMain"));
#ifndef Petal_Enable_PetalMain
	return Petal::UserEntrance::pt_user_main();
#else
	return ::PetalMain();
#endif
}
int main(int argc, char* argv[], char* envp[])
{
	if (Petal::Unnamed::protection.Used() == true)
	{
		Petal::Unnamed::protection.VSDebugOutputWarning();
		return -1;
	}
	Petal::Unnamed::protection.Use();
	Petal::Unnamed::Main::arguments.Init(argc, argv, envp);
	Petal::Unnamed::WinMain::arguments.InitAsInvalid();
	Petal::Unnamed::XMain::VSDebugOutput(Petal_DbgStr("main"), Petal_DbgStr("Main"));
#ifndef Petal_Enable_PetalMain
	return Petal::UserEntrance::pt_user_main();
#else
	return ::PetalMain();
#endif
}
#endif
