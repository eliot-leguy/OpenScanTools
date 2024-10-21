#ifndef ICONTROL_H
#define ICONTROL_H

#include <unordered_map>

#define CONTROLLOG Logger::log(LoggerMode::ControlLog)
#define LOGENDL Logger::endl

class Controller;

enum class ControlType
{
	nullType,
	// control::application
	quitApplication, //Free
	redoApplication, //Free
	undoApplication, //Free

	// control::meta
	metaControl, //Free
	startMetaControl,
	stopMetaControl,

	// control::application::author
	temporaryPath, //Free

	// control::application
	saveAuthors,
	selectAuthor, //Free
	createAuthor, //Free
	deleteAuthor, //Free
	sendAuthorList, //Free
	setLangage, //Free
	setMouseWheel, //Free
	setDigitsDisplay, //Free
	setProjectsFolder, //Free
	setTemporaryFolder, //Free
	setUserColor, //Free
    setDecimationOptions, //Free
    setRenderPointSize, //Free
	setExamineOptions, //Free
	setFramelessMode, //Free
	setGizmoParameters, //Free
	setManipulatorSize, //Free
	setExamineDisplayMode, //Free
	setRecentProjects, //Free
	sendRecentProjects, //Free
	setAutosaveParameters, //Free
	setIndexationMethod, //Free
	setValueSettingsDisplay,
	setNavigationParameters,
	setPerspectiveZBounds,
	setOrthoGridParams,
	unlockScanManipulation,
	// setOrthoZBounds, // TODO

	// control::project
	createProject,
	dropLoadProject, //Free
	loadProject, //Free
	saveProjectContext,
	saveProject,
	autosaveProject,
	closeProject, //Free
	editProject, //Free
	saveCreateProject,
	saveCloseProject, //Free
	saveCloseLoadProject, //Free
	saveCloseLoadProjectCentral, //Free
	saveQuitProject, //Free
	functionImportScanProject,
	importScanProject,
	applyProjectTransformation, //Free
	deleteScanProject,
	showPropertiesProject, //Free

	// control::projectTemplate
	createProjectTemplate, //Free
	loadProjectTemplate, //Free
	saveProjectTemplate, //Free
	renameProjectTemplate, //Free
	deleteProjectTemplate, //Free
	sendListProjectTemplate, //Free
	stopEditionProjectTemplate, //Free

	// control::modal
	ModalReturnValue, //Free
	ModalReturnFiles, //Free

	// control::io
	exportSubProject, //Standard
	itemsToDXFIO, 
	itemsToCSVIO,
	itemsToStepIO, //Standard
	itemsToOSToolsIO, //Standard
	itemsToObjIO, //Standard
	itemsToFbxIO, //Standard
	importOSTObjects, //Standard
	setupImageHD,
	generateVideoHD,
	linkOSTObjectsContext,
	refreshScanLink,
	importScantraModifications,
	startScantraInterprocess,
	convertImage,

    // control::exportPC
    startExport, //Free
	startExportPCO, //Free
	startSingleExport, //Free
	startGridExport,
    startDeletePoints,

	// control::function
	functionAbort, //Free
	abortSelection, //Free
	functionCancel, //Free
	forwardMessage, //Free
	addNode, //Free

	// control::userlist
	sendUserLists,
	createUserList, 
	deleteUserList, 
	duplicateUserList, 
	renameUserList, 
	sendInfoUserList, 
	addItemUserList,
	removeItemUserList,
	renameItemUserList,
	clearUserList,
	saveLists,
	reloadLists,
	importList,
	exportList,

	// control::standards
	selectStandard,
	sendStandards,
	createStandard,
	deleteStandard,
	duplicateStandard,
	renameStandard,
	sendInfoStandard,
	addItemStandard,
	removeItemStandard,
	renameItemStandard,
	clearStandard,
	saveStandards,
	reloadStandards,
	importStandard,
	exportStandard,

	// control::tagTemplate
	sendTemplateList,
	sendTagTemplate,
	createTagTemplate,
	deleteTagTemplate,
	duplicateTagTemplate,
	renameTagTemplate,
	exportTagTemplate,
	importTagTemplate,
	saveTemplate,
	createFieldTemplate,
	deleteFieldTemplate,
	renameFieldTemplate,
	changeTypeFieldTemplate,
	changeRefFieldTemplate,
	changeDefaultValueFieldTemplate,

