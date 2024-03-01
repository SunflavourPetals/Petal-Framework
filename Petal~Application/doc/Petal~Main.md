# Petal~Main

## 概述

1. 提供入口函数的参数，但在进入用户入口函数后才可能有效。  
2. 提供封装后的模块实例句柄获取函数 `::HINSTANCE Petal::WinMain::HIns() noexcept`。  
3. 提供封装后的命令行参数获取函数 `Petal::TCStringRef Petal::WinMain::CmdLine() noexcept`。  
4. 未定义宏 `Petal_Enable_PetalMain` 情况下提供设置用户入口函数的宏：  
   * Petal_SetMainFunc
   * Petal_SetMainSpace
   * Petal_SetMainClass
   * Petal_SetDefaultMainClass

### 用户入口函数

程序入口函数可能是 `main` `wmain` `WinMain` `wWinMain` 四者之一，为了免去选择入口函数的苦恼，将其定义在 `Petal~Main.cpp` 中。  
若预处理器定义了 `Petal_Enable_PetalMain`，那么用户需要参考 `Petal~UserEntrance.h/.cpp` 的内容定义“用户入口函数”。否则需要用户定义类型为 `int()` 类型的用户入口函数并使用 `Petal_SetMainFunc` 等宏注册用户入口函数。  

* 用户不可以定义 `main` `wmain` `WinMain` `wWinMain` 中的任何函数；  
* 用户不应当调用 `main` `wmain` `WinMain` `wWinMain` 中的任何函数；  
* 用户不应当修改工程的入口函数，除非你知道这样做会对你的程序有何影响，如有需要，忽略上面两条规则。  
* 如有需要，请自行修改 `Petal~Main.cpp` 等的代码改变程序的行为。  

不改变 `Petal~Main.cpp` 代码的情况下：  

入口函数示例：  

```cpp
// 示例入口函数
int main(int argc, char* argv[], char* envp[])
{
    if (PetalUnnamed::protection.Used() == true)
    {
        PetalUnnamed::protection.VSDebugOutputWarning();
        return -1;
    }
    PetalUnnamed::protection.Use();
    PetalUnnamed::Main::arguments.Init(argc, argv, envp);
    PetalUnnamed::WinMain::arguments.InitAsInvalid();
    PetalUnnamed::XMain::VSDebugOutput(Petal_DbgStr("main"), Petal_DbgStr("Main"));
    return Petal::UserEntrance::pt_user_main();
}
```

1. 若入口函数为 main / wmain，则下列位于 Petal::Main 的对象将在进入用户入口函数后有效，因为它们所引用的数据会在入口函数中、调用用户入口函数前被赋为有效值(Petal::Main::valid 变为 true)。  
     * arcg: `int`  
       argv 中的元素个数  
     * argv: `ptrc<ptrc<TChar>>`  
       main / wmain 接收的参数  
     * envp: `ptrc<ptrc<TChar>>`  
       环境变量参数  
2. 若入口函数为 WinMain / wWinMain，则下列位于 Petal::WinMain 的对象将在进入用户入口函数后有效，因为它们所引用的数据会在入口函数中、调用用户入口函数前或进入入口函数前被赋为有效值(Petal::WinMain::valid 变为 true)  
     * hins: `HINSTANCE`  
       当前实例句柄，在入口函数开始前需要使用 `Petal::WinMain::HIns` 获得
     * cmd_line: `ptrc<TChar>`  
        应用程序的命令行，不包括程序名称。若要检索整个命令行，请使用 `Petal::WinMain::CmdLine` 函数。
     * cmd_show: `INT`  
        控制窗口的显示方式。

然而即使入口函数是 main / wmain，Petal::WinMain::hins 所引用的对象仍会在进入入口函数前被赋为有效值(但是不可在进入入口函数之前使用它们)，而 HIns 和 CmdLine 函数作为对 WIN32 API 的封装则始终可用。  

## 参考

### 全局命名空间

#### 宏 Petal_Header_Main

指示头文件添加状态。  

#### 宏 Petal_SetMainFunc

当未定义宏 `Petal_Enable_PetalMain` 时被定义，用于设置用户入口函数。  
只能在入口函数所在 `.cpp` 源文件中的全局命名空间中使用，参数不可以以`::`开始。  
此宏实际上是在 Petal::UserEntrance 命名空间里定义指向目标函数的指针，使 `Petal~Main.cpp` 中的入口函数能够调用设置的用户入口函数。  

```cpp
#define Petal_SetMainFunc(MainFunc) \
namespace Petal::UserEntrance \
{   \
    extern const fptr<int> pt_user_main{ &::MainFunc };  \
    extern const ptrc<TChar> pt_user_main_name{ Petal_TStr(#MainFunc) };  \
    extern const ptrc<DbgChar> pt_user_main_name_dbgc{ Petal_DbgStr(#MainFunc) };  \
}
```

