#ifndef QTUSER_3D_MODELNENTITY_1595161543232_H
#define QTUSER_3D_MODELNENTITY_1595161543232_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"
#include <QtGui/QVector2D>

namespace qtuser_3d
{
	class LineEntity;
	class BoxEntity;
	class QTUSER_3D_API ModelNEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		ModelNEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~ModelNEntity();

		void setBoxVisibility(bool visible);
		void updateBoxLocal(Box3D& box, QMatrix4x4& parentMatrix);
		void setBoxColor(QVector4D color);
		void updateConvex(QVector<QVector3D>& points, QMatrix4x4& matrix, bool loop);

		void setState(float state);
		void setErrorState(bool error);
		void setVertexBase(QPoint vertexBase);
		void setSupportCos(float supportCos);
		void enterSupportStatus();
		void leaveSupportStatus();
		void setWaterPoint(QVector3D pt, float zcha);
		void unSetWaterPoint();
		void setFanZhuan(int fz);
		
		// �Զ�����ɫ���� state ֵ���� 5 ʱ��Ч
		void setCustomColor(QColor color);
		QColor getCustomColor();

		// ͸����
		void setTransparency(float alpha);
		float getTransparency();

		// �Ƿ���ӹ���Ч��
		void setLightingEnable(bool enable);
		bool getLightingEnable();

		void setNeedCheckScope(int checkscope);

		void setNozzle(float nozzle);

		//������
		void setNormaldir(int normaldir);
	protected:
		qtuser_3d::BoxEntity* m_boxEntity;
		qtuser_3d::LineEntity* m_convexXYEntity;

		Qt3DRender::QParameter* m_stateParameter;
		Qt3DRender::QParameter* m_vertexBaseParameter;
		Qt3DRender::QParameter* m_errorParameter;

		Qt3DRender::QParameter* m_customColorParameter;
		Qt3DRender::QParameter* m_transparencyParameter;
		Qt3DRender::QParameter* m_lightingFlagParameter;

		Qt3DRender::QParameter* m_supportCosParameter;
		Qt3DRender::QParameter* m_hoverParameter;
		Qt3DRender::QParameter* m_waterShowParameter;
		Qt3DRender::QParameter* m_waterParameter;
		Qt3DRender::QParameter* m_zchaParameter;
		Qt3DRender::QParameter* m_fanzhuanParameter;
		Qt3DRender::QParameter* m_checkScopeParameter;

		Qt3DRender::QParameter* m_nozzleParameter;

		Qt3DRender::QParameter* m_normaldirParameter;
	};
}
#endif // QTUSER_3D_MODELNENTITY_1595161543232_H