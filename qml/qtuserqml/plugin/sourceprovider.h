#ifndef QTUSER_QML_SOURCEPROVIDER_1615883776969_H
#define QTUSER_QML_SOURCEPROVIDER_1615883776969_H
#include "qtuserqml/qtuserqmlexport.h"

namespace qtuser_qml
{
	class QTUSER_QML_API SourceProvider : public QObject
	{
		Q_OBJECT
	public:
		SourceProvider(QObject* parent = nullptr);
		virtual ~SourceProvider();

		virtual QString source() = 0;
	};
}

#endif // QTUSER_QML_SOURCEPROVIDER_1615883776969_H