<!-- EXAMPLE.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 五 1月 18 23:47:43 2019 (+0800)
;; Last-Updated: 六 1月 19 20:07:52 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 5
;; URL: http://wuhongyi.cn -->

# 示例教程

<!-- toc -->

## NIM 逻辑信号

下图为示波器观察的 NIM 逻辑信号，我们将信号经过分路器分成两路送到获取中，

![NIM 逻辑信号](/img/NIM_INPUT.png)

两路信号的参数设置如下，仅供参考

```bash
[RecordLength]
16
[ChannelMask]
11111111
[ChannelDynamicRange]
00000000
[TriggerHoldOff]
0
Channel POL  Offset  PreTrg  thr   selft   csecs  sgate  lgate  pgate   tvaw   nsbl  discr  cfdf   cfdd   trgc  
[COMMON]
  ALL   NEG   -43      4     50      1       0    24     100     4       50     1     0     3      3      1
[INDIVIDUAL]
  0     NEG   -43      4     3000     1       0     4      8      4       0     1     0     2      1      1
  1     NEG   -43      4     3000     1       0     4      8      4       0     1     0     2      1      1
```

关键参数在于 cfd 两个参数的调节，可以尝试排列组合看看哪个参数组合下的分辨达到最佳。

![NIM 脉冲时间差](/img/nim_timediff.png)

本征分辨在 40 ps 左右。

## 液闪探测器信号

下图为示波器观察的液闪探测器信号，我们将信号经过分路器分成两路送到获取中，

![液闪探测器信号](/img/liqdetsignal.png)

信号的参数设置如下，仅供参考。关键参数在于 cfd 两个参数的调节，建议尝试排列组合看看哪个参数组合下的分辨达到最佳。

```bash
[RunStartStopDelay]
0
[RecordLength]
16
[ChannelMask]
11111111
[ChannelDynamicRange]
00000000
[TriggerHoldOff]
0
Channel POL  Offset  PreTrg  thr   selft   csecs  sgate  lgate  pgate   tvaw   nsbl  discr  cfdf   cfdd   trgc  
[COMMON]
  ALL   NEG   -43      4     150     1       0    24     100     8       50     1     0     3      2      1
[INDIVIDUAL]
```


![液闪探测器时间差](/img/gammasource_100ps.png)

本征分辨在 100 ps 左右。

## PSD 积分门宽优化

下图为采用伽马元测试的PSD图，

![PSD gamma source](/img/gammasource_psd.png)

为了得到最好的中子，需要确定最佳短门积分门宽。

这里我们给出一种较容易操作的方案来：首先按照初步长短门积分门进行PSD，得到中子伽马鉴别图，从图中选择中子、伽马事件各 3000 个，对每种粒子的 3000 个事件的波形依次进行面积归一化，将归一化的 3000 个波形取平均，如下图，黑线为伽马粒子的波形，红线为中子的波形，对两种粒子的波形作差，得到图中蓝线，为两种粒子的差异。途中两个绿线指示范围即为最佳短门积分门宽。

![积分门优化](/img/neutrongammagate.jpg)




<!-- EXAMPLE.md ends here -->
