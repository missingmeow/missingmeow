# 关闭防火墙firewall以及selinux

## 关闭防火墙

### firewall

直接关闭防火墙：

```sh
systemctl stop firewalld.service        #停止firewall
systemctl disable firewalld.service     #禁止firewall开机启动

systemctl restart iptables.service      #重启防火墙使配置生效
systemctl enable iptables.service       #设置防火墙开机启动
```

### iptables

设置iptables service

```sh
yum -y install iptables-services
```

#### 1、重启后永久性生效

开启：

```sh
chkconfig iptables on
```

关闭：

```sh
chkconfig iptables off
```

#### 2、即时生效，重启后失效

开启：

```sh
service iptables start
```

关闭：

```sh
service iptables stop
```

在开启了防火墙时，做如下设置，开启相关端口，修改 ==/etc/sysconfig/iptables== 文件，添加以下内容：

```sh
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT        #允许80端口通过防火墙
-A INPUT -m state --state NEW -m tcp -p tcp --dport 3306 -j ACCEPT      #允许3306端口通过防火墙
```

备注：把这两条规则添加到防火墙配置的最后一行，会导致防火墙启动失败，正确的应该是添加到默认的`22端口`这条规则的下面。

## 二、关闭SELinux

`vim /etc/selinux/config`

将 `SELINUX=enforcing`

改为`SELINUX=disabled`

保存退出，重启服务器。

### 禁用SeLinux

1、永久禁用，需要重启生效（跟以上效果一致）。

```sh
sed -i 's/SELINUX=enforcing/SELINUX=disabled/g' /etc/selinux/config  
```

2、临时禁用，不需要重启

```sh
setenforce 0  
```
