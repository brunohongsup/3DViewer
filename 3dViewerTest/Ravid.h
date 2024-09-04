#pragma once

// RavidFramework DLLs

#include "RavidCore/LibTags.h"

//////////////////////////////////////////////////////////////////////////


// Definitions

#include "RavidCore/Definitions.h"
#include "RavidFramework/Definitions.h"
#include "Ravid3D/Definitions.h"

//////////////////////////////////////////////////////////////////////////
// Core Algorithms

#include "RavidCore/BarcodeDecoder.h"
#include "RavidCore/BarcodeEncoder.h"
#include "RavidCore/CircleGauge.h"
#include "RavidCore/ColorSpace.h"
#include "RavidCore/ConnectedComponent.h"
#include "RavidCore/ConvolutionPreset.h"
#include "RavidCore/CoordinateMapper.h"
#include "RavidCore/DataMatrixDecoder.h"
#include "RavidCore/DataMatrixEncoder.h"
#include "RavidCore/EllipseGauge.h"
#include "RavidCore/HoughTransformLine.h"
#include "RavidCore/HoughTransformCircle.h"
#include "RavidCore/LineGauge.h"
#include "RavidCore/ObjectFinder.h"
#include "RavidCore/OpticalCharacterRecognizer.h"
#include "RavidCore/PerspectiveProjection.h"
#include "RavidCore/PointGauge.h"
#include "RavidCore/QRCodeDecoder.h"
#include "RavidCore/QRCodeEncoder.h"
#include "RavidCore/QuadrangleGauge.h"
#include "RavidCore/RectangleGauge.h"
#include "RavidCore/RavidImage3DProfile.h"
#include "RavidCore/ShapeFinder.h"
#include "RavidCore/GridCalibration.h"

//////////////////////////////////////////////////////////////////////////
// Core Database

#include "RavidCore/RavidDatabase.h"
#include "RavidCore/RavidRecordset.h"
#include "RavidCore/RavidRecordsetFieldInformation.h"

//////////////////////////////////////////////////////////////////////////
// Core Framework

#include "RavidCore/ThreadHelper.h"
#include "RavidCore/MultipleCondition.h"
#include "RavidCore/MultipleVariable.h"

//////////////////////////////////////////////////////////////////////////
// Core Geometries

#include "RavidCore/RavidCircle.h"
#include "RavidCore/RavidEllipse.h"
#include "RavidCore/RavidGeometry.h"
#include "RavidCore/RavidGeometryArray.h"
#include "RavidCore/RavidLine.h"
#include "RavidCore/RavidPoint.h"
#include "RavidCore/RavidPolygon.h"
#include "RavidCore/RavidQuadrangle.h"
#include "RavidCore/RavidRect.h"

//////////////////////////////////////////////////////////////////////////
// Core Image

#include "RavidCore/RavidImage.h"
#include "RavidCore/ImageOperationInfo.h"
#include "RavidCore/ImageDistortionCorrection.h"

//////////////////////////////////////////////////////////////////////////
// Mathematics

#include "RavidCore/Classifier.h"
#include "RavidCore/Equation.h"
#include "RavidCore/FiniteField.h"
#include "RavidCore/LeastSquares.h"
#include "RavidCore/Matrix.h"
#include "RavidCore/ReedSolomon.h"
#include "RavidCore/Statistics.h"

//////////////////////////////////////////////////////////////////////////
// Miscellaneous

#include "RavidCore/CadImage.h"
#include "RavidCore/CyclicRedundancyCheck.h"
#include "RavidCore/DxfReader.h"
#include "RavidCore/FileUtility.h"
#include "RavidCore/HardwareInfo.h"
#include "RavidCore/NumberFormat.h"
#include "RavidCore/OperatingSystemInfo.h"
#include "RavidCore/PerformanceCounter.h"
#include "RavidCore/SimpleEncryptor.h"

//////////////////////////////////////////////////////////////////////////
// Device

