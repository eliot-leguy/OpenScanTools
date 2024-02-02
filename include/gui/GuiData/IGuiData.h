#ifndef I_GUI_DATA_H
#define I_GUI_DATA_H

enum class guiDType
{
	cameraData = 0,
	projectLoaded,
	projectTemplateList,
	authorSelection,
	nameSelection,
	identifierSelection,
	focusViewport,
	popupMsgData,
	popupWrnData,
	popupModalData,
	activatedFunctions,
	userList,

	tagDefaultAction,
	tagDefaultCategory,
	tagDefaultIcon,
	tagDefaultColor,

	globalColorPickerValue,

	currentScanData,
	tmpMsgData,
	pointCountData,
	renderActiveCamera,
	renderBackgroundColor,
	renderAlignView,
    renderCameraMoveTo,
	renderRotateThetaPhiView,
	renderExamine,
	renderTargetExamine,
	renderTargetClick,
	renderColorMode,
	renderPointSize,
	renderBrightness,
	renderContrast,
	renderLuminance,
	renderBlending,
	renderSaturation,
	renderTransparency,
	renderTransparencyOptions,
	renderViewPoint,
	renderViewPointAnimation,
	renderFlatColor,
	renderDistanceRampValues,
	renderDisplayAllMarkersTexts,
	renderStartAnimation,
	renderStopAnimation,
	renderCleanAnimationList,
	renderAnimationSpeed,
	renderAnimationLoop,
	renderRecordPerformance,
	renderImagesFormat,
	renderMeasureOptions,
	renderValueDisplay,
    renderDecimationOptions,
	renderNavigationConstraint,
	renderApplyNavigationConstraint,
	renderIsPanoramic,
	renderTextFilter,
	renderTextTheme,
	renderTextFontSize,
    renderMarkerDisplayOptions,
	renderFovValueChanged,
	renderAlphaObjectsRendering,
	renderPostRenderingNormals,
	renderRampScale,
	renderDisplayParameters,
	renderUpdate,
	renderGuizmo,
	renderGizmoParameters,
	renderNavigationParameters,
	renderPerspectiveZ,
	renderOrthographicZ,

    hdPrepare,
	hdGenerate,
	hdCall,

	sendListsList,
	sendUserList,
	sendListsStandards,
	sendStandardList,
	sendDisciplineSelected,
	sendPhaseSelected,

	sendAuthorsList,
	closeAuthorList,

	temporaryPath,
	projectPath,

	sendTemplateList,
	sendTagTemplate,
	sendTagTemplateSelected,

	undoRedoData,
	tolerancesProperties,
	defaultClipParams,
	defaultRampParams,

	objectSelected,
	multiObjectProperties,

	projectDataProperties,
	projectDataPropertiesNoOpen,
	clippingSettingsProperties,
    hidePropertyPanels,
	quitEvent,
	abortEvent,

    newProject,
    openProject,
	openProjectCentral,
    importScans,
	importMeshObject,
	//finishSimplifyStep,

	objectsToDelete,
	manipulatorMode,
	manipulatorModeCallBack,
	manipulatorLocGlob,
	manipulatorSize,
	//manipulatorSelection,
	markersDisplay,
	measuresToDisplay,
	gridDowngrade,
	pointsToDisplay,
	tagsToDisplay,
	scansToDisplay,
	viewpointsToDisplay,
	scanRemove,
	resetScans,
    moveToData,
	point,
	projectTransformation,
	selectElems,

	cleanTree,
    actualizeNodes,
	removeElemTree,
	deleteElemTree,
	moveElemTree,
	showStateTree,
	createAuthorTree,

	conversionOptionsDisplay,
	conversionFilePaths,
    exportParametersDisplay,
    deletePointsDialogDisplay,
	pcoCreationParametersDisplay,

	processingSplashScreenStart,
	processingSplashScreenProgressBarUpdate,
	processingSplashScreenEnableCancel,
	processingSplashScreenLogUpdate,
	processingSplashScreenEnd,
	processingSplashSignalCancel,
	processingSplashScreenForceClose,

	splashScreenStart,
	splashScreenEnd,

	screenshot,

	cursorChange,
	//Measure
	examineOptions,

	// UserOrientations
	userOrientationProperties,
	userOrientation,
	callbackUO,
	closeUOProperties,
	projectOrientation,
	sendUserOrientationList,


	importFileObjectDialog,
	exportFileObjectDialog,

	deleteScanDialog,
	sendRecentProjects,
	projectTemplateDialog,
	openInExplorer,

	disableFullScreen,

	contextRequestCameraPosition,

    maxEnum
};

/*! Interface des objets messages (pouvant contenir des donn�es autre que le guiDType) entre l'UI et le back.
*
*/
class IGuiData
{
public:
    virtual ~IGuiData() = 0 {};
	/*! Type du GuiData, permettant de les diff�rencier*/
	virtual guiDType getType() = 0;
};

/*
inline std::ostream& operator<<(std::ostream& out, const guiDType& dType)
{
	out << "[" << magic_enum::enum_name(dType) << "]";
    return (out);
}
*/

#endif // !_IGUIDATA_H_