使用示例：  

```cpp
// main.cpp

#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"

namespace MyProgram
{
    int my_main()
    {
        Petal::Debug::println("Hello world!");
        return 0;
    }
}

Petal_SetMainFunc(MyProgram::my_main); // 入口函数将调用 MyProgram::my_main

// Petal_SetMainFunc(::MyProgram::my_main); 错误
```

#### 宏 Petal_SetMainClass

当未定义宏 `Petal_Enable_PetalMain` 时被定义，用于设置用户入口函数。  

```cpp
#define Petal_SetMainClass(MainClass) Petal_SetMainFunc(MainClass::main)
```

使用示例：  

```cpp
#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"

namespace MyProgram
{
    class MyMain
    {
    public:
        static int main()
        {
            Petal::Debug::println("Hello world!");
            return 0;
        }
    }
}

Petal_SetMainClass(MyProgram::MyMain); // 入口函数将调用 MyProgram::MyMain::main
```

参考[宏 Petal_SetMainFunc](#宏-petal_setmainfunc)。  

#### 宏 Petal_SetMainSpace

当未定义宏 `Petal_Enable_PetalMain` 时被定义，用于设置用户入口函数。  

```cpp
#define Petal_SetMainSpace(MainSpace) Petal_SetMainClass(MainSpace)
```

使用示例：  

```cpp
#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"

namespace MyProgram
{
    int main()
    {
        Petal::Debug::println("Hello world!");
        return 0;
    }
}

Petal_SetMainSpace(MyProgram); // 入口函数将调用 MyProgram::main
```

参考[宏 Petal_SetMainFunc](#宏-petal_setmainfunc)。  

#### 宏 Petal_SetDefaultMainClass

```cpp
#define Petal_SetDefaultMainClass     Petal_SetMainClass(Main)
```

参考[宏 Petal_SetMainClass](#宏-petal_setmainclass)。  

### 命名空间 Petal::Main

#### 命名空间局部引用 argc

`extern const int& argc;`  

当入口函数为 `main` / `wmain` 时，`argc` 引用的对象将在入口函数中被赋为 `main` / `wmain` 的第一个参数，指示第二个参数 `argv` 中的元素数量。  

#### 命名空间局部引用 argv

`extern const ptrc<ptrc<TChar>>& argv;`  

当入口函数为 `main` / `wmain` 时，`argv` 引用的对象将在入口函数中被赋为 `main` / `wmain` 的第二个参数，为程序接收的命令行参数。  

#### 命名空间局部引用 envp

`extern const ptrc<ptrc<TChar>>& envp;`  

当入口函数为 `main` / `wmain` 时，`envp` 引用的对象将在入口函数中被赋为 `main` / `wmain` 的第三个参数，环境变量。  

#### 命名空间局部引用 valid(Petal::Main)

`extern const boolean& valid;`  

当入口函数为 `main` / `wmain` 时，进入入口函数后、调用用户入口函数前会被赋为 `true`，否则它将保持为 `false` 的状态。  

### 命名空间 Petal::WinMain

#### 函数 CmdLine

`TCStringRef CmdLine() noexcept;`  

对 WIN32 API `GetCommandLineA/W` 的封装，使用 A 版本或 W 版本视情况而定(是否定义宏 Petal_Enable_Unicode)。  

#### 命名空间局部引用 cmd_line

`extern const ptrc<TChar>& cmd_line;`  

当入口函数为 `WinMain` / `wWinMain` 时，`cmd_line` 引用的对象将在入口函数中被赋为 `WinMain` / `wWinMain` 的第三个参数，命令行参数。  

#### 命名空间局部引用 cmd_show

`extern const win32int& cmd_show;`  

当入口函数为 `WinMain` / `wWinMain` 时，`cmd_show` 引用的对象将在入口函数中被赋为 `WinMain` / `wWinMain` 的第四个参数。  

#### 函数 HIns

`win32hins HIns() noexcept;`  

对 WIN32 API `GetModuleHandleA/W(NULL)` 的封装，使用 A 版本或 W 版本视情况而定(是否定义宏 Petal_Enable_Unicode)。  

#### 命名空间局部引用 hins

`extern const win32hins& hins;`  

当入口函数为 `WinMain` / `wWinMain` 时，`hins` 引用的对象将在入口函数中被赋为 `WinMain` / `wWinMain` 的第一个参数，当前模块的句柄，与调用 `HIns` 的结果相同。  

#### 命名空间局部引用 valid(Petal::WinMain)

`extern const boolean& valid;`  

当入口函数为 `WinMain` / `wWinMain` 时，进入入口函数后、调用用户入口函数前会被赋为 `true`，否则它将保持为 `false` 的状态。  
