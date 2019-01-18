<!-- RUNNING.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 五 1月 18 15:01:28 2019 (+0800)
;; Last-Updated: 五 1月 18 23:46:52 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 5
;; URL: http://wuhongyi.cn -->

# 程序运行

<!-- toc -->

## 程序编译

根据编译器版本不同（关键在于编译**ROOT**时候是否支持C++11，gcc4.8及以上），需要修改**CMakeLists.txt**文件中的以下内容：

```bash
##C99  ROOT不支持C++11采用以下两行
set(CMAKE_CXX_FLAGS " -fPIC -W -Wall -s")#
set(CMAKE_C_FLAGS " -fPIC -W -Wall -s")#

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
- 输入**t**表示给一个外部触发信号。
- 输入**w**来切换是否写文本。
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

<!-- ![获取开启界面](/img/dgtzstart.png) -->

<!-- ![数据写入界面](/img/dgtzwrite.png) -->

<!-- ![在线监视与数据写入界面](/img/dgtzplot.png) -->

## 原始数据到 ROOT 文件的转换

文件夹**analysis**内程序**raw2root.cc**用来将输出的二进制文件转成ROOT文件。

```cpp
char filepath[128] ="../data";
```

需要修改里面指向数据文件夹的路径。

具体运行：

```bash
make
./raw2root
```
会提示你输入需要转的文件最小编号跟最大编号。例如我要转文件编号0000到0120的文件，只需要输入0跟120即可。如果里面某些编号文件不存在会自动跳过。同一个运行标号的几个子文件会存在一个root文件中，例如**run0100_0,run0100_1,run0100_2**数据会转成**run0100.root**。

----


## 输入卡参数

在输入卡**GlobalParameters.txt**中

```bash
PathToRawData ../data
PlotChooseN 1000
```

**PathToRawData**后面填写数据文件存放文件夹路径。**PlotChooseN** 后面填写监视路每多少个信号更新一次。


在输入卡**BoardParameters.txt**中对每个 channel 的参数进行设置。



<!-- RUNNING.md ends here -->
