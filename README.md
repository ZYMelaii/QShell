# QShell
轻型跨平台shell终端。

#### 使用QShell环境
```c 
int main(int argc, char *argv[])
{
    shell_t sh;
    qsh_open(&sh); //! 创建QShell对象

    const char *cmdline = "echo -e \"\x02\033Hello \x02\000World\x02\031!\"";

    if (qsh_load_cmdline() == -1) //! 装载命令行
    {   //! 装载失败
        qshw_xprint("QShell: \x02\031[ERROR] \x02\032invalid cmdline.");
        exit(-1);
    }

    cmd_t *cmd; //! QCmd对象
    int T = qsh_parse(&sh, cmd); //! 解析并创建QCmd对象
    switch (T)
    {
        case 0: //! 语句
        {
            int exitCode = qsh_eval(cmd);
            //! TODO 1
            break;
        }
        case 1: //! 内建函数
        case 2: //! 外部指令
        {
            int exitCode = qsh_exec(cmd);
            //! TODO 2
            break;
        }
        case -1: //! 无效命令
        default:
        {
            qshw_xprint("QShell: \x02\031[ERROR] \x02\032invalid cmdline.");
            exit(-1);
        }
    }

    //! TODO 3

    qsh_close(&sh); //! 销毁QShell
    return cmd;
}
```
