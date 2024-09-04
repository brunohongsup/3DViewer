#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <map>


namespace Ravid
{
	namespace Framework
	{
		enum ELanguageParameter
		{
			ELanguageParameter_ModelManager = 0,
			ELanguageParameter_DevicePropertyPage,
			ELanguageParameter_LogIn,
			ELanguageParameter_LogOut,
			ELanguageParameter_AuthorityManager,
			ELanguageParameter_ScreenSaverSetting,
			ELanguageParameter_LogViewer,
			ELanguageParameter_Open,
			ELanguageParameter_Close,
			ELanguageParameter_New,
			ELanguageParameter_Copy,
			ELanguageParameter_Delete,
			ELanguageParameter_Configuration,
			ELanguageParameter_Initialize,
			ELanguageParameter_Terminate,
			ELanguageParameter_Message,
			ELanguageParameter_Status,
			ELanguageParameter_Grab,
			ELanguageParameter_Live,
			ELanguageParameter_Stop,
			ELanguageParameter_SetSoftwareTrigger,
			ELanguageParameter_Input,
			ELanguageParameter_Output,
			ELanguageParameter_MotorControlling,
			ELanguageParameter_Velocity,
			ELanguageParameter_Acceleration,
			ELanguageParameter_Distance,
			ELanguageParameter_Position,
			ELanguageParameter_SensorStatus,
			ELanguageParameter_Home,
			ELanguageParameter_ServoMode,
			ELanguageParameter_CommandPosition,
			ELanguageParameter_ActualPosition,
			ELanguageParameter_Value,
			ELanguageParameter_Alarm,
			ELanguageParameter_Reset,
			ELanguageParameter_On,
			ELanguageParameter_Off,
			ELanguageParameter_Information,
			ELanguageParameter_StatusLED,
			ELanguageParameter_EncoderCount,
			ELanguageParameter_TriggerCount,
			ELanguageParameter_EnableTrigger,
			ELanguageParameter_DisableTrigger,
			ELanguageParameter_ResetCounter,
			ELanguageParameter_SendingData,
			ELanguageParameter_waiting,
			ELanguageParameter_failed,
			ELanguageParameter_succeed,
			ELanguageParameter_ReceivedData,
			ELanguageParameter_Send,
			ELanguageParameter_NetworkStatus,
			ELanguageParameter_TotalReceivedByte,
			ELanguageParameter_TotalSendByte,
			ELanguageParameter_ConnectionInfo,
			ELanguageParameter_ClientConnected,
			ELanguageParameter_LimitPlus,
			ELanguageParameter_LimitMinus,
			ELanguageParameter_Inposition,
			ELanguageParameter_EncoderAndTriggerInfo,
			ELanguageParameter_QueueInfo,
			ELanguageParameter_GoogolExternalIO,
			ELanguageParameter_Page,
			ELanguageParameter_Axis,
			ELanguageParameter_Selector,
			ELanguageParameter_ID,
			ELanguageParameter_Password,
			ELanguageParameter_Add,
			ELanguageParameter_ModifyPassword,
			ELanguageParameter_ModifyUserType,
			ELanguageParameter_AddUser,
			ELanguageParameter_Name,
			ELanguageParameter_Type,
			ELanguageParameter_Normal,
			ELanguageParameter_PowerUser,
			ELanguageParameter_Administrator,
			ELanguageParameter_Developer,
			ELanguageParameter_ModifyPasswordOf,
			ELanguageParameter_CurrentPassword,
			ELanguageParameter_NewPassword,
			ELanguageParameter_CheckNewPassword,
			ELanguageParameter_ModifyUserTypeOf,
			ELanguageParameter_DeleteAccountOf,
			ELanguageParameter_Use,
			ELanguageParameter_Enable,
			ELanguageParameter_Disable,
			ELanguageParameter_Title,
			ELanguageParameter_WaitingTime,
			ELanguageParameter_UsedLogin,
			ELanguageParameter_DeleteUser,
			ELanguageParameter_LoggedOn,
			ELanguageParameter_NotLoggedOn,
			ELanguageParameter_Root,
			ELanguageParameter_success,
			ELanguageParameter_unknownerror,
			ELanguageParameter_usernotfound,
			ELanguageParameter_databaseerror,
			ELanguageParameter_incorrectpassword,
			ELanguageParameter_notloggedon,
			ELanguageParameter_accessdenied,
			ELanguageParameter_useralreadyexist,
			ELanguageParameter_User,
			ELanguageParameter_OK,
			ELanguageParameter_Cancel,
			ELanguageParameter_Doyouwanttologout,
			ELanguageParameter_Pleasecheckyourusernameorpasswordagain,
			ELanguageParameter_DeleteROI,
			ELanguageParameter_CopygeometrytotheIPT,
			ELanguageParameter_Copygeometrytotheproject,
			ELanguageParameter_TransfergeometrytotheIPT,
			ELanguageParameter_Transfergeometrytotheproject,
			ELanguageParameter_Operation,
			ELanguageParameter_Rename,
			ELanguageParameter_Script,
			ELanguageParameter_Converttoboundaryrect,
			ELanguageParameter_Typechange,
			ELanguageParameter_Modifygeometry,
			ELanguageParameter_Bringtotop,
			ELanguageParameter_Bringtobottom,
			ELanguageParameter_Removetopoint,
			ELanguageParameter_Converttoarc,
			ELanguageParameter_Converttoline,
			ELanguageParameter_Converttocircle,
			ELanguageParameter_Converttoellipse,
			ELanguageParameter_Converttoarccloseno,
			ELanguageParameter_Converttoarccloseeachother,
			ELanguageParameter_Converttoarcclosecenter,
			ELanguageParameter_Clearviewobject,
			ELanguageParameter_Load,
			ELanguageParameter_Save,
			ELanguageParameter_Createimage,
			ELanguageParameter_CreateROI,
			ELanguageParameter_Renameimage,
			ELanguageParameter_CopyimagefromtheIPT,
			ELanguageParameter_Copyimagefromtheproject,
			ELanguageParameter_CopyimagetotheIPT,
			ELanguageParameter_Copyimagetotheproject,
			ELanguageParameter_Copyimage,
			ELanguageParameter_TransferimagetotheIPT,
			ELanguageParameter_Transferimagetotheproject,
			ELanguageParameter_BringimagefromtheIPT,
			ELanguageParameter_Bringimagefromtheproject,
			ELanguageParameter_Moveimage,
			ELanguageParameter_CopygeometryfromtheIPT,
			ELanguageParameter_Copygeometryfromtheproject,
			ELanguageParameter_BringgeometryfromtheIPT,
			ELanguageParameter_Bringgeometryfromtheproject,
			ELanguageParameter_Movegeometry,
			ELanguageParameter_Inspection,
			ELanguageParameter_Teaching,
			ELanguageParameter_EditTeaching,
			ELanguageParameter_Arrow,
			ELanguageParameter_Zoomin,
			ELanguageParameter_Zoomout,
			ELanguageParameter_Zoomall,
			ELanguageParameter_Panning,
			ELanguageParameter_Measure,
			ELanguageParameter_Displacement,
			ELanguageParameter_Distancecircle,
			ELanguageParameter_Circumcenteroftriangle,
			ELanguageParameter_Paralleldistance,
			ELanguageParameter_Angle,
			ELanguageParameter_Anglemeasurement,
			ELanguageParameter_Azimuth,
			ELanguageParameter_Gradient,
			ELanguageParameter_Area,
			ELanguageParameter_Polygonarea,
			ELanguageParameter_Layer,
			ELanguageParameter_Showgrid,
			ELanguageParameter_Hidegrid,
			ELanguageParameter_Showpixelspectrum,
			ELanguageParameter_Hidepixelspectrum,
			ELanguageParameter_Showpixelvalue,
			ELanguageParameter_Hidepixelvalue,
			ELanguageParameter_Showstatusbar,
			ELanguageParameter_Hidestatusbar,
			ELanguageParameter_Setpixelaccuracy,
			ELanguageParameter_Maximize,
			ELanguageParameter_Restore,
			ELanguageParameter_Pixelaccuracysetting,
			ELanguageParameter_Accuracyvaluecantbezeroornegative,
			ELanguageParameter_Project,
			ELanguageParameter_Selectsource,
			ELanguageParameter_Selectgeometry,
			ELanguageParameter_Source,
			ELanguageParameter_Geometry,
			ELanguageParameter_Image,
			ELanguageParameter_Doyouwanttosavestatusimageprocessingtool,
			ELanguageParameter_Doyoureallywanttoterminatetheprogram,
			ELanguageParameter_No,
			ELanguageParameter_Description,
			ELanguageParameter_1X1Layout,
			ELanguageParameter_1X2Layout,
			ELanguageParameter_2X1Layout,
			ELanguageParameter_2X2Layout,
			ELanguageParameter_2X3Layout,
			ELanguageParameter_3X2Layout,
			ELanguageParameter_3X3Layout,
			ELanguageParameter_Xaxisscale,
			ELanguageParameter_Yaxisscale,
			ELanguageParameter_Xaxisunit,
			ELanguageParameter_Yaxisunit,
			ELanguageParameter_Xaxisgrid,
			ELanguageParameter_Yaxisgrid,
			ELanguageParameter_Crosshair,
			ELanguageParameter_Tooltip,
			ELanguageParameter_Xaxishighlight,
			ELanguageParameter_Yaxishighlight,
			ELanguageParameter_XYaxishighlight,
			ELanguageParameter_Xaxisindicator,
			ELanguageParameter_Yaxisindicator,
			ELanguageParameter_Plotpoint,
			ELanguageParameter_Line,
			ELanguageParameter_Setequation,
			ELanguageParameter_Addleastsquaresdata,
			ELanguageParameter_Removelastleastsquaresdata,
			ELanguageParameter_Clearleastsquaresdata,
			ELanguageParameter_Showleastsquares,
			ELanguageParameter_Hideleastsquares,
			ELanguageParameter_Equation,
			ELanguageParameter_Saveas,
			ELanguageParameter_Licenceinfo,
			ELanguageParameter_Youhavetounlockdatafilefirst,
			ELanguageParameter_Linear,
			ELanguageParameter_Quadratic,
			ELanguageParameter_Cubic,
			ELanguageParameter_Quartic,
			ELanguageParameter_Quintic,
			ELanguageParameter_Sextic,
			ELanguageParameter_Septic,
			ELanguageParameter_Octic,
			ELanguageParameter_Nonic,
			ELanguageParameter_Decic,
			ELanguageParameter_Initialized,
			ELanguageParameter_Terminated,
			ELanguageParameter_Grabbing,
			ELanguageParameter_Living,
			ELanguageParameter_Couldntfind_s_APIdlls,
			ELanguageParameter_Thedevicehasbeenalreadyinitialized,
			ELanguageParameter_DoesntsupporttheDeviceID,
			ELanguageParameter_Couldntcreatetheautomationoutput,
			ELanguageParameter_Couldntcreatetheautomationinput,
			ELanguageParameter_Couldntsupporttheoutput,
			ELanguageParameter_Couldntsupporttheinput,
			ELanguageParameter_CouldntfindIOport,
			ELanguageParameter_Couldntfindthedeviceofoutput,
			ELanguageParameter_Couldntfindthedeviceofinput,
			ELanguageParameter_Failedtoselectdeviceofoutput,
			ELanguageParameter_Failedtoselectdeviceofinput,
			ELanguageParameter_Failedtowrite_s_tothedevice,
			ELanguageParameter_Failedtoread_s_fromthedevice,
			ELanguageParameter_Failedtowrite_s_tothedatabase,
			ELanguageParameter_Failedtoread_s_fromthedatabase,
			ELanguageParameter_TheDevicehasbeensuccessfullyinitialized,
			ELanguageParameter_TheDevicehasbeensuccessfullyterminated,
			ELanguageParameter_Failedtofindtheparameter,
			ELanguageParameter__s_tochangetheparameter_s_from_s_to_s,
			ELanguageParameter__s_d_s_toload_s,
			ELanguageParameter_Couldntopenthelibrary,
			ELanguageParameter_CouldntfindIOmodule,
			ELanguageParameter_Ithasalreadybeeninitializedtoanotherdevice,
			ELanguageParameter_DoesntsupporttheSubUnitID,
			ELanguageParameter_Theaxisisnotvalid,
			ELanguageParameter__s_tochangetheparameter_s_from_f_to_f,
			ELanguageParameter_Failedtofindthedevice,
			ELanguageParameter_Failedtofindthemoduleofdevice,
			ELanguageParameter_Couldntcontrolthedevice,
			ELanguageParameter_Failedtoconverttriggerposition,
			ELanguageParameter_Triggerisenabled,
			ELanguageParameter_Triggerisdisabled,
			ELanguageParameter_Counterisreset,
			ELanguageParameter_Couldntopenthedevice,
			ELanguageParameter_CouldntinitializetheIOmodule,
			ELanguageParameter_CouldntopentheIOmodule,
			ELanguageParameter_FailedtoinitializeCAMCFS10module,
			ELanguageParameter_FailedtoinitializeCAMCFS20module,
			ELanguageParameter_FailedtoinitializeCNTmodule,
			ELanguageParameter_Failedtogeneratethedevicehandle,
			ELanguageParameter_Failedtocreateimagebuffer,
			ELanguageParameter_Ithasnotbeeninitializedyet,
			ELanguageParameter_Ithasbeenlivingorgrabbing,
			ELanguageParameter_Failedtocreatethread,
			ELanguageParameter_Succeededtoexecutecommand_s,
			ELanguageParameter_Thedevicewasalreadystop,
			ELanguageParameter_Couldntclosethedevice,
			ELanguageParameter_Failedtofreethedevice,
			ELanguageParameter__s_tochangetheparameter_s_from_d_to_d,
			ELanguageParameter_Couldntsupportthe_s,
			ELanguageParameter_CouldntfindthesameSN,
			ELanguageParameter_Itcantrunthex86processonthex64OS,
			ELanguageParameter_Failedtocreateport,
			ELanguageParameter_FailedtocreateIOevent,
			ELanguageParameter_Failedtoinitializeport,
			ELanguageParameter_Failedtosetportevent,
			ELanguageParameter_Failedtoclearport,
			ELanguageParameter_Failedtosetcommunicationparameter,
			ELanguageParameter_RS232CdatasendingerrorErrorcode_0x08x,
			ELanguageParameter__s_tochangetheparameter_s_from_u_to_u,
			ELanguageParameter_Itcantrunthex64process,
			ELanguageParameter_Tryingtoconnecttoserver,
			ELanguageParameter_Failedtoconnecttoserver,
			ELanguageParameter_ServerIP_s_Port_s,
			ELanguageParameter_Timeout,
			ELanguageParameter_Failedtosendapackettotheserver,
			ELanguageParameter_Sendapackettotheserverhasbeencompleted,
			ELanguageParameter_Couldntreaddata,
			ELanguageParameter_Connectedtotheserversuccessfully,
			ELanguageParameter_Unplugged,
			ELanguageParameter_Serverconnectionislost,
			ELanguageParameter_Bufferisntenough,
			ELanguageParameter_Connectionreset,
			ELanguageParameter_Connectionaborted,
			ELanguageParameter_Invalidsocket,
			ELanguageParameter_Itsnotconnected,
			ELanguageParameter_Unknwonerroroccurscode_0x08x,
			ELanguageParameter_Failedtosendapackettoallclients,
			ELanguageParameter_Failedtosendapackettoparticularclients,
			ELanguageParameter_Sendapackettoalloftheclientshasbeencompleted,
			ELanguageParameter_Failedtosendapackettotheclient,
			ELanguageParameter_Sendapackettotheclienthasbeencompleted,
			ELanguageParameter_Aclienthasconnected,
			ELanguageParameter_Aclienthasdisconnected,
			ELanguageParameter_Aclientbufferisntenough,
			ELanguageParameter_Aclientconnectionreset,
			ELanguageParameter_Aclientconnectionaborted,
			ELanguageParameter_Aclienthasinvalidsocket,
			ELanguageParameter_Aclientisntconnected,
			ELanguageParameter_Aclientunknwonerroroccurscode_0x08x,
			ELanguageParameter_IP_s_Port_d,
			ELanguageParameter_Theserverisestablishedsuccessfully,
			ELanguageParameter_BindingIP_s_BindingPort_s,
			ELanguageParameter_Serverhasdisconnected,
			ELanguageParameter_Theserveristerminatedsuccessfully,
			ELanguageParameter_User_s_hasbeenloggedon,
			ELanguageParameter_User_s_hasbeenloggedoff,
			ELanguageParameter_Themodelinformationdoesntexist,
			ELanguageParameter_Themodelinformationisinvalid,
			ELanguageParameter_InputnumberisnegativenumberModelnumberhastobepositivenumber,
			ELanguageParameter_SamemodelnumberfoundModelnumberhastobeunique,
			ELanguageParameter_Failedtochangemodelnumber,
			ELanguageParameter_Failedtochangemodelname,
			ELanguageParameter_Failedtochangemodeldesciption,
			ELanguageParameter_Notice,
			ELanguageParameter_Active,
			ELanguageParameter_Failedtoopenthemodel,
			ELanguageParameter_Failedtoclosethemodel,
			ELanguageParameter_Selectmodeltype,
			ELanguageParameter_Doyoureallywanttodeletethemodel,
			ELanguageParameter_Failedtodeletethemodel,
			ELanguageParameter_Doyoureallywanttocopythemodel,
			ELanguageParameter_Failedtocopythemodel,
			ELanguageParameter_Modeltype,
			ELanguageParameter_Concurrentsize,
			ELanguageParameter_ModelConfiguration,
			ELanguageParameter_Doyoureallywanttomodifythemodeltype,
			ELanguageParameter_Doyoureallywanttodeletethemodeltype,
			ELanguageParameter_Modify,
			ELanguageParameter_Showstruct,
			ELanguageParameter_Configurationsetting,
			ELanguageParameter_Addparameter,
			ELanguageParameter_Deleteparameter,
			ELanguageParameter_Modeltypenameerror,
			ELanguageParameter_Property,
			ELanguageParameter_Data,
			ELanguageParameter__UniqueID_lu__hasfailed,
			ELanguageParameter__Datatype_error,
			ELanguageParameter__Datatype_s__hasfailed,
			ELanguageParameter__Variablename_s__hasfailed,
			ELanguageParameter__Inputboxtype_error,
			ELanguageParameter__Inputboxtype_s__hasfailed,
			ELanguageParameter_UniqueIDerror,
			ELanguageParameter_Datatypeerror,
			ELanguageParameter_Variablenameerror,
			ELanguageParameter_Inputboxtypeerror,
			ELanguageParameter_UniqueID,
			ELanguageParameter_Datatype,
			ELanguageParameter_Variablename,
			ELanguageParameter_Defaultvalue,
			ELanguageParameter_Inputboxtype,
			ELanguageParameter_Extradata,
			ELanguageParameter_Decimalcount,
			ELanguageParameter_SaveROItoStorage,
			ELanguageParameter_Closealltools,
			ELanguageParameter_Loadimage,
			ELanguageParameter_Saveimage,
			ELanguageParameter_Closeimage,
			ELanguageParameter_ShowROIStorage,
			ELanguageParameter_Loadimagesinfolder,
			ELanguageParameter_Closeallimages,
			ELanguageParameter_Showtool,
			ELanguageParameter_Hidetool,
			ELanguageParameter_Closetool,
			ELanguageParameter_Selectaimagefilepath,
			ELanguageParameter_Select,
			ELanguageParameter_Couldntfindviewtoprocess,
			ELanguageParameter_Imageview,
			ELanguageParameter_Template,
			ELanguageParameter_SizeX,
			ELanguageParameter_SizeY,
			ELanguageParameter_Fillvalue,
			ELanguageParameter_Channels,
			ELanguageParameter_Depth,
			ELanguageParameter_ROIstorage,
			ELanguageParameter_Openimage,
			ELanguageParameter_Inspect,
			ELanguageParameter_Zoomfit,
			ELanguageParameter_Grid,
			ELanguageParameter_Pixelspectrum,
			ELanguageParameter_Pixelvalue,
			ELanguageParameter_Statusbar,
			ELanguageParameter_Undo,
			ELanguageParameter_Redo,
			ELanguageParameter_Devicemanager,
			ELanguageParameter_Screensaver,
			ELanguageParameter_SequenceStart,
			ELanguageParameter_SequencePause,
			ELanguageParameter_SequenceStop,
			ELanguageParameter_UIconfiguration,
			ELanguageParameter_Openlogviewer,
			ELanguageParameter_Closelogviewer,
			ELanguageParameter_ImageProcessingTool,
			ELanguageParameter_Viewconfigurationtool,
			ELanguageParameter_Deviceconfigurationtool,
			ELanguageParameter_Frameworkconfigurationtool,
			ELanguageParameter_Security,
			ELanguageParameter_Index,
			ELanguageParameter_Shape,
			ELanguageParameter_CenterX,
			ELanguageParameter_CenterY,
			ELanguageParameter_Intersection,
			ELanguageParameter_Union,
			ELanguageParameter_Subtraction,
			ELanguageParameter_Exclusiveor,
			ELanguageParameter_operation,
			ELanguageParameter_ModifyType,
			ELanguageParameter_ModifyValue,
			ELanguageParameter_Apply,
			ELanguageParameter_Preview,
			ELanguageParameter_Parameter,
			ELanguageParameter_Offset,
			ELanguageParameter_Point,
			ELanguageParameter_Left,
			ELanguageParameter_Right,
			ELanguageParameter_Top,
			ELanguageParameter_Bottom,
			ELanguageParameter_Radius,
			ELanguageParameter_Destination,
			ELanguageParameter_Execute,
			ELanguageParameter_Tool,					// 툴
			ELanguageParameter_CannyEdgeDetector,		// 캐니 에지 검출기
			ELanguageParameter_Convolution,				// 컬볼루션
			ELanguageParameter_Crop,					// 크롭
			ELanguageParameter_FFT,						// 빠른 퓨리에 변환
			ELanguageParameter_Flip,					// 플립
			ELanguageParameter_Gain,					// 게인
			ELanguageParameter_HarrisCornerDetector,	// 해리스 코너 검출기
			ELanguageParameter_Histogram,				// 히스토그램
			ELanguageParameter_HoughTransformCircle,	// 허프 변환 [원]
			ELanguageParameter_HoughTransformLine,		// 허프 변환 [선]
			ELanguageParameter_Mask,					// 마스크
			ELanguageParameter_Morphology,				// 모폴로지
			ELanguageParameter_Move,					// 이동
			ELanguageParameter_Normalize,				// 정규화
			ELanguageParameter_Paste,					// 페이스트
			ELanguageParameter_Projection,				// 프로젝션
			ELanguageParameter_Rotate,					// 회전 변환
			ELanguageParameter_Scale,					// 크기 변환
			ELanguageParameter_Statistics,				// 통계
			ELanguageParameter_Stretch,					// 스트레치
			ELanguageParameter_Threshold,				// 이진화
			ELanguageParameter_Conversion,				// 전환
			ELanguageParameter_ColorConversion,			// 컬러
			ELanguageParameter_Integral,				// 인테그랄
			ELanguageParameter_Object,
			ELanguageParameter_ConnectedComponent,		// 커넥티드-컴포넌트
			ELanguageParameter_Gauge,					// 게이지
			ELanguageParameter_PointGauge,
			ELanguageParameter_LineGauge,
			ELanguageParameter_RectangleGauge,
			ELanguageParameter_QuadrangleGauge,
			ELanguageParameter_CircleGauge,
			ELanguageParameter_EllipseGauge,
			ELanguageParameter_Code,					// 코드
			ELanguageParameter_BarcodeDecoder,			// 바코드 디코더
			ELanguageParameter_BarcodeEncoder,			// 바코드 인코더
			ELanguageParameter_DataMatrixDecoder,		// 자료정렬 디코더
			ELanguageParameter_DataMatrixEncoder,		// 자료정렬 인코더
			ELanguageParameter_QRcodeDecoder,
			ELanguageParameter_QRcodeEncoder,
			ELanguageParameter_Find,					// 파인드
			ELanguageParameter_ObjectFinder,			// 오브젝트파인더
			ELanguageParameter_View,					// 뷰
			ELanguageParameter_FailedToExecute,			// 실행에 실패했습니다.
			ELanguageParameter_ElapsedTime,				// 경과된 시간
			ELanguageParameter_Result,					// 결과
			ELanguageParameter_SucceedToExecute,		// 실행에 성공했습니다.
				// AfxMessage
			ELanguageParameter_Error_n_ModelParameterFormatError_n_FieldName_s_OfFieldIndex_d_DoesntExist,
			ELanguageParameter_Error_n_ModelParameterFormatError_n_FieldName_s_DoesntExist_nn_DBInformation_n_FieldIndex_d,
			ELanguageParameter_Error_n_ModelParameterFormatError_n_FieldToReadFieldType_nn_DBInformation_n_FieldIndex_d,
			ELanguageParameter_Error_n_ModelParameterFormatError_n_FieldName_s_DoesntMatchDBFieldName_nn_DBInformation_n_FieldIndex_dn_FieldType_sn_FieldName_s,
			ELanguageParameter_Error_n_ModelParameterFormatError_n_FieldType_s_DoesntMatchDBFieldType_nn_DBInformation_n_FieldIndex_dn_FieldType_sn_FieldName_s,
			ELanguageParameter_Error_n_ModelParameterFormatError_n_InvalidDBFieldType,
			ELanguageParameter_Error_n_ModelParameterReadingError_n_TheDataTypeIsUndefinedInMVTechRAVIDFramework_nn_DBInformation_n_Sequence_dn_ParameterName_s_DataType_s,
			ELanguageParameter_Error_n_ModelParameterElementCantBeAPointerVariable_nn_DataPositionInStructure_ndnn_DataTypeInStructure_nsnn_DBInformation_n_Sequence_dn_ParameterName_sn_DataType_s,
			ELanguageParameter_Error_n_DataTypeOfTheModelParameterElementDoesntMatch_nn_DataPositionInStructure_ndnn_DataTypeInStructure_nsnn_DBInformation_n_Sequence_dn_ParameterName_sn_DataType_s,
			ELanguageParameter_Error_n_UnknownError_nn_DataPositionInStructure_ndnn_DataTypeInStructure_nsnn_DBInformation_n_Sequence_dn_ParameterName_sn_DataType_s,
			ELanguageParameter_Error_n_ModelParameterStructureIsGreaterThanModelParameterDatabase,
			ELanguageParameter_Error_n_ModelParameterStructureIsLessThanModelParameterDatabase,
			ELanguageParameter_GeometryLocatedAtOutOfImageDoYouWantToResizeIt,
			ELanguageParameter_FindingCountAllIsNotYetSupported,
			ELanguageParameter_ThisModeIsNotYetSupported,
			ELanguageParameter_CodeIsOutOfLengthPleaseInput11Numbers,
			ELanguageParameter_InvalidParameter,
			ELanguageParameter_PleaseEnterAnOddNumber,
			ELanguageParameter_CodeIsOutOfLength,
			ELanguageParameter_HoughTransformNeedsMonoImage_n_SoYouMustProcessCannyEdgeDetectorToThisImageBeforeRunningHoughTransform_n_IsItMonoImage,
			ELanguageParameter_FailedToLoad_s,
			ELanguageParameter_DoYouWantToSave,
			ELanguageParameter_TheSameTitleAlreadyExists,
			ELanguageParameter_ReallyRemove,
			ELanguageParameter_ReallyClear,
			ELanguageParameter_DuplicateTitle,
			ELanguageParameter_YouCanTryItLater,
			ELanguageParameter_FailedToChangeDataFileKey,
			ELanguageParameter_FailedToUnlockDataFile,
			ELanguageParameter_FailedToSetDataFileKey,
			ELanguageParameter_OLEInitializationFailedMakeSureThatTheOLELibrariesAreTheCorrectVersion,
			ELanguageParameter_UnableToSaveGridList,
			ELanguageParameter_UnableToLoadGridData,
			ELanguageParameter_DepthConversion,	// 픽셀 깊이
			ELanguageParameter_UserAlreadyLoggedOn,
			ELanguageParameter_ShowScrollbar,
			ELanguageParameter_HideScrollbar,
			ELanguageParameter_Learn,
			ELanguageParameter_AddFilter,
			ELanguageParameter_DeleteFilter,
			ELanguageParameter_Results,
			ELanguageParameter_Drawings,
			ELanguageParameter_Item,
			ELanguageParameter_Condition,
			ELanguageParameter_BlobResultOption,
			ELanguageParameter_BlobResultDrawing,
			ELanguageParameter_Source1,
			ELanguageParameter_Source2,
			ELanguageParameter_SuccededToLoad,
			ELanguageParameter_FailedToLoad,
			ELanguageParameter_Processing,
			ELanguageParameter_SucceededToLearn,
			ELanguageParameter_FailedToLearn,
			ELanguageParameter_LanguageChange,							   				
			ELanguageParameter_SucceededToSave,
			ELanguageParameter_FailedToSave,
			ELanguageParameter_AvailableView,
			ELanguageParameter_RegisteredView,
			ELanguageParameter_UIConfigurationMode,
			ELanguageParameter_ElementView,
			ELanguageParameter_ViewRegisterMode,
			ELanguageParameter_SetDataFileKey,
			ELanguageParameter_Key,
			ELanguageParameter_CheckKey,
			ELanguageParameter_UnlockDataFile,
			ELanguageParameter_ChangeDataFileKey,
			ELanguageParameter_CurrentKey,
			ELanguageParameter_NewKey,
			ELanguageParameter_CheckNewKey,
			ELanguageParameter_AvailableDevice,
			ELanguageParameter_RegisteredDevice,
			ELanguageParameter_Scrollbar,
			ELanguageParameter_ConfigurationNew,
			ELanguageParameter_Duplicate,
			ELanguageParameter_Doyoureallywanttocopythemodeltype,
			ELanguageParameter_ConfigurationTitle,
			ELanguageParameter_AddElement,
			ELanguageParameter_ViewName,
			ELanguageParameter_InspectionMenu,
			ELanguageParameter_TeachingMenu,
			ELanguageParameter_PleaseEnterTheTitle,
			ELanguageParameter_ItisTheSameAsTheExistingTitle,
			ELanguageParameter_PositionPercentage,
			ELanguageParameter_PositionCoordinate,
			ELanguageParameter_GetThreshold,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Scale, //RavidGraphCtrl
			ELanguageParameter_RavidGraphCtrlPopupMenu_MouseWheel,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Fit,
			ELanguageParameter_RavidGraphCtrlPopupMenu_All,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Screen,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Set,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Show,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Grid,
			ELanguageParameter_RavidGraphCtrlPopupMenu_ValueScale,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Legend,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Crosshair,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Location,
			ELanguageParameter_RavidGraphCtrlPopupMenu_Value,
			ELanguageParameter_ToolHistogramTitle, //ToolHistogram
			ELanguageParameter_ToolHistogramXAxis,
			ELanguageParameter_ToolHistogramYAxis,
			ELanguageParameter_ToolProjection_Title_Row, //ToolProjection
			ELanguageParameter_ToolProjection_Title_Col,
			ELanguageParameter_ToolProjection_Title_Angle,
			ELanguageParameter_ToolProjection_Unit_Row,
			ELanguageParameter_ToolProjection_Unit_Col,
			ELanguageParameter_ToolProjection_Unit_Angle,
			ELanguageParameter_ToolProjection_Unit_Index,
			ELanguageParameter_RavidGraphCtrlPopupMenu_XInputBox,  //RavidGraphCtrl
			ELanguageParameter_RavidGraphCtrlPopupMenu_XInputBox_Min,
			ELanguageParameter_RavidGraphCtrlPopupMenu_XInputBox_Max,
			ELanguageParameter_RavidGraphCtrlPopupMenu_YInputBox,
			ELanguageParameter_RavidGraphCtrlPopupMenu_YInputBox_Min,
			ELanguageParameter_RavidGraphCtrlPopupMenu_YInputBox_Max,
			ELanguageParameter_RavidGraphCtrlPopupMenu_InputBox_Error,
			ELanguageParameter_RavidGraphCtrlPopupMenu_InputBox_ValidError,
			ELanguageParameter_RavidGraphCtrlPopupMenu_InputBox_ValidScaleError,
			ELanguageParameter_OpticalCharacterRecognizer, //OCR
			ELanguageParameter_OCR,    
			ELanguageParamter_OCR_Preprocess,
			ELanguageParameter_OCR_Save,
			ELanguageParameter_OCR_Learn,
			ELanguageParamter_OCR_Load,
			ELanguageParameter_OCR_Decode,
			ELanguageParameter_Extraction,
			ELanguageParameter_Channel,
			ELanguageParameter_Close_Shortcut,
			ELanguageParameter_Save_Shortcut,
			ELanguageParameter_CreateImage_Shortcut,
			ELanguageParameter_CreateROI_Shortcut,
			ELanguageParameter_RenameImage_Shortcut,
			ELanguageParameter_CopyImage_Shortcut,
			ELanguageParameter_MoveImage_Shortcut,
			ELanguageParameter_MoveGeometry_Shortcut,
			ELanguageParameter_Arrow_Shortcut,
			ELanguageParameter_ZoomIn_Shortcut,
			ELanguageParameter_ZoomOut_Shortcut,
			ELanguageParameter_Measure_Shortcut,
			ELanguageParameter_Layer_Shortcut,
			ELanguageParameter_ShowGrid_Shortcut,
			ELanguageParameter_HideGrid_Shortcut,
			ELanguageParameter_ShowPixelSpectrum_Shortcut,
			ELanguageParameter_HidePixelSpectrum_Shortcut,
			ELanguageParameter_ShowPixelValue_Shortcut,
			ELanguageParameter_HidePixelValue_Shortcut,
			ELanguageParameter_ShowStatusBar_Shortcut,
			ELanguageParameter_HideStatusBar_Shortcut,
			ELanguageParameter_SetPixelAccuracy_Shortcut,
			ELanguageParameter_Maximize_Shortcut,
			ELanguageParameter_Restore_Shortcut,
			ELanguageParameter_ShowROIStorage_Shortcut,
			ELanguageParameter_ZoomFit_Shortcut,
			ELanguageParameter_ShowScrollBar_Shortcut,
			ELanguageParameter_HideScrollBar_Shortcut,
			ELanguageParameter_Panning_Shortcut,
			ELanguageParameter_Inspection_Shortcut,
			ELanguageParameter_Teaching_Shortcut,
			ELanguageParameter_Open_Shortcut,
			ELanguageParameter_Distance_Shortcut,
			ELanguageParameter_Displacement_Shortcut,
			ELanguageParameter_Distancecircle_Shortcut,
			ELanguageParameter_Circumcenteroftriangle_Shortcut,
			ELanguageParameter_Paralleldistance_Shortcut,
			ELanguageParameter_Angle_Shortcut,
			ELanguageParameter_Anglemeasurement_Shortcut,
			ELanguageParameter_Azimuth_Shortcut,
			ELanguageParameter_Gradient_Shortcut,
			ELanguageParameter_Area_Shortcut,
			ELanguageParameter_Polygonarea_Shortcut,
			ELanguageParameter_LoadImage_Shortcut,
			ELanguageParameter_LoadImagesInFolder_Shortcut,
			ELanguageParameter_CloseAllImages_Shortcut,
			ELanguageParameter_CloseAllTools_Shortcut,
			ELanguageParameter_SaveImage_Shortcut,
			ELanguageParameter_CloseImage_Shortcut,
			ELanguageParameter_Rename_Shortcut,
			ELanguageParameter_SaveROIToStorage_Shortcut,
			ELanguageParameter_DeleteROI_Shortcut,
			ELanguageParameter_Operation_Shortcut,
			ELanguageParameter_Script_Shortcut,
			ELanguageParameter_ConvertToBoundaryRect_Shortcut,
			ELanguageParameter_TypeChange_Shortcut,
			ELanguageParameter_ModifyGeometry_Shortcut,
			ELanguageParameter_ShowTool_Shortcut,
			ELanguageParameter_HideTool_Shortcut,
			ELanguageParameter_CloseTool_Shortcut,
			ELanguageParameter_Delete_Shortcut,
			ELanguageParameter_Configuration_Shortcut,
			ELanguageParameter_Modify_Shortcut,
			ELanguageParameter_ShowStruct_Shortcut,
			ELanguageParameter_Duplicate_Shortcut,
			ELanguageParameter_Add_Shortcut,
			ELanguageParameter_ModifyPassword_Shortcut,
			ELanguageParameter_ModifyUserType_Shortcut,
			ELanguageParameter_SaveAs_Shortcut,
			ELanguageParameter_DeleteFilter_Shortcut,
			ELanguageParameter_AddFilter_Shortcut,
			ELanguageParameter_Results_Shortcut,
			ELanguageParameter_Drawings_Shortcut,
			ELanguageParameter_Learn_Shortcut, 
			ELanguageParameter_Training_Shortcut,
			ELanguageParameter_Decoding_Shortcut,
			ELanguageParameter_Source_Shortcut,
			ELanguageParameter_Source1_Shortcut,
			ELanguageParameter_Source2_Shortcut,
			ELanguageParameter_Destination_Shortcut,
			ELanguageParameter_ObjectFinder_Learn_Shortcut,
			ELanguageParameter_Load_Shortcut,
			ELanguageParameter_Save_Shortcut2,
			ELanguageParameter_InspectionMenu_Shortcut,
			ELanguageParameter_TeachingMenu_Shortcut,
			ELanguageParameter_Title_Shortcut,
			ELanguageParameter_AddMenu_Shortcut,
			ELanguageParameter_DeleteMenu_Shortcut,
			ELanguageParameter_ModifyMenu_Shortcut,
			ELanguageParameter_AddElement_Shortcut,
			ELanguageParameter_DeleteElement_Shortcut,
			ELanguagaParameter_ModifyElement_Shortcut,
			ELanguageParameter_UIConfigurationTool_Shortcut,
			ELanguageParameter_ViewRegisterMode_Shortcut,
			ELanguageParameter_OCR_Load_Shortcut,
			ELanguageParameter_Update_Shortcut,
			ELanguageParameter_Cut_Shortcut,
			ELanguageParameter_Paste_Shortcut,
			ELanguageParameter_Copy_Shortcut,
			ELanguageParameter_ShapeFinder,
			ELanguageParameter_InvertValue,				// 색상반전
			ELanguageParameter_ReviseParameterName,
			ELanguageParameter_ArrayTypeDefaultParameterName,
			ELanguageParameter_ArrayTypeUserParameterName,
			ELanguageParameter_NoUp,
			ELanguageParameter_NoDown,
			ELanguageParameter_NameUp,
			ELanguageParameter_NameDown,
			ELanguageParameter_TypeUp,
			ELanguageParameter_TypeDown,
			ELanguageParameter_DescriptionUp,
			ELanguageParameter_DescriptionDown,
			ELanguageParameter_StatusUp,
			ELanguageParameter_StatusDown,
			ELanguageParameter_Search,
			ELanguageParameter_Refresh,
			ELanguageParameter_OnInvalidate_Shortcut,
			ELanguageParameter_OffInvalidate_Shortcut,
			ELanguageParameter_Invalidate,
			ELanguageParameter_UserStructParameterProperty,
			ELanguageParameter_ModifyAuthority,
			ELanguageParameter_Count,
		};
	}
}


