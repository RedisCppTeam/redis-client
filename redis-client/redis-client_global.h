/**
* Copyright (c) 2015, 爱wifi（版权声明）
*
* @file xxxxxxx
* @brief 文件的简单介绍
*
* 文件的详细介绍
*
* @author: yuhaiyang
* @date: 2016/5/16
*/

#ifndef REDISCLIENT_GLOBAL_H
#define REDISCLIENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(REDISCLIENT_LIBRARY)
#  define REDISCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define REDISCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // REDISCLIENT_GLOBAL_H
