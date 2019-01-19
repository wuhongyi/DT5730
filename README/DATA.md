<!-- DATA.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 五 1月 18 23:52:09 2019 (+0800)
;; Last-Updated: 六 1月 19 10:22:57 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# 数据结构

<!-- toc -->

## 原始数据

原始数据文件中包含通道编号、QDC 的短门积分、QDC 的长门积分、format 32 bit 数据（当前模式下包含 CFD 数据），波形。

对一个事件，数据依次为 16 bit 通道编号，32 bit 时间戳的低31位，16 bit QDC 的短门积分，16 bit QDC 的长门积分，32 bit 的 format(低 10 bit 的 CFD)，32 bit 的时间戳高 16 位，16 bit 的波形长度，紧接着是 n 个波形点，每个 16 bit。


## 一级 ROOT 文件





## 时标排序







<!-- DATA.md ends here -->
