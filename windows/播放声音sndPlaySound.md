# 播放声音sndPlaySound

```c++
#include <MMSystem>
#program comment(lib, "winmm.lib")

CString csPath(_T("C://sound.wav");
sndPlaySound(csPath, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

// 当提示音需要循环播放的时候，应用定时器可以实现，但当中如果执行其他操作声音会被中断，暂时无解。所以应用以下解决办法，加上循环播放标志，另外SND_NOWAIT | SND_NOSTOP也要加上。

sndPlaySound(csPath, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP | SND_NOWAIT | SND_NOSTOP); // 循环播放
sndPlaySound(_T(""), NULL); // 关闭循环播放
```
