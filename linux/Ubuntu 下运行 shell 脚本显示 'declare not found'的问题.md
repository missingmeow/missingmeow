# Ubuntu 下运行 shell 脚本显示 'declare not found'的问题

在学习 Shell 脚本的时候，有两个例子在运行时出现异常。比如，在`test-dash.sh`脚本输入如下内容：

```sh
echo -e "Trekshot. \a \n"
declare -i number=7
echo $number
```

在 Ubuntu 系统下使用`sh test-bash.sh`运行将出现如下提示信息：

```sh
-e Trekshot.
test-dash.sh: 2: declare: not found:
```

有两个问题：1. echo 的参数 '-e' 怎么也打印出来了？2. 一个简单的整数声明语句为何提示 "declare: not found"?

1. 出现此问题的原因并非代码本身有误，而是 Ubuntu 系统的问题。我们在编写脚本时，第一行经常要写这么一行内容：#!/bin/bash. 这是对该脚本所使用的 shell 进行声明，因为Linux 上的 shell 并非只有一种，而各个 shell 的语法是由差别的。之所以把 echo 命令的参数误打出来，就是因为 sh 命令没有理解该语法，即当前使用的并不是 bash.下面来证明：

获取 /bin 中相关文件的属性：

```sh
-rwxr-xr-x 1 root root 725136 2008-05-13 02:48 bash
-rwxr-xr-x 1 root root  87924 2008-06-21 00:07 dash
lrwxrwxrwx 1 root root      4 2010-03-25 14:29 sh -> dash
```

这表明在执行 sh test-dash.sh 的时候，我们使用的是一个叫 "dash" 的命令（注意 sh 实为链接文件），而并不是脚本第一行中声明的 bash. 这就是上面现象的原因。 其实，如果使用 bash test-dash.sh 命令执行的话，上面两个错误根本不会出现。

2. dash(Debian Almquist SHell) Ubuntu 自 6.10 后，将先前的 bashshell 更换成了 dash (有待考证). 在设置 dash 的说明中有下面文字：

```sh
The default /bin/sh shell on Debian and Debian-based systems is bash.

However, since the default shell is required to be POSIX-compliant, any
shell that conforms to POSIX, such as dash, can serve as /bin/sh. You may
wish to do this because dash is faster and smaller than bash.
```

大致意思是说默认的 shell 是 bash shell, 但只要是能兼容 POSIX 的 shell 都可以，而dash 比 bash 速度更快、更小巧，因此 Ubuntu 安装了 dash.

3. 怎么能把 dash 去掉而使用默认的 bash？使用如下命令：

```sh
sudo dpkg-reconfigure dash
```

此命令是对已安装的包进行重新配置，在菜单（dash-bash-sh.png）中选择是否将 sh 链接到 dash (Install dash as /bin/sh?) 选择否即可。

附图一：Ubuntu 下设置 dash 界面

![image](https://images.cnblogs.com/cnblogs_com/trekshot/dash-bash-sh.png)

ps: 文章来源点[这里](https://www.cnblogs.com/Trekshot/archive/2010/03/29/ubuntu-shell-bash-dash.html)
