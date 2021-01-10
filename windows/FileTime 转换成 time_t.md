# FileTime 转换成 time_t

```c++
time_t& FileTimeToTime_t( FileTime & ft)
{
  ULARGE_INTEGER ui;
  ui.HighPart = ft.dwHighDateTime;
  ui.LowPart = ft.dwLowDateTime;
  return ui.QuadPart / 10000000ULL - 11644473600ULL;
}
```
