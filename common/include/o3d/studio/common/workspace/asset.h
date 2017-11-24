/**
 * @brief Common asset object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-12
 * @details
 */

#ifndef _O3DS_COMMON_ASSET_H
#define _O3DS_COMMON_ASSET_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

#include "../exception.h"
#include "../objectref.h"

#include "entity.h"

namespace o3d {
namespace studio {
namespace common {

class Project;

/**
 * @brief The Asset base class
 */
class O3S_API Asset : public Entity
{
    Q_DECLARE_TR_FUNCTIONS(Asset)

public:

    Asset(const String &name, Entity *parent = nullptr);
    virtual ~Asset();

    void setProject(Project *project);

    virtual Project* project() override;
    virtual const Project* project() const override;

    virtual void create() override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    virtual Bool serializeContent(OutStream &stream) const;
    virtual Bool deserializeContent(InStream &stream);

private:

    // @todo
};

/**
 * @brief The AssetException class
 */
class E_AssetException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_AssetException)

    //! Ctor
    E_AssetException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_AssetException, "Objective-3D Studio asset exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ASSET_H
