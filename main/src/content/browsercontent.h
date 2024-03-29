/**
 * @brief Application main browser content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_MAIN_BROWSERCONTENT_H
#define _O3DS_MAIN_BROWSERCONTENT_H

#include <QtWidgets/QTextBrowser>
#include "o3d/studio/common/ui/content.h"

namespace o3d {
namespace studio {
namespace main {

class BrowserContent : public QTextBrowser, public common::Content
{
    Q_OBJECT

public:

    BrowserContent(QWidget *parent = nullptr);
    virtual ~BrowserContent();

    virtual QWidget* ui() override;
    virtual String elementName() const override;

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

signals:

protected:

private slots:

private:

    // void setup();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MAINTOOLBAR_H
