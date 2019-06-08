<!-- PRINCIPLE.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 二 1月 15 22:41:03 2019 (+0800)
;; Last-Updated: 六 6月  8 20:22:24 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 12
;; URL: http://wuhongyi.cn -->

# 工作原理

<!-- toc -->

下图为液闪探测器典型波形示意图，

![Typical Gamma Neutron Waveforms](/img/TypicalGammaNeutronWaveforms.png)

下图为 DPP-PSD 固件的功能框图

![Functional Block Diagram Of The DPP-PSD](/img/FunctionalBlockDiagramOfTheDPPPSD.png)

DPP-PSD 固件的目的是计算 Qshort 和 Qlong 的两个电荷，计算输入脉冲的双门积分。 尾部电荷（慢成分）与总电荷之间的比率给出了用于伽马中子甄别的PSD参数：

PSD = (Qlong-Qshort)/Qlong


![Long And Short Gate Graphic Position With Respect To A Couple Of Input Pulses](/img/LongAndShortGateGraphicPositionWithRespectToACoupleOfInputPulses.png)


<<<<<<< HEAD

![Diagram Summarizing The DPP-PSD Parameters](/img/DiagramSummarizingTheDPPPSDParameters.png)
=======
## 基线计算

基线计算是 DPP-PSD 固件的一个重要特性，因为它是输入脉冲电荷积分时的参考值。 此外，大多数 DPP 参数与基线值相关。 

用户可以选择将基线设置固定值，或让 DPP 固件动态计算它。 在第一种情况下，用户必须以 LSB 单位设置基线值。 对于整个采集运行期间，此值保持固定。 在后一种情况下，固件动态地将移动时间窗口内的 N 个点的平均值作为基线评估。 用户可以为 730 系列为选择 16,64,256,1024。

然后在门开始之前从几个时钟冻结基线的计算，直到长门和触发延迟（trigger hold-off）之间的最大值结束。 之后，基线再次重新开始计算，同时考虑平均值以及冻结前的点数。 由于基线计算，这允许几乎没有死区时间。
>>>>>>> 33c5a31240eedfa3fd25fe2e98bb7eff5865e6e2

## 基线计算

基线计算是 DPP-PSD 固件的一个重要特性，因为它是输入脉冲电荷积分时的参考值。 此外，大多数 DPP 参数与基线值相关。 

用户可以选择将基线设置固定值，或让 DPP 固件动态计算它。 在第一种情况下，用户必须以 LSB 单位设置基线值。 对于整个采集运行期间，此值保持固定。 在后一种情况下，固件动态地将移动时间窗口内的 N 个点的平均值作为基线评估。 用户可以为 730 系列为选择 16,64,256,1024。

然后在门开始之前从几个时钟冻结基线的计算，直到长门和触发延迟之间的最大值结束。 之后，基线再次重新开始计算，同时考虑平均值以及冻结前的点数。 由于基线计算，这允许几乎没有死区时间。
![Baseline Calculation As Managed By The DPP-PSD Algorithm](/img/BaselineCalculationAsManagedByTheDPPPSDAlgorithm.png)

## 采集原理

![Diagram Summarizing The DPP-PSD Parameters](/img/DiagramSummarizingTheDPPPSDParameters.png)

由于 FPGA 只能处理正信号，因此在输入参数中我们必须指定输入信号的极性，这样内部才能对负极性的信号先进行反相然后送到 FPGA 中。

前面我们已经介绍了基线的计算，阈值的概念是相对于基线的，当信号大于我们所设定的阈值时，表示一个事件被触发。该时刻表示trigger点，很多采集的参数都是相对该触发点来设置的。

这里用通俗的例子来解释说明下阈值与基线的相对关系。例如，一个正信号，其基线在 15000 道，假设阈值设置为 1000 道，意味当 ADC 采集的道址小于 14000 时，该时刻即为触发点。如果是一个正信号，假设其基线在 1400 道，阈值设为 200，那么当 ADC 采集的道址大于 1600时，该时刻即为触发点。

