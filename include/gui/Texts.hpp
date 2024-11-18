#ifndef TEXTS_HPP
#define TEXTS_HPP

#include <QtCore/qobject.h>

// Basic Wording
#define TEXT_WARNING QObject::tr("Warning!")
#define TEXT_INFORMATION QObject::tr("Information")
#define TEXT_QUESTION QObject::tr("Question")

//SaveLoadSystem

#define TEXT_ERROR_NO_TLS_PATH QObject::tr("Error : TLS path not found on : %1")
#define TEXT_CANT_LOAD_PROJECT QObject::tr("Error : The project cannot be loaded.")
#define TEXT_CANT_LOAD_PROJECT_NO_AUTHOR QObject::tr("Error : The project can't be loaded.\r\nThe tld file : [%1] do not have author.")
#define TEXT_CANT_LOAD_TAG QObject::tr("Error : The tag cannot be loaded.")
#define TEXT_CANT_LOAD_TAG_NO_AUTHOR QObject::tr("Error : The tag can't be loaded.\r\nThe tld file : [%1] do not have author.")
#define TEXT_TEMPLATE_NOT_REGISTERED QObject::tr("Error : the file contain at least one template that is not registered.")
#define TEXT_TEMPLATE_INVALID_TAG QObject::tr("Error : name or userId data in tag not found.")
#define TEXT_CANT_LOAD_PROJECT_TLP_ERROR QObject::tr("The tlp project file may contains an error or has been removed or the filepath contains unrecognized caracter. ")
#define TEXT_CANT_LOAD_PROJECT_OUTDATED_SAVELOADSYTEM QObject::tr("This project can't be opened with this version, please download and install the latest version first")
#define TEXT_TEMPLATE_INVALID_MEASURE QObject::tr("Error : name data in measure not found.")
#define TEXT_TEMPLATE_INVALID_CLIPPING QObject::tr("Error : name data in boxes not found.")
#define TEXT_TEMPLATE_INVALID_PIPE QObject::tr("Error : name data in pipe not found.")
#define TEXT_TEMPLATE_INVALID_POINT QObject::tr("Error : name data in point not found.")
#define TEXT_TEMPLATE_INVALID_PCO QObject::tr("Error : position data in pipe not found.")
#define TEXT_TEMPLATE_INVALID_MESHOBJECT_PATH QObject::tr("Error : path data in pipe not found.")
#define TEXT_TEMPLATE_INVALID_MESHOBJECT_NAME QObject::tr("Error : name data in pipe not found.")
#define TEXT_TEMPLATE_INVALID_MESHOBJECT_POSITION QObject::tr("Error : name data in pipe not found.")
#define TEXT_TEMPLATE_INVALID_USER_ORIENTATION QObject::tr("Error : user orientation data not found.")
#define TEXT_LOAD_FAILED_FILE_NOT_FOUND QObject::tr("Failed to load %1.\nFile not found : \n%2")

//StatusPanel.cpp
#define TEXT_CAMERA QObject::tr("Camera")
#define TEXT_MOUSE QObject::tr("Mouse")
#define TEXT_DISTANCE QObject::tr("Distance")
#define TEXT_MODE QObject::tr("Mode")
#define TEXT_SCAN QObject::tr("Scan")
#define TEXT_CURRENT_FUNCTION QObject::tr("Current function")

//ScansPanel
#define TEXT_SCAN_NAME QObject::tr("Scan name")
#define TEXT_SCANNER_MODEL QObject::tr("Scanner model")
#define TEXT_SCANNER_SERIAL_NUMBER QObject::tr("Scanner serial number")
#define TEXT_ACQUISITION_DATE QObject::tr("Acquisition date")
#define TEXT_DESCRIPTION QObject::tr("Description")
#define TEXT_RGB QObject::tr("RGB")
#define TEXT_INTENSITY QObject::tr("Intensity")
#define TEXT_POINTS_NUMBER QObject::tr("Number of points")
#define TEXT_IMPORT_DATE QObject::tr("Import date")
#define TEXT_POSTION QObject::tr("Position")
#define TEXT_ROTATION QObject::tr("Rotation")

