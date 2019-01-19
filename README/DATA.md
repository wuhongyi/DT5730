<!-- DATA.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 五 1月 18 23:52:09 2019 (+0800)
;; Last-Updated: 六 1月 19 14:34:25 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 4
;; URL: http://wuhongyi.cn -->

# 数据结构

<!-- toc -->

## 原始数据

原始数据文件中包含通道编号、QDC 的短门积分、QDC 的长门积分、format 32 bit 数据（当前模式下包含 CFD 数据），波形。

对一个事件，数据依次为 16 bit 通道编号，32 bit 时间戳的低31位，16 bit QDC 的短门积分，16 bit QDC 的长门积分，32 bit 的 format(低 10 bit 的 CFD)，32 bit 的时间戳高 16 位，16 bit 的波形长度，紧接着是 n 个波形点，每个 16 bit。

## 一级 ROOT 文件

原始 ROOT 文件中包含以下 branch，
- ch 通道编号
- qs 短门积分
- ql 长门积分
- format 参数标记，无用
- ts 时间戳，每个数值表示 2 ns
- ft cfd数值，该数值除以 1024 然后乘以 2 之后，单位为 ns，ts 时间戳加上 cfd 时间即口得到亚 ns 时间精度
- size 表示记录的波形长度，即采集了多少个点
- wave 表示波形
- sample 加入的一个branch，为了方便查看波形

如下图，通过在 root 命令行中输入以下命令可查看该文件的 branch，

```cpp
t->Print()
```

![raw root data](/img/rootdata.png)

如下图，通过在 root 命令行中输入以下命令可打印查看某事件的具体数据，

```cpp
t->Show(0)

# 这里的 0 表示查看文件中的第0个事件
# 默认下，数组只显示前 20 个数据，需要如果想要查看更长的数据

t->Show(0,100)
# 后面的 100 表示数组最长打印前 100 个数据
```

![raw root data show](/img/rootdatashow.png)

如下图，通过在 root 命令行中输入以下命令可画出某事件的波形，

```cpp
t->Draw("wave:sample","Entry$==0")

#这里的 0 表示文件中第 0 个事件 
```

![raw root data draw](/img/rootdatadrawwaveform.png)



## 时标排序







<!-- DATA.md ends here -->
