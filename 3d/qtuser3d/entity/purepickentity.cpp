#include "purepickentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include <QDepthTest>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QRenderStateSet>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QCameraSelector>

namespace qtuser_3d
{
	PurePickEntity::PurePickEntity(Qt3DCore::QNode* parent)
		:PickEntity(parent)
	{
		//setEffect(EFFECT("pure.view_pickFace.pick"));
		setEffect(EFFECT("xyzIndicator.xyzAxis_pickFace.pick"));
		m_colorParameter = createParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));

	}

	PurePickEntity::~PurePickEntity()
	{
	}

	void PurePickEntity::setColor(const QVector4D& color)
	{
		m_color = color;
		updateColor();
	}

	void PurePickEntity::setHoverColor(const QVector4D& color)
	{
		m_hoverColor = color;
		updateColor();
	}

	void PurePickEntity::updateColor()
	{
		QVector4D color = m_color;
		if (m_pickable && m_pickable->state() == ControlState::hover)
			color = m_hoverColor;

		m_colorParameter->setValue(color);
	}

	void PurePickEntity::onStateChanged(ControlState state)
	{
		updateColor();
	}
}
