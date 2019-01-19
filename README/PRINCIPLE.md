<!-- PRINCIPLE.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 二 1月 15 22:41:03 2019 (+0800)
;; Last-Updated: 六 1月 19 20:02:02 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 8
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



![Diagram Summarizing The DPP-PSD Parameters](/img/DiagramSummarizingTheDPPPSDParameters.png)

## 基线计算

基线计算是 DPP-PSD 固件的一个重要特性，因为它是输入脉冲电荷积分时的参考值。 此外，大多数 DPP 参数与基线值相关。 

用户可以选择将基线设置固定值，或让 DPP 固件动态计算它。 在第一种情况下，用户必须以 LSB 单位设置基线值。 对于整个采集运行期间，此值保持固定。 在后一种情况下，固件动态地将移动时间窗口内的 N 个点的平均值作为基线评估。 用户可以为 730 系列为选择 16,64,256,1024。

然后在门开始之前从几个时钟冻结基线的计算，直到长门和触发延迟之间的最大值结束。 之后，基线再次重新开始计算，同时考虑平均值以及冻结前的点数。 由于基线计算，这允许几乎没有死区时间。
![Baseline Calculation As Managed By The DPP-PSD Algorithm](/img/BaselineCalculationAsManagedByTheDPPPSDAlgorithm.png)




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





<!-- PRINCIPLE.md ends here -->
