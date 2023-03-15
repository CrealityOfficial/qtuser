#include "worldindicatorentity.h"
#include "qtuser3d/geometry/basicshapecreatehelper.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/geometrycreatehelper.h"
#include "qtuser3d/effect/texturecreator.h"
#include "qtuser3d/camera/cameracontroller.h"

#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>

#include <QtCore/qmath.h>

namespace qtuser_3d {

	WorldIndicatorEntity::WorldIndicatorEntity(Qt3DCore::QNode* parent)
		: PickableEntity(parent),
        m_cameraController(nullptr),
        m_animation(nullptr),
        m_lambda(0.0f),
        m_theme(-1)
	{
        m_pickable = new IndicatorPickable(this);
        m_pickable->setPickableEntity(this);

        setupGeometry();

		qtuser_3d::UEffect* effect = qobject_cast<qtuser_3d::UEffect*>(EFFECTCREATE("sceneindicator.view_pickindicator.pick", this));     
        effect->setPassCullFace("pickindicator.pick", Qt3DRender::QCullFace::Back);
        effect->setPassDepthTest("pickindicator.pick", Qt3DRender::QDepthTest::Less);
		setEffect(effect);

        //setScreenPos(QPoint(50, 50));
	}

	WorldIndicatorEntity::~WorldIndicatorEntity()
	{
        if (m_cameraController)
        {
            m_cameraController->screenCamera()->removeCameraObserver(this);
        }
	}

