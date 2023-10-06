# Petal~BasicTypes

## 概述

命名空间 Petal  

此标头定义了常用基本数据类型的别名。

 * using i8 = typename ::std::int8_t;
 * using i16 = typename ::std::int16_t;
 * using i32 = typename ::std::int32_t;
 * using i64 = typename ::std::int64_t;
 * using u8 = typename ::std::uint8_t;
 * using u16 = typename ::std::uint16_t;
 * using u32 = typename ::std::uint32_t;
 * using u64 = typename ::std::uint64_t;
 * using f32 = float;
 * using f64 = double;
 * using boolean = bool;
 * using achar = char;
 * using wchar = wchar_t;
 * using u8char = char8_t;
 * using u16char = char16_t;
 * using u32char = char32_t;

其他别名

 * using tsize = typename ::std::size_t;
 * using byte = u8;
 * using word = u16;
 * using dword = u32;
 * using qword = u64;
 * template &lt;typename Ty> using ptr = Ty*;
 * template &lt;typename Ty> using ptrc = const Ty*;
 * template &lt;typename ReturnT, typename... Args> using fptr = ReturnT(*)(Args...);

注：boolean等别名在子命名空间或其他情况可能有指代不明确的问题，在非全局命名空间使用`using Petal::boolean`等方式解决。