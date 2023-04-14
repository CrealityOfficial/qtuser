#pragma once

#include <functional>

#include <Qt3DCore/QNode>

#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/entity/basicentity.h"

namespace qtuser_3d {

class QTUSER_3D_API MirrorEntity : public BasicEntity {
  Q_OBJECT;

public:
  using MirrorHandler = std::function<void(void)>;

public:
  explicit MirrorEntity(Qt3DCore::QNode* parent = nullptr,
                        MirrorHandler x_mirror_handler = nullptr,
                        MirrorHandler y_mirror_handler = nullptr,
                        MirrorHandler z_mirror_handler = nullptr);
  virtual ~MirrorEntity();

public:
  auto getXMirrorHandler() const -> MirrorHandler;
  void setXMirrorHandler(MirrorHandler x_mirror_handler);

public:
  auto getYMirrorHandler() const -> MirrorHandler;
  void setYMirrorHandler(MirrorHandler y_mirror_handler);

public:
  auto getZMirrorHandler() const -> MirrorHandler;
  void setZMirrorHandler(MirrorHandler z_mirror_handler);

private:
  MirrorHandler x_mirror_handler_;
  MirrorHandler y_mirror_handler_;
  MirrorHandler z_mirror_handler_;

  
};

}  // namespace qtuser_3d
