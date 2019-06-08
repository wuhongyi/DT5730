<!-- RUNNING.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 五 1月 18 15:01:28 2019 (+0800)
;; Last-Updated: 一 1月 21 11:20:02 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 21
;; URL: http://wuhongyi.cn -->

# 程序运行

<!-- toc -->

## 程序编译

根据编译器版本不同（关键在于编译**ROOT**时候是否支持C++11，gcc4.8及以上），需要修改**CMakeLists.txt**文件中的以下内容：

```bash
## g++ 版本小于 4.8时
##C99  ROOT不支持C++11采用以下两行
set(CMAKE_CXX_FLAGS " -fPIC -W -Wall -s")#
set(CMAKE_C_FLAGS " -fPIC -W -Wall -s")#

## g++ 版本大于 4.8时
##C++11 ROOT支持C++11采用以下两行
set(CMAKE_CXX_FLAGS "-std=c++11  -fPIC -W -Wall -s")#
set(CMAKE_C_FLAGS "-std=c++11  -fPIC   -W -Wall -s")#
```

修改好**CMakeLists.txt**文件之后。

```bash
# 进入 build 文件夹
cd build

# 执行 cmake 生成 Makefile 文件
cmake ..
# 执行 make 编译成可执行文件
make

# 生成的可执行文件在 bin 文件夹内
cd ../bin
./pkuDigitizer
```


程序启动后，将会显示以下内容：

![程序启动](/img/dgtzinit.png)

- 输入**q**退出程序。
- 输入**s**来切换启动/关闭获取。
- 输入**t**表示软件发送一个触发信号。当发送该命令后，所有启用通道都将被动触发采集一个事件。
- 输入**w**来开启/关闭数据流写入硬盘。
- 输入**R**重新读取获取参设。
- 输入**p**来切换是否打开图形监视波形。
- 输入数字**0**表示在监视界面画出监视路最近的一个波形。
- 输入数字**2，4，6，8**用来改变监视路。

获取开启，将会向**run.log**写入开始时间，获取关闭时也会写入结束时间。当开启写数据模式时，先读取**Log**文件夹下的**RunNumber**文件中的数值为当前的运行编号（用在数据文件命名），并使该数值加一保存。在**Log**文件夹下生成当前时刻命名的文件夹(例如20190118150912)，文件夹内复制保存当前获取所用输入卡。并将当前运行编号写进**run.log**.

开启写文本模式时候，获得当前运行编号 N，第一次文件名为 runN\_0,文件大小达到 2 GB 自动保存，打开 runN\_1，依次类推。

```
run0001_0 run0001_1
run0125_0
run6241_0 run6241_1 run6241_2
```

![获取开启界面](/img/dgtzstart.png)

![数据写入界面](/img/dgtzwrite.png)

![在线监视](/img/dgtzplot.png)

## 原始数据

为了最大效率地进行数据采集，我们将采集卡传输过来的数据流进行必要的解码之后，事件按照数据流来的先后顺序直接以二进制形式写入硬盘。为了方便数据文件的拷贝，我们限制了每个数据文件的大小，当达到设定值时，将写入另一文件。

原始数据文件中包含通道编号、QDC 的短门积分、QDC 的长门积分、format 32 bit 数据（当前模式下包含 CFD 数据），波形。

对一个事件，数据依次为 16 bit 通道编号，32 bit timestamp 的低31位，16 bit QDC 的短门积分，16 bit QDC 的长门积分，32 bit 的 format(低 10 bit 的 CFD)，32 bit 的 timestamp 高 16 位，16 bit 的波形长度，紧接着是 n 个波形点，每个 16 bit。


## 一级 ROOT 文件

原始的二进制文件我们不好读取及分析，因此应该先将原始二进制数据转换成我们熟悉的 ROOT 文件。

该转换直接按照原始二进制的中事件的顺序，依次转换成 ROOT 中的 event 形式。每一个 event/entry 表示一个事件，转换之后的 ROOT 文件中包含以下 branch，
- ch 通道编号
- qs 短门积分
- ql 长门积分
- format 参数标记，开发调试使用
- ts timestamp，每个数值表示 2 ns
- ft cfd 数值，该数值除以 1024 然后乘以 2 之后，单位为 ns，ts timestamp 加上 cfd 时间即可得到亚 ns 时间精度
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

用户如果需要对波形进行分析，基于该 ROOT 文件进行即可。

----

文件夹 **analysis** 内程序 **raw2root** 用来将输出的二进制文件转成 ROOT 文件。

需要先修改 **main.cc** 中原始数据的路径：

```cpp
char filepath[1024] ="../../data";//不要以 / 结尾
```

这里修改指向数据文件夹的路径。

具体运行：

```bash
make       #编译
./raw2root
```

之会提示您输入需要转的文件最小编号跟最大编号。例如我要转文件编号 0000 到 0120 的文件，只需要输入 0 空格 120 ，然后回车即可。如果里面某些编号文件不存在会自动跳过。同一个运行编号的几个子文件会存在一个 ROOT 文件中，例如 **run0100_0,run0100_1,run0100_2** 数据会转成 **run0100.root**。

----

## 二级 ROOT 文件

由于 CAEN 采集卡中内存分配的缘故，对每个开启的通道分配一定的数据空间，每个通道的数据暂存在自己的数据空间中。因此一次读取中，每个通道对应的数据都在一起，例如某次读取中，通道0 有1000个事件，通道1 有300个事件，通道 2 有500 个事件，...。数据流中先是 1000 个通道 0 的事件，然后才是 300 个通道 1 的事件，然后才是 500 个通道 2 的事件，...。也就是说，出来的数据流并不是按照timestamp的从小到大。而我们的物理分析往往需要的是事件按照timestamp排列好的。 

因此，该转换将原始的 ROOT 文件中的事件，按照 timestamp 从小到大进行排序。同时丢弃原始数据中的波形数据。

转换后的数据中包含以下 branch，

- ch 通道编号
- qs 短门积分
- ql 长门积分
- format 参数标记，开发调试使用
- ts 时间戳，每个数值表示 2 ns
- ft cfd数值，该数值除以 1024 然后乘以 2 之后，单位为 ns，ts timestamp 加上 cfd 时间即口得到亚 ns 时间精度
- nevt 事件在文件中的编号

![time sort root file](/img/timesortfile.png)

----

文件夹 **analysis** 内程序 **timesort** 用来将 ROOT 文件中的事件按照 timestamp 从小到大进行排序。

需要先修改 **main.cc** 中原始数据的路径：

```cpp
TString filepath ="../../data";//不要以 / 结尾
```

这里修改指向数据文件夹的路径。

具体运行：

```bash
make       #编译
./timesort
```

之会提示您输入需要转的文件最小编号跟最大编号。例如我要转文件编号 0000 到 0120 的文件，只需要输入 0 空格 120 ,然后回车即可。




<!-- RUNNING.md ends here -->