//ClustersPanel
#define TEXT_CLUSTER_NAME QObject::tr("Cluster name")
#define TEXT_CLUSTER_USER QObject::tr("User")
#define TEXT_COLOR  QObject::tr("Color") // TEXT_OBJECT_PROPERTY_COLOR
#define TEXT_CLUSTER_NAME_ALREADY_EXIST QObject::tr("Name already taken!")

//ProjectPanel
#define TEXT_SCANS_NUMBER QObject::tr("Number of scans")
#define TEXT_PROJECT_NAME QObject::tr("Project name")
#define TEXT_AUTOR QObject::tr("Author")
#define TEXT_COMPANY QObject::tr("Company")
#define TEXT_LOCATION QObject::tr("Location")

//PolylinePropertyPanel
#define TEXT_POLYLINEPROPERTYPANEL_POINT_TABLE_PT QObject::tr("Point %1")
#define TEXT_POLYLINEPROPERTYPANEL_SEGMENT_TABLE_SEG QObject::tr("D%1")
#define TEXT_POLYLINEPROPERTYPANEL_SEGMENT_TABLE_TOTAL QObject::tr("Total")
#define TEXT_POLYLINEPROPERTYPANEL_SEGMENT_TABLE_HOR QObject::tr("Hor.")
#define TEXT_POLYLINEPROPERTYPANEL_SEGMENT_TABLE_VERT QObject::tr("Vert.")

//AuthorSystem
#define TEXT_WARNING_AUTHOR_SELECT QObject::tr("Error : cannot select user : unknow error.") //author renomm� en user pour coh�rence avec panneau
#define TEXT_WARNING_AUTHOR_CREATE QObject::tr("Error : cannot create a user that already exists.") //author renomm� en user pour coh�rence avec panneau
#define TEXT_WARNING_AUTHOR_DELETE QObject::tr("Error : cannot delete user : unknow error.") //author renomm� en user pour coh�rence avec panneau
#define TEXT_WARNING_AUTHOR_WRONG_CLOSE QObject::tr("Error : you must select a user in order to continue.") //author renomm� en user pour coh�rence avec panneau
#define TEXT_WARNING_AUTHOR_SELECT_ONE QObject::tr("You must select an author.")
#define TEXT_DELETE_AUTHOR_QUESTION QObject::tr("Are you sure that you want to delete the selected user(s)?") //author renomm� en user pour coh�rence avec panneau
#define TEXT_DELETE_AUTHOR_TITLE QObject::tr("Delete user(s)?") //author renomm� en user pour coh�rence avec panneau

//Colors
#define TEXT_BLUE QObject::tr("Blue")
#define TEXT_GREEN QObject::tr("Green")
#define TEXT_RED QObject::tr("Red")
#define TEXT_ORANGE QObject::tr("Orange")
#define TEXT_YELLOW QObject::tr("Yellow")
#define TEXT_PURPLE QObject::tr("Purple")
#define TEXT_LIGHT_GREY QObject::tr("Light Grey")
#define TEXT_BROWN QObject::tr("Brown")

//ClippingStatus
#define TEXT_ACTIVE QObject::tr("Active")
#define TEXT_INACTIVE QObject::tr("Inactive")

//ClippingMethod
#define TEXT_CLIPINTERN QObject::tr("Show interior")
#define TEXT_CLIPEXTERN QObject::tr("Show exterior")

//ClippingControl
#define TEXT_ACTIVE_CLIPPING_RAMP_MAX_REACHED QObject::tr("You can't activate more clippings or rampes, the maximum is reached (max: %1).")

//Controller
#define TEXT_ERROR_PROJECT_DIRECTORY_DELETE QObject::tr("Error : The project couldn't be saved.\nMaybe the project folder has been deleted")

//ControlPorject
#define TEXT_NO_PROJECT_LOADED QObject::tr("No project loaded")

