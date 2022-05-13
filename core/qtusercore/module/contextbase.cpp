#include "contextbase.h"
#include <QtCore/QDebug>

namespace qtuser_core
{
	ContextBase::ContextBase(QObject* parent)
		:QObject(parent)
	{
		m_contextName = QString("%1").arg((size_t)this);
	}

	ContextBase::~ContextBase()
	{

	}

	void ContextBase::setContextName(const QString& name)
	{
		m_contextName = name;
	}

	QObject* ContextBase::obj(const QString& name)
	{
		return obj(this, name);
	}

	QObject* ContextBase::obj(ContextBase* parent, const QString& childName)
	{
		QObject* child = nullptr;
		int index = childName.indexOf('.');
		QString name = childName;
		if (index >= 0)
			name = childName.left(index);
		
		QMap<QString, QObject*>::iterator it = parent->m_objects.find(name);
		if (it != parent->m_objects.end())
			child = it.value();

		if (!child)
		{
			qWarning() << QString("ContextBase::obj try find noexsit child [%1] in object [%2].").arg(name).arg(parent->contextName());
			return nullptr;
		}

		if (index < 0)
		{
			qInfo() << QString("ContextBase::obj find [%1].").arg(name);
			return child;
		}

		ContextBase* base = qobject_cast<ContextBase*>(child);
		QString resetName = childName.right(childName.size() - index - 1);
		if (!base && index >= 0)
		{
			qWarning() << QString("ContextBase::obj [%1] is not ContextBase object, skip rest children [%2].").arg(name).arg(resetName);
			return child;
		}

		return obj(base, resetName);
	}

	QString ContextBase::contextName()
	{
		return m_contextName;
	}

	bool ContextBase::registerObj(const QString& name, QObject* object)
	{
		if (name.isEmpty() || !object)
		{
			qWarning() << "ContextBase::registerObj with empty name or object";
			return false;
		}

		if (m_objects.contains(name))
		{
			qCritical() << "ContextBase::registerObj with exist name.";
			return false;
		}

		m_objects.insert(name, object);
		return true;
	}
}