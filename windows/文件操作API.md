# 文件操作API

文件读写实质也是调用API函数，微软给我们提供了强大的文件读写编程接口。
读写的一般步骤是：
    1.调用CreateFile函数打开或者创建文件，返回文件句柄

```c++
HANDLE CreateFile( 
  LPCTSTR lpFileName, 
  DWORD dwDesiredAccess, 
  DWORD dwShareMode, 
  LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
  DWORD dwCreationDisposition, 
  DWORD dwFlagsAndAttributes, 
  HANDLE hTemplateFile
);
```

——lpFileName —— 参数指向要创建或打开的文件名，需要包含全路径 。例如：c:\\a.txt
——dwDesiredAccess —— 指定类型的访问对象。
    GENERIC_READ：表示允许对设备进行读访问；
    GENERIC_WRITE：表示允许对设备进行写访问（可组合使用）；
    0：表示只允许获取与一个设备有关的信息 。
——dwShareMode —— 参数表示存取文件的方式，指明了要对文件进行何种操作。
    0：表示不允许再被打开
    FILE_SHARE_READ：表示允许其他程序以只读方式打开
    FILE_SHARE_WRITE：表示允许其他程序以写方式打开
    FILE_SHARE_DELETE：表示运行其他程序可以对其进行删除操作
——lpSecurityAttributes —— 指向一个SECURITY_ATTRIBUTES结构的指针，定义了文件的安全特性（如果操作系统支持的话）。
——dwCreationDisposition —— 指明文件存在或不存在时的操作。
    CREATE_NEW：如果文件不存在则创建文件，存在则调用失败
    CREATE_ALWAYS：如果存在，以覆盖方式创建
    OPEN_EXISTING：如果存在则打开，否则失败
    OPEN_ALWAYS：如果文件存在则打开，否则创建文件
——dwFlagsAndAttributes：表示新建文件的属性
    FILE_ATTRIBUTE_ARCHIVE：存档类型
    FILE_ATTRIBUTE_NORMAL：默认属性
    FILE_ATTRIBUTE_HIDDEN：隐藏类型
    FILE_ATTRIBUTE_READONLY：只读类型
    FILE_ATTRIBUTE_SYSTEM：系统类型
——hTemplateFile：指向用于存取的模板文件的句柄，系统会复制模板文件的所有属性到新创建的文件，没有则为NULL

如执行成功，则返回文件句柄。 INVALID_HANDLE_VALUE表示出错，会设置GetLastError。即使函数成功，但若文件存在，且指定了CREATE_ALWAYS 或 OPEN_ALWAYS，GetLastError也会设为ERROR_ALREADY_EXISTS。

    2.利用文件句柄调用WriteFile或ReadFile函数写入或读取文件
    3.调用CloseHandle函数关闭打开的文件句柄
