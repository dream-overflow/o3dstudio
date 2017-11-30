/**
 * @brief Common scene UI element base class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-30
 * @details
 */

#include "o3d/studio/common/ui/scene/sceneuielement.h"

using namespace o3d::studio::common;

SceneUIElement::SceneUIElement(SceneUIElement::UIType uiType, Bool directDraw) :
    m_uiType(uiType),
    m_directDraw(directDraw)
{

}

SceneUIElement::~SceneUIElement()
{

}

o3d::Bool SceneUIElement::isDirectDraw() const
{
    return m_directDraw;
}