	// control::filter
	/*keywordFilter,//Standard
	showHideMarkers,
	MarkersSoftFilter,//Standard
	addColorFilter,//Standard
	removeColorFilter,//Standard
	clearcolorFilter,//Standard
	setTimeMinFilter,//Standard
	setTimeMaxFilter,//Standard
	setIconFilter,//Standard
	setIconStatusFilter,//Standard
	setKeyworStatusFilter,//Standard
	setUserStatusFilter,//Standard
	setDisciplineStatusFilter,//Standard
	setPhaseStatusFilter,//Standard
	setTimeStatusFilter,//Standard
	setUserFilter,//Standard
	setDisciplineFilter,//Standard
	setPhaseFilter,//Standard
	*/

	// control::function::tag
	activateCreateFunctionTag,
	activateMoveFunctionTag,
	activateDuplicateFunctionTag,
	setDefaultColorFunctionTag,
	setCurrentTagTemplate,
	setDefaultActionFunctionTag,
	setDefaultCategoryFunctionTag,
	setDefaultIconFunctionTag,
	duplicateTag,

	// control::function::clipping
	activateCreateLocalFunctionClipping,
	activateCreateAttachedBox3Points,
	activateCreateAttachedBox2Points,
	createGlobalFunctionClipping,
	setDefaultSizeFunctionClipping,
	setDefaultOffsetFunctionClipping,
	activateDuplicateFunctionClipping,
	setClippingAlignementValue,

	// control::duplication
	setDefaultDuplicationModeFunction,
	setDefaultDuplicationStepSizeFunction,
	setDefaultDuplicationOffsetFunction,
	setDefaultDuplicationIsLocalFunction,
	
	// control::function::torus
	disconnectPiping,

	// control::function::measure
	setPolylineOptions,
	addMeasureToPolylineMeasure,

	// control::special
	deleteElementSpecial,
	deleteSelectedElementSpecial,
	deleteTotal,
	showhideCurrentMarkers,
	showhideUncurrentMarkers,
	showhideAll,
	showhideDatas,
	showHideObjects,

	// control::picking
	clickPicking,
	findScanFromPicking,

	// control::viewport
	multiSelect,
	examine,
	changeBackgroundColor,
	adjustZoomToScene,
	alignView2PointsFunction,
	alignView3PointsFunction,
	alignViewBoxFunction,
	moveManipFunction,
	renderModeUpdate,
	changeScanVisibility,
	quickScreenshot,
	quickVideo,
	recordPerformance,

	// control::tree
	createClusterTree,
	deleteTree,
	clearAndDeleteCluster,
	removeFromHierarchy,
	childrenSelection,
	changeHideData,
	DropElement,
	CopySelectedTo,

	// control::dataEdition
	setColorEdit,
	setUserIdEdit,
	setDescriptionEdit,
	setDisciplineEdit,
	setNameEdit,
	setPhaseEdit,
	setIdentifierEdit,
	setHyperLinks,
	addHyperLink,
	removeHyperLink,

	// control::attributesEdition
	setColorAttribute,
	setDisciplineAttribute,
	setNameAttribute,
	setPhaseAttribute,
	setIdentifierAttribute,

	// control::tagEdition
	setFieldDatatagEdit,
	setShapeTagEdit,

	// control::clippingEdition
	setClippingTypeEdit,
	setClippingModeEdit,
	setMinClipDistance,
	setMaxClipDistance,
	setClippingActiveEdit,
	disableAllActiveClipping,
	setDefaultMinClipDistance,
	setDefaultMaxClipDistance,
	setClippingUserIdEdit,
	setClippingBoxExtend,
	setGridType,
	setGridValue,
	setDefaultClipMode,
	setDefaultRampValues,

	activeRamp,
	setMaxRamp,
	setMinRamp,
	setRampSteps,
	setRampClamped,

	// control::pointEdition
	setPointPositionEdit,

	// control::objectEdition
	setObject3DCenterEdit, 
	addObject3DCenterEdit,
	setObject3DSizeEdit,
	setObject3DRotationEdit,
	addObject3DRotationEdit,
	setObject3DTransformationEdit,
	addObject3DTransformationEnd,
	addObject3DExtrudeEdit,
	addObject3DExtrudeEditEnd,
	setSphereRadius,
	launchManipulateContext,

