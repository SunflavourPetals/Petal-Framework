# 预处理器部分

本框架尽可能少使用宏来完成功能，大部分宏用于控制要编译的代码分支、Debug输出，少部分功能的宏请参考提供它们的头文件。  

本框架定义的宏都以“用下划线分隔的大驼峰字符串”作为命名风格，并且必须以 `Petal_` 开头。  
形如 `Petal_Enable_VSDebugOutput` `Petal_VSDbg`。  
宏会污染命名空间，因此这些宏都具有这样的形式用于防止命名空间的污染，约定我们不会给本框架的宏以外的内容具有这样风格的名称。  

## 在配置属性中定义的宏

在“配置属性`->`C/C++`->`预处理器`->`预处理器定义”中定义的宏，它们作用于整个项目。  

注：是否有作用只与宏是否被定义有关，与宏被定义的值无关，例如即使 `Petal_Enable_PetalMain` 被定义为 `FALSE`，它仍将发挥作用。  

### Petal_Enable_DebugCStringRefIterator

类模板 `CStringRefIterator` 模仿了 msvc 对字符串视图迭代器的 debug 行为(使用宏`_STL_VERIFY`)，条件是本宏(`Petal_Enable_DebugCStringRefIterator`)被定义。  
关闭此宏以禁止该行为，如 msvc 版本不同或更新导致代码失效，取消定义该宏是好方法。  

### Petal_Enable_ForceDbgANSI

宏 `Petal_VSDbg` 系列存在 A/W 两个版本，开启此宏时 `Petal_VSDbg` 将一定会选择 A 版本，`Petal~String.h` 中的 `DbgChar` 别名等相关内容也将受影响，否则将根据 [宏 `Petal_Enable_Unicode`](#petal_enable_unicode) 的定义情况选择使用 A 版本 API 还是 W 版本 API。  

详见 `Petal~VSDebugOutput` 的文档。  

### Petal_Enable_ForceDbgRemoveNUL

若定义此宏，本框架 `Petal~VSDebugOutput.h` 中的 Debug 对象在输出 C++ 的字符串时，会先生成一个待输出字符串的副本，并剔除副本中结尾前的 NUL 字符再输出副本中的内容，否则要输出的内容可能会被意外截断(C++ 的字符串允许存在 NUL，而真正进行输出的 API 接受的是 C 风格字符串)。  

### Petal_Enable_PetalMain

框架源文件 `Petal~Main.cpp` 中定义了 C++/WIN32 的入口函数，尤其是在 WIN32 程序中，`WinMain` 系列的入口函数通常比较长，本框架简化了这些，提供了选择其他函数作为“用户入口函数”的选择。  

定义此宏后，可以使用全局/命名空间作用域中的函数充当“用户入口函数”(应当是 `int()` 类型的函数)，使用方法参见 `Petal~UserEntrance.h`、`Petal~UserEntrance.cpp` 中的描述。  
即通常在 `Petal~UserEntrance.h` 中声明用户入口函数，在`Petal~UserEntrance.cpp` 中注册用户入口函数。通常在其他源文件定义用户入口函数。

若不定义此宏，`Petal~Main.h` 中将提供 `Petal_SetMainFunc`、`Petal_SetMainClass` 宏用于注册用户入口函数，规定这几个宏只能在一个翻译单元内使用其中一个且仅使用一次。  
不定义宏 `Petal_Enable_PetalMain` 时，除可以使用全局/命名空间中的函数作为用户入口外，还对使用类中的静态成员函数作为用户入口函数提供了较好的支持，但是应当引入 `"Petal~Main.h"` 以使用其中的宏。  

```C++
// main.cpp

#include "Petal~Main.h"

struct Test
{
    static int main()
    {
        return 0;
    }
}

Petal_SetMainClass(Test)
```

宏的详细使用方法见 `Petal~Main` 的文档。  

定义 `Petal_Enable_PetalMain` 宏的方便之处在于把注册用户入口函数的工作放到了一个由框架提供的专门的翻译单元中，以便我们在定义用户入口函数时不必引入头文件 `Petal~Main.h`，这两种方式形式不同，但行为相同。  

以上仅是通常情况及建议的使用方法，本框架不对使用方法做任何限制，若有需要，即使修改框架也是被提倡的。  

### Petal_Enable_Unicode

若定义此宏，框架内的函数如果有使用 WIN32 的 API，将使用 W 版本。  
若不定义此宏，框架内的函数如果有使用 WIN32 的 API，将尽可能使用 A 版本。

有一些内容有额外的规则，比如宏 `Petal_VSDbg` 同时受此宏和 [宏 `Petal_Enable_ForceDbgANSI`](#petal_enable_forcedbgansi) 影响。  

### Petal_Enable_VSDebugOutput

若定义此宏，本框架 `Petal~VSDebugOutput.h` 中的 Debug 输出系列宏，都将被定义为对 Debug 输出函数的调用，若不定义此宏，它们都将被定义为空，不会参与编译，不会输出信息。当为发布软件编译项目时，不要定义此宏。  
是否执行 Debug 输出还与 WINAPI `OutputDebugStringA/W` 有关，参见 MSDN。  

## 其他

用于 Debug 输出的宏参见 `Petal~VSDebugOutput`，一般情况下用户不使用它们。  

防止头文件重复包含的宏(惯例，即使使用了 `#pragma once` 我们仍然提供)：  
`Petal_Header_名称`。  
