# MFC中list列表的操作

d_list 绑定到指定控件，在初始化时DoDataExchange
> CListCtrl d_list;
>
> DDX_Control(pDX, IDC_LIST, d_list);

或者使用时找到指定的控件
> CListCtrl* d_list = GetDlgItem(IDC_LIST);

```c++
// 设置样式，插入标题栏

d_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
  
d_list.InsertColumn(0,L"姓名",LVCFMT_CENTER,70,LVCFMT_IMAGE|LVCFMT_CENTER);
d_list.InsertColumn(1,L"年纪",LVCFMT_CENTER,60);
d_list.InsertColumn(2,L"班级",LVCFMT_CENTER,60);
d_list.InsertColumn(3,L"成绩",LVCFMT_CENTER,60);

// 插入数据
d_list.InsertItem(0, L"zhangsan");

//设置/修改数据项
d_list.SetItemText(0, 1, L"9");
d_list.SetItemText(0, 2, L"三年纪一班");
d_list.SetItemText(0, 3, L"80");

// 删除数据项
d_list.DeleteItem(i);

// 如果需要双击实现修改数据，通过一个编辑框来添加数据
// 假设 d_edit 已经初始化完毕了
void CDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{

  NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
  CRect rc;
  if(pNMListView->iItem!=-1)
  {
      d_row=pNMListView->iItem;//m_row为被选中行的行序号（int类型成员变量）
      d_column=pNMListView->iSubItem;//m_column为被选中行的列序号（int类型成员变量）
      d_list.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc);//取得子项的矩形
      rc.left+=3;
      rc.top+=2;
      rc.right+=3;
      rc.bottom+=2;

      CString str=L"";
    //  char * ch=new char [128];
    str= d_list.GetItemText(pNMListView->iItem, pNMListView->iSubItem);//取得子项的内容
    d_edit.SetWindowText(str);//将子项的内容显示到编辑框中
    d_edit.ShowWindow(SW_SHOW);//显示编辑框
    d_edit.MoveWindow(&rc);//将编辑框移动到子项上面，覆盖在子项上
    d_edit.SetFocus();//使编辑框取得焦点
    d_edit.CreateSolidCaret(1,rc.Height()-5);//创建一个光标
    d_edit.ShowCaret();//显示光标
    d_edit.SetSel(-1);//使光标移到最后面
  }
  *pResult = 0;
}
```

关于刷新数据时闪屏解决方法，请多试几种方法，在某种情况下有些并不适用（实测1，2不行）

1. 使用SetRedraw禁止窗口重绘,操作完成后,再恢复窗口重绘

```c++
d_list.SetRedraw(FALSE); 
//以下为更新数据操作
//……
//恢复窗口重绘
d_list.SetRedraw(TRUE);

```

2. 使用LockWindowUpdate禁止窗口重绘，操作完成后，用UnlockWindowUpdate恢复窗口重绘

```c++
d_list.LockWindowUpdate(); 
//以下为更新数据操作
//……
//恢复窗口重绘
d_list.UnlockWindowUpdate(); 
```

3. 使用ListCtrl的内部双缓冲(推荐使用)

```c++
d_list.SetExtendedStyle(d_list.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER);
//VC6未定义LVS_EX_DOUBLEBUFFER宏，使用者可以自定义，如下：
#define LVS_EX_DOUBLEBUFFER 0x00010000
```

4. Custom Redraw

```c++
//既然是自绘，首先当然是重载CListCtrl类，并接管WM_ERASEBKGND消息，去掉默认的处理，改为不处理
BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
//响应WM_ERASEBKGND消息 
 return false;
 //屏蔽默认处理
 //return CListCtrl::OnEraseBkgnd(pDC);
}
void CListCtrlEx::OnPaint()
{
    //响应WM_PAINT消息
    CPaintDC dc(this); // device context for painting
    CRect rect;
    CRect headerRect;
    CDC MenDC;//内存ID表  
    CBitmap MemMap;
    GetClientRect(&rect);   
    GetDlgItem(0)->GetWindowRect(&headerRect);  
    MenDC.CreateCompatibleDC(&dc);  
    MemMap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
    MenDC.SelectObject(&MemMap);
    MenDC.FillSolidRect(&rect,RGB(228,236,243));  
    //这一句是调用默认的OnPaint(),把图形画在内存DC表上  
    DefWindowProc(WM_PAINT,(WPARAM)MenDC.m_hDC,(LPARAM)0);      
    //输出  
    dc.BitBlt(0,headerRect.Height(),rect.Width(),  rect.Height(),&MenDC,0, headerRect.Height(),SRCCOPY);  
    MenDC.DeleteDC();
    MemMap.DeleteObject();
}
```
