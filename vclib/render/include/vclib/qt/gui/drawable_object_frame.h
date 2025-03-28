/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_QT_GUI_DRAWABLE_OBJECT_FRAME_H
#define VCL_QT_GUI_DRAWABLE_OBJECT_FRAME_H

#include <vclib/render/drawable/drawable_object.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class DrawableObjectFrame;
} // namespace Ui

class DrawableObjectFrame : public QFrame
{
    Q_OBJECT

    Ui::DrawableObjectFrame*        mUI;
    std::shared_ptr<DrawableObject> mObj;

public:
    explicit DrawableObjectFrame(
        const std::shared_ptr<DrawableObject>& obj,
        QWidget*                               parent = nullptr);
    ~DrawableObjectFrame();

    void setIcon(const QIcon& icon, const QString& tooltip = "");

signals:
    void visibilityChanged();

    void resized();

private slots:
    void visibilityCheckBoxStateChanged(Qt::CheckState arg1);

    void showInfoToolButtonChecked(bool checked);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_DRAWABLE_OBJECT_FRAME_H