#include "RavidFramework/DeviceAdvantechDIO.h"
#include "RavidFramework/DeviceAlliedGigE.h"
#include "RavidFramework/DeviceAxlDIO.h"
#include "RavidFramework/DeviceAxlMotion.h"
#include "RavidFramework/DeviceAxlMotionEthercat.h"
#include "RavidFramework/DeviceAxlTriggerSIOCN2CH.h"
#include "RavidFramework/DeviceAxtDIO.h"
#include "RavidFramework/DeviceAxtMotionFS10.h"
#include "RavidFramework/DeviceAxtMotionFS20.h"
#include "RavidFramework/DeviceBaslerGigE.h"
#include "RavidFramework/DeviceBaslerTOFGigE.h"
#include "RavidFramework/DeviceBaslerUSB.h"
#include "RavidFramework/DeviceBaumerGigE.h"
#include "RavidFramework/DeviceCrevisGigE.h"
#include "RavidFramework/DeviceDahuaGigE.h"
#include "RavidFramework/DeviceDalsaGenieNanoGigE.h"
#include "RavidFramework/DeviceEuresysDominoHarmony.h"
#include "RavidFramework/DeviceEuresysDominoMelody.h"
#include "RavidFramework/DeviceEuresysDominoSymphony.h"
#include "RavidFramework/DeviceEuresysCoaxlinkHX.h"
#include "RavidFramework/DeviceEuresysGrablinkBase.h"
#include "RavidFramework/DeviceEuresysGrablinkDualBase.h"
#include "RavidFramework/DeviceEuresysGrablinkFull.h"
#include "RavidFramework/DeviceEuresysGrablinkValue.h"
#include "RavidFramework/DeviceEziPlusEDio.h"
#include "RavidFramework/DeviceEziServo2PlusE.h"
#include "RavidFramework/DeviceGenICam.h"
#include "RavidFramework/DeviceHIKVisionGigE.h"
#include "RavidFramework/DeviceMatroxSolios.h"
#include "RavidFramework/DeviceMeereTOFGigE.h"
#include "RavidFramework/DeviceMFB_I.h"
#include "RavidFramework/DeviceMFB_T.h"
#include "RavidFramework/DeviceMVTechMFB_I.h"
#include "RavidFramework/DeviceMVTechMFB_T.h"
#include "RavidFramework/DeviceMVTechDio.h"
#include "RavidFramework/DeviceMVTechMSPDio.h"
#include "RavidFramework/DeviceMVTechMSPTrigger.h"
#include "RavidFramework/DeviceMVTechTrigger.h"
#include "RavidFramework/DevicePhotonfocus3D.h"
#include "RavidFramework/DevicePointGrey.h"
#include "RavidFramework/DeviceRS232C.h"
#include "RavidFramework/DeviceSensovationGigE.h"
#include "RavidFramework/DeviceSocketMNETG.h"
#include "RavidFramework/DeviceSoftservoWMX2.h"
#include "RavidFramework/DeviceSoftservoWMX3.h"
#include "RavidFramework/DeviceTcpClient.h"
#include "RavidFramework/DeviceTcpServer.h"
#include "RavidFramework/DeviceUmac.h"
#include "RavidFramework/DeviceDalsaXtiumMX4.h"
#include "RavidFramework/DeviceDalsaXtiumCXP.h"
#include "RavidFramework/RavidImage3DProfilePhotonFocus.h"
#include "RavidFramework/DeviceMicroview.h"

//////////////////////////////////////////////////////////////////////////
// Framework

#include "RavidFramework/EventHandlerBase.h"
#include "RavidFramework/MeasurementObject.h"
#include "RavidFramework/MeasurementObjectElement.h"
#include "RavidFramework/MessageBase.h"
#include "RavidFramework/ModelConfigurationInfo.h"
#include "RavidFramework/ModelInfo.h"
#include "RavidFramework/Packet.h"
#include "RavidFramework/RavidObjectEx.h"
#include "RavidFramework/ROIStorageElement.h"
#include "RavidFramework/SequenceInfo.h"
#include "RavidFramework/TeachingObject.h"
#include "RavidFramework/TeachingObjectElement.h"
#include "RavidFramework/ViewObject.h"
#include "RavidFramework/ViewObjectElement.h"


//////////////////////////////////////////////////////////////////////////
// Managers

#include "RavidFramework/AuthorityManager.h"
#include "RavidFramework/DeviceManager.h"
#include "RavidFramework/EventHandlerManager.h"
#include "RavidFramework/FrameworkManager.h"
#include "RavidFramework/LibraryManager.h"
#include "RavidFramework/LogManager.h"
#include "RavidFramework/MessageManager.h"
#include "RavidFramework/ModelManager.h"
#include "RavidFramework/MultiLanguageManager.h"
#include "RavidFramework/SequenceManager.h"
#include "RavidFramework/UIManager.h"

//////////////////////////////////////////////////////////////////////////
// Others
#include "RavidFramework/LibraryEcMotionAlign.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// UI

