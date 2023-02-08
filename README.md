## 这是什么
这个仓库收集了一些自己对嵌入式c的思考以及第三方嵌入式相关的开源代码，以方便自己以及他人学习之用。对于第三方的模块，大多来自[github](https://github.com/)。如果你点击链接后网页无法显示，为了让自己有更多的时间享受生活，你可能需要准备一把梯子。

仓库会不定时更新，为了避免迷路，最好点颗星或者Watch。

## 嵌入式常用数据结构与算法

[malloc](https://danluu.com/malloc-tutorial/) - malloc的实现原理讲解。

[write-a-hash-table](https://github.com/jamesroutley/write-a-hash-table) - 用C写一个哈希表（键值对），是一分很好的参考资料。

IMU融合算法
* [Fusion](https://github.com/xioTechnologies/Fusion) - 用于IMU的传感器融合库，针对嵌入式系统进行了优化。

* [Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS) - 多种AHRS算法实现。

* [MPU9250](https://github.com/kriswiner/MPU9250) - Arduino上使用MPU9250传感器的AHRS算法实现。



## 嵌入式c的程序设计方法
[book_cprogramming](https://github.com/gurugio/book_cprogramming) - C中的框架与插件设计，里面介绍了如何使用C编写更高质量的程序。

[bare-metal-programming-guide](https://github.com/cpq/bare-metal-programming-guide) - 裸机编程指南。为那些希望用GCC编译器和数据手册而无需其他任何东西就能开始为微控制器（单片机）编程的开发者而写。

[lowlevelprogramming-university](https://github.com/gurugio/lowlevelprogramming-university) - 怎样称为底层程序员，该库更多针对的是Linux的学习。

## 第三方嵌入式开源库
GPS数据解析库
* [libnmea](https://github.com/jacketizer/libnmea) - 用于解析NMEA 0183的C库。

* [lwgps](https://github.com/MaJerle/lwgps) - 轻量级GPS NMEA解析库。

GUI库
* [UGUI](https://github.com/achimdoebler/UGUI) - 开源小型GUI，该仓库已经停止维护。虽然提供的功能很少，但做为学习GUI原理的却是一份相当不错的资料。

* [awtk](https://github.com/zlgopen/awtk/blob/master/README_zh.md) - 周立功出品的开源GUI，看上去比较流行。该开源GUI目前是由《系统程序员成长计划》的作者李先静维护。

* [lvgl](https://github.com/lvgl/lvgl) - 在嵌入式中很流行的开源GUI库。

* [MiniGUI](https://github.com/VincentWei/MiniGUI) - 可用在嵌入式平台的开源GUI库。

* [GuiLite](https://github.com/idea4good/GuiLite) - 该GUI库虽然号称只有4000+行C++代码，但是也正因为小，所以很多功能需要自己去实现。

* [LingLongGUI](https://gitee.com/gzbkey/LingLongGUI) - 适用于嵌入式平台的国产GUI。

文件系统
* [fatfs](https://github.com/abbrev/fatfs) - 用于小型嵌入式系统的通用 FAT 文件系统模块。

* [littlefs](https://github.com/littlefs-project/littlefs) - 为微控制器设计的小型文件系统。

RTOS
* [uC-OS3](https://github.com/weston-embedded/uC-OS3) - 嵌入式RTOS但不免费。

* [FreeRTOS](https://github.com/FreeRTOS/FreeRTOS) - 免费开源的嵌入式RTOS。

* [rt-thread](https://gitee.com/rtthread/rt-thread) - 国产免费开源RTOS。

CLI(命令行界面)
* [lwshell](https://github.com/MaJerle/lwshell) - 用于嵌入式系统的轻量级、平台无关的命令行 shell。

* [xcmd](https://gitee.com/two_salted_eggs/xcmd) - 为单片机提供一个能够快速搭建且占用资源很小的命令行工具。

* [nr_micro_shell](https://gitee.com/nrush/nr_micro_shell) - 针对资源较少的MCU编写的基本命令行工具。

[CherryUSB](https://gitee.com/RT-Thread-Mirror/CherryUSB?_from=gitee_search) - 用于嵌入式系统的USB（带USB IP）主从协议栈。

[stm32-bootloader](https://github.com/akospasztor/stm32-bootloader) - 用于STM32微控制器的可定制的Bootloader。该项目可应用于利用外部SD卡对STM32进行IAP（in-application-programming）编程。

[Regexp](https://github.com/nickgammon/Regexp) - 可以用在嵌入式平台的正则表达式库。

[EasyLogger](https://gitee.com/Armink/EasyLogger) - 超轻量级的C/C++日志库，适合对资源敏感的软件项目。

[FlashDB](https://gitee.com/Armink/FlashDB) - 超轻量级的嵌入式数据库。

[CmBacktrace](https://gitee.com/Armink/CmBacktrace) - 针对 ARM Cortex-M 系列 MCU 的错误代码自动追踪、定位，错误原因自动分析的开源库。

[SFUD](https://gitee.com/Armink/SFUD) - 串行 Flash 通用驱动库。

[MultiButton](https://github.com/0x1abin/MultiButton) - 小巧简单易用的事件驱动型按键驱动模块，可无限量扩展按键。

[lwrb](https://github.com/MaJerle/lwrb) - 通用的 FIFO 环形缓冲区实现，最大亮点在于该FIFO可与DMA无缝结合。

[lwmem](https://github.com/MaJerle/lwmem) - 适合嵌入式平台的动态内存分配库。

[cAT](https://github.com/marcinbor85/cAT) - 在嵌入式设备中解析AT指令的库。

[mOTA](https://gitee.com/DinoHaw/mOTA) - 专为32位 MCU 开发的 OTA 组件。

[btstack](https://github.com/bluekitchen/btstack) - 蓝牙开源协议栈，但并非完全开源。

## 编程风格与规范
[360 安全规则集合](https://github.com/Qihoo360/safe-rules) - 360出品的C/C++的编程规范与风格。

[Linux kernel coding style](https://www.kernel.org/doc/Documentation/process/coding-style.rst) - Linux内核代码风格。

[c-code-style](https://github.com/MaJerle/c-code-style) - 来自ST的工程师的c代码风格。

## 工具
[RTT-T](https://gitee.com/bds123/RTT-T) - 如果你只使用官方RTT Viewer收/发来自SEGGER RTT的数据，这款软件在这点上比RTT Viewer做的更好。

## 书籍与资料
[Software-Engineering-at-Google](https://github.com/qiangmzsx/Software-Engineering-at-Google) - 谷歌软件工程，可以观摩一下。

[GitHubDaily](https://github.com/GitHubDaily/GitHubDaily) - 定期推送各种语言的流行库。

[document-style-guide](https://github.com/ruanyf/document-style-guide) - 阮一峰的中文文档写作指南。

[科技爱好者周刊](https://github.com/ruanyf/weekly) - 阮一峰的科技分享。

## 其他
[Apollo-11](https://github.com/chrislgarry/Apollo-11) - 阿姆斯特朗登月时登月舱的AGC（制导计算机）系统，仅用来膜拜。

[How-To-Ask-Questions-The-Smart-Way](https://github.com/ryanhanwu/How-To-Ask-Questions-The-Smart-Way/blob/main/README-zh_CN.md) - 怎么问问题？与编程没有直接关系，但也很重要。

## 贡献
贡献规则：
> \[仓库名称](仓库网址) - 简要的描述。 