/**
 * @brief Common hub object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_HUB_H
#define _O3DS_COMMON_HUB_H

#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

#include "../exception.h"
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Project;

/**
 * @brief The Hub base class
 */
class Hub
{
    Q_DECLARE_TR_FUNCTIONS(Hub)

public:

    Hub(const QString &name, Project *project = nullptr);
    virtual ~Hub();

    void setProject(Project *project);

    Project* project();
    const Project* project() const;

    void create();

    void setRef(const ObjectRef &ref);

    const ObjectRef& ref() const;
    const QString& name() const;

    bool load();
    bool save();

    bool exists() const;
    bool hasChanges();

private:

    Project *m_project;        //!< Project where the hub is currently instancied

    QString m_name;            //!< Project display name
    ObjectRef m_ref;           //!< Unique projet identifier
};

/**
 * @brief The HubException class
 */
class HubException : public BaseException
{
public:

    HubException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_HUB_H
