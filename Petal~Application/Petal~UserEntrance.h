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
	extern const fptr<int> user_main{ &::MainFunc };  \
	extern const ptrc<TChar> user_main_name{ Petal_TStr(#MainFunc) };  \
	extern const ptrc<DbgChar> user_main_name_dbgc{ Petal_DbgStr(#MainFunc) };  \
}

// Declare user entrance "fn_user_entrance"(any valid name you want) here.
// declare a function and do not declaration a class or struct.
// if want a static function member of class be user entrance then make Petal_Enable_PetalMain undefined.

namespace App
{
	int main();
}

// Use macro Petal_Main_Config(fn_user_entrance) in "Petal~UserEntrance.cpp"

#endif

#endif
