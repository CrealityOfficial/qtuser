#include "mirrorentity.h"

namespace qtuser_3d {

MirrorEntity::MirrorEntity(Qt3DCore::QNode* parent,
                           MirrorHandler x_mirror_handler,
                           MirrorHandler y_mirror_handler,
                           MirrorHandler z_mirror_handler)
    : BasicEntity(parent)
    , x_mirror_handler_(x_mirror_handler)
    , y_mirror_handler_(y_mirror_handler)
    , z_mirror_handler_(z_mirror_handler) {

}

MirrorEntity::~MirrorEntity() {}

auto MirrorEntity::getXMirrorHandler() const -> MirrorHandler {
  return x_mirror_handler_;
}

void MirrorEntity::setXMirrorHandler(MirrorHandler x_mirror_handler) {
  x_mirror_handler_ = x_mirror_handler;
}

auto MirrorEntity::getYMirrorHandler() const -> MirrorHandler {
  return y_mirror_handler_;
}

void MirrorEntity::setYMirrorHandler(MirrorHandler y_mirror_handler) {
  y_mirror_handler_ = y_mirror_handler;
}

auto MirrorEntity::getZMirrorHandler() const -> MirrorHandler {
  return z_mirror_handler_;
}

void MirrorEntity::setZMirrorHandler(MirrorHandler z_mirror_handler) {
  z_mirror_handler_ = z_mirror_handler;
}

}  // namespace qtuser_3d
