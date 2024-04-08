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

#include "ui_drawable_object_vector_frame.h"
#include <vclib/ext/qt/gui/drawable_object_vector_frame.h>

#include <QStandardItemModel>

namespace vcl::qt {

DrawableObjectVectorFrame::DrawableObjectVectorFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::DrawableObjectVectorFrame)
{
    mUI->setupUi(this);
}

DrawableObjectVectorFrame::DrawableObjectVectorFrame(
    std::shared_ptr<DrawableObjectVector> v,
    QWidget*                              parent) :
        DrawableObjectVectorFrame(parent)
{
    mDrawList = v;
    updateDrawableVectorWidget();
}

DrawableObjectVectorFrame::~DrawableObjectVectorFrame()
{
    delete mUI;
}

void DrawableObjectVectorFrame::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    mDrawList = v;
    updateDrawableVectorWidget();
}

uint DrawableObjectVectorFrame::selectedDrawableObject() const
{
    auto item = mUI->listWidget->selectedItems().first();
    return mUI->listWidget->row(item);
}

void DrawableObjectVectorFrame::on_listWidget_itemSelectionChanged()
{
    if (mUI->listWidget->selectedItems().size() > 0) {
        emit drawableObjectSelectionChanged(selectedDrawableObject());
    }
    else {
        mUI->listWidget->item(0)->setSelected(true);
    }
}

void DrawableObjectVectorFrame::updateDrawableVectorWidget()
{
    mUI->listWidget->clear();
    for (auto* d : *mDrawList) {
        QListWidgetItem*     item  = new QListWidgetItem(mUI->listWidget);
        DrawableObjectFrame* frame = new DrawableObjectFrame(d, mUI->listWidget);

        item->setSizeHint(frame->sizeHint());
        mUI->listWidget->addItem(item);
        mUI->listWidget->setItemWidget(item, frame);
        connect(
            frame,
            SIGNAL(visibilityChanged()),
            this,
            SIGNAL(drawableObjectVisibilityChanged()));
    }
    if (mDrawList->size() > 0) {
        mUI->listWidget->item(0)->setSelected(true);
    }
}

} // namespace vcl::qt
