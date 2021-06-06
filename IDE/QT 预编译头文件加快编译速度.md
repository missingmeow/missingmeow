# Qt 预编译头文件(precompiled headers) 加快编译速度

```txt
版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
本文链接：https://blog.csdn.net/wangmi0354/article/details/51581845
```

简单点说就是: 可以把不常更改的头文件, 通过预编译头文件的方式进行一次编译, 提高之后的编译速度. 就是为了加快编译速度, 就是为了加快编译速度, 就是为了加快编译速度!

## 1. Qt支持的平台

qmake 支持的预编译头(precompiled headers)的编译器包括:

- Windows
1. nmake
2. Visual Studio projects (VS 2008 and later)
- OS X and iOS
1. Makefile
2. Xcode
- Unix
1. GCC 3.4 and above

## 2. 具体使用

非常简单, 官方有介绍.

### 2.1 先写一个`stable.h`

这个stable.h里面包含的就是需要预编译的头文件

```c++
// Add C includes here

#if defined __cplusplus
// Add C++ includes here
#include <stdlib>
#include <iostream>
#include <vector>
#include <QApplication> // Qt includes
#include <QPushButton>
#include <QDebug>
#include "thirdparty/include/libmain.h"//第三方类
#include "my_stable_class.h"//自己的稳定类
...
#endif 
```

### 2.2 工程配置文件.pro中添加

```pro
PRECOMPILED_HEADER = stable.h
```

就这两步就可以了! 这样就可以缩短编译时间了!

## 3 实例应用:解决中文乱码

解决windows下vc++ 12编译器qDebug()等中文乱码问题

在 `stable.h` 中添加

```c++
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
```
