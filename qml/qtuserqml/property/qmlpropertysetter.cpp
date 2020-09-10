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
}