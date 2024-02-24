#pragma once

#ifndef Petal_Header_UserEntrance
#define Petal_Header_UserEntrance

#ifdef Petal_Enable_PetalMain

#ifdef Petal_PetalMain
#undef Petal_PetalMain
#endif

#define Petal_PetalMain(MainFunc) \
namespace Petal::UserEntrance \
{   \
	extern const fptr<int> pt_user_main{ &::MainFunc };  \
	extern const ptrc<TChar> pt_user_main_name{ Petal_TStr(#MainFunc) };  \
	extern const ptrc<DbgChar> pt_user_main_name_dbgc{ Petal_DbgStr(#MainFunc) };  \
}

// Declare user entrance "fn_user_entrance"(any valid name you want) here.
// declaration function and do not declaration a class/struct, if want a static function member of class be user entrance, set Petal_Enable_PetalMain undefined.
namespace App
{
	int main();
}

// Use macro Petal_PetalMain(fn_user_entrance) in "Petal~UserEntrance.cpp"

#endif

#endif
