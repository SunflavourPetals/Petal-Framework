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
	win32hins HIns() noexcept;
	ptrc<TChar> CmdLine() noexcept;
}

#ifndef Petal_Enable_PetalMain

#define Petal_SetMainFunc(MainFunc) \
namespace Petal::UserEntrance \
{   \
	extern const fptr<int> pt_user_main{ &::MainFunc };  \
	extern const ptrc<TChar> pt_user_main_name{ Petal_TStr(#MainFunc) };  \
	extern const ptrc<DbgChar> pt_user_main_name_dbgc{ Petal_DbgStr(#MainFunc) };  \
}
#define Petal_SetMainClass(MainClass) Petal_SetMainFunc(MainClass::main)
#define Petal_SetMainSpace(MainSpace) Petal_SetMainClass(MainSpace)
#define Petal_SetDefaultMainClass     Petal_SetMainClass(Main)

#endif // !Petal_Enable_PetalMain

#endif // !Petal_Header_Main
