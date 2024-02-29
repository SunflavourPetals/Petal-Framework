#pragma once

#ifndef Petal_Header_UserEntrance
#define Petal_Header_UserEntrance

#ifdef Petal_Enable_PetalMain

#ifdef Petal_Main_Config
#undef Petal_Main_Config
#endif

// using Petal_Main_Config in "Petal~UserEntrance.cpp", and do not reuse.

#define Petal_Main_Config(MainFunc) \
namespace Petal::UserEntrance \
{   \
	extern const fptr<int> pt_user_main{ &::MainFunc };  \
	extern const ptrc<TChar> pt_user_main_name{ Petal_TStr(#MainFunc) };  \
	extern const ptrc<DbgChar> pt_user_main_name_dbgc{ Petal_DbgStr(#MainFunc) };  \
}

// Declare user entrance "fn_user_entrance"(any valid name you want) here.
// declaration function and do not declaration a class/struct.
// if want a static function member of class be user entrance then make Petal_Enable_PetalMain undefined.

namespace App
{
	int main();
}

// Use macro Petal_Main_Config(fn_user_entrance) in "Petal~UserEntrance.cpp"

#endif

#endif
