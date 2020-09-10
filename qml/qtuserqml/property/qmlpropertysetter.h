#ifndef QTUSER_QML_QMLPROPERTYSETTER_1596678501815_H
#define QTUSER_QML_QMLPROPERTYSETTER_1596678501815_H
#include "qtuserqml/qtuserqmlexport.h"
#include <QtCore/QVariant>

namespace qtuser_qml
{
	QTUSER_QML_API void writeProperty(QObject* object, const QString& name, const QVariant& value);
	QTUSER_QML_API void writeObjectProperty(QObject* object, const QString& name, QObject* value);
}

#endif // QTUSER_QML_QMLPROPERTYSETTER_1596678501815_H