	// control::cylinderEdition
	setForcedRadiusEdit,
	setDetectedRadiusEdit,
	setStandardEdit,
	setCylinderLengthEdit,

	// control::folderEdition
	update3DCluster,

	// control::ScanEdition
	setScanClippable, // TODO - Free or Premium control ?
	randomScansColors,
    changeScanGuid,

	// control::pcObject
	createPCObjectFromFile,
	createPCObjectFromBox,
	activateDuplicateFunctionPCO,

	// control::meshObject
	createWavefrontFromFile,//Standard
	activateDuplicateFunctionWavefront,//Standard
	stepSimplification,//Standard

	// control::ScanTools
	showConvertionOptions,

	// control::SignalHandling
	SIGINTSignalHandling,
	SIGTERMSignalHandling,
	SIGILLSignalHandling,
	SIGABRTSignalHandling,
	SIGSEGVSignalHandling,
	SIGFPESignalHandling,

	// control::animation
	addAnimationKeyPoint,
	addScansAnimationKeyPoint,

	// control::Measure
	SendPipeDetectionOptions,//Standard
	ActivateFastCylinder,//Standard
	ActivateRobustCylinder,//Standard
	ActivateBeamBending,//Standard
	ActivateColumnTilt,//Standard
	SetBeamBendingTolerance,//Standard
	SetColumnTiltTolerance,//Standard
	ActivateSimpleMeasure,
	ActivatePolylineMeasure,
	ActivateBigCylinderFit,//Standard
	ActivatePointToCylinderMeasure,//Standard
	ActivatePointToPlaneMeasure,//Standard
	ActivatePointToPlane3Measure,//Standard
	ActivateCylinderToPlaneMeasure,//Standard
	ActivateCylinderToPlane3Measure,//Standard
	Switch3PlanMeasure,//Standard
	ActivateCylinderToCylinderMeasure,//Standard
	ActivateMultipleCylindersMeasure,//Standard
	ActivatePointMeasure,
	ActivatePointCreation,
	ActivateSphere,//Standard
	Activate4ClicsSphere,//Standard
	ActivateExtendCylinder,//Standard
	ActivatePipeDetectionConnexion,//Standard
	ActivatePipePostConnexion,//Standard
	ActivateDetectBeam,//Standard
	ActivateSlabDetection,//Standard
	ActivatePointToMeshMeasure,//Standard
	ActivateSlabMeasure,//Standard
	ActivatePlaneConnexion,//Standard
	ActivatePlaneDetection,//Standard
	ActivatePeopleRemover,//Standard
	ActivateSetOfPoints,//Standard
	ActivateTorus,//Standard
	ActivateTrajectory,//Standard

	//Control::userOrientation
	userOrientationProperties,
	sendUserOrientations,
	setUserOrientation,
	unsetUserOrientation,
	createEditUserOrientation,
	deleteUserOrientation,

	//Control::viewpoint
	contextViewPointCreation,
	contextViewPointUpdate,
	updateViewPoint,
	updateStateFromViewPoint,

	//Control::Test
	test_autoGenerateData
};

//const std::unordered_map< UIControlFamily, std::vector<ControlType>> UIControlFamilies;

/*! Classe servant de base de construction des contr�les modifiant le mod�le */
class AControl
{
public:
    virtual ~AControl() = 0 {};
	/*! Action du contr�le. */
	virtual void doFunction(Controller& controller) = 0;
	/*! Retourne si le contr�le se rajoute dans la pile d'historique des contr�les qui sont annulables (undoFunction) et r�p�table (doFunction � nouveau) */
	virtual bool canUndo() const 
	{
		return false;
	};
	/*! Fonction pour annuler les actions du contr�le. 
	Ces actions peut �tre refait via le doFunction. 
	
	Fonction � d�finir si canUndo() peut �tre �gale � vrai*/
	virtual void undoFunction(Controller& controller) {};
	virtual void redoFunction(Controller& controller) { doFunction(controller); }; //Par d�faut, pour la r�trocompatibilit�, le redo va faire do
	virtual ControlType getType() const = 0 ;
};

#endif // !ICONTROL_H_
