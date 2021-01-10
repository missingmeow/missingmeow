# Windows窗口改变大小

```c++
//有个Button按钮，按钮ID为“IDC_BUTTON2”，初始化的字符串为“收缩<<”
//以下是该按钮的消息响应函数，用于改变窗口的大小
//PS：IDC_SEPARATOR是个空的picture控件用于区分窗口收缩前和扩展后的大小


CString str;
 if(GetDlgItemText(IDC_BUTTON2,str),str=="收缩<<")
 {
  SetDlgItemText(IDC_BUTTON2,"扩展>>");
 }
 else
 {
  SetDlgItemText(IDC_BUTTON2,"收缩<<");
 }

 static CRect rectLarge;
 static CRect rectSmall;
 
 if(rectLarge.IsRectNull())
 {
  CRect rectSeparator;
  GetWindowRect(&rectLarge);
  GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);

  rectSmall.left=rectLarge.left;
  rectSmall.top=rectLarge.top;
  rectSmall.right=rectLarge.right;
  rectSmall.bottom=rectSeparator.bottom;
 }
 if(str=="收缩<<")
 {
  SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),
   SWP_NOMOVE | SWP_NOZORDER);
 }
 else
 {
  SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),
   SWP_NOMOVE | SWP_NOZORDER);
 }
```
