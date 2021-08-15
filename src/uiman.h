#ifndef QSHELL_UIMAN
#define QSHELL_UIMAN

#include "core.h"

#define QSH_RAW 0 //! 不使用UI
#define QSH_CUI 1 //! 使用CUI
#define QSH_GUI 2 //! 使用GUI(不支持)

/********************************
 *  @author: ZYmelaii
 *  @brief: QShell-UI环境预搭建
 *  @param:
 *     # mode: QShell-UI模式
 *  @note: 使用QShell前调用，全局范围在环境销毁前仅调用一次
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qshui_setup(int mode);

/********************************
 *  @author: ZYmelaii
 *  @brief: 清理QShell-UI环境
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
 *  @brief: 中断QShell-UI并清理环境
 *  @param:
 *     # exitCode: 程序退出代码
 *  @note: 调用后程序结束
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qshui_exit(int exitCode);

#endif