//ControlTemplateEdit
#define TEXT_EDIT_TEMPLATE_NOT_EXIST QObject::tr("Error : the template you try to edit does not exist.") //signification? Comment peut-on essayer d'�diter quelque chose d'inexistant?
#define TEXT_TAG_NAME_TEMPLATE_ALREADY_TAKEN QObject::tr("Error : tag template name already used.")
#define TEXT_EDIT_TAG_TEMPLATE_NOT_EXIST QObject::tr("Error : you tried to edit a tag template that doesn't exists anymore.") //signification?
#define TEXT_DELETE_TAG_TEMPLATE_WITH_TAGS QObject::tr("Error : you tried to delete a tag template that is used in the current project. Remove the tags before deleting the template.") 
#define TEXT_DELETE_TAG_TEMPLATE_NOT_EXIST QObject::tr("Error : you tried to delete a tag template that doesn't exist anymore.") //signification?
#define TEXT_CANCELED_DELETE_TAG_TEMPLATE_ALREADY_EXIST QObject::tr("Error : you tried to cancel the deletion of a tag template that is already present in your template list.") //signification?
#define TEXT_DUPLICATED_TAG_TEMPLATE_ALREADY_EXIST QObject::tr("Error : you tried to duplicate a tag template that already exists.") //signification?
#define TEXT_TEMPLATE_DUPLICATE_UNDO_FAILED QObject::tr("Error : Undo failed : this template does not exist.") //signification?
#define TEXT_TEMPLATE_RENAME_FAILED QObject::tr("Error : you tried to rename a template that does not exist anymore.") //signification?
#define TEXT_TEMPLATE_ALREADY_PRESENT QObject::tr("This template already exists under the name %1.")


// TODO  - Remove _TITLE_ , _NAME_,
//NewProjectDialog
#define TEXT_PROJECT_CREATION QObject::tr("Project Creation")
#define TEXT_ENTER_PROJECT_NAME QObject::tr("Enter project name")
#define TEXT_ENTER_PROJECT_DIRECTORY QObject::tr("Enter project folder") //directory renomm� en folder, plus couramment utilis�
#define TEXT_SELECT_CUSTOM_SCAN_FOLDER  QObject::tr("Select custom scan folder")
#define TEXT_CREATE_PROJECT QObject::tr("Create Project")
#define TEXT_CANCEL QObject::tr("Cancel")

//ConvertionOptionsBox
#define TEXT_CONVERTION_TITLE QObject::tr("Conversion Options")
#define TEXT_SAVE_FILENAME QObject::tr("Select destination")
#define TEXT_PROJECT_ERROR_TITLE QObject::tr("Error : cannot create project.")
#define TEXT_PROJECT_ERROR_NOT_EMPTY QObject::tr("Error : project name and project folder are mandatory fields.") //directory renomm� en folder, plus couramment utilis�
#define TEXT_PROJECT_ERROR_NO_AUTHOR QObject::tr("Error : the author name is missing.") //obsol�te, car le panneau utilisateur est d�sormais � part
#define TEXT_SELECT_DIRECTORY  QObject::tr("Select folder") //directory renomm� en folder, plus couramment utilis�
#define TEXT_NO_DIRECTORY_SELECTED QObject::tr("Please select a destination folder.")
#define TEXT_MISSING_FILE_NAME QObject::tr("Please enter a file name or header name.")

//TemplatePropertiesPanel
#define TEXT_NAME QObject::tr("Name")
#define TEXT_SHAPE QObject::tr("Shape")
#define TEXT_POS QObject::tr("Position")
#define TEXT_TAG_INDEX QObject::tr("Index")
#define TEXT_DISCPLINE QObject::tr("Discipline")
#define TEXT_PHASE QObject::tr("Phase")
#define TEXT_ID QObject::tr("Identifier") //ID r�actualis� en Identifier. Auparavant, cela s'appelait prefix. Pour m�moire, ce champ est un g�n�rique
#define TEXT_TAG_LINKS QObject::tr("Links")
#define TEXT_TAG_ADDLINK QObject::tr("add hyperlink")
#define TEXT_TAG_REMOVELINK QObject::tr("delete hyperlink")