这里介绍两组容易混淆的参数：

- pre-trigger/record length
- gate offset/short gate/long gate

这两组参数的设置是独立的，她们存在的共同点是，都是采用触发点作为参考点。

pre-trigger/record length 参数组合是设置记录波形的长度，pre-trigger 表示触发点前记录多少个采样点，record length 表示记录的总长度。

gate offset/short gate/long gate 参数组合是 PSD 长短门积分的相关参数，gate offset 表示积分的起点，相对于触发点提前多少个采样点，short gate 表示短门积分门宽，long gate 表示长门的积分门宽。

## CFD 原理

在模拟电子学中，传统上使用 CFD（恒比定时甄别）模块来完成时间戳的确定。该技术提取振幅达到全振幅的固定分数的时间作为脉冲的时间戳。

数字 CFD 信号以经典方式实现。输入波形衰减的因子 f 等于全振幅的所需定时分数，然后信号被反转并延迟时间 d，该时间 d 等于脉冲从恒定分数水平上升到脉冲峰值所需的时间。
将最新的两个信号相加以产生双极性脉冲，CFD与其零交叉点对应于输入脉冲的分数 f ，该点作为时间触发。如下图所示

![Classical Implementation Of The Constant Fraction Discriminator](/img/ClassicalImplementationOfTheConstantFractionDiscriminator.png)

CFD 在数字化中的实现如图所示。输入样本分为两个路径：第一个以采样时钟的步长执行延迟（2 ns），第二个执行衰减。 衰减的可能选择是：相对于输入幅度的 25％，50％，75％ 和 100％（即无衰减）。 CFD 信号被称为动态的中间道址，即在 730 系列的情况下为道址 8192。

![Implementation Of The Digital CFD In The DPP-PSD Firmware Of 730 Series](/img/ImplementationOfTheDigitalCFDInTheDPPPSDFirmwareOf730Series.png)

来自 CFD 的典型信号如下图所示，其中红点是数字采样点。 我们这里定义：过零点前的采样点（SBZC）和过零后的采样点（SAZC）。 
SBZC对应于粗略时间戳（Tcoarse），即由标准 PSD 算法评估的触发时间戳。
精细时间标记 T 的值，根据公式计算为 SBZC 和 SAZC 的线性插值：

Tfine = (midScale - SBZC)/(SAZC - SBAC) x Tsample

则过零点的时间戳由粗略时间戳加上精细时间戳： AC = Tcoarse + Tfie

![A typical CFD Signal](/img/AtypicalCFDSignal.png)

----

## 输入卡 GlobalParameters

在输入卡**GlobalParameters.txt**中

```bash
# PathToRawData 后面填写数据文件存放文件夹路径。
# PlotChooseN 后面填写监视路每多少个信号更新一次。该参数仅在开启波形监视时生效。

PathToRawData ../data
PlotChooseN 1000
```

```bash
# 当采用 USB 通讯时，参数按照以下设置
LinkType  CAEN_DGTZ_USB
VMEBaseAddress  0


# 当采用光纤通讯时，参数按照以下设置
LinkType  CAEN_DGTZ_PCI_OpticalLink
VMEBaseAddress  0
```

当前输入卡中的其余参数，当前请勿修改。


## 输入卡BoardParameters

 
在输入卡 **BoardParameters.txt** 中对每个 channel 的参数进行设置。

```bash
[RecordLength]
144

#波形记录采样点个数，最小可设置参数为16，数值需要为4的倍数
```

```bash
[ChannelMask]
11111111

# 通道是否开启的标记，从右往左依次为0-7通道，标记为1表示开启该通道，标记为0表示不启用该通道
```

```bash
[ChannelDynamicRange]
00000000

# 通道输入动态范围，从右往左依次为0-7通道，标记0表示2V动态范围，标记1表示0.5V动态范围。
```


