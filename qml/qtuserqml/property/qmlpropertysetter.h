#ifndef QTUSER_QML_QMLPROPERTYSETTER_1596678501815_H
#define QTUSER_QML_QMLPROPERTYSETTER_1596678501815_H
#include "qtuserqml/qtuserqmlexport.h"
#include <QtCore/QVariant>

namespace qtuser_qml
{
	QTUSER_QML_API void writeProperty(QObject* object, const QString& name, const QVariant& value);
	QTUSER_QML_API void writeObjectProperty(QObject* object, const QString& name, QObject* value);

	QTUSER_QML_API void invokeFunc(QObject* obj, const char* member, QObject* receiver,
        QGenericArgument val0 = QGenericArgument(nullptr),
        QGenericArgument val1 = QGenericArgument(),
        QGenericArgument val2 = QGenericArgument(),
        QGenericArgument val3 = QGenericArgument(),
        QGenericArgument val4 = QGenericArgument(),
        QGenericArgument val5 = QGenericArgument(),
        QGenericArgument val6 = QGenericArgument(),
        QGenericArgument val7 = QGenericArgument(),
        QGenericArgument val8 = QGenericArgument());

    QTUSER_QML_API void writeObjectNestProperty(QObject* object, const QString& childNest, const QString& name, QObject* value);
}

#endif // QTUSER_QML_QMLPROPERTYSETTER_1596678501815_H