//ControlPCObject
#define TEXT_SCAN_IMPORT_AS_OBJECT_FAILED QObject::tr("Failed to create the point cloud object.")

// TemplateManager
#define TEXT_EXPORT_TEMPLATE QObject::tr("Export tag template")
#define TEXT_IMPORT_TEMPLATE QObject::tr("Import tag template")

// ToolBarImportObjects
#define TEXT_IMPORT_SHARED_OBJECTS QObject::tr("Import shared objects")
#define TEXT_IMPORT_TYPE_TLD QObject::tr("Tags (*.tld)")
#define TEXT_IMPORT_TYPE_TLO QObject::tr("Objects (*.tlo)")
#define TEXT_IMPORT_TYPE_TLV QObject::tr("Viewpoints (*.tlv)")
#define TEXT_IMPORT_TYPE_ALL_OBJECTS QObject::tr("All objects %1")
#define TEXT_IMPORT_TYPE_ALL_OBJECTS_OPEN TEXT_IMPORT_TYPE_ALL_OBJECTS.arg("(*.tld *.tlo *.tlv)") + ";;" \
	+ TEXT_IMPORT_TYPE_TLD  + ";;" \
	+ TEXT_IMPORT_TYPE_TLO + ";;" \
	+ TEXT_IMPORT_TYPE_TLV
#define TEXT_IMPORT_SCANTRA QObject::tr("Import Scantra registration")
#define TEXT_IMPORT_SCANTRA_FILES QObject::tr("Scantra database (*.scdb)")

//PropertyClippingSettings.cpp
#define TEXT_POINT_PICKING QObject::tr("Ready for point picking.")
#define TEXT_POINT_PICKING_REJECTED QObject::tr("Point rejected.")
#define TEXT_POINT_PICKING_DONE QObject::tr("Measure done.")
#define TEXT_BOXES_SETTINGS_PROPERTIES_NAME QObject::tr("Boxes creation settings")
#define TEXT_3DMODEL_SETTINGS_PROPERTIES_NAME QObject::tr("3D Model duplication settings")
#define TEXT_PCO_SETTINGS_PROPERTIES_NAME QObject::tr("Point cloud object duplication settings")

//PropertyUserOrientation.cpp
#define TEXT_USER_ORIENTATION_PROPERTIES_WARNING_NAME_ALREADY_USED QObject::tr("This orientation name already exists")
#define TEXT_USER_ORIENTATION_PROPERTIES_WARNING_NO_NAME QObject::tr("Please give a name to the orientation")
#define TEXT_USER_ORIENTATION_PROPERTIES_WARNING_MISSING_POINT QObject::tr("Points are missing. The orientation cannot be created")
#define TEXT_USER_ORIENTATION_PROPERTIES_SAME_POINT_AXIS QObject::tr("Same %1 points. Axis is null")
#define TEXT_USER_ORIENTATION_PROPERTIES_NAN_VEC QObject::tr("Incorrect %1 point")

//PropertyPointCloud
#define TEXT_COLOR QObject::tr("Color")
#define TEXT_ADD_ANIMATION_KEYPOINT QObject::tr("Add as a key point")

//MultiProperty
#define TEXT_MULTI_ATTRIBUTES_CHANGE_QUESTION QObject::tr("You will replace data of selected objets.\nDo you confirm?")
#define TEXT_MULTI_ATTRIBUTES_CHANGE_TITLE QObject::tr("Replace data ?")

//ControlIO
#define TEXT_TAG_TO_PATH_ERROR QObject::tr("Error : incorrect file path (you may have specified a folder with administrator rights).")
#define TEXT_SCANTRA_NOT_FOUND_SCANS QObject::tr("Some transformations could not be applied, please read the report.")

//ControlProject
#define TEXT_PROJECT_CLOSING QObject::tr(" Closing Project...")
#define TEXT_PROJECT_ERROR_LOADING QObject::tr("Failed to load project:\n%1\n")
#define TEXT_PROJECT_ERROR_CENTRAL QObject::tr("You are trying to open a central project !")

