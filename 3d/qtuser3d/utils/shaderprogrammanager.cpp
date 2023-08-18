#include "qtuser3d/utils/shaderprogrammanager.h"
#include <Qt3DRender/QShaderProgram>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

#include "qtuser3d/module/glcompatibility.h"
#include "../../buildinfo.h"

#include "../../shaders/GL.code"
#include "qtuser3d/refactor/xrenderpass.h"
#include <QVector4D>
#include <QVector3D>
namespace qtuser_3d
{
	ShaderMeta typeOrder[5] =
	{
		{ Qt3DRender::QShaderProgram::ShaderType::Vertex, {".vert"}} ,
		{ Qt3DRender::QShaderProgram::ShaderType::TessellationControl, {".tcs"}} ,
		{ Qt3DRender::QShaderProgram::ShaderType::TessellationEvaluation, {".tes"}} ,
		{ Qt3DRender::QShaderProgram::ShaderType::Geometry, {".geom"} },
		{ Qt3DRender::QShaderProgram::ShaderType::Fragment, {".frag", ".fragment"} },
	};

	QVector4D defaultAmbient(0.8, 0.8, 0.8, 1.0);
	QVector4D defaultDiffuse(0.8, 0.8, 0.8, 1.0);
	QVector4D defaultSpecular(0.8, 0.8, 0.8, 1.0);
	float defaultSpecularPower = 4.5;

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
	
	void ShaderProgramManager::initRenderPass(XRenderPass *pass, const QString &name)
	{
		if (name == "antialiasing")
		{
			pass->setParameter("antiflag", 1);
		}
		else if (name == "modeldlp")
		{
			pass->setParameter("ambient", defaultAmbient);
			pass->setParameter("diffuse", defaultDiffuse);
			pass->setParameter("specular", defaultSpecular);
			pass->setParameter("specularPower", defaultSpecularPower);  
			pass->setParameter("lightDirection", QVector3D(0.0, 0.0, 1.0));
			pass->setParameter("checkScope", 1);
			pass->setParameter("supportCos", 0.5);
			pass->setParameter("hoverState", 0);
			pass->setParameter("waterState", 0);
		}
		else if (name == "gcodepreviewG")
		{
			pass->setParameter("darkColor", QVector4D(0.161, 0.161, 0.161, 1.0));
			pass->setParameter("light_direction", QVector3D(0.0, 0.0, 1.0));
			pass->setParameter("front_ambient", QVector4D(0.8, 0.8, 0.8, 1.0));
			pass->setParameter("front_diffuse", QVector4D(0.6, 0.6, 0.6, 1.0));
			pass->setParameter("back_ambient", QVector4D(0.3, 0.3, 0.3, 1.0));
			pass->setParameter("back_diffuse", QVector4D(0.3, 0.3, 0.3, 1.0));
			pass->setParameter("specular", QVector4D(0.5, 0.5, 0.5, 1.0));
			pass->setParameter("specularPower", 32.0);
			pass->setParameter("layerHeight", 0.1);
		}
	}

	Qt3DCore::QNode* ShaderProgramManager::root()
	{
		return m_root;
	}

	Qt3DRender::QShaderProgram* ShaderProgramManager::Get(const QString& name)
	{
		QMap<QString, Qt3DRender::QShaderProgram*>::iterator it = m_shaderPrograms.find(name);
		if (it != m_shaderPrograms.end())
			return (*it);

		Qt3DRender::QShaderProgram* shaderProgram = loadShaderProgram(name);

		if (shaderProgram)
		{
			it = m_shaderPrograms.insert(name, shaderProgram);
			if (it == m_shaderPrograms.end())
			{//insert error
				delete shaderProgram;
				shaderProgram = nullptr;
			}

			if (shaderProgram)
			{
				shaderProgram->setObjectName(name);
				shaderProgram->setParent(m_root);
			}
		}
		return shaderProgram;
	}
	
	Qt3DRender::QShaderProgram* ShaderProgramManager::Create(const QString& name)
	{
		return loadShaderProgram(name);
	}

	void ShaderProgramManager::cache(const QString& name)
	{
		Get(name);
	}

