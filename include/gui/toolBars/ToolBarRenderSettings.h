#ifndef TOOLBAR_RENDER_SETTINGS_H
#define TOOLBAR_RENDER_SETTINGS_H

#include "ui_toolbar_rendersettingsgroup.h"
#include "gui/IPanel.h"
#include "gui/IDataDispatcher.h"
#include "pointCloudEngine/RenderingTypes.h"
#include "gui/UnitConverter.h"

#include "models/OpenScanToolsModelEssentials.h"

class CameraNode;

class ToolBarRenderSettings : public QWidget, public IPanel
{
	Q_OBJECT

public:
	explicit ToolBarRenderSettings(IDataDispatcher &dataDispatcher, QWidget *parent = 0, float guiScale = 1.f);
	~ToolBarRenderSettings();

	void switchRenderMode(const int& mode = 0);

private:
	void informData(IGuiData *data) override;
	void onProjectLoad(IGuiData* data);
	void onRenderBrightness(IGuiData* data);
	void onRenderContrast(IGuiData* data);
	void onRenderColorMode(IGuiData* data);
	void onRenderLuminance(IGuiData* data);
	void onRenderBlending(IGuiData* data);
	void onRenderPointSize(IGuiData* data);
	void onRenderSaturation(IGuiData* data);
	void onRenderAlphaObjects(IGuiData* data);
	void onRenderUnitUsage(IGuiData* data);

	void onActiveCamera(IGuiData* data);
	void onFocusViewport(IGuiData* data);

	typedef void (ToolBarRenderSettings::* GuiDataFunction)(IGuiData*);
	inline void registerGuiDataFunction(guiDType type, GuiDataFunction fct)
	{
		m_dataDispatcher.registerObserverOnKey(this, type);
		m_methods.insert({ type, fct });
	};

	void blockAllSignals(bool block);

    void showContrastBrightness();
    void showSaturationLuminance();
	void enableFalseColor(bool);
	bool rampValidValue(float& min, float& max, int& step);
	void sendTransparency();

	void changeEvent(QEvent* event) override;

private slots:
	void slotBrightnessLuminanceValueChanged(int value);
	void slotContrastSaturationValueChanged(int value);
	void slotTranparencyActivationChanged(int value);
	void slotTransparencyValueChanged(int value);
	void slotFakeColorValueChanged(int value);
	void slotAlphaBoxesValueChanged(int value);
	void slotSetPointSize(int pointSize);
	void slotSetRenderMode(int mode);
	void slotColorPicking();
	void slotRampValues();
	void slotNormalsChanged();

private:
	std::unordered_map<guiDType, GuiDataFunction> m_methods;
	Ui::toolbar_rendersettingsgroup m_ui;
	IDataDispatcher &m_dataDispatcher;
    UiRenderMode m_currentRenderMode;
	int m_brightness;
	int m_contrast;
	int m_lumiance;
	int m_saturation;
	int m_alphaBoxes;
    QColor m_selectedColor;
	bool m_intensityActive;
	SafePtr<CameraNode>	m_focusCamera;
};

#endif // TOOLBAR_RENDERSETTINGS_H
