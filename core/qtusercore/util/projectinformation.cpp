#include "projectinformation.h"

namespace qtuser_core
{
	ProjectInformation::ProjectInformation()
	{

	}

	ProjectInformation::~ProjectInformation()
	{

	}

	const Version& ProjectInformation::version() const
	{
		return m_version;
	}

	void ProjectInformation::setVersion(const Version& version)
	{
		m_version = version;
	}

	QString ProjectInformation::name()
	{
		return m_projectName;
	}

	void ProjectInformation::setName(const QString& name)
	{
		m_projectName = name;
	}
}