	Qt3DRender::QShaderProgram* ShaderProgramManager::loadShaderProgram(const QString& name)
	{
		Qt3DRender::QShaderProgram* program  = LoadLocal(name);
		if (!program)
			program = LoadFromShaderSource(name);

		qDebug() << "load shader" << name;
		return program;
	}

	Qt3DRender::QShaderProgram* ShaderProgramManager::LoadLocal(const QString& name)
	{
#if (_WIN32 || __APPLE__) && _DEBUG
		ShaderCode code;
		code.name = name;

		QString rootSourceDir = QString(CMAKE_MODULE) + "/../qtuser/3d/shaders/gl/3.3/";
		if (qtuser_3d::isGles())
		{
			rootSourceDir = QString(CMAKE_MODULE) + "/../qtuser/3d/shaders/gles/2/";
		}
		QString rootDir = rootSourceDir;
		for (int i = 0; i < 5; ++i)
		{
			QByteArray source;
			for (int j = 0; j < typeOrder[i].extensions.size(); j++)
			{
				QUrl url = QUrl::fromLocalFile(rootDir + name + QString(typeOrder[i].extensions[j].c_str()));
				source = Qt3DRender::QShaderProgram::loadSource(url);
				if (source.size() > 0)
					break;
			}
			code.source[i] = source;
		}

		return buildFromShaderCode(code);
#else
		return nullptr;
#endif
	}

	Qt3DRender::QShaderProgram* ShaderProgramManager::buildFromShaderCode(const ShaderCode& code)
	{
		if (code.source[0].size() == 0 && code.source[4].size() == 0)
			return nullptr;
	
		Qt3DRender::QShaderProgram* program = new Qt3DRender::QShaderProgram();
		if (code.source[0].size() > 0)
			program->setVertexShaderCode(code.source[0]);
		if (code.source[1].size() > 0)
			program->setTessellationControlShaderCode(code.source[1]);
		if (code.source[2].size() > 0)
			program->setTessellationEvaluationShaderCode(code.source[2]);
		if (code.source[3].size() > 0)
			program->setGeometryShaderCode(code.source[3]);
		if (code.source[4].size() > 0)
			program->setFragmentShaderCode(code.source[4]);

		qDebug() << QString("Program name [%1]").arg(code.name);
		return program;
	}

	Qt3DRender::QShaderProgram* ShaderProgramManager::LoadFromShaderSource(const QString& name)
	{
		ShaderCode code;
		ProgramDef* def = nullptr;
		for (int i = 0; i < programs_meta_size; ++i)
		{
			if (!strcmp(programs_meta[i].name, name.toStdString().c_str()))
			{
				def = &programs_meta[i];
				break;
			}
		}
		if (def)
		{
			code.name = def->name;
			if (qtuser_3d::isGles())
			{
				code.source[0] = def->vIndex >= 0 ? gles_shader_code_array[def->vIndex] : "";
				code.source[1] = def->tcsIndex >= 0 ? gles_shader_code_array[def->tcsIndex] : "";
				code.source[2] = def->tesIndex >= 0 ? gles_shader_code_array[def->tesIndex] : "";
				code.source[3] = def->gIndex >= 0 ? gles_shader_code_array[def->gIndex] : "";
				code.source[4] = def->fIndex >= 0 ? gles_shader_code_array[def->fIndex] : "";
			}
			else
			{
				code.source[0] = def->vIndex >= 0 ? shader_code_array[def->vIndex] : "";
				code.source[1] = def->tcsIndex >= 0 ? shader_code_array[def->tcsIndex] : "";
				code.source[2] = def->tesIndex >= 0 ? shader_code_array[def->tesIndex] : "";
				code.source[3] = def->gIndex >= 0 ? shader_code_array[def->gIndex] : "";
				code.source[4] = def->fIndex >= 0 ? shader_code_array[def->fIndex] : "";
			}
		}
		
		return buildFromShaderCode(code);
	}

	void ShaderProgramManager::releaseShaderPrograms()
	{
		delete m_root;
		m_root = nullptr;
		m_shaderPrograms.clear();
	}
}
