#include "qtuser3d/utils/shaderprogrammanager.h"
#include <Qt3DRender/QShaderProgram>
#include <QtCore/QUrl>

namespace qtuser_3d
{
	ShaderProgramManager ShaderProgramManager::m_shaderProgramManager;
	ShaderProgramManager::ShaderProgramManager(QObject* parent)
		:QObject(parent)
	{
		m_root = new Qt3DCore::QNode();
	}

	ShaderProgramManager::~ShaderProgramManager()
	{
		releaseShaderPrograms();
	}

	ShaderProgramManager& ShaderProgramManager::Instance()
	{
		return m_shaderProgramManager;
	}

	Qt3DCore::QNode* ShaderProgramManager::root()
	{
		return m_root;
	}

	Qt3DRender::QShaderProgram* ShaderProgramManager::Get(const QString& name)
	{
		QMap<QString, Qt3DRender::QShaderProgram*>::iterator it = m_shaderPrograms.find(name);
		if (it != m_shaderPrograms.end()) return (*it);

		Qt3DRender::QShaderProgram* shaderProgram = loadShaderProgram(name);

		if (shaderProgram)
		{
			it = m_shaderPrograms.insert(name, shaderProgram);
			if (it == m_shaderPrograms.end())
			{//insert error
				delete shaderProgram;
				shaderProgram = nullptr;
			}
		}
		return shaderProgram;
	}

	void ShaderProgramManager::cache(const QString& name)
	{
		QMap<QString, Qt3DRender::QShaderProgram*>::iterator it = m_shaderPrograms.find(name);
		if (it != m_shaderPrograms.end()) return;

		Qt3DRender::QShaderProgram* shaderProgram = loadShaderProgram(name);

		if (shaderProgram)
		{
			it = m_shaderPrograms.insert(name, shaderProgram);
			if (it == m_shaderPrograms.end())
			{//insert error
				delete shaderProgram;
				shaderProgram = nullptr;
			}
		}
	}

	Qt3DRender::QShaderProgram* ShaderProgramManager::loadShaderProgram(const QString& name)
	{
		Qt3DRender::QShaderProgram* shaderProgram = new Qt3DRender::QShaderProgram(m_root);

		const static QString prefix = QString("qrc:/renderhelper/shaders/");
		const static QString extensions[3] = { ".vert", ".frag", ".geom" };

		QUrl vertexUrl(prefix + name + extensions[0]);
		QUrl fragmentUrl(prefix + name + extensions[1]);
		QUrl geometryUrl(prefix + name + extensions[2]);

		QByteArray vertexByteArray = Qt3DRender::QShaderProgram::loadSource(vertexUrl);
		QByteArray fragmentByteArray = Qt3DRender::QShaderProgram::loadSource(fragmentUrl);
		QByteArray geometryByteArray = Qt3DRender::QShaderProgram::loadSource(geometryUrl);

		if (vertexByteArray.size() > 0) shaderProgram->setVertexShaderCode(vertexByteArray);
		if (fragmentByteArray.size() > 0) shaderProgram->setFragmentShaderCode(fragmentByteArray);
		if (geometryByteArray.size() > 0) shaderProgram->setGeometryShaderCode(geometryByteArray);

		return shaderProgram;
	}

	void ShaderProgramManager::releaseShaderPrograms()
	{
		delete m_root;
		m_root = nullptr;
		m_shaderPrograms.clear();
	}
}