	void WorldIndicatorEntity::setupGeometry()
	{
        int  ifront = 1 << 0;
        int  iback = 1 << 1;
        int  ileft = 1 << 2;
        int  iright = 1 << 3;
        int  ibottom = 1 << 4;
        int  itop = 1 << 5;

        float front = ifront;
        float back = iback;
        float left = ileft;
        float right = iright;
        float bottom = ibottom;
        float top = itop;

        float k = 0.15f;

        float uvstep = 1.0f / 6.0f;

        float vertices[] = {

            // positions        // normals             // texture coords  //face index

            //front
            -0.5f,  -0.5f,   0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + left,
            -0.5f + k,-0.5f,   0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + left,
            -0.5f + k,-0.5f + k, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + left,
            -0.5f + k,-0.5f + k, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + left,
            -0.5f,  -0.5f + k, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + left,
            -0.5f,  -0.5f,   0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + left,

            -0.5f + k, -0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom,
             0.5f - k, -0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom,
             0.5f - k,  -0.5f + k, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom,
             0.5f - k,  -0.5f + k, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom,
            -0.5f + k,  -0.5f + k, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom,
            -0.5f + k, -0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom,


            0.5f - k,  -0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + right,
            0.5f,    -0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + right,
            0.5f,    -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + right,
            0.5f,    -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + right,
            0.5f - k,  -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + right,
            0.5f - k,  -0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + bottom + right,

            -0.5f,   -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + left,
            -0.5f + k, -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + left,
            -0.5f + k,  0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + left,
            -0.5f + k,  0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + left,
            -0.5f,    0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + left,
            -0.5f,   -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + left,

            -0.5f + k, -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front,
            0.5f - k,  -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  uvstep,  0.0f,    front,
            0.5f - k,  0.5f - k,   0.5f,  0.0f,  0.0f,  1.0f,  uvstep,  1.0f,    front,
            0.5f - k,  0.5f - k,   0.5f,  0.0f,  0.0f,  1.0f,  uvstep,  1.0f,    front,
            -0.5f + k, 0.5f - k,   0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,    front,
            -0.5f + k, -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front,

            0.5f - k, -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + right,
            0.5f,   -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + right,
            0.5f,    0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + right,
            0.5f,    0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + right,
            0.5f - k,  0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + right,
            0.5f - k, -0.5f + k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    front + right,



            -0.5f,    0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + left,
            -0.5f + k,  0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + left,
            -0.5f + k,  0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + left,
            -0.5f + k,  0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + left,
            -0.5f,    0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + left,
            -0.5f,    0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + left,

            -0.5f + k,  0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front,
             0.5f - k,  0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front,
             0.5f - k,  0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front,
             0.5f - k,  0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front,
            -0.5f + k,  0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front,
            -0.5f + k,  0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front,


            0.5f - k, 0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + right,
             0.5f,  0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + right,
             0.5f,  0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + right,
             0.5f,  0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + right,
            0.5f - k, 0.5f,    0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + right,
            0.5f - k, 0.5f - k,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    top + front + right,


            //back
            -0.5f,  -0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + bottom + left,
            -0.5f + k,-0.5f + k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  back + bottom + left,
            -0.5f + k,-0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom + left,
            -0.5f + k,-0.5f + k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  back + bottom + left,
            -0.5f,  -0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + bottom + left,
            -0.5f,  -0.5f + k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom + left,

            -0.5f + k, -0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + bottom,
             0.5f - k, -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom,
             0.5f - k, -0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + bottom,
             0.5f - k, -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom,
            -0.5f + k, -0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + bottom,
            -0.5f + k, -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom,


            0.5f - k, -0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom + right,
             0.5f,  -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom + right,
             0.5f,  -0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + bottom + right,
             0.5f,  -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom + right,
            0.5f - k, -0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + bottom + right,
            0.5f - k, -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  back + bottom + right,

            -0.5f,   -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,     back + left,
            -0.5f + k,  0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,   back + left,
            -0.5f + k, -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,   back + left,
            -0.5f + k,  0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,   back + left,
            -0.5f,   -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,     back + left,
            -0.5f,    0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,     back + left,

            -0.5f + k, -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  uvstep * 2,  0.0f,    back,
            0.5f - k,  0.5f - k,   -0.5f,  0.0f,  0.0f, -1.0f,  uvstep,  1.0f,    back,
            0.5f - k,  -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  uvstep,  0.0f,    back,
            0.5f - k,  0.5f - k,   -0.5f,  0.0f,  0.0f, -1.0f,  uvstep,  1.0f,    back,
            -0.5f + k, -0.5f + k,  -0.5f,  0.0f,  0.0f, -1.0f,  uvstep * 2,  0.0f,    back,
            -0.5f + k, 0.5f - k,   -0.5f,  0.0f,  0.0f, -1.0f,  uvstep * 2,  1.0f,    back,

            0.5f - k, -0.5f + k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + right,
            0.5f,   0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + right,
            0.5f,   -0.5f + k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + right,
            0.5f,   0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + right,
            0.5f - k, -0.5f + k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + right,
            0.5f - k, 0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + right,



            -0.5f,   0.5f - k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top + left,
            -0.5f + k, 0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top + left,
            -0.5f + k, 0.5f - k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  back + top + left,
            -0.5f + k, 0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top + left,
            -0.5f,   0.5f - k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top + left,
            -0.5f,   0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + top + left,

            -0.5f + k,  0.5f - k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  back + top,
             0.5f - k,  0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top,
             0.5f - k,  0.5f - k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  back + top,
             0.5f - k,  0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top,
            -0.5f + k,  0.5f - k, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  back + top,
            -0.5f + k,  0.5f,   -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top,

            0.5f - k, 0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top + right,
            0.5f,   0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,        back + top + right,
            0.5f,   0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + top + right,
            0.5f,   0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,        back + top + right,
            0.5f - k, 0.5f - k,  -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,    back + top + right,
            0.5f - k, 0.5f,    -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,      back + top + right,


            //left
            -0.5f,  -0.5f + k,  -0.5f + k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom + back,
            -0.5f,  -0.5f + k,  -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + bottom + back,
            -0.5f,  -0.5f,    -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,        left + bottom + back,
            -0.5f,  -0.5f,    -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,        left + bottom + back,
            -0.5f,  -0.5f,    -0.5f + k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + bottom + back,
            -0.5f,  -0.5f + k,  -0.5f + k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom + back,

            -0.5f,  -0.5f + k, 0.5f - k,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom,
            -0.5f,  -0.5f + k, -0.5f + k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom,
            -0.5f,  -0.5f,   -0.5f + k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + bottom,
            -0.5f,  -0.5f,   -0.5f + k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + bottom,
            -0.5f,  -0.5f,   0.5f - k,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + bottom,
            -0.5f,  -0.5f + k, 0.5f - k,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom,

            -0.5f,  -0.5f + k, 0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom + front,
            -0.5f,  -0.5f + k, 0.5f - k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  left + bottom + front,
            -0.5f,  -0.5f,   0.5f - k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom + front,
            -0.5f,  -0.5f,   0.5f - k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom + front,
            -0.5f,  -0.5f,   0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + bottom + front,
            -0.5f,  -0.5f + k, 0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + bottom + front,


            -0.5f,  0.5f - k,  -0.5f + k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + back,
            -0.5f,  0.5f - k, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + back,
            -0.5f, -0.5f + k, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + back,
            -0.5f, -0.5f + k, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + back,
            -0.5f, -0.5f + k,  -0.5f + k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + back,
            -0.5f,  0.5f - k,  -0.5f + k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + back,


            -0.5f,  0.5f - k,  0.5f - k, -1.0f,  0.0f,  0.0f,  uvstep * 3,  1.0f,    left,
            -0.5f,  0.5f - k, -0.5f + k, -1.0f,  0.0f,  0.0f,  uvstep * 2,  1.0f,    left,
            -0.5f, -0.5f + k, -0.5f + k, -1.0f,  0.0f,  0.0f,  uvstep * 2,  0.0f,    left,
            -0.5f, -0.5f + k, -0.5f + k, -1.0f,  0.0f,  0.0f,  uvstep * 2,  0.0f,    left,
            -0.5f, -0.5f + k,  0.5f - k, -1.0f,  0.0f,  0.0f,  uvstep * 3,  0.0f,    left,
            -0.5f,  0.5f - k,  0.5f - k, -1.0f,  0.0f,  0.0f,  uvstep * 3,  1.0f,    left,


            -0.5f,  0.5f - k,  0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + front,
            -0.5f,  0.5f - k,  0.5f - k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + front,
            -0.5f, -0.5f + k,  0.5f - k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + front,
            -0.5f, -0.5f + k,  0.5f - k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + front,
            -0.5f, -0.5f + k,  0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + front,
            -0.5f,  0.5f - k,  0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + front,

            -0.5f,  0.5f,  -0.5f + k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top + back,
            -0.5f,  0.5f,  -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + top + back,
            -0.5f, 0.5f - k, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top + back,
            -0.5f, 0.5f - k, -0.5f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top + back,
            -0.5f, 0.5f - k, -0.5f + k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  left + top + back,
            -0.5f,  0.5f,  -0.5f + k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top + back,

            -0.5f,  0.5f,  0.5f - k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top,
            -0.5f,  0.5f, -0.5f + k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top,
            -0.5f, 0.5f - k, -0.5f + k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  left + top,
            -0.5f, 0.5f - k, -0.5f + k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  left + top,
            -0.5f, 0.5f - k,  0.5f - k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  left + top,
            -0.5f,  0.5f,  0.5f - k,  -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top,

            -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + top + front,
            -0.5f,  0.5f,  0.5f - k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top + front,
            -0.5f, 0.5f - k, 0.5f - k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  left + top + front,
            -0.5f, 0.5f - k, 0.5f - k, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  left + top + front,
            -0.5f, 0.5f - k, 0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    left + top + front,
            -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      left + top + front,


            //right
            0.5f,  -0.5f + k, 0.5f,        1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom + front,
            0.5f,  -0.5f,     0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom + front,
            0.5f,  -0.5f + k, 0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom + front,
            0.5f,  -0.5f,     0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom + front,
            0.5f,  -0.5f + k, 0.5f,        1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom + front,
            0.5f,  -0.5f,     0.5f,        1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom + front,


            0.5f,  -0.5f + k, 0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom,
            0.5f,  -0.5f,   -0.5f + k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + bottom,
            0.5f,  -0.5f + k, -0.5f + k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom,
            0.5f,  -0.5f,   -0.5f + k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + bottom,
            0.5f,  -0.5f + k, 0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom,
            0.5f,  -0.5f,   0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + bottom,

            0.5f,  -0.5f + k,  -0.5f + k, 1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + bottom + back,
            0.5f,  -0.5f,    -0.5f,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + bottom + back,
            0.5f,  -0.5f + k,  -0.5f,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom + back,
            0.5f,  -0.5f,    -0.5f,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + bottom + back,
            0.5f,  -0.5f + k,  -0.5f + k, 1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + bottom + back,
            0.5f,  -0.5f,    -0.5f + k, 1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + bottom + back,

            0.5f,  0.5f - k,  0.5f,     1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + front,
            0.5f, -0.5f + k,  0.5f - k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + front,
            0.5f,  0.5f - k,  0.5f - k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + front,
            0.5f, -0.5f + k,  0.5f - k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + front,
            0.5f,  0.5f - k,  0.5f,     1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + front,
            0.5f, -0.5f + k,  0.5f,     1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + front,


            0.5f,  0.5f - k,  0.5f - k,   1.0f,  0.0f,  0.0f,  uvstep * 3,  1.0f,    right,
            0.5f, -0.5f + k, -0.5f + k,   1.0f,  0.0f,  0.0f,  uvstep * 4,  0.0f,    right,
            0.5f, 0.5f - k, -0.5f + k,    1.0f, 0.0f,    0.0f, uvstep * 4,  1.0f,    right,
            0.5f, -0.5f + k, -0.5f + k,   1.0f,  0.0f,  0.0f,  uvstep * 4,  0.0f,    right,
            0.5f,  0.5f - k,  0.5f - k,   1.0f,  0.0f,  0.0f,  uvstep * 3,  1.0f,    right,
            0.5f, -0.5f + k, 0.5f - k,    1.0f, 0.0f,    0.0f, uvstep * 3,  0.0f,    right,

            0.5f,  0.5f - k,  -0.5f + k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + back,
            0.5f, -0.5f + k, -0.5f,      1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + back,
            0.5f,  0.5f - k, -0.5f,      1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + back,
            0.5f, -0.5f + k, -0.5f,      1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + back,
            0.5f,  0.5f - k,  -0.5f + k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + back,
            0.5f, -0.5f + k,  -0.5f + k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + back,

            0.5f,  0.5f,  0.5f,      1.0f,  0.0f,  0.0f,  0.0f,  0.0f,       right + top + front,
            0.5f, 0.5f - k, 0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,   right + top + front,
            0.5f,  0.5f,  0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,     right + top + front,
            0.5f, 0.5f - k, 0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,   right + top + front,
            0.5f,  0.5f,  0.5f,      1.0f,  0.0f,  0.0f,  0.0f,  0.0f,       right + top + front,
            0.5f, 0.5f - k, 0.5f,      1.0f,  0.0f,  0.0f,  0.0f,  0.0f,     right + top + front,


            0.5f,  0.5f,  0.5f - k,     1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + top,
            0.5f, 0.5f - k, -0.5f + k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + top,
            0.5f,  0.5f, -0.5f + k,     1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + top,
            0.5f, 0.5f - k, -0.5f + k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + top,
            0.5f,  0.5f,  0.5f - k,     1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + top,
            0.5f, 0.5f - k,  0.5f - k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + top,

            0.5f,  0.5f,  -0.5f + k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + top + back,
            0.5f, 0.5f - k, -0.5f,      1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + top + back,
            0.5f,  0.5f, -0.5f,       1.0f,  0.0f,  0.0f,  0.0f,  0.0f,      right + top + back,
            0.5f, 0.5f - k, -0.5f,      1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + top + back,
            0.5f,  0.5f,  -0.5f + k,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,    right + top + back,
            0.5f, 0.5f - k,  -0.5f + k,   1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  right + top + back,

            //bottom
            -0.5f,   -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + back,
            -0.5f + k, -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + back,
            -0.5f + k, -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + back,
            -0.5f + k, -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + back,
            -0.5f,   -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + back,
            -0.5f,   -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + back,

            -0.5f + k, -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back,
            0.5f - k,  -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back,
            0.5f - k,  -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back,
            0.5f - k,  -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back,
            -0.5f + k, -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back,
            -0.5f + k, -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back,

            0.5f - k, -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back + right,
            0.5f,   -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back + right,
            0.5f,   -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back + right,
            0.5f,   -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back + right,
            0.5f - k, -0.5f,  -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back + right,
            0.5f - k, -0.5f,  -0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + back + right,

            -0.5f, -0.5f,   -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left,
            -0.5f + k, -0.5f, -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left,
            -0.5f + k, -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left,
            -0.5f + k, -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left,
            -0.5f, -0.5f,    0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left,
            -0.5f, -0.5f,   -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left,

            -0.5f + k, -0.5f,  -0.5f + k, 0.0f, -1.0f,  0.0f,  uvstep * 4,  0.0f,    bottom,
            0.5f - k,  -0.5f,  -0.5f + k, 0.0f, -1.0f,  0.0f,  uvstep * 5,  0.0f,    bottom,
            0.5f - k,  -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  uvstep * 5,  1.0f,    bottom,
            0.5f - k,  -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  uvstep * 5,  1.0f,    bottom,
            -0.5f + k, -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  uvstep * 4,  1.0f,    bottom,
            -0.5f + k, -0.5f,  -0.5f + k, 0.0f, -1.0f,  0.0f,  uvstep * 4,  0.0f,    bottom,

            0.5f - k, -0.5f, -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + right,
            0.5f,   -0.5f, -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + right,
            0.5f,   -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + right,
            0.5f,   -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + right,
            0.5f - k, -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + right,
            0.5f - k, -0.5f, -0.5f + k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + right,


            -0.5f,   -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + front,
            -0.5f + k, -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + front,
            -0.5f + k, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + front,
            -0.5f + k, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + front,
            -0.5f,   -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + front,
            -0.5f,   -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + left + front,


            -0.5f + k, -0.5f,  0.5f - k, 0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front,
            0.5f - k,  -0.5f,  0.5f - k, 0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front,
            0.5f - k,  -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front,
            0.5f - k,  -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front,
            -0.5f + k, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front,
            -0.5f + k, -0.5f,  0.5f - k, 0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front,

            0.5f - k, -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front + right,
            0.5f,   -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front + right,
            0.5f,   -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front + right,
            0.5f,   -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front + right,
            0.5f - k, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front + right,
            0.5f - k, -0.5f,  0.5f - k,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,    bottom + front + right,


            //top
            -0.5f,    0.5f,  0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + front,
            -0.5f + k,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + front,
            -0.5f + k,  0.5f,  0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + front,
            -0.5f + k,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + front,
            -0.5f,    0.5f,  0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + front,
            -0.5f,    0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + front,

            -0.5f + k, 0.5f,   0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front,
            0.5f - k,  0.5f,   0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front,
            0.5f - k,  0.5f,   0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front,
            0.5f - k,  0.5f,   0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front,
            -0.5f + k, 0.5f,   0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front,
            -0.5f + k, 0.5f,   0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front,

            0.5f - k, 0.5f,  0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front + right,
            0.5f,   0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front + right,
            0.5f,   0.5f,  0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front + right,
            0.5f,   0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front + right,
            0.5f - k, 0.5f,  0.5f - k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front + right,
            0.5f - k, 0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + front + right,


            -0.5f,   0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left,
            -0.5f + k, 0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left,
            -0.5f + k, 0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left,
            -0.5f + k, 0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left,
            -0.5f,   0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left,
            -0.5f,   0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left,

            -0.5f + k, 0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  uvstep * 5,  1.0f,    top,
            0.5f - k,  0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,    top,
            0.5f - k,  0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,    top,
            0.5f - k,  0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,    top,
            -0.5f + k, 0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  uvstep * 5,  1.0f,    top,
            -0.5f + k, 0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  uvstep * 5,  0.0f,    top,

            0.5f - k, 0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + right,
            0.5f,   0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + right,
            0.5f,   0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + right,
            0.5f,   0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + right,
            0.5f - k, 0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + right,
            0.5f - k, 0.5f,  0.5f - k,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + right,

            -0.5f,    0.5f,  -0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + back,
            -0.5f + k,  0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + back,
            -0.5f + k,  0.5f,  -0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + back,
            -0.5f + k,  0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + back,
            -0.5f,    0.5f,  -0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + back,
            -0.5f,    0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + left + back,

            -0.5f + k, 0.5f,  -0.5f,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back,
            0.5f - k,  0.5f,  -0.5f + k, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back,
            0.5f - k,  0.5f,  -0.5f,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back,
            0.5f - k,  0.5f,  -0.5f + k, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back,
            -0.5f + k, 0.5f,  -0.5f,   0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back,
            -0.5f + k, 0.5f,  -0.5f + k, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back,

            0.5f - k, 0.5f,  -0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back + right,
            0.5f,   0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back + right,
            0.5f,   0.5f,  -0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back + right,
            0.5f,   0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back + right,
            0.5f - k, 0.5f,  -0.5f,    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back + right,
            0.5f - k, 0.5f,  -0.5f + k,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,    top + back + right,
        };

        int vertSize = sizeof(vertices) / sizeof(float) / 9;
        int stride = 9 * sizeof(float);
        QByteArray* bytes = new QByteArray((const char*)&vertices[0], sizeof(vertices));

        Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
        qBuffer->setData(*bytes);
        delete bytes;

        Qt3DRender::QAttribute* position = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, vertSize, 0, stride);
        Qt3DRender::QAttribute* normal = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultNormalAttributeName(), Qt3DRender::QAttribute::Float, 3, vertSize, 3*sizeof(float), stride);
        Qt3DRender::QAttribute* texCoord = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultTextureCoordinateAttributeName(), Qt3DRender::QAttribute::Float, 2, vertSize, 6 * sizeof(float), stride);
        Qt3DRender::QAttribute* dirId = new Qt3DRender::QAttribute(qBuffer, "facesIndex", Qt3DRender::QAttribute::Float, 1, vertSize, 8 * sizeof(float), stride);

        Qt3DRender::QGeometry* geo = GeometryCreateHelper::create(nullptr, position, normal, texCoord, dirId);

        setGeometry(geo);
	}

    Pickable* WorldIndicatorEntity::pickable() const
    {
        return m_pickable;
    }

    void WorldIndicatorEntity::setHighlightDirections(int dirs)
    {
        setParameter("highlightFaces", float(dirs));
    }

    void WorldIndicatorEntity::setSelectedDirections(int dirs)
    {
        adaptCamera(dirs);
    }

    void WorldIndicatorEntity::setScreenPos(const QPoint& p)
    {
        m_showOnPoint = p;

        if (m_cameraController)
        {
            QSize size = m_cameraController->screenCamera()->size();
            float x = float(p.x()) / size.width();
            float y = float(p.y()) / size.height();

            float pixNum = 100.0;
            float w = pixNum / size.width();
            float h = pixNum / size.height();
            setViewport(x, y, w, h);
        }   
    }

    void WorldIndicatorEntity::resetCameraAngle() {
      constexpr int TOP{ 1 << 5 };
      constexpr int FRONT{ 1 << 0 };
      constexpr int DEFAUT_ANGLE{ TOP + FRONT };
      adaptCamera(DEFAUT_ANGLE);
    }

    void WorldIndicatorEntity::setViewport(float x, float y, float w, float h)
    {
        QMatrix4x4 viewportMatrix;
        QMatrix4x4 m1, m2, m3;
        m1.translate(1.0f, 1.0f, 0.0f);
        m2.scale(w, h, 1.0f);
        m3.translate(2.0f * x - 1.0f, 2.0f * y - 1.0f);
        viewportMatrix = m3 * m2 * m1;
        setParameter("viewportMatrix", viewportMatrix);
    }

    void WorldIndicatorEntity::setCameraController(CameraController* cc)
    {
        if (m_cameraController)
        {
            m_cameraController->screenCamera()->removeCameraObserver(this);
            disconnect(m_cameraController->screenCamera()->camera(), SIGNAL(aspectRatioChanged(float)), this, SLOT(aspectRatioChanged(float)));
        }

        m_cameraController = cc;

        if (m_cameraController)
        {
            qtuser_3d::ScreenCamera* sc = m_cameraController->screenCamera();
            sc->addCameraObserver(this);
            connect(sc->camera(), SIGNAL(aspectRatioChanged(float)), this, SLOT(aspectRatioChanged(float)));
            onCameraChanged(sc);
        }
    }

    void WorldIndicatorEntity::adaptCamera(int dirs)
    {
        if (!m_cameraController)
            return;

        int  ifront = 1 << 0;
        int  iback = 1 << 1;
        int  ileft = 1 << 2;
        int  iright = 1 << 3;
        int  ibottom = 1 << 4;
        int  itop = 1 << 5;

        QVector3D vec(0.0f, 0.0f, 0.0f);

        if (dirs & ifront)
        {
            qDebug() << "front";
            vec += QVector3D(0.0f, -1.0f, 0.0f);
        }

        if (dirs & iback)
        {
            qDebug() << "back";
            vec += QVector3D(0.0f, 1.0f, 0.0f);
        }

        if (dirs & ileft)
        {
            qDebug() << "left";
            vec += QVector3D(-1.0f, 0.0f, 0.0f);
        }

        if (dirs & iright)
        {
            qDebug() << "right";
            vec += QVector3D(1.0f, 0.0f, 0.0f);
        }

        if (dirs & ibottom)
        {
            qDebug() << "bottom";
            vec += QVector3D(0.0f, 0.0f, -1.0f);
        }

        if (dirs & itop)
        {
            qDebug() << "top";
            vec += QVector3D(0.0f, 0.0f, 1.0f);
        }

        QVector3D viewDir = -vec;
        QVector3D up = QVector3D(0.0f, 0.0f, 1.0f);

        if (viewDir == up)
        {
            //m_cameraController->viewFromBottom();
            viewDir = QVector3D(0.0f, 0.0f, 1.0f);
            QVector3D right(1.0f, 0.0f, 0.0f);
            up = QVector3D::crossProduct(right, viewDir).normalized();
        }
        else if (viewDir == -up) {
            //m_cameraController->viewFromTop();
            viewDir = QVector3D(0.0f, 0.0f, -1.0f);
            QVector3D right(1.0f, 0.0f, 0.0f);
            up = QVector3D::crossProduct(right, viewDir).normalized();
        }
        else {
            /*QVector3D right = QVector3D::crossProduct(viewDir, up);
            m_cameraController->view(viewDir, right);*/
        }

        qtuser_3d::ScreenCamera* sc = m_cameraController->screenCamera();
        Qt3DRender::QCamera* camera = sc->camera();
        m_startDir = camera->viewVector();
        m_startUp = camera->upVector();

        m_endDir = viewDir;
        m_endUp = up;

        if (m_animation == nullptr)
        {
            QPropertyAnimation* animation = new QPropertyAnimation(this);
            animation->setTargetObject(this);
            animation->setEasingCurve(QEasingCurve::InOutQuad);
            animation->setPropertyName("lambda");
            animation->setStartValue(QVariant::fromValue(0.0f));
            animation->setEndValue(QVariant::fromValue(1.0f));
            animation->setDuration(500);
            animation->setLoopCount(1);
            
            m_animation.reset(animation);
        }
        m_animation->start();
    }

    void WorldIndicatorEntity::onCameraChanged(ScreenCamera* sc)
    {
        Qt3DRender::QCamera* camera = sc->camera();

        QVector3D viewDir = camera->viewVector();
        QVector3D newPosition = camera->viewCenter() + viewDir.normalized() * -2.0;

        QMatrix4x4 model;
        model.translate(camera->viewCenter());
        model.rotate(90.0f, QVector3D(1.0, 0.0, 0.0));
        setParameter("modelMatrix", model);

        QMatrix4x4 view;
        view.lookAt(newPosition, camera->viewCenter(), camera->upVector());
        setParameter("viewMatrix", QVariant(view));

        QMatrix4x4 projection;
        projection.perspective(60.0f, 1.0, 1.0, 1000.0);
        setParameter("projectionMatrix", QVariant(projection));
        
        setScreenPos(m_showOnPoint);
    }

    void WorldIndicatorEntity::setLambda(float lambda)
    {
        m_lambda = lambda;
        qDebug() << "setLambda:" << lambda;

        if (!m_cameraController)
            return;

        QVector3D up(0.0f, 0.0f, 1.0f);
        QQuaternion q1up = QQuaternion::rotationTo(up, m_startUp);
        QQuaternion q2up = QQuaternion::rotationTo(up, m_endUp);
        QQuaternion qup = QQuaternion::slerp(q1up, q2up, lambda);
        QVector3D newUp = qup * up;


        QVector3D dir(0.0f, 1.0f, 0.0f);
        QQuaternion q1dir = QQuaternion::rotationTo(dir, m_startDir);
        QQuaternion q2dir = QQuaternion::rotationTo(dir, m_endDir);
        QQuaternion qdir = QQuaternion::slerp(q1dir, q2dir, lambda);
        QVector3D newDir = qdir * dir;

        QVector3D right = QVector3D::crossProduct(newDir, newUp);
        m_cameraController->view(newDir, right);

        onCameraChanged(m_cameraController->screenCamera());
    }

    float WorldIndicatorEntity::lambda() const
    {
        return m_lambda;
    }

    void WorldIndicatorEntity::aspectRatioChanged(float aspectRatio)
    {
        onCameraChanged(m_cameraController->screenCamera());
    }

    /*void WorldIndicatorEntity::setupBasicTexture(const QUrl& url)
    {
        Qt3DRender::QTexture2D* t = createFromSource(url);
        setParameter("colorMap", QVariant::fromValue(t));
    }*/

    void WorldIndicatorEntity::setTheme(int theme)
    {
        m_theme = theme;
        updateBasicTexture();
    }

    void WorldIndicatorEntity::setupLightTexture(const QUrl& url)
    {
        m_lightTextureUrl = url;
        updateBasicTexture();
    }

    void WorldIndicatorEntity::setupDarkTexture(const QUrl& url)
    {
        m_darkTextureUrl = url;
        updateBasicTexture();
    }

    void WorldIndicatorEntity::setupSelectTexture(const QUrl& url)
    {
        Qt3DRender::QTexture2D* t = createFromSource(url);
        setParameter("selectMap", QVariant::fromValue(t));
    }

    void WorldIndicatorEntity::updateBasicTexture()
    {
        if (m_theme == -1) return;

        switch (m_theme)
        {
        case 0:
        {
            if (m_darkTextureUrl.isEmpty() == false)
            {
                Qt3DRender::QTexture2D* t = createFromSource(m_darkTextureUrl);
                setParameter("colorMap", QVariant::fromValue(t));
            }
 
        }
            break;

        case 1:
        {
            if (m_lightTextureUrl.isEmpty() == false)
            {
                Qt3DRender::QTexture2D* t = createFromSource(m_lightTextureUrl);
                setParameter("colorMap", QVariant::fromValue(t));
            }
        }
            break;

        default:
            break;
        }

    }
}
