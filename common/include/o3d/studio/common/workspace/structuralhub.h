/**
 * @brief Common structural hub abstract class.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-25
 * @details
 */

#ifndef _O3DS_COMMON_STRUCTURALHUB_H
#define _O3DS_COMMON_STRUCTURALHUB_H

#include "hub.h"

#include <o3d/core/vector3.h>
#include <o3d/core/quaternion.h>

#include <map>

namespace o3d {

class Node;
class SceneObject;
class Transform;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;
class Vector3Property;

/**
 * @brief The StructuralHub class
 */
class O3S_API StructuralHub : public Hub
{
public:

    enum NodePolicy
    {
        POLICY_DEFAULT = 0
    };

    enum TransformType
    {
        MATRIX_TRANSFORM = 0,       //!< Common SRT transform
        FPS_TRANSFORM,              //!< First person view transform
        DUAL_QUATERNION_TRANSFORM   //!< Dual quaternion based transform
    };

    explicit StructuralHub(const String &name, Entity *parent = nullptr);
    virtual ~StructuralHub();

    virtual Role role() const override;

    //
    // Transformations
    //

    // assume one transform for the moment
//    void setPosition(UInt32 transformIndex, const o3d::Vector3f &pos);
//    void setRotation(UInt32 transformIndex, const o3d::Vector3f &rot);
//    void setRotation(UInt32 transformIndex, const o3d::Quaternion &rot);
//    void setScale(UInt32 transformIndex, const o3d::Vector3f &scale);

//    UInt32 getNumTransforms() const;

    virtual const Transform* transform(UInt32 transformIndex) const = 0;

//    virtual const o3d::Matrix4& absoluteMatrix(MasterScene *masterScene) const override;

//    virtual Bool isSpacialNode() const override;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_STRUCTURALHUB_H
