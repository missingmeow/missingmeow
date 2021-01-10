# VS 编译时功能扩展

打开项目属性 - Build Events - Post-Build Event
在Command Line栏添加你想要添加功能的Dos命令（可在window菜单 - 运行 - cmd -help 查看全部命令）
Use In Build 中选 Yes

常用Dos命令：（具体用法可在cmd上敲 <<命令  /？>> 查看）

COPY                将至少一个文件复制到另一个位置。
ROBOCOPY      复制文件和目录树的高级实用工具
XCOPY              复制文件和目录树。
MOVE               将一个或多个文件从一个目录移动到另一个目录。

DEL        删除至少一个文件。
ERASE    删除一个或多个文件。

MD           创建一个目录。
MKDIR      创建一个目录。
RD             删除目录。
RMDIR      删除目录。

FOR      为一组文件中的每个文件运行一个指定的命令。
IF          在批处理程序中执行有条件的处理操作。

HELP     提供 Windows 命令的帮助信息。
