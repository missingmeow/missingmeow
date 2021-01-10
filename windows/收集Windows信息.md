# 收集Windows信息

```c++
//定义一个类，存放主机信息
class CClientInf //记录信息
{
public:
    std::wstring  wstrEmail;
    std::wstring  wstrSystemInf;
    std::wstring  wstrIEVersion;
    std::vector<std::wstring> vecNetFrameVer;
    ULONG         nMemorySize;
    int           nDPI;
};

bool CollectHostInf(CClientInf& pCClientInf)
{
  DWORD dwSize;
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != NO_ERROR) return false;

  //[1]获取系统信息
  HKEY  hKey;
  LONG  ReturnValue;
  DWORD type;
  WCHAR strBuf[100];
  WCHAR strSysInfo[256];
  //打开注册表
  ReturnValue = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
    L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
    0,
    KEY_ALL_ACCESS,
    &hKey);
  if (ReturnValue != ERROR_SUCCESS)  return false;
  //获取操作系统名称
  LONG lReturn = RegQueryValueExW(hKey, L"ProductName", NULL, \
    &type, (BYTE *)strBuf, &dwSize);
  if (lReturn != ERROR_SUCCESS)    return false;
  wcscpy(strSysInfo, strBuf);
  //获取InstallationType
  dwSize = 100;
  lReturn = RegQueryValueExW(hKey, L"InstallationType", NULL,
    &type, (BYTE *)strBuf, &dwSize);
  if (lReturn != ERROR_SUCCESS)    return false;
  wcscat(strSysInfo, L"_");
  wcscat(strSysInfo, strBuf);
  //获取CurrentVersion
  dwSize = 100;
  lReturn = RegQueryValueExW(hKey, L"CurrentVersion", NULL,
    &type, (BYTE *)strBuf, &dwSize);
  if (lReturn != ERROR_SUCCESS)    return false;
  wcscat(strSysInfo, L"_");
  wcscat(strSysInfo, strBuf);
  pCClientInf.wstrSystemInf = strSysInfo;
  RegCloseKey(hKey);

  //[2]获取内存大小
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  GlobalMemoryStatusEx(&statex);
  pCClientInf.nMemorySize = (ULONG)((statex.ullTotalPhys) / (1024 * 1024));

  //[3]获取Net Framework版本 https://msdn.microsoft.com/zh-cn/library/hh925568(v=vs.110).aspx
  ReturnValue = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
    L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\",
    0,
    KEY_ALL_ACCESS,
    &hKey);
  if (ReturnValue != ERROR_SUCCESS)  return false;
  int ind = 0;
  WCHAR lpName[100];
  dwSize = 100;
  while (RegEnumKeyW(hKey, ind, lpName, dwSize) == ERROR_SUCCESS)
  {
    ind++;

    if (lpName[0] == 118) // 第一个字母是“v”，即118
    {
      HKEY hSubKey;
      if (RegOpenKeyExW(hKey, lpName, 0, KEY_ALL_ACCESS, &hSubKey) != ERROR_SUCCESS)
        continue;
      dwSize = 100;
      if (RegQueryValueExW(hSubKey, L"Version", NULL, &type, (BYTE *)strBuf, &dwSize) == ERROR_SUCCESS)
      {
        pCClientInf.vecNetFrameVer.push_back(strBuf);
                RegCloseKey(hSubKey);
        continue;
      }
      WCHAR lpSubName[100];
      dwSize = 100;
      int indSub = 0;
      while (RegEnumKeyW(hSubKey, indSub, lpSubName, dwSize) == ERROR_SUCCESS)
      {
        indSub++;

        HKEY hSubSubKey;
        if (RegOpenKeyExW(hSubKey, lpSubName, 0, KEY_ALL_ACCESS, &hSubSubKey) != ERROR_SUCCESS)
          continue;
        dwSize = 100;
        if (RegQueryValueExW(hSubSubKey, L"Version", NULL, &type, (BYTE *)strBuf, &dwSize) == ERROR_SUCCESS)
        {
          pCClientInf.vecNetFrameVer.push_back(strBuf);
                    RegCloseKey(hSubSubKey);
          continue;
        }
      }
            RegCloseKey(hSubKey);
    }
  }
    RegCloseKey(hKey);

  //[4]获取IE版本 http://www.cnblogs.com/dashouqianxiaoshou/articles/da.html
  const TCHAR szFilename[] = _T("mshtml.dll");
  DWORD dwMajorVersion = 0, dwMinorVersion = 0;
  DWORD dwBuildNumber = 0, dwRevisionNumber = 0;
  DWORD dwHandle = 0;
  DWORD dwVerInfoSize = GetFileVersionInfoSize(szFilename, &dwHandle);
  if (dwVerInfoSize)
  {
    LPVOID lpBuffer = LocalAlloc(LPTR, dwVerInfoSize);
    if (lpBuffer)
    {
      if (GetFileVersionInfo(szFilename, dwHandle, dwVerInfoSize, lpBuffer))
      {
        VS_FIXEDFILEINFO * lpFixedFileInfo = NULL;
        UINT nFixedFileInfoSize = 0;
        if (VerQueryValue(lpBuffer, TEXT("\\"), (LPVOID*)&lpFixedFileInfo, &nFixedFileInfoSize) && (nFixedFileInfoSize))
        {
          dwMajorVersion = HIWORD(lpFixedFileInfo->dwFileVersionMS);
          dwMinorVersion = LOWORD(lpFixedFileInfo->dwFileVersionMS);
          dwBuildNumber = HIWORD(lpFixedFileInfo->dwFileVersionLS);
          dwRevisionNumber = LOWORD(lpFixedFileInfo->dwFileVersionLS);
          CString cstr;
          cstr.Format(_T("%d.%d.%d.%d"), dwMajorVersion, dwMinorVersion, dwBuildNumber, dwRevisionNumber);
          pCClientInf.wstrIEVersion = cstr;
        }
      }
      LocalFree(lpBuffer);
    }
  }

  //[5]获取系统设置DPI
  HDC hDc = GetDC(NULL);
  pCClientInf.nDPI = GetDeviceCaps(hDc, LOGPIXELSX);
  ReleaseDC(NULL, hDc);

  WSACleanup();
  return true;
}
```
