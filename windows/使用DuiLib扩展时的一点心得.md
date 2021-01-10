# 使用DuiLib扩展时的一点心得

对于基类的设计对于一般属性外的函数最好都是虚函数。比如在`CTreeNodeUI`控件中，新添加了一个`SetItemText()`函数，当我想派生这个类实现更丰富的功能时发现这个函数不通用，因为该对象的基类指针调用这个函数时就不能调用到我需要的那个，也就是实际对象应该调用的那个。不过一般属性的设置函数就不用虚函数，比如常用的`SetBkColor()`。

在树控件部分实现中，由于原生`CTreeNodeUI`不支持部分实现效果，需要派生此类，但此时遇到一个问题。在向`CTreeViewUI`添加元素时

```c++
if (_tcsicmp(pControl->GetClass(), _T("TreeNodeUI")) != 0)
    return false;
```

这里每个类的类名都是唯一的，也就是说这里能且只能添加`GetClass()`返回是“TreeNodeUI”的控件，显然这是不合理，如果是这个类派生出去的子类应该也能被添加到`CTreeViewUI`里面，在不能更改第三方库的情况下这种情况就会让人很困扰。

其设计过程中可以参考向`CTreeNodeUI`添加child控件，如下

```c++
if(NULL == static_cast<CTreeNodeUI*>(pControl->GetInterface(_T("TreeNode")))) 
    return FALSE;
```

这里调用的是`GetInterface()`，虽然跟类名比较差不多，但这个接口可以很灵活，具体实现是可以先拿自身类名比较，如果不成功，还可以用基类的类名比较，这时，就算是派生出去的类可以兼容原先的实现，需要派生的内容也会少很多，工作也轻松。

GetInterface()接口一个参考如果：

```c++
LPVOID CListContainerElementUI::GetInterface(LPCTSTR pstrName)
{
  if( _tcsicmp(pstrName, DUI_CTR_LISTITEM) == 0 ) return static_cast<IListItemUI*>(this);
  if( _tcsicmp(pstrName, DUI_CTR_LISTCONTAINERELEMENT) == 0 ) return static_cast<CListContainerElementUI*>(this);
  return CContainerUI::GetInterface(pstrName);
}
```
