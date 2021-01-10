# 在内核中之获取HKEY_CURRENT_USER对应路径

在内核中操作注册表,HKEY_LOCAL_MACHINE的路径可以用\Registry\Machine来表示.

HKEY_USERS可以用 \Registry\User表示.

而HKEY_CLASSES_ROOT和HKEY_CURRENT_CONFIG,HKEY_CURRENT_USER,在内核没有对应的路径来表示.

实际上HKEY_CLASSES_ROOT是链接到HKEY_LOCAL_MACHINE\SOFTWARE\Classes

HKEY_CURRENT_CONFIG是链接到HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Hardware Profiles\Current

可以这么理解为:

HKEY_CLASSES_ROOT = HKEY_LOCAL_MACHINE\SOFTWARE\Classes

HKEY_CURRENT_CONFIG = HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Hardware Profiles\Current

因为用户态的程序是由系统当前登录用户运行,而内核态的程序是由system用户运行,而HKEY_CURRENT_USER是保存系统当前登录用户的相关信息,所以在内核中,无法直接访问HKEY_CURRENT_USER.

但实际上,HKEY_CURRENT_USER也是链接到HKEY_USERS\[当前登录用户的SID]

```c++
int GetCurrentSID(CString& strSID)
{
   HANDLE hProcess = GetCurrentProcess();
   if(!hProcess)
    return 0;

   HANDLE hToken;
   if( !OpenProcessToken(hProcess, TOKEN_QUERY, &hToken) || !hToken )
   {
    CloseHandle(hProcess);
    return 0;
   }

   DWORD dwTemp = 0;
   char tagTokenInfoBuf[256] = {0};
   PTOKEN_USER tagTokenInfo = (PTOKEN_USER)tagTokenInfoBuf;  
   if( !GetTokenInformation( hToken, TokenUser, tagTokenInfoBuf, sizeof(tagTokenInfoBuf), &dwTemp ) )
   {  
    CloseHandle(hToken);  
    CloseHandle(hProcess);  
    return 0;  
   }

   typedef BOOL (WINAPI* PtrConvertSidToStringSid)(PSID, LPTSTR*);

   PtrConvertSidToStringSid dwPtr = (PtrConvertSidToStringSid)GetProcAddress( GetModuleHandle(L"Advapi32.dll"), "ConvertSidToStringSidA" );

   LPTSTR MySid = NULL;
   dwPtr( tagTokenInfo->User.Sid, (LPTSTR*)&MySid );
   int len = MultiByteToWideChar(CP_ACP, 0,(LPCSTR)MySid, -1, NULL, NULL);
   LPWSTR lpszW = new WCHAR[len];
   MultiByteToWideChar(CP_ACP, 0, (LPCSTR)MySid, -1, lpszW, len);
   strSID= lpszW;
   delete[] lpszW;

   LocalFree( (HLOCAL)MySid );

   CloseHandle(hToken);
   CloseHandle(hProcess);
}
```
