#ifndef QTUSER_3D_BOXCLIPPERENTITY_1652409537539_H
#define QTUSER_3D_BOXCLIPPERENTITY_1652409537539_H
#include "qtuser3d/entity/basicentity.h"

namespace qtuser_3d
{
	class QTUSER_3D_API BoxClipperEntity : public qtuser_3d::BasicEntity
	{
	public:
		BoxClipperEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~BoxClipperEntity();

		void updateClipperBox(const QVector3D& cmin, const QVector3D& cmax);
	protected:
		Qt3DRender::QParameter* m_boxClipParameter;
	};
}

#endif // QTUSER_3D_BOXCLIPPERENTITY_1652409537539_H