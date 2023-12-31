
#include <QDebug>
#include "glcompatibility.h"

#include "visualinfo.h"

#include <QtCore/QDebug>
#include <QtGui/QSurfaceFormat>
#include <QtCore/QProcess>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>

#define FORCE_USE_GLES 0

namespace qtuser_3d
{
	QSurfaceFormat gFormat;
    Qt::ApplicationAttribute gAttribute = Qt::AA_UseDesktopOpenGL;
	bool isGles()
	{
        #ifdef __APPLE__
            return false;
        #else
            return gAttribute == Qt::AA_UseOpenGLES;
        #endif
		
	}

	bool isSoftwareGL()
	{
        #ifdef __APPLE__
            return false;
        #else
		    return gAttribute == Qt::AA_UseSoftwareOpenGL;
        #endif
	}

    bool _isNotInstalledVideoDriver() {
        QScopedPointer<QProcess> wmic(new QProcess());
        wmic->setProcessChannelMode(QProcess::MergedChannels);

        QString infName, drvName;
        QObject::connect(wmic.data(), &QProcess::readyReadStandardOutput, [&wmic, &infName, &drvName]() {
            QTextStream stream(wmic->readAllStandardOutput());
            QString s;
            while (stream.readLineInto(&s)) {
                QString line = s.trimmed();
                if (line.isEmpty())
                    continue;
                if (line.startsWith("InfFilename", Qt::CaseSensitive)) {
                    infName = line.split("=")[1];
                }
                else if (line.startsWith("Name", Qt::CaseSensitive)) {
                    drvName = line.split("=")[1];
                }
            }
            });
        wmic->start("wmic path win32_videoController get InfFilename,name /format:list");
        wmic->waitForFinished();
        return infName.startsWith("display.inf");
    }

    Qt::ApplicationAttribute chooseAttribute()
    {
#ifdef __APPLE__
        return Qt::AA_UseDesktopOpenGL;
#endif
        GLContext ctx;
        /* ---------------------------------------------------------------------- */
        /* create OpenGL rendering context */
        InitContext(&ctx);
        if (GL_TRUE == CreateContext(&ctx))
        {
            qDebug() << QString("Error: CreateContext failed.");
            DestroyContext(&ctx);
            return Qt::AA_UseOpenGLES;
        }

        GLuint err = glewInit();

        if (GLEW_OK != err)
        {
            const GLubyte* error = glewGetErrorString(err);
            qDebug() << QString("glewInit failed: %1.").arg(QString((const char*)error));
            return Qt::AA_UseOpenGLES;
        }

        qDebug() << QString("Running on a %1 from %2").arg((const char*)glGetString(GL_RENDERER))
                                                      .arg((const char*)glGetString(GL_VENDOR));

        QString version = QString((const char*)glGetString(GL_VERSION));
        qDebug() << QString("OpenGL version %1 is supported").arg(version);

        int major = 1;
        QStringList strs = version.split(' ');
        if (strs.count() > 0)
        {
            QString gVersion = strs.at(0);
            QStringList versions = gVersion.split('.');
            if (versions.count() >= 1)
                major = versions.at(0).toInt();
        }

        if (major < 3) {
            return Qt::AA_UseOpenGLES;
        }

        DestroyContext(&ctx);
        return Qt::AA_UseDesktopOpenGL;
    }

	void setBeforeApplication()
	{
        bool haveDriver = !_isNotInstalledVideoDriver();

        if (haveDriver)
        {
#if FORCE_USE_GLES
            gAttribute = Qt::AA_UseOpenGLES;
#else
            gAttribute = chooseAttribute();
#endif
        }
        else
        {
            qDebug() << "Driver isn't installed.";
            gAttribute = Qt::AA_UseSoftwareOpenGL;
        }
	
        switch (gAttribute)
        {
        case Qt::AA_UseSoftwareOpenGL:
            QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL, false);
            QCoreApplication::setAttribute(Qt::AA_UseOpenGLES, false);
            QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, true);
            qDebug() << "Qt::AA_UseSoftwareOpenGL.";
            break;
        case Qt::AA_UseDesktopOpenGL:
            QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);
            QCoreApplication::setAttribute(Qt::AA_UseOpenGLES, false);
            QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, false);
            qDebug() << "Qt::AA_UseDesktopOpenGL.";
            break;
        case Qt::AA_UseOpenGLES:
            QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL, false);
            QCoreApplication::setAttribute(Qt::AA_UseOpenGLES, true);
            QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, false);
            qDebug() << "Qt::AA_UseOpenGLES.";
            break;
        default:
            break;
        }
	}


    bool checkMRTSupported()
    {
        if (!QOpenGLContext::currentContext()->functions()->hasOpenGLFeature(QOpenGLFunctions::MultipleRenderTargets)) {
            qWarning("Multiple render targets not supported");
            return false;
        }

        int maxColorAttachments = 0;
        QOpenGLContext::currentContext()->functions()->glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
        qInfo() << "GL_MAX_COLOR_ATTACHMENTS = " << maxColorAttachments;
        if (maxColorAttachments < 8)
        {
            qWarning("GL_MAX_COLOR_ATTACHMENTS  < 8, NOT support!!!");
            return false;
        }

        GLint maxDrawBuf = 0;
        QOpenGLContext::currentContext()->functions()->glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuf);
        qInfo() << "GL_MAX_DRAW_BUFFERS = " << maxDrawBuf;
        if (maxDrawBuf < 8)
        {
            qWarning("GL_MAX_DRAW_BUFFERS  < 8, NOT support!!!");
            return false;
        }

        return true;
    }
}