//AuthorCreateDialog
#define TEXT_DEFAULT QObject::tr("default")

//TemplateEditorDialog
#define TEXT_REFERENCE QObject::tr("Reference")
#define TEXT_TYPE QObject::tr("Type")
#define TEXT_DEFAULT_VALUE QObject::tr("Default value")
#define TEXT_TITLE_NEW_FIELD QObject::tr("New Field")
#define TEXT_TITLE_FIELD_REMOVAL_BOX QObject::tr("Field Removal")
#define TEXT_MESSAGE_FIELD_REMOVAL_BOX QObject::tr("Warning: by removing this field from the template, you will also remove it in the related tags. Do you confirm?")
#define TEXT_TEMPLATEEDITOR_LIST QObject::tr("List")
#define TEXT_TEMPLATEEDITOR_DATE QObject::tr("Date")
#define TEXT_TEMPLATEEDITOR_NUMBER QObject::tr("Number")
#define TEXT_TEMPLATEEDITOR_STRING QObject::tr("String")

//TemplateManagerDialog
#define TEXT_TITLE_DELETE_TEMPLATE_BOX QObject::tr("Delete template(s)?")
#define TEXT_MESSAGE_DELETE_TEMPLATE_BOX QObject::tr("Do you really want to delete this template?\nWarning: you cannot undo this action.")
#define TEXT_DELETE_TEMPLATE_BUTTON QObject::tr("Delete template(s)")
#define TEXT_EXPORT_TEMPLATE_BOX QObject::tr("Export template")
#define TEXT_DUPLICATE_TEMPLATE_BOX QObject::tr("Duplicate template")
#define TEXT_EDIT_TEMPLATE_BOX QObject::tr("Edit template")
#define TEXT_TEMPLATE_WARNING_SELECT_ONE QObject::tr("You must select a template.")
#define TEXT_TEMPLATE_WARNING_SELECT_MULTIPLE QObject::tr("You must select at least one template.")

// MainToolBar
#define TEXT_MAIN_TOOLBAR_NEW QObject::tr("New")
#define TEXT_MAIN_TOOLBAR_OPEN QObject::tr("Open")
#define TEXT_MAIN_TOOLBAR_RECENT_PROJECTS QObject::tr("Recent Projects")
#define TEXT_MAIN_TOOLBAR_SAVE QObject::tr("Save")
#define TEXT_MAIN_TOOLBAR_CLOSE_PROJECT QObject::tr("Close Project")
#define TEXT_MAIN_TOOLBAR_IMPORT_TLS QObject::tr("Import scans")
#define TEXT_MAIN_TOOLBAR_UNDO QObject::tr("Undo")
#define TEXT_MAIN_TOOLBAR_REDO QObject::tr("Redo")
#define TEXT_MAIN_TOOLBAR_MAX_SCREEN QObject::tr("Enter Maximize Screen")
#define TEXT_MAIN_TOOLBAR_RESTORE_DOCKS QObject::tr("Restore Docks")
#define TEXT_MAIN_TOOLBAR_QUIT_MAXIMIZE_SCREEN QObject::tr("Quit Maximize Screen")
#define TEXT_MAIN_TOOLBAR_FULL_SCREEN QObject::tr("Enter Full Screen")
#define TEXT_MAIN_TOOLBAR_MINIMIZE_SCREEN QObject::tr("Minimize")
#define TEXT_MAIN_TOOLBAR_SETTINGS QObject::tr("General settings")
#define TEXT_MAIN_TOOLBAR_SHORTCUTS QObject::tr("Show shortcuts")
#define TEXT_MAIN_TOOLBAR_ABOUT QObject::tr("About")
#define TEXT_MAIN_TOOLBAR_RESTORE_SCREEN QObject::tr("Restore Down")
#define TEXT_MAIN_TOOLBAR_MAXIMIZE_SCREEN QObject::tr("Maximize")
#define TEXT_MAIN_TOOLBAR_EXIT QObject::tr("Exit")
#define TEXT_MAIN_TOOLBAR_NAME QObject::tr("Main toolbar")

