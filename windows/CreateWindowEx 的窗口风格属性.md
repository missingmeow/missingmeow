# CreateWindowEx 的窗口风格属性

在Window环境下，创建一个窗口都要调用`CreateWindowEx`，个人觉得最重要的还是其中的风格属性，比如要实现windows7下最大化最小化时动画显示要怎么做呢？这是笔者之前在用DuiLib时构建窗口遇到的问题，这里是根据窗口属性的组合来实现的！

在DuiLib中`WindowImplBase`类里实现的`OnCreate`函数，里面会重新`SetWindowLong`设置属性，导致一开始设置的属性会变，一直出不来最小化动画显示，这里是个坑啊。

解决了上个问题后，看看把`dwStyle`设置成`WS_OVERLAPPEDWINDOW`试下（`dwExStyle`不能设置为`WS_EX_TOOLWINDOW`），会有惊喜哦，前提是把系统属性性能里的`在最大化和最小化动态显示窗口打开`。

```c++
HWND CreateWindowEx(
DWORD dwExStyle,        //窗口的扩展风格
LPCTSTR lpClassName,    //指向注册类名的指针
LPCTSTR lpWindowName,   //指向窗口名称的指针
DWORD dwStyle,          //窗口风格
int x,                  //窗口的水平位置
int y,                  //窗口的垂直位置
int nWidth,             //窗口的宽度
int nHeight,            //窗口的高度
HWND hWndParent,        //父窗口的句柄
HMENU hMenu,            //菜单的句柄或是子窗口的标识符
HINSTANCE hInstance,    //应用程序实例的句柄
LPVOID lpParam          //指向窗口的创建数据
);
```

`dwExStyle` 指定窗口的扩展风格。该参数可以是下列值：

1. `WS_EX_ACCEPTFILES`：指定以该风格创建的窗口接受一个拖拽文件。
2. `WS_EX_APPWINDOW`：当窗口可见时，将一个顶层窗口放置到任务条上。
3. `WS_EX_CLIENTEDGE`：指定窗口有一个带阴影的边界。
4. `WS_EX_COMPOSITED`
5. `WS_EX_CONTEXTHELP`：在窗口的标题条包含一个问号标志。当用户点击了问号时，鼠标光标变为一个问号的指针、如果点击了一个子窗口，则子窗口接收到WM_HELP消息。子窗口应该将这个消息传递给父窗口过程，父窗口再通过HELP_WM_HELP命令调用WinHelp函数。这个Help应用程序显示一个包含子窗口帮助信息的弹出式窗口。 `WS_EX_CONTEXTHELP`不能与`WS_MAXIMIZEBOX`和`WS_MINIMIZEBOX`同时使用。
6. `WS_EX_CONTROLPARENT`：允许用户使用Tab键在窗口的子窗口间搜索。
7. `WS_EX_DLGMODALFRAME`：创建一个带双边的窗口；该窗口可以在dwStyle中指定WS_CAPTION风格来创建一个标题栏。
8. `WS_EX_LAYERED`：创建一个分层窗口
9. `WS_EX_LAYOUTRTL`
10. `WS_EX_LEFT`：窗口具有左对齐属性，这是缺省设置的。
11. `WS_EX_LEFTSCROLLBAR`：如果外壳语言是如Hebrew，Arabic，或其他支持reading order alignment的语言，则标题条（如果存在）则在客户区的左部分。若是其他语言，在该风格被忽略并且不作为错误处理。
12. `WS_EX_LTRREADING`：窗口文本以LEFT到RIGHT（自左向右）属性的顺序显示。这是缺省设置的。
13. `WS_EX_MDICHILD`：创建一个MDI子窗口。
14. `WS_EX_NOACTIVATE`
15. `WS_EX_NOINHERITLAYOUT`
16. `WS_EX_NOPATARENTNOTIFY`：指明以这个风格创建的窗口在被创建和销毁时不向父窗口发送WM_PARENTNOTFY消息。
17. `WS_EX_NOREDIRECTIONBITMAP`
18. `WS_EX_OVERLAPPEDWINDOW`：`WS_EX_CLIENTEDGE`和`WS_EX_WINDOWEDGE`的组合。
19. `WS_EX_PALETTEWINDOW`：`WS_EX_WINDOWEDGE`, `WS_EX_TOOLWINDOW`和`WS_WX_TOPMOST`风格的组合。
20. `WS_EX_RIGHT`:窗口具有普通的右对齐属性，这依赖于窗口类。只有在外壳语言是如Hebrew,Arabic或其他支持读顺序对齐（reading order alignment）的语言时该风格才有效，否则，忽略该标志并且不作为错误处理。
21. `WS_EX_RIGHTSCROLLBAR`：垂直滚动条在窗口的右边界。这是缺省设置的。
22. `WS_EX_RTLREADING`：如果外壳语言是如Hebrew，Arabic，或其他支持读顺序对齐（reading order alignment）的语言，则窗口文本是一自左向右）RIGHT到LEFT顺序的读出顺序。若是其他语言，在该风格被忽略并且不作为错误处理。
23. `WS_EX_STATICEDGE`：为不接受用户输入的项创建一个3一维边界风格
24. `WS_EX_TOOLWINDOW`：创建工具窗口，即窗口是一个游动的工具条。工具窗口的标题条比一般窗口的标题条短，并且窗口标题以小字体显示。工具窗口不在任务栏里显示，当用户按下alt+Tab键时工具窗口不在对话框里显示。如果工具窗口有一个系统菜单，它的图标也不会显示在标题栏里，但是，可以通过点击鼠标右键或Alt+Space来显示菜单。
25. `WS_EX_TOPMOST`：指明以该风格创建的窗口应放置在所有非最高层窗口的上面并且停留在其L，即使窗口未被激活。使用函数SetWindowPos来设置和移去这个风格。
26. `WS_EX_TRANSPARENT`：指定以这个风格创建的窗口在窗口下的同属窗口已重画时，该窗口才可以重画。
由于其下的同属窗口已被重画，该窗口是透明的。
27. `WS_EX_WINDOWEDGE`


