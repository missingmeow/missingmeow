# 获取系统指定路径 SHGetSpecialFolderPath

在头文件 sdlobj.h 中声明

```c++
#include   <shlobj.h>  
#pragma comment(lib, "shell32.lib")
```

用法

```c++
BOOL SHGetSpecialFolderPath(
    HWND hwndOwner,    //可用GetDesktopWindow API取一个窗口句柄。或直接传null.
    LPTSTR lpszPath,         //返回路径的缓冲区 TCHAR path[]
    int nFolder,                  //标识代号
    BOOL fCreate              //true ：如果文件夹不存在则创建，false：不创建
);
```

nFolder | lpszPath
|--|--|
1 | C:/Documents and Settings/当前用户/「开始」菜单/程序
2 | C:/Documents and Settings/当前用户/My Documents
3 | C:/Documents and Settings/当前用户/Favorites
4 | C:/Documents and Settings/当前用户/「开始」菜单/程序/启动
5 | C:/Documents and Settings/当前用户/Recent
6 | C:/Documents and Settings/当前用户/SendTo
7 | C:/Documents and Settings/当前用户/「开始」菜单
8 | C:/Documents and Settings/当前用户/桌面
9 | C:/Documents and Settings/当前用户/NetHood
10 | C:/WINDOWS/Fonts
11 | C:/Documents and Settings/当前用户/Templates
12 | C:/Documents and Settings/All Users/「开始」菜单
13 | C:/Documents and Settings/All Users/「开始」菜单/程序
14 | C:/Documents and Settings/All Users/「开始」菜单/程序/启动
15 | C:/Documents and Settings/All Users/桌面
16 | C:/Documents and Settings/当前用户/Application Data
17 | C:/Documents and Settings/当前用户/PrintHood
18 | Settings/Application Data
19 | C:/Documents and Settings/All Users/Favorites
20 | Settings/Temporary Internet Files
21 | C:/Documents and Settings/当前用户/Cookies
22 | Settings/History
23 | C:/Documents and Settings/All Users/Application Data
24 | C:/WINDOWS
25 | C:/WINDOWS/system32
26 | C:/Program Files

例子：

```c++
TCHAR path[MAX_PATH + 2];
SHGetSpecialFolderPath(NULL, path, CSIDL_DESKTOPDIRECTORY, FALSE); //获取桌面的地址
```
