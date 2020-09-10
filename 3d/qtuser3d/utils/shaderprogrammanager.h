#ifndef _QTUSER_3D_SHADERPROGRAMMANAGER_1588130738105_H
#define _QTUSER_3D_SHADERPROGRAMMANAGER_1588130738105_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DCore/QNode>
#include <QtCore/QMap>

namespace Qt3DRender
{
	class QShaderProgram;
}

namespace qtuser_3d
{
	class QTUSER_3D_API ShaderProgramManager : QObject
	{
		ShaderProgramManager(QObject* parent = nullptr);
	public:
		~ShaderProgramManager();

		static ShaderProgramManager& Instance();

		void cache(const QString& name);
		Qt3DRender::QShaderProgram* Get(const QString& name);
		Qt3DCore::QNode* root();
	protected:
		Qt3DRender::QShaderProgram* loadShaderProgram(const QString& name);
		void releaseShaderPrograms();
	private:
		static ShaderProgramManager m_shaderProgramManager;

		Qt3DCore::QNode* m_root;
		QMap<QString, Qt3DRender::QShaderProgram*> m_shaderPrograms;   //manager owns all shader programs
	};
}

#define SHADERS(x) qtuser_3d::ShaderProgramManager::Instance().Get(x)
#define SHADERROOT qtuser_3d::ShaderProgramManager::Instance().root()
#define SHADERCACHE(x) qtuser_3d::ShaderProgramManager::Instance().cache(x)
#endif // _QTUSER_3D_SHADERPROGRAMMANAGER_1588130738105_H
