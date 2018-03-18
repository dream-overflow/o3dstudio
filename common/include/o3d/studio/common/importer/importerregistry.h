/**
 * @brief Common importer registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#ifndef _O3DS_COMMON_IMPORTERREGISTRY_H
#define _O3DS_COMMON_IMPORTERREGISTRY_H

#include <QtCore/QCoreApplication>

#include <map>

#include <o3d/core/baseobject.h>

#include "../exception.h"
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Importer;

class O3S_API ImporterRegistry : public BaseObject
{
    Q_DECLARE_TR_FUNCTIONS(ImporterRegistry)

public:

    ImporterRegistry(BaseObject *parent = nullptr);
    virtual ~ImporterRegistry();

    /**
     * @brief Register the default importers.
     */
    void initialize();

    /**
     * @brief Register an importer, that comes from a plugin or from the common.
     * @param importer Valid importer and must have a unique signature.
     */
    void registerImporter(Importer *importer);

    Bool unregisterImporter(const String &name);
    Bool unregisterImporter(UInt64 id);
    Bool unregisterImporter(const TypeRef &ref);

    Importer* importer(const String &name);
    const Importer* importer(const String &name) const;

    Importer* importer(const TypeRef &ref);
    const Importer* importer(const TypeRef &ref) const;

    std::list<const Importer*> importerList() const;

    /**
     * @brief Build the list of supported files extensions from registered importers.
     */
    String supportedExts() const;

public /*signals*/:

    Signal<String> onImporterRegistered{this};
    Signal<String> onImporterUnregistered{this};

protected:

    std::map<String, Importer*> m_importers;
    std::map<UInt64, Importer*> m_importersById;
};

/**
 * @brief The ImporterException class
 */
class E_ImporterException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_ImporterException)

    //! Ctor
    E_ImporterException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_ImporterException, "Objective-3D Studio importer exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_IMPORTERREGISTRY_H
