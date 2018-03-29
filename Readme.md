﻿# 一个简单的光线追踪器

一个简单的光线追踪器，用于练习图形学算法。

使用方法：编译成exe文件后，命令行执行```[可执行文件文件名] -样本数 -光线反射次数```，运行完成后会在当前文件夹下生成渲染图片image.ppm。模型目前是硬编码在代码里的.[模型信息来源](http://www.kevinbeason.com/smallpt/)

目前实现的功能有：

* 全局光照
* Phone模型光照
* 反射，漫反射，折射
* 多线程处理
* 命令行进度显示（不完善）
* 支持渲染的物体：球,三角形

希望实现的功能有：

* BVH、kd-tree加速算法
* 支持读取.obj文件
* 支持材质贴图、纹理贴图
* 裁剪
* 命令行控制->gui界面
* 即时切换摄像机视角，即时生成预览图（实时光照）
* 支持多种渲染方式
* 非真实性渲染
* 物体动画渲染
* 等等。。。