//ToolBarExportGroup
#define TEXT_EXPORT_GENERAL_FILE QObject::tr("Export %1 file(s)")
#define TEXT_EXPORT_OPENSCANTOOLS_FILE QObject::tr("Export shared files")

#define TEXT_EXPORT_FILTER_ALL QObject::tr("All")
#define TEXT_EXPORT_FILTER_SELECTED QObject::tr("Selected")
#define TEXT_EXPORT_FILTER_DISPLAYED QObject::tr("Displayed")
#define TEXT_EXPORT_FILTER_NONE QObject::tr("None")



//ToolBarShowHideGroup - A REVOIR - pas n�cessaire d'avoir 2 textes - le changement d'ic�ne d'oeil suffit (oeil ouvert ou barr�). Donc oeil + scan marker suffit
#define TEXT_HIDE_MODEL_SCAN_MARKER QObject::tr("Hide scan markers")
#define TEXT_SHOW_MODEL_SCAN_MARKER QObject::tr("Show scan markers")
#define TEXT_HIDE_TAG_MARKER QObject::tr("Hide tags")
#define TEXT_SHOW_TAG_MARKER QObject::tr("Show tags")
#define TEXT_HIDE_TEXTS_MARKER QObject::tr("Hide texts")
#define TEXT_SHOW_TEXTS_MARKER QObject::tr("Show texts")
#define TEXT_HIDE_MEASURES_MARKER QObject::tr("Hide measures")
#define TEXT_SHOW_MEASURES_MARKER QObject::tr("Show measures")
#define TEXT_HIDE_SELECTED_OBJECT QObject::tr("Hide selected object")
#define TEXT_SHOW_SELECTED_OBJECT QObject::tr("Show selected object")
#define TEXT_SHOWHIDE_TOOLBAR_NAME QObject::tr("Show hide toolbar")

//Settings
#define TEXT_SETTINGS_FAILED_TO_SAVE QObject::tr("Failed to save the configuration change.")
#define TEXT_SETTINGS_NEED_RESTART_LANGUAGE QObject::tr("Please restart OpenScanTools to apply the new language.")
#define TEXT_SETTINGS_NEED_RESTART_MEMORY QObject::tr("Please restart OpenScanTools to apply the new memory setting.")
#define TEXT_SETTINGS_MINUTES QObject::tr("%1 minutes")

// Custom widget
#define TEXT_ENTER_VALUE_PLACEHOLDER QObject::tr("Enter value")

//ControlUserList
#define TEXT_USERLIST_NAME_EXIST QObject::tr("Error : List name already used")
#define TEXT_USERLIST_NAME_EXIST_RENAME QObject::tr("Error : You tried to rename an item with a name that is already used")
#define TEXT_USERLIST_ITEM_EXIST QObject::tr("Error : you tried to add an item that already exists in this list")
#define TEXT_USERLIST_ITEM_EXIST_RENAME QObject::tr("Error : You tried to rename an item with a name that is already used")

//ControlUserList & ControlPipesStandards
#define TEXT_LIST_IMPORT_FAILED QObject::tr("Error : Unexpected error, try to import your file once again")
#define TEXT_LIST_ALREADY_EXIST QObject::tr("Error : List with the same name already exist.")
#define TEXT_LIST_OPEN_FILE_FAILED QObject::tr("Error : The file cannot be opened.")
#define TEXT_LIST_EXPORT_FAILED QObject::tr("Error : export failed.")
#define TEXT_LIST_EXPORT_SUCCESS QObject::tr("Success : File exported at %1")
#define TEXT_LIST_EXPORT_ERROR QObject::tr("Error : export failed due to an internal error. Try to relaunch OpenScanTools.")

//ControlStandards
#define TEXT_STANDARDS_NAME_EXIST QObject::tr("Error : Standard name already used.")
#define TEXT_STANDARDS_NAME_EXIST_RENAME QObject::tr("Error : you tried to rename an item with a name that is already used.")
#define TEXT_STANDARDS_ITEM_EXIST QObject::tr("Error : you tried to add a value that already exists in this list.")
#define TEXT_STANDARDS_ITEM_EXIST_RENAME QObject::tr("Error : you tried to change to a value that already exists in this list.")
#define TEXT_STANDARDS_REDO_NOT_VALID QObject::tr("Error : you tried to redo a delete of a list that already exists or that is not valid")

