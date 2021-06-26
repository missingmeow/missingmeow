## 说明

Windows在加载DLL的时候，需要一个入口函数，就如同控制台或DOS程序需要main函数、Win32程序需要WinMain函数一样。根据编写规范，Windows必须查找并执行DLL里的DllMain函数作为加载DLL的依据，它使得DLL得以保留在内存里。这个函数并不属于导出函数，而是DLL的内部函数。这意味着不能直接在应用工程中引用DllMain函数，DllMain是自动被调用的。

**注：** 一些例子中，DLL并没有提供DllMain函数，应用工程也能成功引用DLL，这是因为Windows在找不到DllMain的时候，系统会从其它运行库中引入一个不做任何操作的缺省DllMain函数版本，并不意味着DLL可以放弃DllMain函数。

## 函数原型

```c
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved)
```

## 参数说明

其中，APIENTRY被定义为__stdcall，它意味着这个函数以标准Pascal的方式进行调用，也就是WINAPI方式；

函数参数hModule是进程中的每个DLL模块被全局唯一的32字节的HINSTANCE句柄标识（句柄代表了DLL模块在进程虚拟空间中的起始地址， 只有在特定的进程内部有效 ） 。在Win32中，HINSTANCE和HMODULE的值是相同的，这两种类型可以替换使用。

参数ul_reason_for_call指明了被调用的原因。 共有4种，即PROCESS_ATTACH、PROCESS_DETACH、THREAD_ATTACH和THREAD_DETACH，以switch语句列出。

lpReserved 表示一个保留参数，目前已经很少使用

## 加载方式

DllMain函数在以下几种情况被调用：

### DLL被加载

一个程序要调用Dll里的函数，首先要先把DLL文件映射到进程的地址空间。要把一个DLL文件映射到进程的地址空间，有两种方法：静态链接和动态链接的LoadLibrary或者LoadLibraryEx。

当一个DLL文件被映射到进程的地址空间时，系统调用该DLL的DllMain函数，传递的fdwReason参数为DLL_PROCESS_ATTACH。这种调用只会发生在第一次映射时。如果同一个进程后来为已经映射进来的DLL再次调用LoadLibrary或者LoadLibraryEx，操作系统不会再用DLL_PROCESS_ATTACH调用DLL的DllMain函数，而是增加DLL的使用次数。不同进程用LoadLibrary同一个DLL时，每个进程的第一次映射都会用DLL_PROCESS_ATTACH调用DLL的DllMain函数。

### DLL被卸载

当DLL被从进程的地址空间解除映射时，系统调用了它的DllMain，传递的fdwReason值是DLL_PROCESS_DETACH。当 DLL处理该值时，它应该执行进程相关的清理工作。 DLL被从进程的地址空间解除映射的情况有两种：

1. FreeLibrary() 被调用。（有几个 LoadLibrary，就要有几个FreeLibrary）
2. 进程结束。 在进程结束前还没有解除DLL的映射，进程结束后会解除DLL映射。（如果进程的终结是因为调用了TerminateProcess，系统就不会用DLL_PROCESS_DETACH来调用DLL的DllMain函数。这就意味着DLL在进程结束前没有机会执行任何清理工作。）

注意：当用DLL_PROCESS_ATTACH调用DLL的DllMain函数时，如果返回FALSE，说明没有初始化成功，系统仍会用DLL_PROCESS_DETACH调用DLL的DllMain函数。因此， 必须确保没有清理那些没有成功初始化的东西。
    
### 单个线程启动

当进程创建一线程时，系统查看当前映射到进程地址空间中的所有DLL文件映像，并用值DLL_THREAD_ATTACH调用DLL的DllMain函数。 新创建的线程负责执行这次的DLL的DllMain函数，只有当所有的DLL都处理完这一通知后，系统才允许进程开始执行它的线程函数。

注意跟DLL_PROCESS_ATTACH的区别，我们在前面说过，第n(n>=2)次以后地把DLL映像文件映射到进程的地址空间时，只增加使用次数，而不用DLL_PROCESS_ATTACH调用DllMain。DLL_THREAD_ATTACH不同，进程中的每次建立线程，都会用值DLL_THREAD_ATTACH调用DllMain函数，哪怕是线程中建立线程也一样。

### 单个线程终止

如果线程调用了ExitThread来结束线程（线程函数返回时，系统也会自动调用ExitThread），系统查看当前映射到进程空间中的所有DLL文件映像，并用DLL_THREAD_DETACH来调用DllMain函数，通知所有的DLL去执行线程级的清理工作。
注意：如果线程的结束是因为系统中的一个线程调用了TerminateThread（），系统就不会用值DLL_THREAD_DETACH来调用所有DLL的DllMain函数。


- 引用自：https://blog.csdn.net/tiandao2009/article/details/79839182