```bash
[TriggerHoldOff]
100

# 触发保护，从过阈值时作为时间起点，在设置时间内，不会接受其它触发，该数值单位为 2 ns
```

其余参数是每个通道单独设置。

**以下是所有通道采用同样的参数设置的例子**

```bash
Channel POL  Offset  PreTrg  thr   selft   csecs  sgate  lgate  pgate   tvaw   nsbl  discr  cfdf   cfdd   trgc  
[COMMON]
  ALL   NEG   -43      30     50      1       0       24     100     8     50     1     0     3      3      1
[INDIVIDUAL]

```


**以下是通道2/6单独设置，其余通道采用同样的参数设置的例子**

```bash
Channel POL  Offset  PreTrg  thr   selft   csecs  sgate  lgate  pgate   tvaw   nsbl  discr  cfdf   cfdd   trgc  
[COMMON]
  ALL   NEG   -43      30     50      1       0    24     100     8       50     1     0     3      3      1
[INDIVIDUAL]
  2     POS    43      30     150     1       0    24     100     8       50     1     0     3      3      1
  6     POS    43      30     150     1       0    24     100     8       50     1     0     3      3      1
```

这里重点调节的参数是

- POL 表示输入信号的极性，POS表示正信号，NEG 表示负信号
- Offset 表示偏置，这里提供一个经验参数，如果是负信号，则设置-45 左右，如果是正信号则设置45左右
- PreTrg 表示记录的波形中，触发之前的点的个数
- thr    表示阈值，该数值表示相对基线的道址
- csecs  用来调节增益
- sgate  短门积分门宽，每个点表示 2 ns
- lgate  长门积分门宽，每个点表示 2 ns
- pgate  积分门起始点相对于触发点提前多少时间，每个点表示 2 ns
- tvaw   当前无用
- nsbl   基线平滑，采用多少个点来平均
- discr  当前选择 0
- cfdf   表示cfd参数的比例因子，有四个档可选
- cfdd   表示cfd的延迟参数，每个点表示 2 ns
- trgc   当前选择 1



## CFD 原理

在模拟电子学中，传统上使用 CFD（恒比定时甄别）模块来完成时间戳的确定。该技术提取振幅达到全振幅的固定分数的时间作为脉冲的时间戳。

数字 CFD 信号以经典方式实现。输入波形衰减的因子 f 等于全振幅的所需定时分数，然后信号被反转并延迟时间 d，该时间 d 等于脉冲从恒定分数水平上升到脉冲峰值所需的时间。
将最新的两个信号相加以产生双极性脉冲，CFD与其零交叉点对应于输入脉冲的分数 f ，该点作为时间触发。如下图所示

![Classical Implementation Of The Constant Fraction Discriminator](/img/ClassicalImplementationOfTheConstantFractionDiscriminator.png)

CFD 在数字化中的实现如图所示。输入样本分为两个路径：第一个以采样时钟的步长执行延迟（2 ns），第二个执行衰减。 衰减的可能选择是：相对于输入幅度的 25％，50％，75％ 和 100％（即无衰减）。 CFD 信号被称为动态的中等尺度，即在 730 系列的情况下为道址 8192。

![Implementation Of The Digital CFD In The DPP-PSD Firmware Of 730 Series](/img/ImplementationOfTheDigitalCFDInTheDPPPSDFirmwareOf730Series.png)

来自 CFD 的典型信号如下图所示，其中红点是数字采样点。 我们这里定义：过零点前的采样点（SBZC）和过零后的采样点（SAZC）。 
SBZC对应于粗略时间戳（Tcoarse），即由标准 PSD 算法评估的触发时间戳。
精细时间标记 T 的值，根据公式计算为 SBZC 和 SAZC 的线性插值：

Tfine = (midScale - SBZC)/(SAZC - SBAC) x Tsample

则过零点的时间戳由粗略时间戳加上精细时间戳： AC = Tcoarse + Tfie

![A typical CFD Signal](/img/AtypicalCFDSignal.png)





<!-- PRINCIPLE.md ends here -->
