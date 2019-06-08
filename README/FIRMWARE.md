<!-- FIRMWARE.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 二 1月 15 23:47:46 2019 (+0800)
;; Last-Updated: 五 1月 18 23:45:10 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 4
;; URL: http://wuhongyi.cn -->

# 固件

<!-- toc -->

> **[warning] 注意**
>
> 请确保所使用的插件固件版本与以下一致。
>
> 我们尽可能保证采用最新的固件。


## 当前固件版本

```text
DT5730 DPP-PSD 4.17_136.16
A2818  1.00
A3818  0.05
```

## 查看固件版本

DT5730/A2818/A3818 查看固件版本采用 CAENUpgraderGUI 程序，DT5730/A2818/A3818 升级固件版本同样采用 CAENUpgraderGUI 程序。即在终端中执行

```bash
CAENUpgraderGUI
```

升级固件时候，Browse 选择固件之后会弹出一个警告窗口，提示你“You have chosen to use a raw binary file”，点击确认，然后点击右下角的 Upgrade。等待升级结束，将会有一个窗口提示你重启。

### A2818

如下图，查看 A2818 的固件版本，点击 *Get Fw Rel* 按钮。

![Get A2818 Version](/img/A2818GetVersion.png)

如果该固件版本不是 **当前固件版本** 所列版本，则升级固件。

升级界面如下图所示：

![Update A2818](/img/A2818UpdateVersion.png)


### A3818

如下图，查看 A3818 的固件版本，点击 *Get Fw Rel* 按钮。

![Get A3818 Version](/img/A3818GetVersion.png)

如果该固件版本不是 **当前固件版本** 所列版本，则升级固件。

升级界面如下图所示：

![Update A3818](/img/A3818UpdateVersion.png)

### DT5730

如下图，查看 DT5730 的固件版本，点击 *Get Fw Rel* 按钮。

![Get DT5730 Version](/img/GetFirmwareDT5730.png)

如果该固件版本不是 **当前固件版本** 所列版本，则升级固件。

升级界面如下图所示：  

**其中连接类型选项中，如果采用光纤连接，则选择 OPTLINK，如果采用 USB 连接，则选择 USB 选项。**

![Update DT5730](/img/UpgradeFirmwareDT5730.png)



<!-- FIRMWARE.md ends here -->
