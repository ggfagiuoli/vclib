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

#include <vclib/qt/viewer_widget.h>

#include <QMouseEvent>

#include <vclib/qt/gui/screen_shot_dialog.h>
#include <vclib/qt/input.h>

namespace vcl::qt {

ViewerWidget::ViewerWidget(
    const std::shared_ptr<DrawableObjectVector>& v,
    uint                                         width,
    uint                                         height,
    const std::string&                           windowTitle,
    QWidget*                                     parent) :
        EventManagerWidget(windowTitle, width, height, parent),
        ViewerCanvas((void*) winId(), v, width, height, displayId())
{
}

ViewerWidget::ViewerWidget(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWidget*           parent) :
        ViewerWidget(
            std::make_shared<DrawableObjectVector>(),
            width,
            height,
            windowTitle,
            parent)
{
}

ViewerWidget::ViewerWidget(QWidget* parent) :
        ViewerWidget("Viewer", 1024, 768, parent)
{
}

#if defined(VCLIB_RENDER_BACKEND_BGFX)
void ViewerWidget::update()
{
    // frame();
    EventManagerWidget::update();
}
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
void ViewerWidget::initializeGL()
{
    ViewerCanvas::init(width(), height());
}
#endif

void ViewerWidget::onKeyPress(Key::Enum key)
{
    switch (key) {
    case Key::S:
        if (modifiers()[KeyModifier::CONTROL]) {
            showScreenShotDialog();
        }
        break;

    default: ViewerCanvas::onKeyPress(key); break;
    }
}

#if defined(VCLIB_RENDER_BACKEND_BGFX)
bool ViewerWidget::event(QEvent* event)
{
    return EventManagerWidget::event(event);
}
#endif

#if defined(VCLIB_RENDER_BACKEND_BGFX)
void ViewerWidget::paintEvent(QPaintEvent* event)
{
    frame();
#if defined(__APPLE__) || defined(__linux__)
    bgfx::frame(); // needed on unix systems
#endif             // __APPLE__ || __linux__
    QWidget::paintEvent(event);

    if (mDepthReadRequested) {
        update();
    }
}
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
void ViewerWidget::paintGL()
{
    frame();
}
#endif

void ViewerWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (mDepthReadRequested)
        return;

    mDepthReadRequested = true;
    const auto p = event->pos() * pixelRatio();

#if defined(VCLIB_RENDER_BACKEND_BGFX)
    const bool homogeneousNDC = Context::capabilites().homogeneousDepth;
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    const bool homogeneousNDC = true;
#endif

    // create callback
    const auto proj = projectionMatrix();
    const auto view = viewMatrix();
    std::array<float,4> vp = {
        .0f,
        .0f,
        float(width() * pixelRatio()),
        float(height() * pixelRatio())};
    auto callback = [=, this](float depth) {
        mDepthReadRequested = false;


        // TODO: debug remove
        std::cout << "Depth at point ("
                << p.x() << ", " << p.y() << "): " << depth 
                << std::endl;

        if (depth == 1.0f) {
            return;
        }

        // unproject the point
        const Point3f p2d(p.x(), vp[3] - p.y(), depth);
        auto unproj = unproject(
            p2d, Matrix44<ScalarType>(proj*view), vp, homogeneousNDC);
        
        // TODO: debug remove
        // print the unprojected point
        std::cout << "Unprojected point: " << unproj << std::endl;

        this->focus(unproj);
        this->update();
    };
    
    this->readDepth(Point2i(p.x(), p.y()), callback);
}

void ViewerWidget::showScreenShotDialog()
{
    qt::ScreenShotDialog* dialog = new qt::ScreenShotDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        auto fs = dialog->selectedFiles();
        ViewerCanvas::screenShot(fs.first().toStdString());
    }
    // the dialog stealed the focus, so we need to release the modifiers
    ViewerCanvas::setKeyModifiers({KeyModifier::NO_MODIFIER});
    setModifiers({KeyModifier::NO_MODIFIER});
}

} // namespace vcl::qt
