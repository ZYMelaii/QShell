# QShell
轻型跨平台shell终端。

#### + 使用QShell环境
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

#### + 使用嵌入颜色代码的输出函数
```c
#include <qsh/qshw.h>
int main()
{
    qshw_xprint(
        "QShell: \x02\033[TEST]x02\035 Hello \x02\032World\x02\031! "
        "\x02\030- from user \x02\034`%s`", "ZYmelaii");
    return 0;
}
```
其中"\x02\03d"为颜色代码，'d'的可选值有以下:

<table align="center" height="200">
    <tr>
        <th colspan="6">颜色代码</th>
    </tr>
    <tr>
        <td colspan="6" align="center">白色</td>
    </tr>
    <tr>
        <td colspan="6" align="center">0</td>
    </tr>
    <tr>
        <td align="center">红色</td>
        <td align="center">绿色</td>
        <td align="center">蓝色</td>
        <td align="center">黄色</td>
        <td align="center">品红色</td>
        <td align="center">青色</td>
    <tr>
    <tr>
        <td align="center">1</td>
        <td align="center">2</td>
        <td align="center">3</td>
        <td align="center">4</td>
        <td align="center">5</td>
        <td align="center">6</td>
    <tr>
</table>