namespace Ravid
{
	namespace Database
	{
		class CRavidDatabase;
	}

	namespace Framework
	{
		enum ELanguageType
		{
			ELanguageType_English = 0,
			ELanguageType_Korean,
			ELanguageType_Chinese,
			ELanguageType_Customizing,
			ELanguageType_Count,
		};
		
		/**
		* 프레임워크 언어를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CMultiLanguageManager : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CMultiLanguageManager);

		private:
			CMultiLanguageManager();

		public:
			virtual ~CMultiLanguageManager();

			static bool Initialize(_In_ LPCTSTR lpszLanguageRootPath);
			static bool Terminate();

			static bool IsInitialized();

			// 유효한 언어들을 얻어오는 함수
			static bool GetAvailableLanguageNames(_Out_ std::vector<CString>& vctLanguageNames);

			// 언어 선택 함수
			static EMultiLanguageSelect SelectLanguage(_In_ CString strLanguageName);

			// 현재 선택된 언어 반환 함수
			static CString GetSelectedLanguage();

			// 현재 선택된 언어의 문자열 테이블을 반환하는 함수
			static CString* GetStrings();

			// 현재 선택된 언어에서 해당하는 인데스의 문자열 반환 함수
			static CString GetString(_In_ int nIndex);

			// 현재 선택된 언어에서 해당 내용에 맞는 문자열의 인덱스들을 얻어오는 함수
			static bool FindStringIndexes(_In_ CString strContent, _Out_ std::vector<int>& vctIndexes, _In_opt_ bool bExact = false);

			// 해당 언어에서 해당 내용에 맞는 문자열의 인덱스들을 얻어오는 함수
			static bool FindStringIndexes(_In_ CString strLanguageName, _In_ CString strContent, _Out_ std::vector<int>& vctIndexes, _In_opt_ bool bExact = false);

			// 현재 선택된 언어에서 해당 내용에 맞는 문자열들을 얻어오는 함수
			static bool FindStrings(_In_ CString strContent, _Out_ std::vector<CString>& vctStrings, _In_opt_ bool bExact = false);

			// 해당 언어에서 해당 내용에 맞는 문자열들을 얻어오는 함수
			static bool FindStrings(_In_ CString strLanguageName, _In_ CString strContent, _Out_ std::vector<CString>& vctStrings, _In_opt_ bool bExact = false);

			// 문자열 테이블 개수를 반환하는 함수
			static int GetStringCount();

			// MessageBox용
			static USHORT GetOSMainSelectedLanguage();
			static UCHAR GetOSSubSelectedLanguage();

			static CString ConvertNewLineString(_In_ CString strMessage);

		private:
			static bool CreateFrameworkTable();

			static bool UpdateFramework(_In_opt_ CString strParameter, _In_opt_ CString strValue = _T(""));

			static bool LoadLanguageData();

			static bool LoadDefaultLanguage();

			static CMultiLanguageManager* GetInstance();

			std::vector<std::pair<CString, Ravid::Database::CRavidDatabase*>> m_vctDatabase;

			std::map<CString, CString*> m_mapLanguages;

			CString* m_pStrSelectedLanguage = nullptr;

			CString m_strSelectedLenguage = _T("English");

			volatile bool m_bInitialized = false;

			volatile bool m_bPreSelectLanguage = false;

			volatile USHORT m_usOSMainSelectLanguage = LANG_ENGLISH;
			volatile UCHAR m_ucOSSubSelectLanguage = SUBLANG_ENGLISH_US;
		};
	}
}

