#pragma once

#ifndef Petal_Header_WinTypes
#define Petal_Header_WinTypes

#ifndef NOMINMAX
#define NOMINMAX
#define Petal_Temp_NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define Petal_Temp_WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#ifdef Petal_Temp_NOMINMAX
#undef Petal_Temp_NOMINMAX
#undef NOMINMAX
#endif
#ifdef Petal_Temp_WIN32_LEAN_AND_MEAN
#undef Petal_Temp_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

namespace Petal
{
	using win32short = typename ::SHORT;
	using win32ushort = typename ::USHORT;
	using win32int = typename ::INT;
	using win32uint = typename ::UINT;
	using win32long = typename ::LONG;
	using win32ulong = typename ::ULONG;
	using win32lptr = typename ::LONG_PTR;
	using win32ulptr = typename ::ULONG_PTR;
	using win32bool = typename ::BOOL;
	using win32byte = typename ::BYTE;
	using win32word = typename ::WORD;
	using win32dword = typename ::DWORD;
	using win32error = decltype(::GetLastError());
	using win32atom = typename ::ATOM;
	using win32lres = typename ::LRESULT;
	using win32lprm = typename ::LPARAM;
	using win32wprm = typename ::WPARAM;
	using win32handle = typename ::HANDLE;
	using win32hins = typename ::HINSTANCE;
	using win32hwnd = typename ::HWND;
	using win32hmenu = typename ::HMENU;
	using win32hicon = typename ::HICON;
	using win32hbrush = typename ::HBRUSH;
	using win32hcursor = typename ::HCURSOR;
	using win32msg = typename ::UINT;

#ifdef Petal_Enable_Unicode
	using win32tchar = typename ::CHAR;
	using win32tstr = typename ::LPWSTR;
	using win32ctstr = typename ::LPCWSTR;
#else
	using win32tchar = typename ::WCHAR;
	using win32tstr = typename ::LPSTR;
	using win32ctstr = typename ::LPCSTR;
#endif

	using Win32Rect = typename ::RECT;
	using Win32Message = typename ::MSG;

#ifdef Petal_Enable_Unicode
	using WindowClass = typename ::WNDCLASSEXW;
	using Win32CreateStruct = typename ::CREATESTRUCTW;
#else
	using WindowClass = typename ::WNDCLASSEXA;
	using Win32CreateStruct = typename ::CREATESTRUCTA;
#endif

	inline constexpr win32error win32_no_error{ ERROR_SUCCESS };
	inline constexpr win32bool win32_true{ TRUE };
	inline constexpr win32bool win32_false{ FALSE };
}

#endif // !Petal_Header_WinTypes
