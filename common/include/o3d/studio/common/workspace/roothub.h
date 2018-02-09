/**
 * @brief Common root hub object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-08
 * @details
 */

#ifndef _O3DS_COMMON_ROOTHUB_H
#define _O3DS_COMMON_ROOTHUB_H

#include "hub.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The root hub final class
 */
class O3S_API RootHub : public Hub
{
    Q_DECLARE_TR_FUNCTIONS(RootHub)

public:

    explicit RootHub(const String &name, Project *parent);
    virtual ~RootHub();

    void setProject(Project *project);

    virtual void create() override;
    virtual Bool deletable() const override;

    virtual Bool serializeContent(OutStream &stream) const override;
    virtual Bool deserializeContent(InStream &stream) override;

    //
    // engine proxy
    //

    /**
     * @brief Does nothing virtual root.
     */
    virtual void createToScene(MasterScene *masterScene) override;

    /**
     * @brief Remove children recursively.
     */
    virtual void removeFromScene(MasterScene *masterScene) override;

    /**
     * @brief syncWithScene Does nothing, virtual root.
     */
    virtual void syncWithScene(MasterScene *masterScene) override;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ROOTHUB_H
