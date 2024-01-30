#include "Petal~Main.h"

#include "Petal~RawInputWindow.h"
#include "Petal~Log.h"

#include "Petal~VSDebugOutput.h"

#include <memory>
#include <memory_resource>
#include <array>

class MyWin : public Petal::RawInputWindow
{
public:
	void RawMouseEvent(Petal::RawMouseMessage& e, Petal::Win32RawInput& r, Petal::tsize size) noexcept override
	{
		const auto& ms{ r.data.mouse };
		Petal::Debug::println("MOUS|   F:{:<4}|  BF:{:<4}|  BD:{:<4}|  RB:{:<4}|  LX:{:<4}|  LY:{:<4}|  EX:{:<4}|",
			ms.usFlags, ms.usButtonFlags, ms.usButtonData,
			ms.ulRawButtons, ms.lLastX, ms.lLastY, ms.ulExtraInformation);
	}
	void RawKeyboardEvent(Petal::RawKeyboardMessage& e, Petal::Win32RawInput& r, Petal::tsize size) noexcept override
	{
		const auto& kb{ r.data.keyboard };
		Petal::Debug::println("KB  |  MC:{:<4}|   F:{:<4}| RSV:{:<4}|  VK:{:<4}| MSG:{:<4}|  EX:{:<4}|",
			kb.MakeCode, kb.Flags, kb.Reserved, kb.VKey, kb.Message, kb.ExtraInformation);
	}
	void RawHidEvent(Petal::RawHidMessage& e, Petal::Win32RawInput& r, Petal::tsize size) noexcept override
	{
		Petal::dout + "INPUT-HID" + Petal::ln;
	}
	MyWin()
	{
		using namespace Petal;
		this->Create(WindowClassArgs{ Petal_TStr("my window class") }.Register().class_atom);

		constexpr tsize ridev_count{ 4 };
		::RAWINPUTDEVICE ridev[ridev_count]{};

		ridev[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		ridev[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
		ridev[0].dwFlags = 0; // RIDEV_NOLEGACY;
		ridev[0].hwndTarget = 0;

		ridev[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
		ridev[1].usUsage = HID_USAGE_GENERIC_MOUSE;
		ridev[1].dwFlags = 0; // RIDEV_NOLEGACY;
		ridev[1].hwndTarget = 0;

		ridev[2].usUsagePage = HID_USAGE_PAGE_GENERIC;
		ridev[2].usUsage = HID_USAGE_GENERIC_GAMEPAD;
		ridev[2].dwFlags = RIDEV_INPUTSINK;
		ridev[2].hwndTarget = this->WindowHandle();
		ridev[3] = ridev[2];
		ridev[3].usUsage = HID_USAGE_GENERIC_JOYSTICK;

		if (this->RegisterRawInputDevices(ridev) == win32_false)
		{
			dout + "FAILED IN REGISTER RAW INPUT" + ln;
		}

		

		this->Show();
	}
};

template <typename T>
auto gettype(T&& t) -> const char*
{
	return typeid(T).name();
}
template <typename T>
auto to_csr(T&& t) -> Petal::CStringRef
{
	return { t, sizeof(t) - 1 };
}
template <typename T>
auto to_sv(T&& t) -> Petal::StringView
{
	return { t, sizeof(t) - 1 };
}

template <typename FormatString, typename... Args>
void eprint(FormatString&& fmt, Args&& ...args)
{
	using namespace Petal::DebugLiterals;
	"fmt"_doutln();
}

template <std::size_t char_arr_size, typename... Args>
void eprint(const char (&fmt)[char_arr_size], Args&& ...args)
{
	using namespace Petal::DebugLiterals;
	"const char[N]"_doutln();
}

template <std::size_t char_arr_size, typename... Args>
void eprint(char(&fmt)[char_arr_size], Args&& ...args)
{
	using namespace Petal::DebugLiterals;
	"char[N]"_doutln();
}

class my_traits : public ::std::char_traits<char>
{
public:
	_NODISCARD static _CONSTEXPR17 size_t length(_In_z_ const char* _First) noexcept /* strengthened */ {
		size_t _Count = 0;
		while (*_First != char('!')) {
			++_Count;
			++_First;
		}
		return _Count;
	}
};

struct mc
{
	int data{ 123 };
};

void outln(auto&& obj)
{
	Petal::Debug::println("{}", typeid(decltype(obj)).name());
	Petal::Debug::println("{}", std::forward<decltype(obj)>(obj));
}

template <typename Ty>
void outtln(Ty&& obj)
{
	Petal::Debug::println("{}", typeid(Ty).name());
	Petal::Debug::println("{}", std::forward<Ty>(obj));
}

template <typename T>
void fat2()
{

}
#include <iostream>
template <std::size_t char_arr_size, typename... Args>
constexpr void const_print(const char(&fmt)[char_arr_size], Args&& ...args)
{
	using namespace Petal;
	{
		Debug::print("[!consteval] ");
		Debug::print(fmt, ::std::forward<Args>(args)...);
	}
}

template <typename char_t, std::size_t size>
	requires std::is_same_v<std::remove_cv_t<char_t>, char>
[[nodiscard]] constexpr std::size_t get_arr_size(char_t(&arr)[size]) noexcept
{
	Petal::Debug::println("{}", typeid(arr).name());
	return size;
}

namespace TEST
{
	inline namespace V10001
	{
		int a = 10001;
	}
	namespace V10002
	{
		int a = 10002;
	}
}

template <typename... Args>
std::string fmtto(std::string_view fmt, Args&&... args)
{
	return ::std::vformat(fmt, ::std::make_format_args(args...));
}

namespace Petal
{


}

class Main
{
public:
	static int main()
	{
		using namespace Petal;
		using namespace Petal::StringLiterals;
		using namespace Petal::DebugLiterals;
		{
			/*
				*i
				i->
				++i
				i++
				--i
				i--
				i+=1
				i+1
				1+i
				i-=1
				i-1
				i-i2
				i[1]
				i==i2
				i<=>i2
			*/
			auto csr = "abcde"_csr;
			decltype(""_csr)::iterator i = csr.begin();
			Debug::println("==++==++==++==++==");
			Debug::println("{}", *i);
			Debug::println("{}", *(++i));
			Debug::println("{}", *(i++));
			Debug::println("{}", *(i--));
			Debug::println("{}", *(--i));
			Debug::println("{}", *(i += 1));
			Debug::println("{}", *(i + 1));
			Debug::println("{}", *(1 + i));
			Debug::println("{}", *(i - 1));
			Debug::println("{}", *(i -= 1));
			Debug::println("{}", csr.end() - csr.begin());
			Debug::println("{}", csr.end() == csr.begin());
			Debug::println("{}", (csr.end() <=> csr.begin()) == std::strong_ordering::greater);
		//	*i;
		//	++i;
		//	i++;
		//	--i;
		//	i--;
		//	i += 1;
		//	i + 1;
		//	1 + i;
		//	i -= 1;
		//	i - 1;
		//	csr.end() - csr.begin();
		//	i[1];
		//	csr.end() == csr.begin();
		//	csr.end() <=> csr.begin();
			
		}
		{
			fmtto("{}", 1);
		//	_STL_VERIFY(nullptr, "message");
		}
		{
			using namespace TEST;
			
			std::cout << "cnm"_csr;
		}
		{
			
		//	get_arr_size("");
			char ss[] = "";
			char(&s)[1] = ss;
		//	get_arr_size(s);
		}
		{
			BasicString<char, my_traits> ms{ "123!456" };
			Debug::println("{}", ms[3]);
			String ms2 = "xxx";
			StringView mv = ms2;
			CStringRef mr = ms2;
		//	mv.operator[](1);
		//	mv.at(0);
			try
			{
				R"()";
			//	mv.at(4);
			}
			catch (std::exception& e) { Debug::println("{}", e.what()); }
			mr[1];
		//	mr.at(0);
			try
			{
			//	mr.at(4);
			}
			catch (std::exception& e) { Debug::println("{}", e.what()); }
			
		}
		fat2<int>();
		int itest = 12348765;
		int& irtest = itest;
		outln(irtest);
		outtln(irtest);
		std::formatter<BasicStringView<char>, char> a;
		const WChar* xxxxx{ WinMain::CmdLine() };
		std::basic_string<char, my_traits> my_str{ "123!456" };
		Debug::println("size {}, str {}", my_str.size(), my_str.c_str());
		
		Debug::println("{:>32}", "im cstrref"_csr);
		
		std::formatter<std::basic_string_view<char>, char> axx;
		dout + std::format("{:>16}", StringView{ "sv" }) + ln;
		Log::LogA log;
		Debug::println("{}", log.Opened());
		
		const char* fmt = "{}xxx\0xxx";

		Debug::println(fmt, 123);
		Debug::println("{}xxx\0xxx", 123);

		Debug::print("abc\0def{}", 123);
		Debug::println("abc\0def{}", 123);

		char arr[] = "xxx";
		eprint("xxx"_sv);
		eprint("xxx");
		eprint(arr);

		"{}"_doutln(gettype("xxx"));
		"{}"_doutln(static_cast<StringView>(to_csr("xxx\0xxx")));
		"{}"_doutln(to_sv("xxx\0xxx"));
		
	//	Debug::println("www\0{}xxx"_csr, 123);
		"www\0{}xxx"_dout(123); ""_doutln();
		"www\0{}xxx"_doutln(123);
		L"www\0{}xxx"_dout(123);  L""_doutln();
		L"www\0{}xxx"_doutln(123);

		struct my_char_traits1 : std::char_traits<char> { int pod{}; };
		struct my_char_traits2 : std::char_traits<char> { char pod[sizeof(int)]{}; };
		::std::basic_string<char, my_char_traits1> str1{ "i am str1\0tail" }; // 字符串1 // 注：丢弃了'\0'后的字串
		::std::basic_string<char, my_char_traits2> str2{ "i am str2" }; // 字符串2 // 注：两字符串对象持有的类型不同
		{
			CStringRef t1{ str1 };
			CStringRef t2{ str2 };
		}

		// CStringRef仅使用指针(const CharT*)和std::size_t类型变量记录c风格字符串(保证串以NUL结尾)和length，并对字符串只具有读权限

	//	CStringRef csr_test{ str1 }; // 参数类型不同于 CStringRef 或 String 则无法构建对象(using String = std::string; using StringView = std::string_view;)	
	//	MakeCStringRef<char, char_traits, alloc>(s) 从 持有 std::basic_string<char, char_traits, alloc> 类型的对象 构建 CStringRef 对象
	//	auto csr_test1 /*: CStringRef */ = MakeCStringRef(str1); 

	//	程序员负责确保 CStringView 不在被指向数组的生存期外继续生存：
		{	// csr_test2的生命周期长于String{"foo"}，导致悬垂指针
			CStringRef csr_test2{ String{ "foo" } }; 
			// operate(csr_test2);
		}
		{	// 另一个指向的字符串生存期结束导致指针悬垂的例子
			String temp_str{ "foo" };
			CStringRef csr_test3{ temp_str };
			temp_str = String{ "bar|ooo|ooo|ooo|ooo|ooo|" };
			// operate(csr_test3);
		}
		Debug::println("================");

		{
			char s[] = "\0x2x";
			Debug::println(StringToCStyleString("\0x2x"));
			Debug::println(StringToCStyleString(s));
			String ss = "\0x2x"_s;
			Debug::println(StringToCStyleString(ss));
			Debug::println(StringToCStyleString(static_cast<StringView>(ss)));
		}
		{
			{
				Debug::print("\0{}", 123);
				char fmt[] = "\0{}";
				Debug::print(fmt, 123);
				StringView fmt_sv = "\0{}"_sv;
				Debug::print("\0{}", 123);
			}
			{
				Debug::wprint(L"\0{}", 123);
				WChar fmt[] = L"\0{}";
				Debug::wprint(fmt, 123);
				WStringView fmt_sv = L"\0{}"_sv;
				Debug::wprint(fmt_sv, 123);
			}
		}
		/*
		{
			CStringRef csr = MakeCStringRef(str1); // 使用make方法构建C风格字符串的引用 // 字符串尾置零由std::basic_string保证 注：其不保证非末尾处无NUL
			Debug::println("{}:{}", static_cast<StringView>(csr), csr.length());
		}
		{
			CStringRef csr = MakeCStringRef("i am str1\0tail"); // 使用make方法构建C风格字符串的引用 // 字符串尾置零由std::basic_string保证 注：其不保证非末尾处无NUL
			Debug::println("{}:{}", static_cast<StringView>(csr), csr.length());
		}
		{
			char s[] = "i am str1\0tail";
			CStringRef csr = MakeCStringRef(s); // 使用make方法构建C风格字符串的引用 // 字符串尾置零由std::basic_string保证 注：其不保证非末尾处无NUL
			Debug::println("{}:{}", static_cast<StringView>(csr), csr.length());
		}

		CStringRef csr = MakeCStringRef(str1);
		csr = "literal"_csr; // 字面量运算符重载 // 字符串尾置零由字符串字面量保证
		Debug::println("{}:{}", csr.c_str(), csr.length());
		*/
		CStringRef csr;
		csr = { "12345678\087654321", 8 }; // 字符串尾置零由用户保证，用户不一定知道他在干什么，因此 // 1 2 3 4 5 6 7 8 NUL
		csr = { "12345678\087654321" + 9, 8 }; // 8 7 6 5 4 3 2 1 NUL
		try { csr = { "12345678\087654321", 2 }; } // 字符串非零，抛出异常
		catch (const std::exception& e) { Debug::println("{}", e.what()); }
		StringView str_view = csr; // 可以从“保证末尾以NUL结尾”的CStringRef类型的对象转为“不保证末尾以NUL”结尾的StringView类型的对象，反之则不允许
		Debug::println("{}:{}", static_cast<StringView>(csr), str_view.size());

		Debug::println("csr not null before =nullptr: {}", static_cast<bool>(csr));
		csr = nullptr; // 置空
		Debug::println("csr not null after =nullptr: {}", static_cast<bool>(csr));
		Debug::wprintln(L"{}", 1);
		
		int iobj = 0;

		if (int* i = &iobj)
		{
			Debug::println("{}", *i);
		}
		
		::std::cout << "CSR+++"_csr;
		
	//	std::basic_string_view<char>;
		std::unordered_map<CStringRef, int, CStringRef::Hash> axxa;
		StringView::const_iterator sv_cit;
		axxa["xxx"_csr] = 3;
		
		Debug::println("cnmcnmcnmcnmcnmcnmcnmcnmcnmcnmcnmcnm");

	//	for (auto e : "csr"_csr)
		for (auto it = "csr"_csr.begin(); it != "csr"_csr.end(); ++it)
		{
			auto& e = *it;
			Debug::print("{}", e);
		}
		Debug::println();

		Debug::println("{}", (StringView{"yyy"} <=> StringView{"xxx"}) == std::strong_ordering::equal);



		Window w{};
		DWORD style = WS_OVERLAPPEDWINDOW - WS_SIZEBOX;
		WindowCreatingArgs cr{ Petal_TStr("My Window") };
		cr.style = WS_OVERLAPPEDWINDOW - WS_SIZEBOX;
		w.Create(WindowClassArgs{ Petal_TStr("my_win_class") }.Register().class_atom, cr);

		w.Show();

	



		return MessageLoop();
	}
};

Petal_SetMainClass(Main);

namespace XXXXX
{
	template <typename Ot>
	class T
	{
	public:
		template <typename U>
		T(U v)
		{

		}
	};
}

