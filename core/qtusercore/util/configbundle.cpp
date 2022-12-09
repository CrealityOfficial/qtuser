#include "configbundle.h"

#include <QtCore/QCoreApplication>

using namespace qtuser_core;


ConfigBundle::ConfigBundle(QString bundleName, QString bundleType, QObject* parent)
	: QObject(parent)
	, m_bundleDir("")
{
#if defined (__APPLE__)
	int index = QCoreApplication::applicationDirPath().lastIndexOf("/");
	m_bundleDir = QCoreApplication::applicationDirPath().left(index) + "/Resources/resources/config/";
#else
	m_bundleDir = QCoreApplication::applicationDirPath() + "/resources/config/";
#endif

	m_bundleName = bundleName.isEmpty() ? DEFAULT_BUNDLE_NAME : bundleName;
	m_bundleType = bundleType.isEmpty() ? "" : bundleType + "/";
	QString iniPath = m_bundleDir + m_bundleType + m_bundleName + ".ini";

	m_pConfigSettings = new QSettings(iniPath, QSettings::Format::IniFormat, this);
	m_pConfigSettings->setIniCodec("UTF-8");
}

ConfigBundle::~ConfigBundle()
{
}

void ConfigBundle::setValue(const QString key, const QVariant value, QString group)
{
	if (!group.isEmpty())
		m_pConfigSettings->beginGroup(group);
	
	m_pConfigSettings->setValue(key, value);

	if (!group.isEmpty())
		m_pConfigSettings->endGroup();
}

void ConfigBundle::removeValue(const QString key, QString group)
{
	if (!group.isEmpty())
		m_pConfigSettings->beginGroup(group);

	m_pConfigSettings->remove(key);

	if (!group.isEmpty())
		m_pConfigSettings->endGroup();
}

void ConfigBundle::clear()
{
	m_pConfigSettings->clear();
}

QStringList ConfigBundle::keys(QString group)
{
	if (!group.isEmpty())
		m_pConfigSettings->beginGroup(group);

	QStringList keys = m_pConfigSettings->allKeys();

	if (!group.isEmpty())
		m_pConfigSettings->endGroup();

	return keys;
}

bool ConfigBundle::hasKey(const QString key, QString group)
{
	if (!group.isEmpty())
		m_pConfigSettings->beginGroup(group);

	bool hasKey = m_pConfigSettings->contains(key);

	if (!group.isEmpty())
		m_pConfigSettings->endGroup();

	return hasKey;
}

QVariant qtuser_core::ConfigBundle::value(QString key, QString group)
{
	if (!group.isEmpty())
		m_pConfigSettings->beginGroup(group);

	QVariant value = m_pConfigSettings->value(key, "");

	if (!group.isEmpty())
		m_pConfigSettings->endGroup();

	return value;
}

void ConfigBundle::sync()
{
	m_pConfigSettings->sync();
}
