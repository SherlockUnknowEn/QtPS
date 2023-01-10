# QtPS
QT C++桌面程序，配合openCV实现图像的基本处理，实现画线，画框，画圆等基本ps操作

### 前置工作(pre-required)

本人未装Visual Studio，没有MSVC编译器，使用的是MinGW环境，openCV Windows build包只提供了MSVC预编译库，未提供MinGW的，因此需要自行编译MinGW版本库

根据此博客[编译windows openCV MinGW-w64方法](https://blog.huihut.com/2018/07/31/CompiledOpenCVWithMinGW64/)

或者直接下载编译好的版本 [OpenCV-MinGW-Build](https://github.com/huihut/OpenCV-MinGW-Build)

### 项目编译

修改`CMakeLists.txt`文件中 `OpenCV_DIR`，设置为 OpenCV安装路径，OpenCVConfig.cmake 文件所在目录，一般位于 `x64\mingw\lib` 下，MSVC版本库位于 `x64\vc16\lib` 下

### 功能列表

1. 基本图像处理，如灰度化，伽马变换，直方图均衡，空间滤波等 
2. 频域变换 [TODO]()
3. 绘图功能，如画线，画框，画圆等操作，颜色选择，线条粗细设置 [TODO]()
4. 形态学 腐蚀，膨胀，开，闭 [TODO]()

### 界面截图

![截图1](https://github.com/SherlockUnknowEn/QtPS/blob/main/snapshots/1.png)

![截图2](https://github.com/SherlockUnknowEn/QtPS/blob/main/snapshots/2.png)

![截图3](https://github.com/SherlockUnknowEn/QtPS/blob/main/snapshots/3.png)
