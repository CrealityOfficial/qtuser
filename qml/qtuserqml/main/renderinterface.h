#ifndef CREATIVE_KERNEL_RENDERINTERFACE_1594447238062_H
#define CREATIVE_KERNEL_RENDERINTERFACE_1594447238062_H
#include "qtuser3d/framegraph/rendergraph.h"

void registerResidentNode(Qt3DCore::QNode* node);
void unRegisterResidentNode(Qt3DCore::QNode* node);
void renderRenderGraph(qtuser_3d::RenderGraph* graph);
void registerRenderGraph(qtuser_3d::RenderGraph* graph);
void renderOneFrame();

void setContinousRender();
void setCommandRender();

#endif // CREATIVE_KERNEL_RENDERINTERFACE_1594447238062_H