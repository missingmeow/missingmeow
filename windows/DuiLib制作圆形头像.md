# DuiLib制作圆形头像

来源：[here](http://www.cnblogs.com/chechen/p/5971356.html)

头文件.h：

```c++
#ifndef __UIHEADICON_H__
#define __UIHEADICON_H__


/*
    名称：圆形头像控件（派生CButtonUI类）
*/


class  CHeadUI: public CButtonUI
{
public:

    CHeadUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void PaintBkImage(HDC hDC);

    void SetBkImage(LPCTSTR pStrImage);

    void SetDefaultBkImage(LPCTSTR pStrImage){ m_sDefaultBkImage = pStrImage; }
    CDuiString GetDefaultBkImage(){ return m_sDefaultBkImage; }
    void SetAutoPenColor(bool bAuto){ m_bAutoPenColor = bAuto; }
    bool IsAutoPenColor() { return m_bAutoPenColor; }
    void SetPenColor(DWORD dwColor){ m_dwPenColor = dwColor; }
    DWORD GetPenColor(HDC hDC);
    void SetPenWidth(int nPenWidth){ m_nPenWidth = nPenWidth; }
    int GetPenWidth(){ return m_nPenWidth; }

    bool IsHeadImageExist(LPCTSTR pStrImage);

private:

    CDuiString m_sDefaultBkImage;
    bool m_bAutoPenColor;
    DWORD m_dwPenColor;
    int m_nPenWidth;
};

#endif // __UIHEADICON_H__
```

实现文件.cpp：

```c++
#include "StdAfx.h"
#include "UIHeadIcon.h"

CHeadUI::CHeadUI()
{
    m_sDefaultBkImage = _T("Head\\100_1.png");
    m_bAutoPenColor = false;
    m_dwPenColor = Color(255, 255, 255, 255).GetValue();
    m_nPenWidth = 2;
}

LPCTSTR CHeadUI::GetClass() const
{
    return _T("HeadIconUI");
}

LPVOID CHeadUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("HeadIcon")) == 0 ) return static_cast<CHeadUI*>(this);
    return CControlUI::GetInterface(pstrName);
}

void CHeadUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if (_tcscmp(pstrName, _T("defaultbkimage")) == 0) SetDefaultBkImage(pstrValue);
    else if (_tcscmp(pstrName, _T("bkimage")) == 0) SetBkImage(pstrValue);
    else if (_tcscmp(pstrName, _T("pencolor")) == 0) {
        while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
        if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetPenColor(clrColor);
    }
    else if (_tcscmp(pstrName, _T("autopencolor")) == 0) SetAutoPenColor(_tcscmp(pstrValue, _T("true")) == 0);
    else if (_tcscmp(pstrName, _T("penwidth")) == 0) SetPenWidth(_ttoi(pstrValue));
    else return CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CHeadUI::SetBkImage(LPCTSTR pStrImage)
{
    if (IsHeadImageExist(pStrImage))
    {
        m_sBkImage = pStrImage;
    }
    else
    {
        TCHAR tszModule[MAX_PATH + 1] = { 0 };
        ::GetModuleFileName(CPaintManagerUI::GetInstance(), tszModule, MAX_PATH);
        CDuiString sInstancePath = tszModule;
        int pos = sInstancePath.ReverseFind(_T('\\'));
        if (pos >= 0) sInstancePath = sInstancePath.Left(pos + 1);
        sInstancePath.Append(pStrImage);

        if (IsHeadImageExist(sInstancePath))
        {
            m_sBkImage = sInstancePath;
        }
        else
        {
            m_sBkImage = pStrImage;
        }
    }

    Invalidate();
}

void CHeadUI::PaintBkImage(HDC hDC)
{
    //坐标
    POINT    pt = { m_rcItem.left, m_rcItem.top };

    //大小
    SIZE    sz = { m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top };

    Graphics    graphics(hDC);
    if (graphics.GetLastStatus() != Ok)
        return;

    //消除锯齿
    graphics.SetSmoothingMode(SmoothingModeHighQuality);    

    GraphicsPath graphicspath;
    if (graphicspath.GetLastStatus() != Ok)
        return;

    graphicspath.AddEllipse(pt.x, pt.y, sz.cx, sz.cy);

    //设置裁剪圆
    graphics.SetClip(&graphicspath, CombineModeReplace);

    Image image(GetBkImage());
    if (image.GetLastStatus() != Ok) 
        return;

    //绘制图像
    graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);

    //绘制一个1像素宽度的圆形，用于消除锯齿
    Pen    myPen(GetPenColor(hDC), GetPenWidth());
    if (myPen.GetLastStatus() != Ok)
        return;

    graphics.DrawEllipse(&myPen, pt.x, pt.y, sz.cx, sz.cy);
}

DWORD CHeadUI::GetPenColor(HDC hDC)
{
    if (IsAutoPenColor())
    {
        //像素值颜色取点（ pt.x + 1, pt.y + 1）的值
        RECT rc = GetPos();
        COLORREF color = GetPixel(hDC, rc.left + 1, rc.top + 1);

        BYTE r = GetRValue(color);
        BYTE g = GetGValue(color);
        BYTE b = GetBValue(color);

        return Color(255, r, g, b).GetValue();
    }

    return m_dwPenColor;
}

bool CHeadUI::IsHeadImageExist(LPCTSTR pStrImage)
{
    return GetFileAttributes(pStrImage) == -1 ? false : true;
}
```
