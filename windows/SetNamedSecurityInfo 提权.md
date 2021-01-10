# SetNamedSecurityInfo 提权~~

```c++
#include <Aclapi.h>
BOOL SetRegPrivilege(HKEY hKey, LPSTR SamName)
{
        DWORD dwRet;
        PACL pNewDacl = NULL, pOldDacl = NULL;
        PSECURITY_DESCRIPTOR pSD = NULL;
        EXPLICIT_ACCESS ea; 

        // 获取SAM主键的DACL 
        dwRet = GetNamedSecurityInfo(SamName, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDacl, NULL, &pSD); 
        if (dwRet != ERROR_SUCCESS) 
        { 
            return FALSE;
        }

        // 创建一个ACE，允许Everyone完全控制对象，并允许子对象继承此权限 
        ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS)); 
        BuildExplicitAccessWithName(&ea, "Administrators", KEY_ALL_ACCESS, SET_ACCESS, SUB_CONTAINERS_AND_OBJECTS_INHERIT);

        // 将新的ACE加入DACL 
        dwRet = SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl); 
        if (dwRet != ERROR_SUCCESS) 
        { 
            return FALSE;
        }    

        // 更新SAM主键的DACL 
        dwRet = SetNamedSecurityInfo(SamName, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL); 
        if (dwRet != ERROR_SUCCESS) 
        { 
            return FALSE;
        }

        //释放资源
        if (pNewDacl) 
            LocalFree(pNewDacl); 

        // 还原SAM主键的DACL 
        //     if (pOldDacl) 
        //         SetNamedSecurityInfo(SamName, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION, NULL, NULL, pOldDacl, NULL);     
        if (pOldDacl) 
            LocalFree(pOldDacl); 
        if (pSD) 
            LocalFree(pSD); 

        return TRUE;
}
```

另外一个例子，清除USB的使用痕迹：

详细：http://www.rmbgold.com/?post=3

当你不希望别人知道你使用了USB设备的时候，或者，在一些公用的计算机上，不允许使用USB设备的时候，下面的方法可以清除USB使用的历史记录，喜欢的朋友可以用一下。
在运行中输入regedit.exe,定位到下面的键值，然后：
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\DeviceClasses\{53f56307-b6bf-11d0-94f2-00a0c91efb8b}删除掉下的所有项
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB 下所有Vid开头的项
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USBSTOR 下的所有Disk&Ven开头的项
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\UsbFlags 下所有项
以及这个位置的这个文件，也一并删除：c:\windows\setupapi.log 文件。

文件
c:\windows\setupapi.log
注册表
[HKEY_LOCAL_MACHINE\SYSTEM\.....ControlSet....\Enum\USBSTOR]
[HKEY_LOCAL_MACHINE\SYSTEM\.....ControlSet....\Enum\USB]
[HKEY_LOCAL_MACHINE\SYSTEM\....ControlSet.....\Control\DeviceClasses\{53f56307-b6bf-11d0-94f2-00a0c91efb8b}]还有其他的几个地方，都是在这个下面。不再详述。
以上为目前发现的存储路径。
从以上的条目可以找到设备第一次使用的时间，使用了多少什么设备。

==================================================================================
void CleanUSBLog()
{
 //c:\windows\setupapi.log
 //C:\Windows\inf\setupapi.dev.log
 TCHAR szWinDir[MAX_PATH] = {0}, szTmpPath[MAX_PATH];
 GetWindowsDirectory(szWinDir, MAX_PATH);
 wsprintf(szTmpPath, _T("%s\\setupapi.log"), szWinDir);
 WipeFile(szTmpPath);

 //先设置Everyone允许控制权限
 DWORD dwRet; 
 PACL pNewDacl = NULL; 
 EXPLICIT_ACCESS ea; 
 __try
 {
      // 创建一个ACE，允许Everyone完全控制对象，并允许子对象继承此权限
      ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS)); 
      BuildExplicitAccessWithName(&ea, _T("Everyone"), KEY_ALL_ACCESS, SET_ACCESS, SUB_CONTAINERS_AND_OBJECTS_INHERIT); 

      // 将新的ACE加入DACL 
      dwRet = SetEntriesInAcl(1, &ea, NULL, &pNewDacl); 
      if (dwRet != ERROR_SUCCESS) 
           __leave;

      // 更新USB主键的DACL
      dwRet = SetNamedSecurityInfo(_T("MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\USBSTOR"), SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL);

 }
 __finally
 {
      //释放DACL和SID
      if (pNewDacl) 
           LocalFree(pNewDacl);
 }

 //删除注册表
 DeleteKeysIncludeEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\DeviceClasses"),_T("#USB"));
 DeleteKeysInclude(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\DeviceClasses\\{53f5630a-b6bf-11d0-94f2-00a0c91efb8b}"),_T("#RemovableMedia"));
 SHDeleteKey(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\UsbFlags"));
 DeleteKeysExclude(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Enum\\USB"),_T("ROOT_HUB"));
}
