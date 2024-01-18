/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_QT_BGFX_VIEWER_MAIN_WINDOW_H
#define VCL_EXT_QT_BGFX_VIEWER_MAIN_WINDOW_H

#include <QMainWindow>

#include <vclib/render/drawable_object_vector.h>

#include "minimal_viewer_widget.h"

namespace vcl::qbgf {

namespace Ui {
class ViewerMainWindow;
} // namespace Ui

class ViewerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewerMainWindow(
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count,
        QWidget*                 parent     = nullptr);

    explicit ViewerMainWindow(QWidget* parent);

    ~ViewerMainWindow();

    void setDrawableObjectVector(std::shared_ptr<vcl::DrawableObjectVector> v);

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

public slots:
    void visibilityDrawableObjectChanged();
    void selectedDrawableObjectChanged(uint i);
    void renderSettingsUpdated();

private:
    MinimalViewerWidget*                       viewer = nullptr;
    Ui::ViewerMainWindow*                      ui;
    std::shared_ptr<vcl::DrawableObjectVector> drawVector;
};

} // namespace vcl::qbgf

#endif // VCL_EXT_QT_BGFX_VIEWER_MAIN_WINDOW_H
