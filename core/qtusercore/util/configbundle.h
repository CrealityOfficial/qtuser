#ifndef CORE_CONFIGBUNDLE_H
#define CORE_CONFIGBUNDLE_H

#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QSettings>

#define DEFAULT_BUNDLE_NAME "cxsw_config"

#define GLOBAL_RENDER_BUNDLE_NAME "global_render_config"
#define MODEL_GROUP "model"
#define BACKGROUND_GROUP "background"

#define PLUGIN_RENDER_BUNDLE_NAME "plugin_render_config"


namespace qtuser_core
{
    class QTUSER_CORE_API ConfigBundle : public QObject
    {
    public:
        ConfigBundle(QString bundleName = DEFAULT_BUNDLE_NAME, QString bundleType = "", QObject* parent = nullptr);
        ~ConfigBundle();

        void setValue(const QString key, const QVariant value, QString group = "");
        void removeValue(const QString key, QString group = "");
        void clear();

        QStringList keys(QString group = "");
        bool hasKey(const QString key, QString group = "");
        QVariant value(QString key, QString group = "");

        QString bundleName() { return m_bundleName; }
        QString bundleType() { return m_bundleType; }
        QString bundleDir() { return m_bundleDir; }

        void sync();

    private:
        QString m_bundleName;
        QString m_bundleType;
        QString m_bundleDir;

        QSettings* m_pConfigSettings;
    };
}

#endif // CORE_CONFIGBUNDLE_H

