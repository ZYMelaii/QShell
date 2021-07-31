# QShell
轻型跨平台shell终端。

#### - 使用QShell环境
```c
#include <qsh/core.h>
#include <qsh/eval.h>
#include <qsh/qshw.h>
#include <qsh/uiman.h>

int main(int argc, char *argv[])
{
    qshui_setup(QSH_RAW); //! 搭建环境，使用内嵌模式

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

    qshui_exit(0); //! 销毁环境并退出

    //! QShell should never reach here
    return (~0);
}
```

#### - 使用嵌入颜色代码的输出函数
```c
#include <qsh/qshw.h>
int main()
{
    qshw_xprint(
        "QShell: \x02\033TEST\x02\035 Hello \x02\032World\x02\031! "
        "\x02\030- from user \x02\034`%s`", "ZYmelaii");
    return 0;
}
```
使用"\x02\03d"作为颜色代码，其中'd'的可选值有以下:
<table align="center">
    <tr>
        <td colspan="6">白色</td>
    </tr>
    <tr>
        <td colspan="6">0</td>
    </tr>
    <tr>
        <td>红色</td>
        <td>绿色</td>
        <td>蓝色</td>
        <td>黄色</td>
        <td>品红色</td>
        <td>青色</td>
    <tr>
    <tr>
        <td>1</td>
        <td>2</td>
        <td>3</td>
        <td>4</td>
        <td>5</td>
        <td>6</td>
    <tr>
</table>
