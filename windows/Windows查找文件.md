# Windows查找文件

1.头文件

```c++
#include <windows.h>
```

2.方法

```c++
typedef struct _WIN32_FIND_DATA {
DWORD dwFileAttributes;                 //文件属性
FILETIME ftCreationTime;                  // 文件创建时间
FILETIME ftLastAccessTime;               // 文件最后一次访问时间
FILETIME ftLastWriteTime;                 // 文件最后一次修改时间
DWORD nFileSizeHigh;                     // 文件长度高32位
DWORD nFileSizeLow;                      // 文件长度低32位
DWORD dwReserved0;                      // 系统保留
DWORD dwReserved1;                      // 系统保留
TCHAR cFileName[ MAX_PATH ];     // 长文件名
TCHAR cAlternateFileName[ 14 ];     // 8.3格式文件名
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;
```

这个结构体在用FindFirstFile()和findnext()函数去查找磁盘文件时经常用来保存找到的文件属性，因此可以通过这个结构作为获取和更改文件属性的手段。

```c++
HANDLE FindFirstFile(
  LPCTSTR lpFileName,    // 欲搜索的文件名
  LPWIN32_FIND_DATA lpFindFileData //保存找到的文件的属性
); 
```

搜索文件名可参考如下：

```c
c:\Windows\*.*                    //在c:\Windows目录中查找所有文件
c:\Windows\System32\*.dll         //在c:\Windows\System32目录中查找所有dll文件
c:\Windows\System.ini             //在c:\Windows目录中查找System.ini文件
c:\Windows\a???.*                      //在c:\Windows目录中查找所有以a开头的文件名长度.为4个字符的文件
Test.dat                                       //在当前目录查找Test.dat文件
*.*                                                //在当前目录查找所有文件
```

如果调用成功返回一个句柄，可用来做为 FindNextFile() 或 FindClose() 参数
调用失败，则返回为INVALID_HANDLE_VALUE(即-1) ，可调用 GetLastError() 来获取错误信息

```c++
BOOL FindNextFile( 
  HANDLE hFindFile,     // 上一次查找返回的文件句柄
  LPWIN32_FIND_DATA lpFindFileData  //保存找到的文件的属性
); 
```

与 FindFirstFile() 不同的是，第一个参数返回的是文件句柄。当文件查找成功时返回TRUE，否则返回FALSE。

```c++
BOOL FindClose(  
 HANDLE hFindFile    // 上一次查找返回的文件句柄
); 
```

关闭由FindFirstFile函数创建的一个搜索句柄。返回值非零表示成功，零表示失败，并可调用 GetLastError() 来获取错误信息。
