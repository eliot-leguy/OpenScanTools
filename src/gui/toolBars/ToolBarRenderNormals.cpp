#include "gui/toolBars/ToolBarRenderNormals.h"
#include "gui/GuiData/GuiDataRendering.h"
#include "gui/GuiData/GuiDataGeneralProject.h"

#include "models/graph/CameraNode.h"

ToolBarRenderNormals::ToolBarRenderNormals(IDataDispatcher& dataDispatcher, QWidget* parent, float guiScale)
	: QWidget(parent)
	, m_dataDispatcher(dataDispatcher)
	, m_focusCamera()
{
	m_ui.setupUi(this);
	setEnabled(false);
	m_ui.normals_checkBox->setChecked(true);
	m_ui.spinBox_normalStrength->setEnabled(false);
	m_ui.slider_normalStrength->setEnabled(false);
	m_ui.doubleSpinBox_sharpness->setEnabled(false);
	m_ui.checkBox_blendColor->setEnabled(true);

	// Connect widgets
	connect(m_ui.normals_checkBox, &QCheckBox::stateChanged, this, &ToolBarRenderNormals::slotNormalsChanged);
	connect(m_ui.checkBox_blendColor, &QCheckBox::stateChanged, this, &ToolBarRenderNormals::slotNormalsChanged);

	connect(m_ui.slider_normalStrength, &QSlider::valueChanged, m_ui.spinBox_normalStrength, &QSpinBox::setValue);
	connect(m_ui.slider_normalStrength, &QSlider::valueChanged, this, &ToolBarRenderNormals::slotNormalsChanged);
	connect(m_ui.spinBox_normalStrength, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_ui.slider_normalStrength, &QSlider::setValue);

	connect(m_ui.doubleSpinBox_sharpness, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ToolBarRenderNormals::slotSharpnessChanged);

	m_ui.spinBox_normalStrength->setValue(50);
	m_ui.doubleSpinBox_sharpness->setValue(1.0);

	// GuiData link
	registerGuiDataFunction(guiDType::projectLoaded, &ToolBarRenderNormals::onProjectLoad);
	registerGuiDataFunction(guiDType::renderActiveCamera, &ToolBarRenderNormals::onActiveCamera);
	registerGuiDataFunction(guiDType::focusViewport, &ToolBarRenderNormals::onFocusViewport);
	//registerGuiDataFunction(guiDType::renderPostRenderingNormals, &ToolBarRenderNormals::onRenderNormals);
}

void ToolBarRenderNormals::informData(IGuiData* data)
{
	if (m_methods.find(data->getType()) != m_methods.end())
	{
		GuiDataFunction method = m_methods.at(data->getType());
		(this->*method)(data);
	}
}

void ToolBarRenderNormals::onProjectLoad(IGuiData* data)
{
	GuiDataProjectLoaded* plData = static_cast<GuiDataProjectLoaded*>(data);
	setEnabled(plData->m_isProjectLoad);
}

void ToolBarRenderNormals::onFocusViewport(IGuiData* data)
{
	GuiDataFocusViewport* castData = static_cast<GuiDataFocusViewport*>(data);
	if (castData->m_forceFocus && castData->m_camera)
		m_focusCamera = castData->m_camera;
}

void ToolBarRenderNormals::onActiveCamera(IGuiData* data)
{
	auto infos = static_cast<GuiDataCameraInfo*>(data);
	if (infos->m_camera && m_focusCamera != infos->m_camera)
		return;

	ReadPtr<CameraNode> rCam = m_focusCamera.cget();
	if (!rCam)
		return;
	const DisplayParameters& displayParameters = rCam->getDisplayParameters();
	updateNormals(displayParameters.m_postRenderingNormals);
}

void ToolBarRenderNormals::updateNormals(const PostRenderingNormals& normalsParams)
{
	Qt::CheckState normalState = Qt::CheckState::Unchecked;
	if (normalsParams.show)
	{
		if (normalsParams.inverseTone)
			normalState = Qt::CheckState::PartiallyChecked;
		else
			normalState = Qt::CheckState::Checked;
	}
	blockAllSignals(true);

	m_ui.normals_checkBox->setCheckState(normalState);

	// ambiant value
	int newNormalValue = (int)std::round(normalsParams.normalStrength * 100.f);

	m_ui.slider_normalStrength->setEnabled(normalState != Qt::CheckState::Unchecked);
	m_ui.slider_normalStrength->setValue(newNormalValue);
	m_ui.spinBox_normalStrength->setEnabled(normalState != Qt::CheckState::Unchecked);
	m_ui.spinBox_normalStrength->setValue(newNormalValue);

	m_ui.doubleSpinBox_sharpness->setEnabled(normalState != Qt::CheckState::Unchecked);
	m_ui.checkBox_blendColor->setEnabled(normalState != Qt::CheckState::Unchecked);

	// gloss
	m_ui.doubleSpinBox_sharpness->setValue(normalsParams.gloss);

	// blend
	m_ui.checkBox_blendColor->setChecked(normalsParams.blendColor);

	blockAllSignals(false);
}

void ToolBarRenderNormals::blockAllSignals(bool block)
{
	m_ui.normals_checkBox->blockSignals(block);
	m_ui.slider_normalStrength->blockSignals(block);
	m_ui.spinBox_normalStrength->blockSignals(block);
	m_ui.doubleSpinBox_sharpness->blockSignals(block);
	m_ui.checkBox_blendColor->blockSignals(block);
}

void ToolBarRenderNormals::slotNormalsChanged()
{
	PostRenderingNormals lighting = {};
	bool enable = m_ui.normals_checkBox->isChecked();
	m_ui.slider_normalStrength->setEnabled(enable);
	m_ui.spinBox_normalStrength->setEnabled(enable);
	m_ui.doubleSpinBox_sharpness->setEnabled(enable);
	m_ui.checkBox_blendColor->setEnabled(enable);

	lighting.show = enable;
	lighting.inverseTone = (m_ui.normals_checkBox->checkState() == Qt::CheckState::PartiallyChecked);
	lighting.normalStrength = m_ui.spinBox_normalStrength->value() / 100.f;
	lighting.gloss = m_ui.doubleSpinBox_sharpness->value();
	lighting.blendColor = m_ui.checkBox_blendColor->isChecked();

	m_dataDispatcher.updateInformation(new GuiDataPostRenderingNormals(lighting, false, m_focusCamera), this);
}

void ToolBarRenderNormals::slotSharpnessChanged(double value)
{
	slotNormalsChanged();
}
