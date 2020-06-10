# 本示例展示了 MVVM 中读取串口数据的用法 (Linux Only)。

## 启用虚拟串口

用下面的脚本，可以创建两个虚拟串口，用来互相收发数据。

```
sudo socat -d -d -d -d  pty,link=/dev/master,raw,echo=0,user="$USER",group="$USER" pty,link=/dev/slave,raw,echo=0,user="$USER",group="$USER"
```

创建的串口名称通常为：

```
/dev/pts/X
/dev/pts/X+1
```

> X 是串口的编号，每次创建都不一样，请从 LOG 信息中查看。

## 编译

修改 temperature.c 中 SERIAL_PORT 为实际的串口，如：

```
#define SERIAL_PORT "/dev/pts/1"
```

然后再编译。

## 测试

* 启动 demo20

```
./bin/demo20
```

* 通过 echo 命令，向串口写入数据：

```
echo 123 >/dev/pts/2
```

* 我们可以看到，界面上的温度，变成了相应的值。

> 本例只是演示从串口获取数据并显示到界面。在实际项目中，需要自己定义通讯协议，实现双向通讯。
