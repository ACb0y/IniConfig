# IniConfig
## 1.背景
- 在后端服务中使用ini格式的配置常常见，于是便自己使用了c++实现了一个易于立即的库，后面在使用过一阶段的go之后，发现go中有一个开源库可以把ini的结构直接翻译成go的嵌套的类型。
- 如果把配置文件的结构直接翻译成编译语言中的嵌套类型，则易用性更高，于是在原理的基础上又实现了配置文件结构到c++结构体映射的翻译。
## 2.代码结构
```
.
├── Config.cpp
├── Config.h
├── README.md
└── Tool
    ├── CmdLine.cpp
    ├── CmdLine.h
    ├── DemoConfig.h
    ├── Main.cpp
    ├── demo.cfg
    └── makefile
```
## 3.代码简介
- Config类实现了易用的ini配置文件的解析。
- Tool目录下实现了ConfigTool脚手架，实现了配置文件结构到c++嵌套类型的实现。
- 在Tool目录下执行make命令后可以在Tool目录下生成ConfigTool这个命令行工具。
- 在当前目录下执行`./ConfigTool -iniFile ./demo.cfg -customClass Demo`命令，会自动生成一个DemoConfig.h的头文件。
- 使用DemoConfig.h这个头文件中生成的代码，就可以实现对demo.cfg配置文件的读取，具体的使用方式可以查看Tool下的Main.cpp实现。
## 4.其他
- Tool目录下的CmdLine类是本人另外的一个开源库，这个开源库实现了命令行参数的解析，并实现了一套易用的封装，实现思路是借鉴了go的flag包，传送门：https://github.com/ACb0y/CmdLine 。