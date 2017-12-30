/**
 * @brief Common builder base class.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#ifndef _O3DS_COMMON_BUILDER_H
#define _O3DS_COMMON_BUILDER_H

#include <o3d/core/string.h>
#include <o3d/core/stringlist.h>
#include <o3d/core/serialize.h>

#include "../global.h"
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Entity;

/**
 * @brief Builder options base class. Import can be fully or partial, necessiting somes
 * specifics convertions and much more. Then this is the base class to extend to manage
 * specifics options, and it must be serializable.
 */
class O3S_API BuilderOption : public o3d::Serialize
{
    O3D_DEFINE_SERIALIZATION(BuilderOption)

public:

    BuilderOption();

    virtual ~BuilderOption();

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
class O3S_API Builder
{
public:

    /**
     * @brief Default constructor.
     */
    Builder();

    virtual ~Builder();

    void setTypeRef(const TypeRef &typeRef);
    const TypeRef& typeRef() const;

    /**
     * @brief Unique importer type string name.
     */
    const String& name() const;

    /**
     * @brief Build the importer options related to this importer. Filled with initial values.
     * @return A valid builder options.
     */
    virtual BuilderOption* buildOptions() = 0;

    /**
     * @brief Build to a specified path.
     * @param path Destination path (relative to the project).
     * @return True if success.
     */
    virtual Bool build(const String &path) = 0;

    //
    // ui part
    //

    // @todo

protected:

    TypeRef m_typeRef;
    String m_name;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_BUILDER_H
