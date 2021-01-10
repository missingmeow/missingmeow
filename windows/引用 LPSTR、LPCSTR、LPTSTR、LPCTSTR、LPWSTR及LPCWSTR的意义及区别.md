# 引用 LPSTR、LPCSTR、LPTSTR、LPCTSTR、LPWSTR及LPCWSTR的意义及区别

1、ANSI（即MBCS）：为多字节字符集，它是不定长表示世界文字的编码方式。ANSI表示英文字母时就和ASCII一样，但表示其他文字时就需要用多字节。

2、Unicode：用两个字节表示一个字符的编码方式。比如字符'A'在ASCII下面用一个字节表示，而在Unicode下面用两个字节表示，其中高字节用“0”填充；函数'程'在ASCII下面用两个字节表示，而在Unicode下面也是用两个字节表示。Unicode的用处就是定长表示世界文字，据统计，用两个字节可以编码现存的所有文字而没有二义。

3、Windows下的程序设计可以支持ANSI和Unicode两种编码方法的字符串，具体使用哪种就要看定义了MBCS宏还是Unicode宏。MBCS宏对应的字符串指针为LPSTR（即char*），Unicode对应的指针为LPWSTR（即unsigned char*）。为了写程序的方便，微软定义了类型LPTSTR，在MBCS下它表示char*，在Unicode下它表示unsigned char*，这就可以重定义一个宏进行不同字符集的转换了。

4、关系

- LPSTR： 32bit指针 指向一个字符串，每个字符占1字节。 相当于　char *
　　
- LPCSTR: 32-bit指针 指向一个常字符串，每个字符占1字节。　相当于　const char *
　　
- LPTSTR: 32-bit指针 每字符可能占1字节或2字节，取决于Unicode是否定义
　　
- LPCTSTR:32-bit指针 指向一个常字符串,每字符可能占1字节或2字节，取决于Unicode是否定义
　　
- LPWSTR: 32-bit指针，指向一个unicode字符串的指针,每个字符占2字节。

- LPCWSTR:32-bit指针, 指向一个unicode字符串常量的指针,每个字符占2字节。

　　在上面的类型中，L表示long, P表示指针,C表示constant, T表示指针指向的字符占的字节数取决于Unicode是否定义，W表示wide，STR就是string的意思
　　
```c++
　　LPSTR = char *
　　LPCSTR = const char *
　　
　　LPTSTR = _TCHAR *　（或　TCHAR *）
　　LPCTSTR = const _TCHAR * （或　const TCHAR *）
　　
　　LPWSTR = wchar_t *
　　LPCWSTR = const wchar_t *
```

5、Windows使用两种字符集ANSI和Unicode，前者在处理英文字符时使用单字节方式，在处理中文字符时使用双字节方式。后者不管是英文字符还是中文字符都是采用双字节方式表示。Windows NT的所有与字符有关的函数都提供了两种方式的版本，而Windows 9x只支持ANSI方式。_T一般同字符常量相关，如_T("你好！");。如果编码方式为ANSI，_T不起任何作用；如果编码方式为Unicode，编译器会把"你好！"以
Unicode方式进行保存。_T和_L的区别在于，_L不管你是以什么方式编译，一律以Unicode方法保存。

6、L表示字符串资源采用Unicode编码方式，示例如下：
wchar_t Str[]=L"Hello World!"; //每个字符都用2个字节来存储

7、_T是一个适配宏。当定义了_UNICODE时，_T和L相同；否则，_T的字符串采用ANSI编码方式。示例如
下：

LPTSTR lptStr=new TCHAR[32];
TCHAR* szBuf=_T("Hello");

以上两个语句，无论是在ANSI编码方式，还是在Unicode编码方式下都是正确的。

8、微软推荐使用相匹配的字符串函数。例如，在处理LPTSTR或LPCTSTR的时候，应该使用_tcslen来替代strlen函数。否则，在Unicode编码方式下，strlen不能处理wchar_t*的字符串。

9、T是一个非常重要的符号（TCHAR、LPTSTR、LPCTSTR、_T()和_TEXT()等），它表示使用一个中间类型，既不明确表示使用MBCS，也不明确表示使用Unicode。到底使用哪种字符集，在编译的时候才决定。

10、注意L 和　_T 的使用方法。
　　
　　LPTSTR, LPCTSTR, 及_T(常量字符串) 受_T 影响。
　　
　　介绍字符串类型：char, wchar_t, TCHAR, CHAR, WCHAR.
　　
　　TCHAR　的定义：
　　
　　#ifdef UNICODE
　　 typedef wchar_t TCHAR;
　　#else
　　 typedef unsigned char TCHAR;
　　#endif
　　
　　typedef unsigned char CHAR;
　　typedef unsigned wchar_t WCHAR;
　　
　　由此可以看出，unsigned char 和　wchar_t 都是基本数据类型，　CHAR实施上就是unsigned char, WCHAR为wchar_t ，而TCHAR根据是否支持unicode而不同。
　　
　　在程序中，可以用的名称有５种，unsigned char（CHAR）, wchar_t（WCHAR）, TCHAR，　但为了扩展性和兼容性，推荐使用TCHAR.
