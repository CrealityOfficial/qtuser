#ifndef _NULLSPACE_QTUSERQUICK_1588062165692_H
#define _NULLSPACE_QTUSERQUICK_1588062165692_H
#include <QtCore/QObject>
#include "ccglobal/quick/macro.h"
#include <memory>

#ifdef QTUSER_QUICK_DLL
#define QTUSER_QUICK_API Q_DECL_EXPORT
#else
#define QTUSER_QUICK_API Q_DECL_IMPORT
#endif

#define QTUSER_QUICK_SCOPE "qtuser.quick"
#define QTUSER_QUICK_VERSION_MAJOR 1
#define QTUSER_QUICK_VERSION_MINOR 0
#define QTUSER_QUICK_COMMON_REASON "Created by Cpp"

#define QTUSER_QUICK_REG(x) QUICK_AUTO_TYPE(x, QTUSER_QUICK_SCOPE, QTUSER_QUICK_VERSION_MAJOR, QTUSER_QUICK_VERSION_MINOR)

#endif // _NULLSPACE_QTUSERQUICK_1588062165692_H
