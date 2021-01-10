# SendMessage 和 PostMessage 的区别

１、首先是返回值意义的区别，我们先看一下 MSDN 里的声明：

```c++
LRESULT SendMessage(
        HWND hWnd,
        UINT Msg,
        WPARAM wParam,
        LPARAM lParam
);
BOOL PostMessage(
        HWND hWnd,
        UINT Msg,
        WPARAM wParam,
        LPARAM lParam
);
```

　　其中 4 个参数的意义是一样的，返回值类型不同（其实从数据上看他们一样是一个 32 位的数，只是意义不一样），LRESULT 表示的是消息被处理后的返回值，BOOL 表示的是消息是不是 Post 成功。

２、PostMessage 是异步的，SendMessage 是同步的。
　　PostMessage 只把消息放入队列，不管消息是否被处理就返回，消息可能不被处理；而 SendMessage 等待消息被处理完了之后才返回，如果消息不被处理，发送消息的线程将一直被阻塞。

３、如果在同一个线程内，SendMessage 发送消息时，由 USER32.DLL 模块调用目标窗口的消息处理程序，并将结果返回。SendMessage 在同一线程中发送消息并不入线程消息队列。PostMessage 发送消息时，消息要先放入线程的消息队列，然后通过消息循环分派到目标窗口（DispatchMessage）。

　　如果在不同线程内，SendMessage 发送消息到目标窗口所属线程的消息队列，然后发送消息的线程在 USER32.DLL 模块内监视和等待消息处理，直到目标窗口处理完返回。SendMessage 在返回前还做了很多工作，比如，响应别的线程向它 SendMessage。Post 到别的线程时，最好用 PostThreadMessage 代替 PostMessage，PostMessage 的 hWnd 参数可以是 NULL，等效于 PostThreadMessage + GetCurrentThreadId。Post WM_QUIT 时，应使用 PostQuitMessage 代替。

４、系统只整编（marshal）系统消息（0 到 WM_USER（0x0400） 之间的消息），发送用户消息（WM_USER 以上）到别的进程时，需要自己做整编。

　　用 PostMessage、SendNotifyMessage、SendMessageCallback 等异步函数发送系统消息时，参数里不可以使用指针，因为发送者并不等待消息的处理就返回，接受者还没处理指针就已经被释放了。

５、在 Windows 2000/XP 里，每个消息队列最多只能存放 10,000 个 Post 的消息，超过的还没被处理的将不会被处理，直接丢掉。这个值可以改得更大。
