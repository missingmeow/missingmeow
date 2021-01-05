# 文件操作接口fstream，如何创建utf-8文件

在windows平台下直接调用C++标准库的文件操作，默认都是生成ANSI文件的，有些时候我们想生成utf-8时要怎么做呢？

C++中最简单的操作文件头 `<fstream>`，把读写文件当做普通的输入输出流来处理，确实是方便不少。

常用属性先了解一下

```txt
ios::in     为输入(读)而打开文件
ios::out    为输出(写)而打开文件
ios::ate    初始位置：文件尾
ios::app    每个写操作之前都会移到文件末尾
ios::trunc  如果文件已存在则先删除该文件
ios::binary 将文件设置成二进制模式。没有这个标志，默认的是文本模式。
```

如果文件流已经访问到文件末尾了，也就是EOF标志被设置了，需要调用clear()函数来清除EOF标志。否则，再次访问文件会失败。

定位光标位置需要注意的是：

> seekg() 只能在ifstream中使用，设置文件读指针的位置，对应使用 tellg()
>
> seekp() 只能在ofstream中使用，设置文件写指针的位置，对应使用 tellp()

下面一些常用的操作展示一下：

```c++
#include <fstream> // 包含头文件

// 以二进制方式打开文件，只读
std::iftream infile("file_path", std::ios::in | std::ios::binary); 

if(infile.is_open()) // 判断文件是否打开
{
    // 这两步会返回整个文件的字节大小
    infile.seekg(0, std::ios::end); // 把光标定位到文件末尾
    DWORD length = infile.tellg(); 
    
    // 以二进制方式读取全部文件内容
    char* buffer = new char[length];
    infile.seekg(0); // 把光标定位到文件开头
    infile.read(buffer, length); 
    delete buffer[];
    
    // 非二进制直接读取一行数据
    std::string line;
    infile.seekg(5 * 1024, std::ios::beg); // 定位到距离文件开头 5k 字节的位置
    while(!infile.eof()) // 是否到达文件末尾 
    {
        char buffer[256];
        infile.getline(buffer, 256);
    }
}

infile.close(); // 最后记得关掉

// 以二进制方式打开文件，只写。加上属性（|ios::ate|ios::app）可以保证在文件后面继续添加，否则会清掉内容重写
std::oftream outFile("file_path", std::ios::out | std::ios::binary); 

if(outFile.is_open()) // 判断文件是否打开
{
    // utf-8 格式的BOM头，需要在第一次新建文件时写进去
    char szBOM[4] = {(char)0xEF, (char)0xBB, (char)0xBF, 0}; 
    outfile << szBOM; // 最简单的写文件
    outfile.flush(); // 不缓存，直接写进文件
    
    outFile.seekp(5 * 1024); // 光标定位到 5k 字节处
    outFile.tellp(); // 获取开头到当前光标的大小，5k
    // 基本来说写文件这样就可以实现大部分需求了
    
    // 如果有需要可以用二进制方式进行写文件
    char *buffer = new char[length];
    outFile.write(buffer, length);
}

outFile.close(); // 最后记得关掉

// 可读可写
std::fstream fFile("file_path", ios::in | ios::out);
fFile.close();
```
