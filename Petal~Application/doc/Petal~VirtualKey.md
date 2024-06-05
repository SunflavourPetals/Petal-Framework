# Petal~VirtualKey

## 概述

一个用于枚举虚拟键码的“类枚举”。  

对微软文档所列举的常用键码进行了枚举，服务于这个框架与键盘消息有关的部分，在命名方面更多地贴近键盘本身而不是 WIN32 `VK_` 系列的宏。对 OEM 相关键码的命名参考了微软文档对其用于美国标准键盘的描述。  
当需要更详细的信息时，可以使用 WIN32 中以 `VK_` 开头的宏，它们被定义在 `WinUser.h` 这个头文件中。  

参考[虚拟键码的微软文档](https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes "en page")。  

## 参考

### 宏

#### 宏 Petal_Header_VirtualKey

指示头文件引入情况。  

### 命名空间 Petal

#### 类 VirtualKey

一个用于枚举虚拟键码的“类枚举”。  

对微软文档所列举的常用键码进行了枚举，对 OEM 相关键码的命名参考了微软文档对其用于美国标准键盘的描述。  
当需要使用未被本框架枚举的虚拟键码时，可以使用 WIN32 中以 `VK_` 开头的宏，它们被定义在 `WinUser.h` 这个头文件中，参考[虚拟键码的微软文档](https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes "中文页面")。  

我们没有将这个类定义为 final，目的是当有需要时方便用户扩展它。  