#include "RavidFramework/MultiSelectionObject.h"
#include "RavidFramework/MultiSelectionObjectElement.h"
#include "RavidFramework/RavidOld3DView.h"
#include "RavidFramework/RavidBehavior.h"
#include "RavidFramework/RavidBitmapButton.h"
#include "RavidFramework/RavidChildFrameBase.h"
#include "RavidFramework/RavidChildFrameGraphView.h"
#include "RavidFramework/RavidChildFrameImageView.h"
#include "RavidFramework/RavidChildFrameLogViewer.h"
#include "RavidFramework/RavidChildFrameThumbnailView.h"
#include "RavidFramework/RavidCustomizeButton.h"
#include "RavidFramework/RavidDialogBase.h"
#include "RavidFramework/RavidDialogBehavior.h"
#include "RavidFramework/RavidDlgAuthorityAddUser.h"
#include "RavidFramework/RavidDlgAuthorityManager.h"
#include "RavidFramework/RavidDlgConfigurationNew.h"
#include "RavidFramework/RavidDlgDeviceConfigurationTool.h"
#include "RavidFramework/RavidDlgFrameworkConfigurationTool.h"
#include "RavidFramework/RavidDlgGeometryScript.h"
#include "RavidFramework/RavidDlgImageView.h"
#include "RavidFramework/RavidDlgInputBox.h"
#include "RavidFramework/RavidDlgInspectionMenu.h"
#include "RavidFramework/RavidDlgModelConfiguration.h"
#include "RavidFramework/RavidDlgModelConfigurationProperty.h"
#include "RavidFramework/RavidDlgModelConfigurationPropertyArray.h"
#include "RavidFramework/RavidDlgModelConfigurationSetting.h"
#include "RavidFramework/RavidDlgModelManager.h"
#include "RavidFramework/RavidDlgModelParameterShowStruct.h"
#include "RavidFramework/RavidDlgModelSearch.h"
#include "RavidFramework/RavidDlgModifyGeometry.h"
#include "RavidFramework/RavidDlgNotification.h"
#include "RavidFramework/RavidDlgOperationGeometry.h"
#include "RavidFramework/RavidDlgProgress.h"
#include "RavidFramework/RavidDlgROIStorage.h"
#include "RavidFramework/RavidDlgScreenSaver.h"
#include "RavidFramework/RavidDlgTeachingMenu.h"
#include "RavidFramework/RavidDlgViewConfigurationTool.h"
#include "RavidFramework/RavidDlgShapeFinderMasking.h"
#include "RavidFramework/RavidDoc.h"
#include "RavidFramework/RavidEventHandlerLog.h"
#include "RavidFramework/RavidFileDialogEx.h"
#include "RavidFramework/RavidFormViewBase.h"
#include "RavidFramework/RavidGraphCtrl.h"
#include "RavidFramework/RavidGraphView.h"
#include "RavidFramework/RavidGraphViewPlotInfomation.h"
#include "RavidFramework/RavidImageBehavior.h"
#include "RavidFramework/RavidImageView.h"
#include "RavidFramework/RavidImageViewLayer.h"
#include "RavidFramework/RavidImageViewLayerElement.h"
#include "RavidFramework/RavidImageViewScrollBar.h"
#include "RavidFramework/RavidInputBoxInfo.h"
#include "RavidFramework/RavidLedCtrl.h"
#include "RavidFramework/RavidLogViewer.h"
#include "RavidFramework/RavidMainFrameToolBarHandler.h"
#include "RavidFramework/RavidMainFrm.h"
#include "RavidFramework/RavidMenu.h"
#include "RavidFramework/RavidObjectBehavior.h"
#include "RavidFramework/RavidPopupMenu.h"
#include "RavidFramework/RavidPopupMenuBar.h"
#include "RavidFramework/RavidSheet.h"
#include "RavidFramework/RavidSheetCell.h"
#include "RavidFramework/RavidSheetCellBase.h"
#include "RavidFramework/RavidSheetCellCheck.h"
#include "RavidFramework/RavidSheetCellCombo.h"
#include "RavidFramework/RavidSheetCellDateTime.h"
#include "RavidFramework/RavidSheetCellID.h"
#include "RavidFramework/RavidSheetCellNumeric.h"
#include "RavidFramework/RavidSheetCellRange.h"
#include "RavidFramework/RavidSheetCellURL.h"
#include "RavidFramework/RavidSheetDropTarget.h"
#include "RavidFramework/RavidSheetInPlaceEdit.h"
#include "RavidFramework/RavidSheetMemoryDC.h"
#include "RavidFramework/RavidSheetProperty.h"
#include "RavidFramework/RavidSheetPropertyCell.h"
#include "RavidFramework/RavidSheetTitleTip.h"
#include "RavidFramework/RavidSlider.h"
#include "RavidFramework/RavidThumbnailView.h"
#include "RavidFramework/RavidThumbnailViewLayer.h"
#include "RavidFramework/RavidThumbnailViewLayerElement.h"
#include "RavidFramework/RavidToolBar.h"
#include "RavidFramework/RavidToolBarMenuButton.h"
#include "RavidFramework/RavidToolCmdUI.h"
#include "RavidFramework/RavidTreeCtrl.h"
#include "RavidFramework/RavidViewConfigurationToolSlider.h"
#include "RavidFramework/UIConfig.h"
#include "RavidFramework/UIConfigElement.h"
#include "RavidFramework/UIConfigElementEx.h"
#include "RavidFramework/Ravid3DViewer.h"
#include "RavidFramework/OverwriteImage.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 3D

#include "Ravid3D/3DViewer.h"
#include "Ravid3D/Ravid3D.h"
#include "Ravid3D/Ravid3DData.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Ravid core module import

#ifndef RAVID_LIBRARY_IMPORTED
	#define RAVID_LIBRARY_IMPORTED
	#pragma comment(lib,  "RavidSecurity" LIB_DEBUG_POSTFIX)
	#pragma comment(lib,  "RavidFramework" LIB_DEBUG_POSTFIX)
	#pragma comment(lib,  "RavidCore" LIB_DEBUG_POSTFIX)
	#pragma comment(lib,  "Ravid3D" LIB_DEBUG_POSTFIX)
#endif

//////////////////////////////////////////////////////////////////////////