/**
 * @brief Common fragment object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_FRAGMENT_H
#define _O3DS_COMMON_FRAGMENT_H

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
class Hub;
class MasterScene;

/**
 * @brief The Fragment base class
 */
class O3S_API Fragment : public Entity
{
    Q_DECLARE_TR_FUNCTIONS(Fragment)

public:

    Fragment(const QString &name, Entity *parent = nullptr);
    virtual ~Fragment();

    void setProject(Project *project);

    virtual Project* project() override;
    virtual const Project* project() const override;

    MasterScene* masterScene();
    const MasterScene* masterScene() const;

    void setupMasterScene();

    virtual void create() override;

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;

    void linkToHub(Hub *hub);

    Hub* hub();
    const Hub* hub() const;

    virtual bool serializeContent(QDataStream &stream) const;
    virtual bool deserializeContent(QDataStream &stream);

private:

    ObjectRef m_hubRef;     //! Related hub ref
    Hub *m_hub;             //! Related hub

    MasterScene *m_masterScene;
};

/**
 * @brief The FragmentException class
 */
class FragmentException : public BaseException
{
public:

    FragmentException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_FRAGMENT_H
