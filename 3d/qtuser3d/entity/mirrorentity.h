#pragma once

#include <memory>

#include <QNode>
#include <QPointer>
#include <QGeometry>
#include <QTransform>

#include "qtuser3d/camera/cameracontroller.h"
#include "qtuser3d/refactor/xentity.h"
#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/module/manipulatepickable.h"
#include "qtuser3d/math/box3d.h"
#include "qtuser3d/qtuser3dexport.h"

namespace qtuser_3d {

class QTUSER_3D_API MirrorEntity : public XEntity {
  Q_OBJECT;

public:
  explicit MirrorEntity(QPointer<CameraController> camera_controller = nullptr,
                        Qt3DCore::QNode* parent = nullptr);
  virtual ~MirrorEntity() = default;

public:
  QPointer<CameraController> getCameraController() const;
  void setCameraController(QPointer<CameraController> camera_controller);

public:
  Box3D getSpaceBox() const;
  void setSpaceBox(const Box3D& space_box);

public:
  float getScaleFactor() const;
  void setScaleFactor(float scale_factor);

public:
  std::weak_ptr<Pickable> xPositivePickable() const;
  std::weak_ptr<Pickable> xNegativePickable() const;

  std::weak_ptr<Pickable> yPositivePickable() const;
  std::weak_ptr<Pickable> yNegativePickable() const;

  std::weak_ptr<Pickable> zPositivePickable() const;
  std::weak_ptr<Pickable> zNegativePickable() const;

public:
  void attach();
  void detach();

private:
  Q_SLOT void onCameraChanged(QVector3D position, QVector3D up_vector);

private:
  QPointer<CameraController> camera_controller_;
  Box3D space_box_;

  std::unique_ptr<Qt3DCore::QTransform> transform_;
  std::unique_ptr<Qt3DRender::QGeometry> geometry_;
  
  std::unique_ptr<ManipulateEntity> x_positive_entity_;
  std::unique_ptr<ManipulateEntity> x_negative_entity_;
  std::unique_ptr<ManipulateEntity> y_positive_entity_;
  std::unique_ptr<ManipulateEntity> y_negative_entity_;
  std::unique_ptr<ManipulateEntity> z_positive_entity_;
  std::unique_ptr<ManipulateEntity> z_negative_entity_;

  std::shared_ptr<ManipulatePickable> x_positive_pickable_;
  std::shared_ptr<ManipulatePickable> x_negative_pickable_;
  std::shared_ptr<ManipulatePickable> y_positive_pickable_;
  std::shared_ptr<ManipulatePickable> y_negative_pickable_;
  std::shared_ptr<ManipulatePickable> z_positive_pickable_;
  std::shared_ptr<ManipulatePickable> z_negative_pickable_;
};

}  // namespace qtuser_3d
