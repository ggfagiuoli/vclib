/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "mesh_render_settings_frame.h"
#include "ui_mesh_render_settings_frame.h"

#include <QColorDialog>
#include <QStandardItemModel>

namespace vcl {

MeshRenderSettingsFrame::MeshRenderSettingsFrame(QWidget *parent) :
		QFrame(parent),
		ui(new Ui::MeshRenderSettingsFrame)
{
	ui->setupUi(this);
}

MeshRenderSettingsFrame::~MeshRenderSettingsFrame()
{
	delete ui;
}

const MeshRenderSettings& MeshRenderSettingsFrame::meshRenderSettings() const
{
	return mrs;
}

void MeshRenderSettingsFrame::setMeshRenderSettings(const MeshRenderSettings &settings)
{
	mrs = settings;
	updateGuiFromSettings();
}

void MeshRenderSettingsFrame::on_pointVisibilityCheckBox_stateChanged(int arg1)
{
	mrs.setPointCloudVisibility(arg1 == Qt::Checked);
	emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_pointColorComboBox_currentIndexChanged(int index)
{
	switch(index) {
	case P_VERT:
		mrs.setPointCloudColorPerVertex();
		break;
	case P_MESH:
		mrs.setPointCloudColorPerMesh();
		break;
	case P_USER:
		mrs.setPointCloudColorUserDefined();
		break;
	}
	ui->pointUserColorFrame->setVisible(index == P_USER);
	emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_pointColorDialogPushButton_clicked()
{
	QColor color = QColorDialog::getColor();

	if (color.isValid()) {
		QString s("background: #"
				  + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
				  + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
				  + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
		ui->pointColorDialogPushButton->setStyleSheet(s);
		ui->pointColorDialogPushButton->update();

		mrs.setPointCloudUserDefinedColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
		emit settingsUpdated();
	}
}

void MeshRenderSettingsFrame::on_surfaceVisibilityCheckBox_stateChanged(int arg1)
{
	mrs.setSurfaceVisibility(arg1 == Qt::Checked);
	emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_surfaceShadingSmoothRadioButton_toggled(bool checked)
{
	if (checked) {
		mrs.setSurfaceShadingSmooth();
		emit settingsUpdated();
	}
}


void MeshRenderSettingsFrame::on_surfaceShadingFlatRadioButton_toggled(bool checked)
{
	if (checked) {
		mrs.setSurfaceShadingFlat();
		emit settingsUpdated();
	}
}

void MeshRenderSettingsFrame::on_surfaceColorComboBox_currentIndexChanged(int index)
{
	switch(index) {
	case SC_FACE:
		mrs.setSurfaceColorPerFace();
		break;
	case SC_VERT:
		mrs.setSurfaceColorPerVertex();
		break;
	case SC_MESH:
		mrs.setSurfaceColorPerMesh();
		break;
	case SC_VERT_TEX:
		mrs.setSurfaceColorPerVertexTexcoords();
		break;
	case SC_WEDG_TEX:
		mrs.setSurfaceColorPerWedgeTexcoords();
		break;
	case SC_USER:
		mrs.setSurfaceColorUserDefined();
		break;
	}
	ui->surfaceUserColorFrame->setVisible(index == SC_USER);
	emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_surfaceColorDialogPushButton_clicked()
{
	QColor color = QColorDialog::getColor();

	if (color.isValid()) {
		QString s("background: #"
				  + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
				  + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
				  + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
		ui->surfaceColorDialogPushButton->setStyleSheet(s);
		ui->surfaceColorDialogPushButton->update();

		//mrs.setSurfaceUserDefinedColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
		emit settingsUpdated();
	}
}

void MeshRenderSettingsFrame::on_wireframeVisibilityCheckBox_stateChanged(int arg1)
{
	mrs.setWireframeVisibility(arg1 == Qt::Checked);
	emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_wireframeColorDialogPushButton_clicked()
{
	QColor color = QColorDialog::getColor();

	if (color.isValid()) {
		QString s("background: #"
				  + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
				  + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
				  + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
		ui->wireframeColorDialogPushButton->setStyleSheet(s);
		ui->wireframeColorDialogPushButton->update();

		mrs.setWireframeColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
		emit settingsUpdated();
	}
}

void MeshRenderSettingsFrame::updateGuiFromSettings()
{
	if (mrs.canBeVisible()) {
		ui->tabWidget->setEnabled(true);
		updatePointsTabFromSettings();
		updateSurfaceTabFromSettings();
		updateWireframeTabFromSettings();
	}
	else {
		ui->tabWidget->setEnabled(false);
		ui->pointVisibilityCheckBox->setChecked(false);
		ui->surfaceVisibilityCheckBox->setChecked(false);
		ui->wireframeVisibilityCheckBox->setChecked(false);
	}
}

void MeshRenderSettingsFrame::updatePointsTabFromSettings()
{
	if (mrs.canPointCloudBeVisible()) {
		ui->pointsTab->setEnabled(true);
		ui->pointVisibilityCheckBox->setEnabled(true);
		ui->pointVisibilityCheckBox->setChecked(mrs.isPointCloudVisible());

		// todo
		ui->pointShadingQuadRadioButton->setChecked(true);
		ui->pointShadingCircleRadioButton->setEnabled(false);

		updatePointsColorComboBoxFromSettings();
	}
	else {
		ui->pointsTab->setEnabled(false);
		ui->pointVisibilityCheckBox->setChecked(false);
	}
}

void MeshRenderSettingsFrame::updatePointsColorComboBoxFromSettings()
{
	QStandardItemModel* model =
		qobject_cast<QStandardItemModel *>(ui->pointColorComboBox->model());
	assert(model != nullptr);
	QStandardItem* item = model->item(P_VERT);
	if (mrs.canPointCloudBeColoredPerVertex()) {
		item->setFlags(item->flags() | Qt::ItemIsEnabled);
	}
	else {
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
	}
	item = model->item(P_MESH);
	if (mrs.canPointCloudBeColoredPerMesh()) {
		item->setFlags(item->flags() | Qt::ItemIsEnabled);
	}
	else {
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
	}
	if (mrs.isPointCloudColorPerVertex()) ui->pointColorComboBox->setCurrentIndex(P_VERT);
	if (mrs.isPointCloudColorPerMesh()) ui->pointColorComboBox->setCurrentIndex(P_MESH);
	if (mrs.isPointCloudColorUserDefined()) ui->pointColorComboBox->setCurrentIndex(P_USER);
	ui->pointUserColorFrame->setVisible(mrs.isPointCloudColorUserDefined());
}

void MeshRenderSettingsFrame::updateSurfaceTabFromSettings()
{
	if (mrs.canSurfaceBeVisible()) {
		ui->surfaceTab->setEnabled(true);
		ui->surfaceVisibilityCheckBox->setEnabled(true);
		ui->surfaceVisibilityCheckBox->setChecked(mrs.isSurfaceVisible());
		uptateSurfaceShadingRadioButtonsFromSettings();
		updateSurfaceColorComboBoxFromSettings();
	}
	else {
		ui->surfaceTab->setEnabled(false);
	}
}

void MeshRenderSettingsFrame::updateWireframeTabFromSettings()
{
	if (mrs.canSurfaceBeVisible()) {
		ui->wireframeTab->setEnabled(true);

	}
	else {
		ui->wireframeTab->setEnabled(false);
	}
}

void MeshRenderSettingsFrame::uptateSurfaceShadingRadioButtonsFromSettings()
{
	if (mrs.canSurfaceBeSmooth()) {
		ui->surfaceShadingSmoothRadioButton->setEnabled(true);
	}
	else {
		ui->surfaceShadingSmoothRadioButton->setEnabled(false);
	}
	ui->surfaceShadingFlatRadioButton->setChecked(mrs.isSurfaceShadingFlat());
	ui->surfaceShadingSmoothRadioButton->setChecked(mrs.isSurfaceShadingSmooth());
}

void MeshRenderSettingsFrame::updateSurfaceColorComboBoxFromSettings()
{
	QStandardItemModel* model =
		qobject_cast<QStandardItemModel *>(ui->surfaceColorComboBox->model());
	assert(model != nullptr);
	QStandardItem* item = model->item(SC_VERT);
	if (mrs.canSurfaceBeColoredPerVertex()) {
		item->setFlags(item->flags() | Qt::ItemIsEnabled);
	}
	else {
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
	}
	item = model->item(SC_FACE);
	if (mrs.canSurfaceBeColoredPerFace()) {
		item->setFlags(item->flags() | Qt::ItemIsEnabled);
	}
	else {
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
	}
	item = model->item(SC_MESH);
	if (mrs.canSurfaceBeColoredPerMesh()) {
		item->setFlags(item->flags() | Qt::ItemIsEnabled);
	}
	else {
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
	}
	item = model->item(SC_VERT_TEX);
	if (mrs.canSurfaceBeColoredPerVertexTexcoords()) {
		item->setFlags(item->flags() | Qt::ItemIsEnabled);
	}
	else {
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
	}
	item = model->item(SC_WEDG_TEX);
	if (mrs.canSurfaceBeColoredPerWedgeTexcoords()) {
		item->setFlags(item->flags() | Qt::ItemIsEnabled);
	}
	else {
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
	}

	if (mrs.isSurfaceColorPerVertex()) ui->surfaceColorComboBox->setCurrentIndex(SC_VERT);
	if (mrs.isSurfaceColorPerFace()) ui->surfaceColorComboBox->setCurrentIndex(SC_FACE);
	if (mrs.isSurfaceColorPerMesh()) ui->surfaceColorComboBox->setCurrentIndex(SC_MESH);
	if (mrs.isSurfaceColorPerVertexTexcoords()) ui->surfaceColorComboBox->setCurrentIndex(SC_VERT_TEX);
	if (mrs.isSurfaceColorPerWedgeTexcoords()) ui->surfaceColorComboBox->setCurrentIndex(SC_WEDG_TEX);
	if (mrs.isSurfaceColorUserDefined()) ui->surfaceColorComboBox->setCurrentIndex(SC_USER);
	ui->surfaceUserColorFrame->setVisible(mrs.isSurfaceColorUserDefined());
}

} // namespace vcl

