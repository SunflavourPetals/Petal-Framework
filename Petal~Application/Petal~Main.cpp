#include "Petal~Main.h"

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~VSDebugOutput.h"

#include <Windows.h>

namespace Petal::Unnamed::Abstract
{
	namespace
	{
		// ����� ��ں������� ��
		class EntryPointArguments
		{
		protected:
			constexpr void Enable() noexcept { this->valid = true; }
			constexpr void SetFlagInit() noexcept { this->initialized = true; }
			constexpr boolean Initialized() const noexcept { return this->initialized; }
		public:
			// ����������Ϊ��ʹ�ⲿ�ܹ۲⵽ valid ���ԣ������Ͳ���Ҫ��¶���������ˡ�
			// �˺������� Main::Arguments �ĳ�Ա���ԵĻ�ȡ����Ҳͬ������ const Ty& ���͡�
			constexpr const boolean& Valid() const noexcept { return this->valid; }
		public:
			virtual ~EntryPointArguments() = default;
		private:
			boolean valid{ false }; // ��ʶ�Ƿ����
			boolean initialized{ false }; // ��ʶ�Ƿ��ѱ���ʼ��
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
			constexpr void Init(int argc, const ptrc<TChar> argv[], const ptrc<TChar> envp[]) noexcept // ��ʼ��
			{
				if (this->Initialized() == true) // ��ʼ������������δ����ʼ���Ķ���
				{
					Petal_VSDbgT("[Petal] Invalid call: Petal::Main::Arguments::Init\r\n");
					return;
				}
				this->argc = argc;
				this->argv = argv;
				this->envp = envp;
				this->Enable(); // ���ã��� valid ��������Ϊ true��
				this->SetFlagInit(); // ��ɳ�ʼ������ initialized ��������Ϊ true��
			}
			constexpr void InitAsInvalid() noexcept // ��ʼ��Ϊ��Ч����
			{
				if (this->Initialized() == true) // ��ʼ������������δ����ʼ���Ķ���
				{
					Petal_VSDbgT("[Petal] Invalid call: Petal::Main::Arguments::InitAsInvalid\r\n");
					return;
				}
				this->SetFlagInit(); // ��ɳ�ʼ������ initialized ��������Ϊ true��
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
				this->instance = Petal::WinMain::HIns(); // ��ʼ�� instance ����
			}
		public:
			constexpr void Init(win32hins instance_handle, ptrc<TChar> cmd_line, win32int cmd_show) noexcept // ��ʼ��
			{
				if (this->Initialized() == true) // ��ʼ������������δ����ʼ���Ķ���
				{
					Petal_VSDbgT("[Petal] Invalid call: Petal::WinMain::Arguments::Init\r\n");
					return;
				}
				this->instance = instance_handle;
				this->cmd_line = cmd_line;
				this->cmd_show = cmd_show;
				this->Enable(); // ���ã��� valid ��������Ϊ true��
				this->SetFlagInit(); // ��ɳ�ʼ������ initialized ��������Ϊ true��
			}
			constexpr void InitAsInvalid() noexcept // ��ʼ��Ϊ��Ч����
			{
				if (this->Initialized() == true) // ��ʼ������������δ����ʼ���Ķ���
				{
					Petal_VSDbgT("[Petal] Invalid call: Petal::WinMain::Arguments::InitAsInvalid\r\n");
					return;
				}
				this->SetFlagInit(); // ��ɳ�ʼ������ initialized ��������Ϊ true��
			}
		private:
			win32hins instance{ nullptr }; // ������ Init ���� InitAsInvalid��instance �����ڳ�ʼ����ʼ����Ч
			ptrc<TChar> cmd_line{ nullptr };
			win32int cmd_show{ 0 };
		};
	}
}

namespace Petal::Unnamed
{
	namespace
	{
		class Protection final // ������ں�����ʹ�䲻���ݹ����
		{
		public:
			void Use() { this->pt_called = true; } // ���ó���״̬Ϊ�ѽ�����ں���
			bool Used() { return this->pt_called; } // �Ƿ��ѽ�����ں���
			void VSDebugOutputWarning() // �������
			{
				Petal_VSDbgT("[Petal] Warning: calling entry function is invalid\r\n");
			}
		private:
			boolean pt_called{ false }; // ��ʶ�Ƿ��ѽ�����ں���
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
