#ifndef _QTUSER_3D_IDGENERATOR_1588930164944_H
#define _QTUSER_3D_IDGENERATOR_1588930164944_H
#include "qtuser3d/qtuser3dexport.h"
namespace qtuser_3d
{
	class QTUSER_3D_API IDGenerator: public QObject
	{
		IDGenerator(QObject* parent = nullptr);
	public:
		virtual ~IDGenerator();

		static IDGenerator& Instance();

		unsigned GetID();  //0xffffffff invalid id
		void ReleaseID(unsigned id);
	private:
		std::vector<int> m_ids;   //-1 unused, 0 used

		static IDGenerator m_idGenerator;
	};
}
#define GenID() qtuser_3d::IDGenerator::Instance().GetID()
#define DelID(x) qtuser_3d::IDGenerator::Instance().ReleaseID(x)

#define INVALID_ID (unsigned)(-1)
#endif // _QTUSER_3D_IDGENERATOR_1588930164944_H
