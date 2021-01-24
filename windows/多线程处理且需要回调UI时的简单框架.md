# 多线程处理且需要回调UI时的简单框架

1. kernel 核心模块，在网络模块中，主要是回调时涉及界面刷新必须要在UI线程中操作，可以统一在这里处理。

```c++
class kernel
{
    kernel()
    {
    	WNDCLASS wc;
    	wc.style		= CS_HREDRAW|CS_VREDRAW;
    	wc.lpfnWndProc	= WndProc;
    	wc.cbClsExtra	= 0;
    	wc.cbWndExtra	= 0;
    	wc.hInstance	= ::AfxGetInstanceHandle();
    	wc.hIcon		= NULL;
    	wc.hCursor		= NULL;
    	wc.hbrBackground= NULL;
    	wc.lpszMenuName	= NULL;
    	wc.lpszClassName= DeClassName;
    
    	if (!::RegisterClass(&wc))
    		return NULL;
    
    	m_hWnd = ::CreateWindowEx(0, DeClassName, DeWindowName,
    		0, 0, 0, 1, 1, HWND_MESSAGE, NULL, 0, NULL);
    
    	if (m_hWnd != NULL)
    	{
    		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
    		::SetTimer(hWnd, DeTimerID, 1000, NULL);
    	}
    }
    
    ~kernel(){}
    
    void Post2UI(CRunner* pRun)
    {
        if(pRun)
            ::PostMessage(m_hWnd, (UINT)-1, (WPARAM)pRun, 0);
    }
    
    LRESULT CALLBACK WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if(uMsg == (UINT)-1)
        {
            CRunner* pRun = (CRunner*)wParam;
            if(pRun)
            {
                pRun->Run();
                pRun->Release();
            }
        }
    }
    
private:
    HWND    m_hWnd;
};
```

2. ClientBase 实际逻辑运行基础模块，线程调用的基础模块。

```c++
class ClientBase
{
    ClientBase();
    virtual ~ClientBase();
    
    virtual void Run() = 0;
};
```

3. ThreadBase 线程基础模块。

```c++
class ThreadBase
{
public:
    ThreadBase(ClientBase* pClient);
    ~ThreadBase()
    {
        if(m_pClientBase)
        {
            delete m_pClientBase;
        }
    }
    
    void Create()
    {
        m_thread = std::thread(std::bind(&ThreadBase::WorkThread, this));
    }
    
    void WorkThread()
    {
        if(m_pClientBase)
            m_pClientBase->Run();
    }
private:
    std::thread     m_thread;
    ClientBase*     m_pClientBase;
};
```

4. UICallback 回调模块，访问完需要接受回调操作的须继承这个类。 

```c++
class UICallback
{
    virtual void UIMsgCallback(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
```

5. DataBase 数据模块，所有自定义都继承这个数据处理，可统一释放资源

```c++
struct DataBase
{
    virtual void Release() = 0;
};

struct BaseData : public DataBase
{
    IntData(int n) : m_nData(n) {}
    void Release() { delete this; }
    int m_nData;
}
```

6. Runner UI线程统一处理调用真正的回调函数

```c++
class CRunner
{
public:
    CRunner(UICallback* pCallback, UINT uMsg, int nRes, DataBase* pData);
    ~CRunner()
    {
        if(m_pDataBase)
        {
            m_pDataBase->Release();
            m_pDataBase = nullptr;
        }
    }
    
    void Release() { delete this; }
    
    void Run()
    {
        if(m_pCallback)
            m_pCallback->UIMsgCallback(m_uMsg, (WPARAM)m_nResult, (LPARAM)m_pDataBase);
            
        if(m_pDataBase)
        {
            m_pDataBase->Release();
            m_pDataBase = nullptr;
        }
    }
    
private:
    UINT        m_uMsg;
    int         m_rResult;
    UICallback* m_pCallback;
    DataBase*   m_pDataBase;
};
```
