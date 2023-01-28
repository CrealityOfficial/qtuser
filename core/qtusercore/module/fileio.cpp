#include "fileio.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>

namespace qtuser_core
{
	void writeTextFile(const QString& fileName, const QString& content)
	{
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&file);
			stream << content;
		}
		file.close();
	}

	void writeTextFile(const QString& fileName, const QByteArray& content)
	{
		QString str(content);
		writeTextFile(fileName, str);
	}
}
