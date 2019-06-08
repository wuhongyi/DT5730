# 简介


DT5730 DPP-PSD 固件有 List、Oscilloscope、Mixed 三种模式，List 模式只记录能量时间，但是其 PSD 功能无法使用，Oscilloscope 模式只能记录波形，Mixed 模式拥有 cfd、QSD功能，但是缺点时必须记录波形，不想记录波形时候我们可以设置成记录最小波形长度来减少数据量的输出。因此根据我们的测量需求，在当前硬件及固件能力下，我们只能选择 Mixed 模式。


程序包中包含以下文件夹/文件：
- CAEN  存放本程序依赖的驱动，硬件说明书
- code  程序
	- analysis 存放数据转换等程序
	- bin 存放获取可执行程序
	- cmake 存放 CMake依赖文件
	- data 数据存放路径
	- include 获取程序源码
	- Log 存放运行日记
	- src 获取程序源码
- docs  网页版说明书
- firmware  存放固件
- README    存放md版说明书
- README.md  本文件
- README.pdf pdf版本说明书




