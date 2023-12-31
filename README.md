## workspace_F3 | 关于

备份涉及 STM32F3 系列的工程

## CubeIDE 标准库模板工程构建

> 2023-0721  	OldGerman

### 背景

尝试反汇编需要一份 STM32 标准库编译的固件对照，于是使用熟悉的 CubeIDE 搭建一个 STM32F303CCTx 的标准库开发环境，遇到了比较多的问题，这里记录一下

> 为什么不用Keil？此前也搭过 Keil GCC 环境并可编译运行代码，但本 ID 不熟悉Keil开发环境，不习惯 Keil IDE 的界面风格，且 keil 和 ARMCC 编译器收费，于是就咕了呵呵

### 步骤

按照 CubeMX 自动生成 HAL 库的 F303CCTx_HAL_TEMPLATE 工程目录组织需要构建的标准库 F303CCTx_STD_TEMPLATE 工程目录，涉及到的两个工程文件夹在本仓库根目录是也（若仓库根目录有其他无关的文件夹请忽视），开始操作：

1. 打开CubeIDE，使用cubemx自动生成一个stm32f303cctx随便什么名字的空工程，本示例的工程是 F303CCTx_HAL_TEMPLATE，这是HAL库工程目录
2. 新建一个文件夹，命名为 F303CCTx_STD_TEMPLATE，这就是标准库模板工程目录
3. 复制 F303CCTx_HAL_TEMPLATE 工程根目录下的 .cproject、.mxproject、.project、STM32F303CCTX_FLASH.ld 文件到 F303CCTx_STD_TEMPLATE 文件夹根目录。用 geany 或其他代码浏览工具打开这几个文件，使用文本替换工具替换这几个文件的 F303CCTx_HAL_TEMPLATE 字段为 F303CCTx_STD_TEMPLATE，并保存这些文件
4. 在 F303CCTx_STD_TEMPLATE 文件夹内新建 Core、Drivers 文件夹
5. 在 F303CCTx_STD_TEMPLATE/Core 文件夹下新建 Inc、Src、Startup 文件夹
6. 将 F303CCTx_HAL_TEMPLATE/Core/Startup/startup_stm32f303cctx.s 复制到 F303CCTx_STD_TEMPLATE/Core/Startup 文件夹
7. 将标准外设库（ [STSW-STM32108 STM32F30x/F33x DSP 和标准外设库](https://www.st.com/zh/embedded-software/stsw-stm32108.html) ）的Projects/STM32F30x_StdPeriph_Templates文件夹下的 main.h、main.c、stm32f30x_conf.h、stm32f30x_it.c、stm32f30x_it.h、system_stm32f30x.c 复制到 F303CCTx_STD_TEMPLATE 文件夹下的
  Core/Inc 和 Core/Src 文件夹（ .h 和 .c 文件分别放到 Inc 和 Src ）。复制标准外设库的 Libraries/CMSIS 文件下的 Device 和 Include 文件夹到 F303CCTx_STD_TEMPLATE /Drivers/CMSIS。复制标准外设库的 Libraries/STM32F30x_StdPeriph_Driver 文件夹到 F303CCTx_STD_TEMPLATE /Drivers。删除 F303CCTx_STD_TEMPLATE/Drivers/CMSIS/Device/ST/STM32F30x/Source/Templates文件夹
8. 复制 F303CCTx_HAL_TEMPLATE 工程 Core/Src下的 syscall.c 和 sysmem.c 文件到F303CCTx_STD_TEMPLATE/Core/Src，因为GCC中使用的stdio，printf的实现需要这玩意儿
9. 使用 CubeIDE 打开 F303CCTx_STD_TEMPLATE 工程，右键工程属性，在 C/C++ General 的 Paths and Symbols 选项下重新设置 工程的 includes 和 Source Location 路径。还要在 Symbols 里添加 USE_STDPERIPH_DRIVER 和 USE_FULL_ASSERT两个预定义宏。USE_STDPERIPH_DRIVER 用于 stm32f30x.h 中 启用 #include "stm32f30x_conf.h"。USE_FULL_ASSERT 用于开启 stm32f30x_conf.h 的 assert_param() 宏。
10. 编译 F303CCTx_STD_TEMPLATE 工程，会在工程根目录自动生成 Debug 文件夹和其内部的 makfile 文件夹，可以一键烧录到单片机，正常使用 CubeIDE 内的调试工具、现场表达式，Build Analyzer 等功能。
    但有个bug，如果修改了源码某部分，再次编译，会发现烧录进去的程序还是修改前的版本，
    这个临时解决方法是删除 Debug 文件夹里的所有文件，但每次都要这么搞很麻烦，进一步的解决方法应该是 这个自动生成的 makefile 文件的设置问题，因为不熟悉，等有时间了看看
11. Delay() 函数：最基本的 Delay()  毫秒函数的实现可参考 main.c、main.h、stm32f30x_it.c 的实现
    更进一步可参考野火的在线文档 [19. SysTick—系统定时器](https://doc.embedfire.com/mcu/stm32/f103badao/std/zh/latest/book/SysTick.html)
### 关于链接文件和启动文件

启动文件：不使用标准库包里的 gcc 版本的启动文件，使用cubemx自动生成 F303CCTx_HAL_TEMPLATE 工程 Core/Startup 文件夹里的，启动文件得和链接文件配套

链接文件：使用 cubemx 自动生成 F303CCTx_HAL_TEMPLATE 工程根目录的 STM32F303CCTX_FLASH.ld

### 参考

[STM32CubeIDE移植标准库](https://blog.csdn.net/white_loong/article/details/108461796)

[一些从keil转到GCC时踩的坑（startup和链接脚本文件，还有printf函数的实现）](https://discuss.em-ide.com/d/321-stm32keilgccstartupprintf)

 > GCC的startup文件和keil的是完全不一样的，并且GCC需要自己指定链接脚本，这两个文件可以通过STM32CUBEMX获取，选择使用的芯片之后，点到project manager选项卡，文件夹什么的随便设置一下，Toolchain/IDE选择Makefile，可以从生成的起始代码里拿到startup和.ld文件
