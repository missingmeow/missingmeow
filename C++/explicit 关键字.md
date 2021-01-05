# explicit 关键字

explicit用来防止由构造函数定义的隐式转换。

这个关键字只能用在类构造函数。它的作用是不能进行隐式转换。

要明白它的作用，首先要了解隐式转换：可以用单个实参来调用的构造函数定义了从形参类型到该类类型的一个隐式转换。

例如:

```c++
class things
{
public:
    things(const std::string& name =""):
        m_name(name),height(0),weight(10){}
    int CompareTo(const things & other);
    std::string m_name;
    int height;
    int weight;
};
```

这里things的构造函数可以只用一个实参完成初始化。所以可以进行一个隐式转换，像下面这样：

```c++
things a;
................ //在这里被初始化并使用。
std::string str ="book_1";
//由于可以隐式转换，所以可以下面这样使用
int result = a.CompareTo(str);
```

这段程序使用一个string类型对象作为实参传给things的CompareTo函数。这个函数本来是需要一个tings对象作为实参。现在编译器使用string str来构造并初始化一个 things对象，新生成的临时的things对象被传递给CompareTo函数，并在离开这段函数后被析构。

这种行为的正确与否取决于业务需要。假如你只是想测试一下a的重量与10的大小之比，这么做也许是方便的。但是假如在CompareTo函数中还涉及到了要除以初始化为0的height属性，那么这么做可能就是错误的。需要在构造tings之后更改height属性不为0。所以要限制这种隐式类型转换。

那么这时候就可以通过将构造函数声明为explicit，来防止隐式类型转换。

explicit关键字只能用于类内部的构造函数声明上，而不能用在类外部的函数定义上。现在things类像这样：

```c++
class things
{
public:
    explicit things(const std::string&name =""):
        m_name(name),height(0),weight(0){}
    int CompareTo(const things & other);
    std::string m_name;
    int height;
    int weight;
};
```

这时你仍然可以通过显示使用构造函数完成上面的类型转换：

```c++
things a;
................//在这里被初始化并使用。
std::string nm ="book_1";
//显示使用构造函数
int result = a.CompareTo(things(nm));
```
