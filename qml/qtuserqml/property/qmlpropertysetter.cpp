#include "qmlpropertysetter.h"
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>

namespace qtuser_qml
{
	void writeProperty(QObject* object, const QString& name, const QVariant& value)
	{
		QQmlProperty::write(object, name, value);
	}

	void writeObjectProperty(QObject* object, const QString& name, QObject* value)
	{
		QQmlProperty::write(object, name, QVariant::fromValue<QObject*>(value));
	}

    void invokeFunc(QObject* obj, const char* member, QObject* receiver,
        QGenericArgument val0,
        QGenericArgument val1,
        QGenericArgument val2,
        QGenericArgument val3,
        QGenericArgument val4,
        QGenericArgument val5,
        QGenericArgument val6,
        QGenericArgument val7,
        QGenericArgument val8)
    {
        QMetaObject::invokeMethod(obj, member, Q_ARG(QVariant, QVariant::fromValue(receiver))
            , val0, val1, val2, val3, val4, val5, val6, val7, val8);
    }
}