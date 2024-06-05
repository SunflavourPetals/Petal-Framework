#pragma once

#ifndef Petal_Header_Main
#define Petal_Header_Main

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~WinTypes.h"

namespace Petal::Main
{
	extern const int& argc;
	extern const ptrc<ptrc<TChar>>& argv;
	extern const ptrc<ptrc<TChar>>& envp;
	extern const boolean& valid;
}

namespace Petal::WinMain
{
	extern const win32hins& hins;
	extern const ptrc<TChar>& cmd_line;
	extern const win32int& cmd_show;
	extern const boolean& valid;

	inline win32hins HIns() noexcept
	{
#ifdef Petal_Enable_Unicode
		return ::GetModuleHandleW(nullptr);
#else
		return ::GetModuleHandleA(nullptr);
#endif
	}
	inline TCStringRef CmdLine() noexcept
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

#ifndef Petal_Enable_PetalMain

#define Petal_SetMainFunc(MainFunc) \
namespace Petal::UserEntrance \
{   \
	extern const fptr<int> user_main{ &MainFunc };  \
	extern const ptrc<TChar> user_main_name{ Petal_TStr(#MainFunc) };  \
	extern const ptrc<DbgChar> user_main_name_dbgc{ Petal_DbgStr(#MainFunc) };  \
}
#define Petal_SetMainClass(MainClass) Petal_SetMainFunc(MainClass::main)

#endif // !Petal_Enable_PetalMain

#endif // !Petal_Header_Main
