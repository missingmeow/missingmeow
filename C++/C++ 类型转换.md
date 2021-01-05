# C++ 类型转换

## const_cast

通常用来将对象的常量性转除。也是唯一有此能力的C++-style转型操作符。

## dynamic_cast

主要用来执行“安全向下转型”，也就是用来决定某对象是否归属继承体系中的某个类型。也就是说你可以利用 dynamic_cast ，将“指向 base class objects 的 pointers 或 references”转型为“指向 derived class objects 的 pointers 或 references”，并得知转型是否成功。如果转型失败，会以一个 null 指针（当转型对象是指针）或一个 exception（当转型对象是 reference）表现出来。它是唯一无法由旧式语法执行的动作，也是唯一可能耗费重大运行成本的转型动作。

## reinterpret_cast

意图执行低级转型，实际动作（及结果）可能取决于编译器，这也就表示它不可移植。例如将一个 *pointer to int* 转型为一个 *int* 。这一类转型在低级代码以外很少见。

## static_cast

用来强迫隐式转换，例如将 *non-const* 对象转为 *const* 对象，或将 *int* 转为 *double* 等等。它也可以用来执行上述多种转换的反向转换，例如将 *void** 指针转为 *typed* 指针，将 *pointer-to-base* 转为 *pointer-to-derived* 。但它无法将 const 转为 non-const，这个只有 const_cast 才办得到。

## 旧式转型

(T)*expression* //将expression转型为T

T(*expression*) //将expression转型为T

新式转型更好，原因：

1. 容易在代码中被辨识出来，因而得以简化“找出类型系统在哪个地点被破坏”的过程。
2. 各转型动作的目标愈窄化，编译器愈可能诊断出错误的运用。举个例子，如果你打算将常量性（constness）去掉，除非使用新式转型中的 const_cast 否则无法通过编译。
