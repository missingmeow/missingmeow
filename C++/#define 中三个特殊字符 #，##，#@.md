# define 中的三个特殊符号：#，##，#@

## \#

```c
#define to_string(x) #x

char* str = to_string(12345);
```

该定义是给 x 加双引号，实际上就是把 x 转换成字符串。

## \#\#

```c
#define connect(x, y) x##y

int n = connect(123, 456);    //n = 123456
char* str = connect("abc", "def"); //str = "abcdef"
```

该定义是把 x，y 连接起来，类似字符串的 +，但又不太一样。

## \#@

该字符不是所有的编译都支持

```c
#define to_char(x) #@x

char a = to_char(1); // a = '1'
```

该定义就是给x加上单引号，结果返回是一个const char。

实际的应用可能只能考虑这种情况，如果变量名称长了不一定是什么东西：

```c
#define MESSAGE(x) printf("%s: %d\n", #x, x)

int main(){
    int i = 5;
    MESSAGE(i); // expands to printf("%s: %d\n", "i", x)
    return 0;
}
```

