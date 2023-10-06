# Petal~Exception

## 概述

命名空间 Petal  

本标头提供了框架使用的异常`Petal::Exception`，框架的异常由异常代码和异常描述(可选项)组成。  

注：异常类`Petal::Exception`不派生自`std::exception`。  

如果预处理器定义了宏`Petal_Enable_ExceptionDescription`，那么`Petal::Exception`会有一个C风格字符串`Petal::ptrc<Petal::ExcepChar> pt_desc`，用于引用描述异常的**字符串字面量**，否则，对象将不会记录描述。  

`Petal::Exception`对象的定义和抛出由框架完成，开发者不应该抛出此类型对象，但是需要catch此类异常，并处理。  

`Petal::ExcepChar`、`Petal_ExcepStr`等内容详见[Petal~String.md](Petal~String.md)。  

## 参考

### 命名空间 Petal

#### 类 Exception

##### 构造函数

Exception(Petal_ExcepArgs(code, desc)) noexcept;

```cpp
#if defined(Petal_Enable_ExceptionDescription)
#define Petal_ExcepArgs(code, desc) Petal::ExceptionCode code, Petal::ptrc<Petal::ExcepChar> desc
#define Petal_MakeExcepArgs(code, desc) code, Petal_ExcepStr(desc)
#else
#define Petal_ExcepArgs(code, desc) Petal::ExceptionCode code
#define Petal_MakeExcepArgs(code, desc) code
#endif
```

宏`Petal_ExcepArgs`受宏`Petal_Enable_ExceptionDescription`影响，该构造函数可能只接受`Petal::ExceptionCode code`也可能接受`Petal::ExceptionCode code, Petal::ptrc<Petal::ExcepChar> desc`两个参数。

使用`Petal_MakeExcepArgs`宏制作此构造函数的参数。

注：使用`Petal_MakeExcepArgs`宏制作参数时，desc的形式必须为 char 字符串字面量。
```cpp
throw Exception{ Petal_Make(ExceptionCode::Unknown, "Test") };
```
Petal::ExcepChar 可以是 char 类型也可以是 wchar_t 类型，因此字符串可能是 char 字符串也可能是 wchar_t 字符串。使用宏`Petal_MakeExcepArgs`制作参数时，则不需要关心字符串是 char 字符串还是 wchar_t 字符串，它将根据框架的定义自动把`"description"`替换为`"description"`或`L"description"`

##### 方法 Code

返回类型 Petal::ExceptionCode  

返回 ExceptionCode 类型的值。  

##### 方法 Desc

返回类型 Petal::ptrc&lt;Petal::ExcepChar>  

返回对异常的描述，如果未定义宏`Petal_Enable_ExceptionDescription`，那么它将始终返回`Petal_ExcepStr("[Petal] Exception")`。  

#### 枚举类 ExceptionCode

枚举了异常值，含义见各个枚举名称。  