//DialogDeleteScan
#define TEXT_IMPORTANT_DATA_REMOVAL_TITLE QObject::tr("Important data removal")
#define TEXT_IMPORTANT_SCAN_REMOVAL_MESSAGE QObject::tr("Are you sure you want to delete the file(s) from the hard drive?\nWARNING: this operation is irreversible!")

//ControlIO
#define TEXT_EXPORT_TAGS QObject::tr("Tags successfully exported at %1")

//BeamBending & ColumnTilt
#define TEXT_NA QObject::tr("NA")
#define TEXT_RATIOSUP_YES QObject::tr("Yes")
#define TEXT_RATIOSUP_NO QObject::tr("No")
#define TEXT_RELIABILITY_RELIABLE QObject::tr("Reliable")
#define TEXT_RELIABILITY_UNRELIABLE QObject::tr("Unreliable")

//Align View
#define TEXT_VIEW_TOP QObject::tr("Top view")
#define TEXT_VIEW_BOTTOM QObject::tr("Bottom view")
#define TEXT_VIEW_LEFT QObject::tr("Left view")
#define TEXT_VIEW_RIGHT QObject::tr("Right view")
#define TEXT_VIEW_FRONT QObject::tr("Front view")
#define TEXT_VIEW_BACK QObject::tr("Back view")
#define TEXT_VIEW_ISO QObject::tr("Iso view")

// DialogImportMeshObject
#define TEXT_DIALOG_TITLE_IMPORT_WAVEFRONT QObject::tr("Import settings")
#define TEXT_DIALOG_BROWSER_TITLE_IMPORT_WAVEFRONT QObject::tr("Select file")
#define TEXT_DIALOG_ERROR_FILENAME QObject::tr("Filename is incorrect")
#define TEXT_DIALOG_ERROR_FORMAT QObject::tr("File format not supported")
#define TEXT_DIALOG_ERROR_SCALING QObject::tr("Scaling is incorrect")
#define TEXT_DIALOG_ERROR_DIRECTION QObject::tr("Directions are incorrect")

// DialogImportStepSimplification
#define TEXT_DIALOG_BROWSER_TITLE_IMPORT QObject::tr("Select input file")
#define TEXT_DIALOG_BROWSER_TITLE_EXPORT QObject::tr("Select output file")
#define TEXT_DIALOG_ERROR_INPUT_PATH QObject::tr("Input file doesn't exist")
#define TEXT_DIALOG_ERROR_OUTPUT_PATH QObject::tr("Empty output file")
#define TEXT_DIALOG_ERROR_OUTPUT_FOLDER QObject::tr("Empty output folder")

// DialogImportAsciiPC
#define TEXT_DIALOG_ERROR_NO_XYZ_SET QObject::tr("Error : No XYZ setted.")

// GeneralImport
#define TEXT_GENERAL_ERROR_OPEN_FILE QObject::tr("Error : The file :\n '%1'\ncannot be opened.")
#define TEXT_GENERAL_ERROR_EMPTY_FILE QObject::tr("Error : The file :\n '%1'\nis empty.")

//GeneralDialog
#define TEXT_DIALOG_YES QObject::tr("Yes")
#define TEXT_DIALOG_NO QObject::tr("No")
#define TEXT_DIALOG_CANCEL QObject::tr("Cancel")

// DialogExportVideo
#define TEXT_EXPORT_VIDEO_MISSING_VIEWPOINTS QObject::tr("Error : Viewpoints are missing.")
#define TEXT_EXPORT_VIDEO_SAME_VIEWPOINTS QObject::tr("Error : Same selected viewpoints.")
#define TEXT_EXPORT_VIDEO_ORTHO_VIEWPOINT QObject::tr("Error : Orthographic viewpoint not allowed.")

#endif