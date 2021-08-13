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

#### + 使用QShell提供的挂链式哈希表
```c
#include <qsh/core.h>

//! 以构建一张{string: any...}的哈希表为例

#include <stdio.h>
#include <string.h>

uint32_t fnhash(void *key)
{   //! 字符串哈希函数
    return qsh_hash_str(key);
}

int fncmp(void *x, void *y)
{   //! 键比较函数
    return strcmp((const char*)x, (const char*)y);
}

void* fndup(const void *src)
{   //! 键拷贝函数
    //! 该函数返回的指针内存空间应当由`qsh_malloc`分配
    return (src != NULL ? qsh_strdup((const char*)src) : NULL);
}

int main(int argc, char const *argv[])
{
    const size_t T = 4; //! 哈希表大小

    hashmap_t map;
    qsh_hashmap_init(&map, T); //! 初始化哈希表
    if (map.data == NULL)
    {
        perror("failed to initialize a QShell hashmap.");
        return -1;
    }

    //! 增加键
    typedef struct { int x; int y; double z; char s[32]; } Object;
    qsh_hashmap_add(&map, "KEY.int", fnhash, fncmp, fndup);
    qsh_hashmap_add(&map, "KEY.string", fnhash, fncmp, fndup);
    qsh_hashmap_add(&map, "KEY.Object", fnhash, fncmp, fndup);

    //! 写入键值
    //! NOTES:
    //!   1. 完成写入后，`qsh_hashmap_write`返回的`object_t*`指针将被清理，避免再次使用该指针。
    //!   2. 写入数据的内存空间应当由`qsh_malloc`分配以避免释放空间时的内存泄漏问题。
    //!   3. 除非你能恰当地管理它们，不要写入过于复杂的对象。
    //!   4. 一般情况下，写入的数据指针应当在写入完成之后立即舍弃，除非你想在外部获取该哈希表元素的直接访问权。
    {
        object_t *obj = NULL;

        //! KEY.int
        obj = qsh_hashmap_write(&map, "KEY.int", fnhash, fncmp);
        {
            int *val = (int*)qsh_malloc(sizeof(int));
            *val = 233;
            obj->data = (void*)val;
        }
        qsh_hashmap_done(&map); //! 完成写入

        obj = qsh_hashmap_write(&map, "KEY.string", fnhash, fncmp);
        {
            obj->data = (void*)strdup("Hello World!");
        }
        qsh_hashmap_done(&map); //! 完成写入

        obj = qsh_hashmap_write(&map, "KEY.Object", fnhash, fncmp);
        {
            Object *val = (Object*)qsh_malloc(sizeof(Object));
            val->x = 496;
            val->y = 8218;
            val->z = 3.1415926535897932384626433832795;
            strcpy(val->s, "There're some famous numbers.");
            obj->data = (void*)val;
        }
        qsh_hashmap_done(&map); //! 完成写入
    }

    //! 访问键值
    {
        {   //! KEY.int
            int *val = (int*)qsh_hashmap_getval(&map, "KEY.int", fnhash, fncmp);
            printf("KEY.int: %d;\n", *val);
        }
        {   //! KEY.string
            const char *val = (const char*)qsh_hashmap_getval(&map, "KEY.string", fnhash, fncmp);
            printf("KEY.string: \"%s\";\n", val);
        }
        {   //! KEY.Object
            Object *val = (Object*)qsh_hashmap_getval(&map, "KEY.Object", fnhash, fncmp);
            printf("KEY.Object: \n{\n\tx: %d,\n\ty: %d,\n\tz: %f,\n\ts: \"%s\"\n};\n",
                val->x, val->y, val->z, val->s);
        }
    }

    //! 删除键
    qsh_hashmap_del(&map, "KEY.int", fnhash, fncmp);
    qsh_hashmap_del(&map, "KEY.string", fnhash, fncmp);
    qsh_hashmap_del(&map, "KEY.Object", fnhash, fncmp);

    qsh_hashmap_free(&map); //! 释放哈希表

    return 0;
}

```
