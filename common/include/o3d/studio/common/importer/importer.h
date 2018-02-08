/**
 * @brief Common importer base class.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#ifndef _O3DS_COMMON_IMPORTER_H
#define _O3DS_COMMON_IMPORTER_H

#include <o3d/core/templatebitset.h>
#include <o3d/core/string.h>
#include <o3d/core/stringlist.h>
#include <o3d/core/serialize.h>

#include "../global.h"
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Hub;
class ImportDefinition;

/**
 * @brief Importer options base class. Import can be fully or partial, necessiting somes
 * specifics convertions and much more. Then this is the base class to extend to manage
 * specifics options, and it must be serializable.
 */
class O3S_API ImporterOption : public o3d::Serialize
{
    O3D_DEFINE_SERIALIZATION(ImporterOption)

public:

    ImporterOption();

    virtual ~ImporterOption();

    //! Serialize the object from a file.
    virtual Bool readFromFile(InStream &istream) override;
    //! Serialize the object to a file.
    virtual Bool writeToFile(OutStream &ostream) override;

protected:

    // initially nothing
};

/**
 * @brief Base class for an content data importer. Generally an importer if offered by
 * a particular plugin dedicated to the importation of external content.
 */
class O3S_API Importer
{
public:

    /**
     * @brief Imported features from an instance of importer.
     */
    enum ImporterFeature
    {
        IMPORTER_GEOMETRY = 0,   //!< Static or dynamic geometry
        IMPORTER_MATERIAL,       //!< Material definition
        IMPORTER_SKELETON,       //!< Skeleton for animated objetc
        IMPORTER_NODE,           //!< Node or more exactly sub-scene graph
        IMPORTER_ANIMATION,      //!< Animation data for any animatble object
        IMPORTER_TEXTURE,        //!< Static or dynamic texture resource
        IMPORTER_AUDIO           //!< Static or dynamic audio resource
    };

    /**
     * @brief Default constructor.
     */
    Importer();

    virtual ~Importer();

    void setTypeRef(const TypeRef &typeRef);
    const TypeRef& typeRef() const;

    /**
     * @brief Unique importer type string name.
     */
    const String& name() const;

    /**
     * @brief Build the importer options related to this importer. Filled with initial values.
     * @return A valid importer options.
     */
    virtual ImporterOption* buildOptions() = 0;

    /**
     * @brief Introspect the content file. Does not proceed to importation of datas
     * @param filename File name of the content to introspect.
     * @return ImportDefinition object if success, null else.
     */
    virtual ImportDefinition* introspect(const String &filename) = 0;

    /**
     * @brief Import from a specified file name the content into a parent hub
     * @param filename File name of the content to import.
     * @param options Option (filters, conversion) during importation.
     * @param parent Parent must be the project, or the parent hub.
     * @return ImportDefinition object if success, null else.
     */
    virtual ImportDefinition* import(
            const String &filename,
            ImporterOption *options,
            Hub *parent) = 0;

    /**
     * @brief Array of capacities of the importer (not what is imported but what is importable).
     */
    const BitSet32& capacities() const;

    /**
     * @brief Is a features provided by this importer (not what is imported but what is importable).
     */
    Bool getCapacity(ImporterFeature feature) const;

    //
    // ui part
    //

    // @todo

protected:

    TypeRef m_typeRef;
    String m_name;

    BitSet32 m_capacities;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_IMPORTER_H
