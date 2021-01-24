# VSCode 安装 Go 插件注意事项

## 说明

因为Go的机制，当你在VSCode中直接安装Go插件的时候，一定会有几个插件一直安装不上的。所以，你必须要在安装插件之前，先把坑填上，然后再继续。

## 准备

1. 进入到`%GOPATH%/src`目录下，新建文件夹`golang.org`
2. 进入到`golang.org`目录下，新建文件夹`x`
3. 进入`x`文件夹下，在该文件下使用`git`工具安装官方的组件

## 组件安装

执行以下命令：

```sh
git clone https://github.com/golang/tools.git
git clone https://github.com/golang/lint.git
```

成功之后即可在`x`文件下看到Go官方提供的`tools`和`lint`组件，在`VSCode`安装插件的时候，大部分插件都需要使用这两个组件提供的功能。

其他组件`import`这两个组件的路径为`golang.org/x/lint`和`golang.org/x/tools`，所以知道为什么要新建前面那两个目录了吧！

## 插件安装

打开VSCode，直接在智能提示上点击安装Go插件即可，这时所有插件都会成功哦。

```sh
Installing 8 tools at D:\go\bin
  gocode
  go-outline
  go-symbols
  guru
  gorename
  gocode-gomod
  goreturns
  golint

Installing github.com/mdempsky/gocode SUCCEEDED
Installing github.com/ramya-rao-a/go-outline SUCCEEDED
Installing github.com/acroca/go-symbols SUCCEEDED
Installing golang.org/x/tools/cmd/guru SUCCEEDED
Installing golang.org/x/tools/cmd/gorename SUCCEEDED
Installing github.com/stamblerre/gocode SUCCEEDED
Installing github.com/sqs/goreturns SUCCEEDED
Installing golang.org/x/lint/golint SUCCEEDED

All tools successfully installed. You're ready to Go :).
```
