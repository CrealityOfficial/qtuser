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
		: PickableEntity(parent)
        , m_cameraController(nullptr)
	{
        m_pickable = new IndicatorPickable(this);
        m_pickable->setPickableEntity(this);

        setupGeometry();
        setupTexture();

		qtuser_3d::UEffect* effect = qobject_cast<qtuser_3d::UEffect*>(EFFECTCREATE("sceneindicator.view_pickindicator.pick", this));     
		setEffect(effect);
   
        {            
            QMatrix4x4 projection;
            projection.perspective(60.0f, 1920.0 / 1080.0, 1.0, 1000.0);
            setParameter("projectionMatrix", QVariant(projection));
        }

        setScreenPos(0.5f, 0.85f);
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

        float k = 0.2f;

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

        Qt3DRender::QAttribute* position = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, vertSize, 0, stride);
        Qt3DRender::QAttribute* normal = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultNormalAttributeName(), Qt3DRender::QAttribute::Float, 3, vertSize, 3*sizeof(float), stride);
        Qt3DRender::QAttribute* texCoord = new Qt3DRender::QAttribute(qBuffer, Qt3DRender::QAttribute::defaultTextureCoordinateAttributeName(), Qt3DRender::QAttribute::Float, 2, vertSize, 6 * sizeof(float), stride);
        Qt3DRender::QAttribute* dirId = new Qt3DRender::QAttribute(qBuffer, "facesIndex", Qt3DRender::QAttribute::Float, 1, vertSize, 8 * sizeof(float), stride);

        Qt3DRender::QGeometry* geo = GeometryCreateHelper::create(nullptr, position, normal, texCoord, dirId);

        setGeometry(geo);
	}

    void WorldIndicatorEntity::setupTexture()
    {
        Qt3DRender::QTexture2D *t = createFromSource(QUrl("qrc:/UI/images/scene_all_dir.png"));
        setParameter("colorMap", QVariant::fromValue(t));
    }

    Pickable* WorldIndicatorEntity::pickable() const
    {
        return m_pickable;
    }

    void WorldIndicatorEntity::setHighlightDirections(int dirs)
    {
        setParameter("highlightFaces", QVariant::fromValue(dirs));
    }

    void WorldIndicatorEntity::setSelectedDirections(int dirs)
    {
        adaptCamera(dirs);
        onCameraChanged(m_cameraController->screenCamera());
    }

    void WorldIndicatorEntity::setScreenPos(float x, float y)
    {
        float w = 0.1f;
        float h = 0.1f;
        setViewport(x, y, w, h);
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
        }

        m_cameraController = cc;

        if (m_cameraController)
        {
            qtuser_3d::ScreenCamera* sc = m_cameraController->screenCamera();
            sc->addCameraObserver(this);
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
            m_cameraController->viewFromBottom();
        }
        else if (viewDir == -up) {
            m_cameraController->viewFromTop();
        }
        else {
            QVector3D right = QVector3D::crossProduct(viewDir, up);
            m_cameraController->view(viewDir, right);
        }
    }

    void WorldIndicatorEntity::adaptLocal(int dirs)
    {
        int  ifront = 1 << 0;
        int  iback = 1 << 1;
        int  ileft = 1 << 2;
        int  iright = 1 << 3;
        int  ibottom = 1 << 4;
        int  itop = 1 << 5;

        QVector3D start = QVector3D(0.0, 0.0, 1.0);
        QVector3D dir = QVector3D(0.0, 0.0, 0.0);

        if (dirs & ifront) {
            dir += QVector3D(0.0, 0.0, 1.0);
        }
        if (dirs & iback) {
            dir += QVector3D(0.0, 0.0, -1.0);
        }
        if (dirs & ileft) {
            dir += QVector3D(-1.0, 0.0, 0.0);
        }
        if (dirs & iright) {
            dir += QVector3D(1.0, 0.0, 0.0);
        }
        if (dirs & ibottom) {
            dir += QVector3D(0.0, -1.0, 0.0);
        }
        if (dirs & itop) {
            dir += QVector3D(0.0, 1.0, 0.0);
        }

        QMatrix4x4 model;
        model.setToIdentity();

        QVector3D end = dir;

        if (start == end) {

            model.setToIdentity();

        } if (end == QVector3D(0.0, 1.0, 0.0)) {

            model.rotate(90.0f, QVector3D(1.0, 0.0, 0.0));
        }
        else if (end == QVector3D(0.0, -1.0, 0.0)) {

            model.rotate(-90.0f, QVector3D(1.0, 0.0, 0.0));
        }
        else {

            end.normalize();

            end.setZ(-end.z());

            float theta = atan(end.z() / end.x());  // (-PI/2 ~ PI/2)

            if (end.x() < 0.0) {
                theta += M_PI;
            }

            theta += M_PI_2;
            float angle = qRadiansToDegrees(theta);

            model.rotate(qRadiansToDegrees(-theta), QVector3D(0.0, 1.0, 0.0));

            if (dirs & ibottom) {
                QMatrix4x4 t;
                t.rotate(-45.0f, QVector3D(1.0, 0.0, 0.0));
                model = t * model;
            }
            else if (dirs & itop) {
                QMatrix4x4 t;
                t.rotate(45.0f, QVector3D(1.0, 0.0, 0.0));
                model = t * model;
            }
        }

        setParameter("modelMatrix", QVariant(model));
    }

    void WorldIndicatorEntity::onCameraChanged(ScreenCamera* sc)
    {
        Qt3DRender::QCamera* camera = sc->camera();

        QVector3D viewDir = camera->viewVector();
        QVector3D newPosition = camera->viewCenter() + viewDir.normalized() * -2.0;

        QMatrix4x4 view;
        view.lookAt(newPosition, camera->viewCenter(), camera->upVector());
        setParameter("viewMatrix", QVariant(view));

        QMatrix4x4 model;
        model.translate(camera->viewCenter());
        model.rotate(90.0f, QVector3D(1.0, 0.0, 0.0));
        setParameter("modelMatrix", model);
        
    }
}
