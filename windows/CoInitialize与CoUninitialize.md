# CoInitialize与CoUninitialize

说明：
CoInitialize是Windows提供的API函数，用来告诉 Windows 以单线程的方式创建com对象。应用程序调用com库函数（除CoGetMalloc和内存分配函数）之前必须初始化com库。

语法：

HRESULT CoInitialize(
_In_opt_ LPVOID pvReserved        //参数被保留，且必须为NULL
);

返回结果：

S_OK : 该线程中COM库初始化成功
S_FALSE: 该线程中COM库已经被初始化 CoInitialize () 标明以单线程方式创建。

使用 CoInitialize 创建可以使对象直接与线程连接，得到最高的性能。
CoInitialize并不装载COM 库，它只用来初始化当前线程使用什么样的套件。使用这个函数后，线程就和一个套件建立了对应关系。线程的套件模式决定了该线程如何调用COM对象，是否需要列集等。

CoInitialize ()并不会干扰客户和服务器之间的通信，它所做的事情是让线程注册一个套件，而线程运行过程中必然在此套件。

CoInitialize和CoUninitialize必须成对使用。

创建新的应用程序就调用CoInitializeEx代替CoInitialize。
