# StretchBlt() 放大压缩图片及失真处理方法

## StretchBlt

函数功能：函数从源矩形中复制一个位图到目标矩形，必要时按目前目标设备设置的模式进行图像的拉伸或压缩。

函数原型：BOOL StretchBlt(HDC hdc, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeighDest, HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)；

参数：
```sh
　　hdcDest：指向目标设备环境的句柄。
　　nXOriginDest：指定目标矩形左上角的X轴坐标，按逻辑单位表示坐标。
　　nYOriginDest：指定目标矩形左上角的Y轴坐标，按逻辑单位表示坐标。
　　nWidthDest：指定目标矩形的宽度，按逻辑单位表示宽度。
　　nHeightDest：指定目标矩形的高度，按逻辑单位表示高度。
　　hdcSrc：指向源设备环境的句柄。
　　nXOriginSrc：指向源矩形区域左上角的X轴坐标，按逻辑单位表示坐标。
　　nYOriginSrc：指向源矩形区域左上角的Y轴坐标，按逻辑单位表示坐标。
　　nWidthSrc：指定源矩形的宽度，按逻辑单位表示宽度。
　　nHeightSrc：指定源矩形的高度，按逻辑单位表示高度。
　　dwRop：指定要进行的光栅操作。光栅操作码定义了系统如何在输出操作中组合颜色，这些操作包括刷子、源位图和目标位图等对象。
            下面列出了一些常见的光栅操作代码：
            BLACKNESS：表示使用与物理调色板的索引0相关的色彩来填充目标矩形区域，（对缺省的物理调色板而言，该颜色为黑色）。
            DSTINVERT：表示使目标矩形区域颜色取反。
            MERGECOPY：表示使用布尔型的AND（与）操作符将源矩形区域的颜色与特定模式组合一起。
            MERGEPAINT：通过使用布尔型的OR（或）操作符将反向的源矩形区域的颜色与目标矩形区域的颜色合并。
            NOTSRCCOPY：将源矩形区域颜色取反，于拷贝到目标矩形区域。
            NOTSRCERASE：使用布尔类型的OR（或）操作符组合源和目标矩形区域的颜色值，然后将合成的颜色取反。
            PATCOPY：将特定的模式拷贝到目标位图上。
            PATPAINT：通过使用布尔OR（或）操作符将源矩形区域取反后的颜色值与特定模式的颜色合并。然后使用OR（或）操作符将该操作的结果与目标矩形区域内的颜色合并。
            PATINVERT：通过使用XOR（异或）操作符将源和目标矩形区域内的颜色合并。
            SRCAND：通过使用AND（与）操作符来将源和目标矩形区域内的颜色合并。
            SRCCOPY：将源矩形区域直接拷贝到目标矩形区域。
            SRCERASE：通过使用AND（与）操作符将目标矩形区域颜色取反后与源矩形区域的颜色值合并。
            SRCINVERT：通过使用布尔型的XOR（异或）操作符将源和目标矩形区域的颜色合并。
            SRCPAINT：通过使用布尔型的OR（或）操作符将源和目标矩形区域的颜色合并。
            WHITENESS：使用与物理调色板中索引1有关的颜色填充目标矩形区域。（对于缺省物理调色板来说，这个颜色就是白色）。
```

返回值：如果函数执行成功，那么返回值为非零，如果函数执行失败，那么返回值为零。Windows NT：若想获得更多的错误信息，请调用GetLastError函数。

但是，StretchBlt函数缩放图片后图片失真严重，所以要用SetStretchBltMode函数来设置 StretchBlt（或StretchDIBits）函数的伸缩模式。具体用法是调用StretchBlt前调用SetStretchBltMode()

int SetStretchBltMode(HDC hdc, int iStretchMode)；

```txt
    hdc：设备环境句柄。
    iStretchMode：指定拉伸模式。它可以取下列值，这些值的含义如下：
        BLACKONWHITE：使用消除和现在的像素颜色值进行逻辑AND（与）操作运算。如果该位图是单色位图，那么该模式以牺牲白色像素为代价，保留黑色像素点。
        COLORONCOLOR：删除像素。该模式删除所有消除的像素行，不保留其信息。
        HALFTONE：将源矩形区中的像素映射到目标矩形区的像素块中，覆盖目标像素块的一般颜色与源像素的颜色接近。在设置完HALFTONE拉伸模之后，应用程序必须调用SetBrushOrgEx函数来设置刷子的起始点。如果没有成功，那么会出现刷子没对准的情况。
        STRETCH_ANDSCANS：与BLACKONWHITE一样。
        STRETCH_DELETESCANS：与COLORONCOLOR一样。
        STRETCH_HALFTONE：与HALFTONE相同。
        STRETCH_ORSCANS：与WHITEONBLACK相同。
        WHITEONBLACK：使用颜色值进行逻辑OR（或）操作，如果该位图为单色位图，那么该模式以牺牲黑色像素为代价，保留白色像素点。
```

返回值：如果函数执行成功，那么返回值就是先前的拉伸模式，如果函数执行失败，那么返回值为0。
