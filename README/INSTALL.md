<!-- INSTALL.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 二 1月 15 23:47:26 2019 (+0800)
;; Last-Updated: 六 1月 19 00:09:21 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 8
;; URL: http://wuhongyi.cn -->

# 程序安装

<!-- toc -->

本获取经过 Scientific Linux 7 系统测试。建议采用 CentOS 7 或者 Scientific Linux 7。

- 本获取要求 CERN ROOT 6，要求必须安装 FFTW3 库。
- 要求电脑的内存配置较大，建议 16 GB 或者 32 GB 或者以上。

**用户可通过 yum 安装 fftw3，也可自行下载源代码安装。以下为 CentOS/Scientific Linux 下 yum 安装的命令：**

```bash
yum -y install fftw.x86_64 fftw-devel.x86_64 fftw-libs.x86_64
```

**以下为可选安装，安装之后 ROOT 可使用功能更多。根据需要安装即可。**

```bash
yum -y install lz4.x86_64 lz4-devel.x86_64
yum -y install gsl.x86_64 gsl-devel.x86_64
yum -y install graphviz.x86_64 graphviz-devel.x86_64
yum -y install ruby.x86_64 ruby-devel.x86_64 ruby-libs.x86_64
yum -y install expect.x86_64 expect-devel.x86_64
yum -y install davix.x86_64 davix-devel.x86_64
yum -y install unuran.x86_64 unuran-devel.x86_64
yum -y install avahi-compat-libdns_sd.x86_64 avahi-compat-libdns_sd-devel.x86_64
yum -y install ftgl.x86_64 ftgl-devel.x86_64
yum -y install glew.x86_64 glew-devel.x86_64
yum -y install mysql++.x86_64 mysql++-devel.x86_64
yum -y install cfitsio.x86_64 cfitsio-devel.x86_64
yum -y install libxml2*
yum -y install binutils-devel.x86_64
yum -y install pythia8.x86_64 pythia8-devel.x86_64
yum -y install redhat-lsb.x86_64
yum -y install R.x86_64
yum -y install R-RInside.x86_64 R-RInside-devel.x86_64 R-Rcpp.x86_64 R-Rcpp-devel.x86_64
```


## ROOT 软件安装

**以下示例演示如何将 ROOT 安装到 /opt 目录下**

```bash
## https://root.cern.ch/building-root
# cmake安装方法，以 6.08.06 为例。 安装之后 .bashrc 中添加 source /opt/root60806/bin/thisroot.sh
tar -zxvf root_v6.08.06.source.tar.gz
mkdir buildroot60806
cd buildroot60806/
cmake -DCMAKE_INSTALL_PREFIX=/opt/root60806 -Dall=ON  ../root-6.08.06/
make -j8
make install
rm -rf buildroot60806
```





## CAEN Lib

本程序依赖 CAENVMELib/CAENComm/CAENDigitizer/CAENUpgrader 四个库文件。

其中 CAENVMELib/CAENComm/CAENDigitizer 为获取运行必须的库。CAENUpgrader 用来更新固件。

### CAENVMELib

```bash
tar -xzvf CAENVMELib-2.50.tgz
cd CAENVMELib-2.50/lib
sh install_x64     #需要ROOT权限
```

### CAENComm

```bash
tar -xvzf CAENComm-1.2-build20140211.tgz
cd CAENComm-1.2/lib
sh install_x64     #需要ROOT权限
```

### CAENDigitizer

```bash
tar -zxvf CAENDigitizer_2.11.0_20180212.tgz
sh install_64      #需要ROOT权限
```


### CAENUpgrader

```bash
tar -xvzf CAENUpgrader-1.6.3-build20170511.tgz
cd CAENUpgrader-1.6.3
./configure
make 
make install       #需要ROOT权限
```

## 检查 CAENUpgrader 安装

安装后在终端中输入 
```
CAENUpgraderGUI
```
将会弹出 CAEN Upgrader GUI 的图形界面。


## USB 驱动

如果您使用 USB，则需要安装 USB 驱动。

```bash
tar -xzvf CAENUSBdrvB-1.5.2.tgz
cd CAENUSBdrvB-1.5.2
make
make install     #需要ROOT权限
```

## A2818驱动

如果您使用 A2818，则安装以下驱动。

```bash
# A2818Drv-1.20-build20161118.tgz
#将该文件夹复制到 /opt 并安装在该位置
tar -zxvf A2818Drv-1.20-build20161118.tgz
cp -r A2818Drv-1.20 /opt           #需要ROOT权限
cd /opt/A2818Drv-1.20              #需要ROOT权限
cp ./Makefile.2.6-3.x Makefile     #需要ROOT权限
make                               #需要ROOT权限

#设置开机自动执行该脚本
#在文件 /etc/rc.d/rc.local 中添加以下一行内容
/bin/sh /opt/A2818Drv-1.20/a2818_load
#或者在开启电脑之后执行以上命令
```

重启电脑后，在终端内输入 **dmesg|grep a2818** 将会看到以下的A2818驱动加载信息

```
a2818: CAEN A2818 CONET controller driver v1.20s
a2818:   Copyright 2004, CAEN SpA
pci 0000:05:02.0: enabling device (0000 -> 0003)
pci 0000:05:02.0: PCI INT A -> GSI 19 (level, low) -> IRQ 19
a2818: found A2818 adapter at iomem 0xf7800000 irq 0, PLX at 0xf7900000
a2818:   CAEN A2818 Loaded.
a2818:   CAEN A2818: 1 device(s) found.
```

----

## A3818驱动

如果您使用 A3818，则安装以下驱动。安装该驱动时，电脑机箱必须插入 A3818 卡，否则将会报安装失败。

```bash
tar -zxvf A3818Drv-1.6.1.tgz
cd A3818Drv-1.6.1
make 
make install       #需要ROOT权限
```

然后在终端内输入 dmesg 将会看到以下的A3818驱动加载信息

```
fuse init (API version 7.14)
CAEN A3818 PCI Express CONET2 controller driver v1.6.0s
  Copyright 2013, CAEN SpA
pci 0000:02:00.0: PCI INT A -> GSI 16 (level, low) -> IRQ 16
  alloc irq_desc for 33 on node -1
  alloc kstat_irqs on node -1
pci 0000:02:00.0: irq 33 for MSI/MSI-X
pci 0000:02:00.0: setting latency timer to 64
Found A3818 - Common BAR at iomem ffffc900067d4000 irq 0
Found A3818 with 1 link(s)
found A3818 Link 0 BAR at iomem ffffc900067d6000 irq 0
  CAEN A3818 Loaded.
  CAEN PCIe: 1 device(s) found.
```

**需要注意的是每次启动电脑之后均需要重新加载 A3818 驱动**



<!-- INSTALL.md ends here -->
