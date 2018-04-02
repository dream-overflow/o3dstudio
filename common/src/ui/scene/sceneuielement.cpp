/**
 * @brief Common scene UI element base class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-30
 * @details
 */

#include "o3d/studio/common/ui/scene/sceneuielement.h"

using namespace o3d::studio::common;

SceneUIElement::SceneUIElement(BaseObject *parent,
                               SceneUIElement::UIType uiType,
                               DrawStep drawStep,
                               Bool directDraw) :
    BaseObject(parent),
    m_uiType(uiType),
    m_drawStep(drawStep),
    m_directDraw(directDraw),
    m_focus(False)
{

}

SceneUIElement::~SceneUIElement()
{

}

void SceneUIElement::setup(MasterScene *)
{
    // nothing to do
}

void SceneUIElement::release(MasterScene *)
{
    // nothing to do
}

void SceneUIElement::setFocus(o3d::Bool focus)
{
    m_focus = focus;
}

o3d::Bool SceneUIElement::hasFocus() const
{
    return m_focus;
}

void SceneUIElement::enter()
{
    // nothing to do
}

void SceneUIElement::hover(o3d::UInt32, const o3d::Point3f&)
{
    // nothing to do
}

void SceneUIElement::leave()
{
    // nothing to do
}

o3d::Bool SceneUIElement::isDirectDraw() const
{
    return m_directDraw;
}

SceneUIElement::DrawStep SceneUIElement::drawStep() const
{
    return m_drawStep;
}

o3d::Bool SceneUIElement::mousePressEvent(const MouseEvent &, MasterScene *)
{
    return False;
}

o3d::Bool SceneUIElement::mouseReleaseEvent(const MouseEvent &, MasterScene *)
{
    return False;
}

o3d::Bool SceneUIElement::mouseDoubleClickEvent(const MouseEvent &, MasterScene *)
{
    return False;
}

o3d::Bool SceneUIElement::mouseMoveEvent(const MouseEvent &, MasterScene *)
{
    return False;
}

o3d::Bool SceneUIElement::wheelEvent(const WheelEvent &, MasterScene *)
{
    return False;
}

o3d::Bool SceneUIElement::keyPressEvent(const KeyEvent &, MasterScene *)
{
    return False;
}

o3d::Bool SceneUIElement::keyReleaseEvent(const KeyEvent &, MasterScene *)
{
    return False;
}
