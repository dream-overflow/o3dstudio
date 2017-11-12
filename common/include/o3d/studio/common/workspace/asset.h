/**
 * @brief Common asset object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
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

    Asset(const QString &name, Entity *parent = nullptr);
    virtual ~Asset();

    void setProject(Project *project);

    Project* project();
    const Project* project() const;

    virtual void create() override;

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;

    virtual bool serializeContent(QDataStream &stream) const;
    virtual bool deserializeContent(QDataStream &stream);

private:

    // @todo
};

/**
 * @brief The FragmentException class
 */
class AssetException : public BaseException
{
public:

    AssetException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ASSET_H
