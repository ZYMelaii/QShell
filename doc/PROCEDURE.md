# QShell 工作流程

1. 初始化`QShell`环境
```c
qshui_setup(T); //! T <- { QSH_RAW, QSH_CUI, QSH_GUI }
```

2. 创建`QShell`对象
```c
shell_t sh;
qsh_open(&sh);
```

3. 配置全局运行环境

4. 导入用户配置及扩展文件

5. 生成根部上下文

6. `QShell`主程序
- `QSH_RAW`模式<br>
  - 装载命令
```c

```
- `QSH_CUI`模式
```c
qsh_mainloop(&sh);
```
- `QSH_GUI`模式 `暂不支持`
