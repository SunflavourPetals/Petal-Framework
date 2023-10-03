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
}

#define Petal_SetMainFunc(MainFunc) \
namespace Petal::UserEntrance \
{   \
	extern const fptr<int> pt_user_main{ &::MainFunc };  \
	extern const ptrc<TChar> pt_user_main_name{ Petal_TStr(#MainFunc) };  \
}

#define Petal_SetMainClass(MainClass) Petal_SetMainFunc(MainClass::main)
#define Petal_SetMainSpace(MainSpace) Petal_SetMainClass(MainSpace)
#define Petal_SetDefaultMainClass     Petal_SetMainClass(Main)

#endif // !Petal_Header_Main
