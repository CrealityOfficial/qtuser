#include "qtuser3d/utils/idgenerator.h"

namespace qtuser_3d
{
	IDGenerator IDGenerator::m_idGenerator;
	IDGenerator::IDGenerator(QObject* parent)
		:QObject(parent)
	{
		m_ids.resize(256, -1);
		//m_ids.at(255) = 0;
	}

	IDGenerator::~IDGenerator()
	{

	}

	unsigned IDGenerator::GetID()
	{
		unsigned id = 0xffffffff;
		for (unsigned i = 0; i <= 255; ++i)
		{
			if (m_ids.at(i) < 0)
			{
				id = i;
				m_ids.at(i) = 0;
				break;
			}
		}
		return id;
	}

	void IDGenerator::ReleaseID(unsigned id)
	{
		if (id >= 0 && id <= 255)
			m_ids.at(id) = -1;
	}

	IDGenerator& IDGenerator::Instance()
	{
		return m_idGenerator;
	}
}
