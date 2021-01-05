# boost::locale 字符串转换用法

简单示例：

1、between把 source 这个字串从BIG5 转换到 UTF-8：

```c++
string source = "BIG5字符串";  
string s = boost::locale::conv::between( source, "UTF-8", "BIG5" );
```

2、to_utf可以输出 string ，也可以输出成 wstring。像下面的例子，就是把 sSource 这个 BIG-5 编码的字串，依序转换成 wstring 和 string 的字串。

```c++
string sSource = "BIG-5字符串";  
wstring ws = boost::locale::conv::to_utf<wchar_t>( sSource, "BIG5" );  
string  ss = boost::locale::conv::to_utf<char>( sSource, "BIG5" );
```

3、from_utf 是把 UTF 字串（string 或wstring）、转换为特定编码的字串用的，他可以转换string 或wstring 的字串，但是输出一定是string。

下面的例子，就是把 sSource 和 wSource 这两个 UTF 字串，都转换成 BIG-5 的 string 字串。

```c++
string  sSource =  "字符串";  
wstring wSource = L"...";  
string  ss1 = boost::locale::conv::from_utf( wSource, "BIG5" );  
string  ss2 = boost::locale::conv::from_utf( sSource, "BIG5" );
```

4、utf_to_utf的目的，是在 UTF 的 string 字串和 wstring 字串之间做转换，下面的例子，就是把类型是 string 的 sSource 转换成 wstring、并把类型是 wstring 的 wSource 转换成 string

```c++
string  sSource =  "...";  
wstring wSource = L"...";  
wstring wStr = boost::locale::conv::utf_to_utf<wchar_t>( sSource );  
string  sStr = boost::locale::conv::utf_to_utf<char>( wSource );
```
