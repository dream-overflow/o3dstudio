/**
 * @brief Default master scene drawer.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-02
 * @copyright Copyright (c) 2001-2017 Dream Overflow. All rights reserved.
 * @details
 */

#include "o3d/studio/common/workspace/masterscenedrawer.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/commontype.h"

#include "o3d/core/architecture.h"
#include "o3d/engine/context.h"
#include "o3d/engine/glextensionmanager.h"
#include "o3d/engine/scene/sceneobjectmanager.h"
#include "o3d/engine/object/camera.h"
#include "o3d/engine/object/light.h"
#include "o3d/engine/matrix.h"
#include "o3d/engine/renderer.h"
#include "o3d/engine/viewport.h"
#include "o3d/geom/frustum.h"
#include "o3d/engine/effect/specialeffectsmanager.h"
#include "o3d/engine/visibility/visibilitymanager.h"
#include "o3d/engine/landscape/landscape.h"
#include "o3d/engine/alphapipeline.h"
#include "o3d/engine/scene/scene.h"

#include <algorithm>

using namespace o3d::studio::common;

O3D_IMPLEMENT_DYNAMIC_CLASS1(MasterSceneDrawer, MASTER_SCENE_DRAWER, ShadowVolumeForward)

MasterSceneDrawer::MasterSceneDrawer(BaseObject *parent, MasterScene *masterScene) :
    ShadowVolumeForward(parent),
    m_masterScene(masterScene)
{
}

void MasterSceneDrawer::draw(ViewPort *)
{
    // The camera modelview should be set before draw()
    if (getScene()->getActiveCamera() == nullptr) {
        return;
    }

    Context &context = *getScene()->getContext();
    Camera &camera = *getScene()->getActiveCamera();

    // Computes frustum just after camera put
    getScene()->getFrustum()->computeFrustum(context.projection().get(), camera.getModelviewMatrix());

    // Determines visible objects
    getScene()->getVisibilityManager()->processVisibility();

    //
    // Draw first pass with no lighting, in camera space
    //

    // use MSAA if MS renderer
    if (getScene()->getRenderer()->getSamples() > 1) {
        context.setAntiAliasing(Context::AA_MULTI_SAMPLE);
    }

    // draw the first pass with ambient
    DrawInfo drawInfo(DrawInfo::AMBIENT_PASS);
    drawInfo.setFromCamera(&camera);

    context.disableStencilTest();
    context.setDefaultDepthFunc();

    // pre draw
    for (auto it = m_preDraw.begin(); it != m_preDraw.end(); ++it) {
        (*it)->directRendering(drawInfo, m_masterScene);
    }

    m_masterScene->updateCounters(0);

    // landscape
    getScene()->getLandscape()->draw();

    // world objects
    getScene()->getVisibilityManager()->draw(drawInfo);

    // special effects
    getScene()->getSpecialEffectsManager()->draw(drawInfo);

    context.disableDepthWrite();
        getScene()->getAlphaPipeline()->sort();
        getScene()->getAlphaPipeline()->draw(drawInfo);
    context.setDefaultDepthWrite();

    m_masterScene->updateCounters(1);

    //
    // Process the effectives lights
    //

//	 TODO a manager to get visible objects for any active light
//    for each light
//      for each visible mesh
//        if light volume intersects mesh
//          render

    const String lights[] = { "light1", "light2", "light3", "light4" };

    for (UInt32 i = 0; i < 4; ++i) {
        Light *light = dynamicCast<Light*>(getScene()->getSceneObjectManager()->searchName(lights[i]));
        processLight(light);
    }

    context.disableStencilTest();
    context.setDefaultStencilTestFunc();
    context.setDefaultDepthTest();
    context.setDefaultDepthWrite();
    context.setDefaultDepthFunc();

    //
    // Draw the next pass with post effects
    //

    drawInfo.pass = DrawInfo::AMBIENT_PASS;
    drawInfo.light.type = 0;

    // post effects
    //getScene()->getSpecialEffectsManager()->drawPost(drawInfo);

    m_masterScene->updateCounters(2);

    // post draw
    for (auto it = m_postDraw.begin(); it != m_postDraw.end(); ++it) {
        (*it)->directRendering(drawInfo, m_masterScene);
    }

    m_masterScene->updateCounters(3);

    context.setAntiAliasing(Context::AA_NONE);

    // camera clear
    camera.clearCameraChanged();
}

void MasterSceneDrawer::drawPicking(ViewPort *)
{
    // The camera modelview should be set before draw()
    if (getScene()->getActiveCamera() == nullptr) {
        return;
    }

    Camera &camera = *getScene()->getActiveCamera();

    DrawInfo drawInfo(DrawInfo::PICKING_PASS);
    drawInfo.setFromCamera(&camera);

    // pre draw
    for (auto it = m_preDraw.begin(); it != m_preDraw.end(); ++it) {
        (*it)->directRendering(drawInfo, m_masterScene);
    }

    // world objects
    getScene()->getVisibilityManager()->draw(drawInfo);

    // post draw
    for (auto it = m_postDraw.begin(); it != m_postDraw.end(); ++it) {
        (*it)->directRendering(drawInfo, m_masterScene);
    }

    // TODO
    //getScene()->getContext()->disableDepthWrite();
    //getScene()->getAlphaPipeline()->sort();
    //getScene()->getAlphaPipeline()->draw(drawInfo);
    //getScene()->getContext()->setDefaultDepthWrite();

    // camera clear
    camera.clearCameraChanged();
}

void MasterSceneDrawer::addSceneUIElement(SceneUIElement *elt)
{
    if (!elt) {
        return;
    }

    if (elt->drawStep() == SceneUIElement::PRE_DRAW) {
        m_preDraw.push_back(elt);
    } else if (elt->drawStep() == SceneUIElement::POST_DRAW) {
        m_postDraw.push_back(elt);
    }
}

void MasterSceneDrawer::removeSceneUIElement(SceneUIElement *elt)
{
    if (!elt) {
        return;
    }

    if (elt->drawStep() == SceneUIElement::PRE_DRAW) {
        auto it = std::find(m_preDraw.begin(), m_preDraw.end(), elt);
        if (it != m_preDraw.end()) {
            m_preDraw.erase(it);
        }
    } else if (elt->drawStep() == SceneUIElement::POST_DRAW) {
        auto it = std::find(m_postDraw.begin(), m_postDraw.end(), elt);
        if (it != m_postDraw.end()) {
            m_postDraw.erase(it);
        }
    }
}
