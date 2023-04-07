#ifndef QTUSER_QML_RENDERMANAGER_1679642739346_H
#define QTUSER_QML_RENDERMANAGER_1679642739346_H
#include "qtuserqml/qtuserqmlexport.h"
#include "qtuserqml/gl/glquickitem.h"
#include <QtCore/QMap>

namespace qtuser_qml
{
	class QTUSER_QML_API RenderManager : public QObject
	{
		Q_OBJECT
	public:
		RenderManager(QObject* parent = nullptr);
		virtual ~RenderManager();

		void release();
	};
}

#endif // QTUSER_QML_RENDERMANAGER_1679642739346_H