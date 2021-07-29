#ifndef QSHELL_UIMAN_H
#define QSHELL_UIMAN_H

#include "core.h"

/********************************
 *  @author: ZYmelaii
 *  @brief: QShell-GUI环境预搭建
 *  @param:
 *     # void: /
 *  @note: 使用QShell前调用，全局范围在环境销毁前仅调用一次
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qshui_setup();

/********************************
 *  @author: ZYmelaii
 *  @brief: 清理QShell-GUI环境
 *  @param:
 *     # void: /
 *  @note: 已经创建的QShell对象将被关闭，此时创建QShell对象需重新调用`qshui_setup`
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qshui_cleanup();

/********************************
 *  @author: ZYmelaii
 *  @brief: 中断QShell-GUI并清理环境
 *  @param:
 *     # void: /
 *  @note: 调用后程序结束
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qshui_exit();

#endif