`dwStyle`
指定创建窗口的风格。该参数可以是下列窗口风格的组合再加上说明部分的控制风格。

风格意义:

1. `WS_BORDER`：创建一个带边框的窗口。
1. `WS_CAPTION`：创建一个有标题框的窗口（包括`WS_BODER`风格）。
1. `WS_CHILD`：创建一个子窗口。这个风格不能与`WS_POPUP`风格合用。
1. `WS_CHILDWINDOW`：与`WS_CHILD`相同。
1. `WS_CLIPCHILDREN`：当在父窗口内绘图时，排除子窗口区域。在创建父窗口时使用这个风格。
1. `WS_CLIPSIBLINGS`：排除子窗口之间的相对区域，也就是，当一个特定的窗口接收到WM_PAINT消息时，WS_CLIPSIBLINGS 风格将所有层叠窗口排除在绘图之外，只重绘指定的子窗口。如果未指定`WS_CLIPSIBLINGS`风格，并且子窗口是层叠的，则在重绘子窗口的客户区时，就会重绘邻近的子窗口。
1. `WS_DISABLED`：创建一个初始状态为禁止的子窗口。一个禁止状态的窗口不能接受来自用户的输入信息。
1. `WS_DLGFRAME`：创建一个带对话框边框风格的窗口。这种风格的窗口不能带标题条。
1. `WS_GROUP`：指定一组控制的第一个控制。这个控制组由第一个控制和随后定义的控制组成，自第二个控制开始每个控制，具有`WS_GROUP`风格，每个组的第一个控制带有`WS_TABSTOP`风格，从而使用户可以在组间移动。用户随后可以使用光标在组内的控制间改变键盘焦点。
1. `WS_HSCROLL`：创建一个有水平滚动条的窗口。
1. `WS_ICONIC`：创建一个初始状态为最小化状态的窗口。与`WS_MINIMIZE`风格相同。
1. `WS_MAXIMIZE`：创建一个初始状态为最大化状态的窗口。
1. `WS_MAXIMIZEBOX`：创建一个具有最大化按钮的窗口。该风格不能与`WS_EX_CONTEXTHELP`风格同时出现，同时必须指定`WS_SYSMENU`风格。
2. `WS_MINIMIZE`
3. `WS_MINIMIZEBOX`
1. `WS_OVERLAPPED`：产生一个层叠的窗口。一个层叠的窗口有一个标题条和一个边框。与`WS_TILED`风格相同。
1. `WS_OVERLAPPEDWINDOW`：创建一个具有`WS_OVERLAPPED`，`WS_CAPTION`，`WS_SYSMENU`，`WS_THICKFRAME`，`WS_MINIMIZEBOX`，`WS_MAXIMIZEBOX`风格的层叠窗口，与`WS_TILEDWINDOW`风格相同。
1. `WS_POPUP`：创建一个弹出式窗口。该风格不能与WS_CHILD风格同时使用。
1. `WS_POPUPWINDOW`：创建一个具有`WS_BORDER`，`WS_POPUP`，`WS_SYSMENU`风格的窗口，`WS_CAPTION`和`WS_POPUPWINDOW`必须同时设定才能使窗口某单可见。
1. `WS_SIZEBOX`：创建一个可调边框的窗口，与`WS_THICKFRAME`风格相同。
1. `WS_SYSMENU`：创建一个在标题条上带有窗口菜单的窗口，必须同时设定`WS_CAPTION`风格。
1. `WS_TABSTOP`：创建一个控制，这个控制在用户按下Tab键时可以获得键盘焦点。按下Tab键后使键盘焦点转移到下一具有`WS_TABSTOP`风格的控制。
1. `WS_THICKFRAME`：创建一个具有可调边框的窗口，与`WS_SIZEBOX`风格相同。
1. `WS_TILED`：产生一个层叠的窗口。一个层叠的窗口有一个标题和一个边框。与`WS_OVERLAPPED`风格相同。
1. `WS_TILEDWINDOW`:创建一个具有`WS_OVERLAPPED`，`WS_CAPTION`，`WS_SYSMENU`，`WS_THICKFRAME`，`WS_MINIMIZEBOX`，`WS_MAXIMIZEBOX`风格的层叠窗口。与`WS_OVERLAPPEDWINDOW`风格相同。
1. `WS_VISIBLE`：创建一个初始状态为可见的窗口。
1. `WS_VSCROLL`：创建一个有垂直滚动条的窗口。