| 枚举名称                  | 虚拟键码                  | 对应按键                  |
| :-----------------------: | :-----------------------: | :-----------------------: |
| `Mouse::LButton`          | `VK_LBUTTON`              | 鼠标左键                  |
| `Mouse::RButton`          | `VK_RBUTTON`              | 鼠标右键                  |
| `Mouse::MButton`          | `VK_MBUTTON`              | 鼠标中键                  |
| `Mouse::XButton1`         | `VK_XBUTTON1`             | X1 鼠标按钮               |
| `Mouse::XButton2`         | `VK_XBUTTON2`             | X2 鼠标按钮               |
| `Browser::Back`           | `VK_BROWSER_BACK`         | 浏览器后退键              |
| `Browser::Forward`        | `VK_BROWSER_FORWARD`      | 浏览器前进键              |
| `Browser::Refresh`        | `VK_BROWSER_REFRESH`      | 浏览器刷新键              |
| `Browser::Stop`           | `VK_BROWSER_STOP`         | 浏览器停止键              |
| `Browser::Search`         | `VK_BROWSER_SEARCH`       | 浏览器搜索键              |
| `Browser::Favorites`      | `VK_BROWSER_FAVORITES`    | 浏览器收藏键              |
| `Browser::Home`           | `VK_BROWSER_HOME`         | 浏览器“开始”和“主页”键    |
| `Volume::Mute`            | `VK_VOLUME_MUTE`          | 静音键                    |
| `Volume::Down`            | `VK_VOLUME_DOWN`          | 音量减小键                |
| `Volume::Up`              | `VK_VOLUME_UP`            | 音量增加键                |
| `Media::Next`             | `VK_MEDIA_NEXT_TRACK`     | 下一曲目键                |
| `Media::Prev`             | `VK_MEDIA_PREV_TRACK`     | 上一曲目键                |
| `Media::Stop`             | `VK_MEDIA_STOP`           | 停止媒体键                |
| `Media::PlayPause`        | `VK_MEDIA_PLAY_PAUSE`     | 播放/暂停媒体键           |
| `Numpad::Num0`            | `VK_NUMPAD0`              | 数字键盘 0 键             |
| `Numpad::Num1`            | `VK_NUMPAD1`              | 数字键盘 1 键             |
| `Numpad::Num2`            | `VK_NUMPAD2`              | 数字键盘 2 键             |
| `Numpad::Num3`            | `VK_NUMPAD3`              | 数字键盘 3 键             |
| `Numpad::Num4`            | `VK_NUMPAD4`              | 数字键盘 4 键             |
| `Numpad::Num5`            | `VK_NUMPAD5`              | 数字键盘 5 键             |
| `Numpad::Num6`            | `VK_NUMPAD6`              | 数字键盘 6 键             |
| `Numpad::Num7`            | `VK_NUMPAD7`              | 数字键盘 7 键             |
| `Numpad::Num8`            | `VK_NUMPAD8`              | 数字键盘 8 键             |
| `Numpad::Num9`            | `VK_NUMPAD9`              | 数字键盘 9 键             |
| `Numpad::Multiply`        | `VK_MULTIPLY`             | 数字键盘乘号键            |
| `Numpad::Add`             | `VK_ADD`                  | 数字键盘加号键            |
| `Numpad::Separator`       | `VK_SEPARATOR`            | [数字键盘分隔符键](#说明) |
| `Numpad::Subtract`        | `VK_SUBTRACT`             | 数字键盘减号键            |
| `Numpad::Decimal`         | `VK_DECIMAL`              | 数字键盘小数点键          |
| `Numpad::Divide`          | `VK_DIVIDE`               | 数字键盘除号键            |
| `Launch::Mail`            | `VK_LAUNCH_MAIL`          | 启动邮箱键                |
| `Launch::MediaSelect`     | `VK_LAUNCH_MEDIA_SELECT`  | 选择媒体键                |
| `Launch::App1`            | `VK_LAUNCH_APP1`          | 启动应用程序 1 键         |
| `Launch::App2`            | `VK_LAUNCH_APP2`          | 启动应用程序 2 键         |
| `Backspace`               | `VK_BACK`                 | 退格(Backspace)键         |
| `Tab`                     | `VK_TAB`                  | Tab 键                    |
| `Enter`                   | `VK_RETURN`               | Enter 键                  |
| `Shift`                   | `VK_SHIFT`                | Shift 键                  |
| `Ctrl`                    | `VK_CONTROL`              | Ctrl 键                   |
| `Alt`                     | `VK_MENU`                 | Alt 键                    |
| `Pause`                   | `VK_PAUSE`                | Pause 键                  |
| `CapsLock`                | `VK_CAPITAL`              | 大写锁定(CapsLock)键      |
| `Esc`                     | `VK_ESCAPE`               | Esc 键                    |
| `Space`                   | `VK_SPACE`                | 空格(Space)键             |
| `PageUp`                  | `VK_PRIOR`                | PageUp 键                 |
| `PageDown`                | `VK_NEXT`                 | PageDown 键               |
| `End`                     | `VK_END`                  | End 键                    |
| `Home`                    | `VK_HOME`                 | Home 键                   |
| `Left`                    | `VK_LEFT`                 | 左方向键(←)               |
| `Up`                      | `VK_UP`                   | 上方向键(↑)               |
| `Right`                   | `VK_RIGHT`                | 右方向键(→)               |
| `Down`                    | `VK_DOWN`                 | 下方向键(↓)               |
| `Select`                  | `VK_SELECT`               | Select 键                 |
| `Print`                   | `VK_PRINT`                | Print 键                  |
| `Execute`                 | `VK_EXECUTE`              | Execute 键                |
| `PrintScreen`             | `VK_SNAPSHOT`             | PrintScreen 键            |
| `Insert`                  | `VK_INSERT`               | Insert 键                 |
| `Delete`                  | `VK_DELETE`               | Delete 键                 |
| `Help`                    | `VK_HELP`                 | Help 键                   |
| `Num0`                    | `0x30`                    | 0 键                      |
| `Num1`                    | `0x31`                    | 1 键                      |
| `Num2`                    | `0x32`                    | 2 键                      |
| `Num3`                    | `0x33`                    | 3 键                      |
| `Num4`                    | `0x34`                    | 4 键                      |
| `Num5`                    | `0x35`                    | 5 键                      |
| `Num6`                    | `0x36`                    | 6 键                      |
| `Num7`                    | `0x37`                    | 7 键                      |
| `Num8`                    | `0x38`                    | 8 键                      |
| `Num9`                    | `0x39`                    | 9 键                      |
| `A`                       | `0x41`                    | A 键                      |
| `B`                       | `0x42`                    | B 键                      |
| `C`                       | `0x43`                    | C 键                      |
| `D`                       | `0x44`                    | D 键                      |
| `E`                       | `0x45`                    | E 键                      |
| `F`                       | `0x46`                    | F 键                      |
| `G`                       | `0x47`                    | G 键                      |
| `H`                       | `0x48`                    | H 键                      |
| `I`                       | `0x49`                    | I 键                      |
| `J`                       | `0x4a`                    | J 键                      |
| `K`                       | `0x4b`                    | K 键                      |
| `L`                       | `0x4c`                    | L 键                      |
| `M`                       | `0x4d`                    | M 键                      |
| `N`                       | `0x4e`                    | N 键                      |
| `O`                       | `0x4f`                    | O 键                      |
| `P`                       | `0x50`                    | P 键                      |
| `Q`                       | `0x51`                    | Q 键                      |
| `R`                       | `0x52`                    | R 键                      |
| `S`                       | `0x53`                    | S 键                      |
| `T`                       | `0x54`                    | T 键                      |
| `U`                       | `0x55`                    | U 键                      |
| `V`                       | `0x56`                    | V 键                      |
| `W`                       | `0x57`                    | W 键                      |
| `X`                       | `0x58`                    | X 键                      |
| `Y`                       | `0x59`                    | Y 键                      |
| `Z`                       | `0x5a`                    | Z 键                      |
| `LWin`                    | `VK_LWIN`                 | 左 Win 键                 |
| `RWin`                    | `VK_RWIN`                 | 右 Win 键                 |
| `Apps`                    | `VK_APPS`                 | 应用程序键                |
| `Sleep`                   | `VK_SLEEP`                | 计算机休眠键              |
| `F1`                      | `VK_F1`                   | F1 键                     |
| `F2`                      | `VK_F2`                   | F2 键                     |
| `F3`                      | `VK_F3`                   | F3 键                     |
| `F4`                      | `VK_F4`                   | F4 键                     |
| `F5`                      | `VK_F5`                   | F5 键                     |
| `F6`                      | `VK_F6`                   | F6 键                     |
| `F7`                      | `VK_F7`                   | F7 键                     |
| `F8`                      | `VK_F8`                   | F8 键                     |
| `F9`                      | `VK_F9`                   | F9 键                     |
| `F10`                     | `VK_F10`                  | F10 键                    |
| `F11`                     | `VK_F11`                  | F11 键                    |
| `F12`                     | `VK_F12`                  | F12 键                    |
| `F13`                     | `VK_F13`                  | F13 键                    |
| `F14`                     | `VK_F14`                  | F14 键                    |
| `F15`                     | `VK_F15`                  | F15 键                    |
| `F16`                     | `VK_F16`                  | F16 键                    |
| `F17`                     | `VK_F17`                  | F17 键                    |
| `F18`                     | `VK_F18`                  | F18 键                    |
| `F19`                     | `VK_F19`                  | F19 键                    |
| `F20`                     | `VK_F20`                  | F20 键                    |
| `F21`                     | `VK_F21`                  | F21 键                    |
| `F22`                     | `VK_F22`                  | F22 键                    |
| `F23`                     | `VK_F23`                  | F23 键                    |
| `F24`                     | `VK_F24`                  | F24 键                    |
| `NumLock`                 | `VK_NUMLOCK`              | 数字键盘锁定(NumLock)键   |
| `ScrollLock`              | `VK_SCROLL`               | ScrollLock 键             |
| `LShift`                  | `VK_LSHIFT`               | 左 Shift 键               |
| `RShift`                  | `VK_RSHIFT`               | 右 Shift 键               |
| `LCtrl`                   | `VK_LCONTROL`             | 左 Ctrl 键                |
| `RCtrl`                   | `VK_RCONTROL`             | 右 Ctrl 键                |
| `LAlt`                    | `VK_LMENU`                | 左 Alt 键                 |
| `RAlt`                    | `VK_RMENU`                | 右 Alt 键                 |
| `Colon`                   | `VK_OEM_1`                | `;` `:`                   |
| `Plus`                    | `VK_OEM_PLUS`             | `=` `+`                   |
| `Comma`                   | `VK_OEM_COMMA`            | `,` `<`                   |
| `Minus`                   | `VK_OEM_MINUS`            | `-` `_`                   |
| `Period`                  | `VK_OEM_PERIOD`           | `.` `>`                   |
| `QuestionMark`            | `VK_OEM_2`                | `/` `?`                   |
| `Tilde`                   | `VK_OEM_3`                | `` ` `` `~`               |
| `LBraces`                 | `VK_OEM_4`                | `[` `{`                   |
| `VerticalBar`             | `VK_OEM_5`                | `\` `\|`                  |
| `RBraces`                 | `VK_OEM_6`                | `]` `}`                   |
| `Quote`                   | `VK_OEM_7`                | `'` `"`                   |
| `Semicolon`               | `VK_OEM_1`                | `;` `:`                   |
| `Equal`                   | `VK_OEM_PLUS`             | `=` `+`                   |
| `Less`                    | `VK_OEM_COMMA`            | `,` `<`                   |
| `Underline`               | `VK_OEM_MINUS`            | `-` `_`                   |
| `Greater`                 | `VK_OEM_PERIOD`           | `.` `>`                   |
| `Slash`                   | `VK_OEM_2`                | `/` `?`                   |
| `BackQuote`               | `VK_OEM_3`                | `` ` `` `~`               |
| `LBrackets`               | `VK_OEM_4`                | `[` `{`                   |
| `Backslash`               | `VK_OEM_5`                | `\` `\|`                  |
| `RBrackets`               | `VK_OEM_6`                | `]` `}`                   |
| `SingleQuote`             | `VK_OEM_7`                | `'` `"`                   |

##### 说明

我没有在我的小键盘上找到映射到 `VK_SEPARATOR` 的键，但可以参考这个问题的回答：[StackOverflow](https://stackoverflow.com/questions/67916923/what-physical-key-maps-to-keycode-108-vk-separator)。  

对于 OEM 相关虚拟键码，使用美国标准键盘描述。  

##### 成员类型 Type

使用八位无符号整型作为枚举的基类型。  

```C++
private:
    using vk = u8;
public:
    using Type = vk;
```
