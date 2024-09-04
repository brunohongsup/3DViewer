#pragma once

#include "stdafx.h"

// ÀÌ¹«½Ã~
#pragma comment(lib, COMMONDLL_PREFIX "RavidSecurity/RavidSecurity" LIB_DEBUG_POSTFIX)

#define RAVID_MAIN_IMAGECODEC_DIR "ImageCodec/"
#define RAVID_MAIN_BOOST_DIR "Boost/"
#define RAVID_MAIN_OPENCV_DIR "OpenCV4/"

#ifdef RAVID_MAIN_IMAGECODEC_DIR
#define CODEC_PREFIX COMMONLIB_PREFIX RAVID_MAIN_IMAGECODEC_DIR 
#define CODEC_POSTFIX LIB_DEBUG_POSTFIX
#pragma comment(lib, CODEC_PREFIX "turbojpeg-static" CODEC_POSTFIX)
#pragma comment(lib, CODEC_PREFIX "libpng16_static"  CODEC_POSTFIX)
#pragma comment(lib, CODEC_PREFIX "zlibstatic"       CODEC_POSTFIX)
#pragma comment(lib, CODEC_PREFIX "tiff"             CODEC_POSTFIX)
#endif

#ifdef RAVID_MAIN_BOOST_DIR

#define BOOST_PREFIX COMMONLIB_PREFIX RAVID_MAIN_BOOST_DIR "libboost_"
#define BOOST_VS_VER "-vc141-mt"

#ifdef _DEBUG
#define BOOST_BUILD_MODE "-gd"
#else
#define BOOST_BUILD_MODE ""
#endif

#ifdef _WIN64
#define BOOST_BUILD_ARCH "-x64"
#else
#define BOOST_BUILD_ARCH "-x32"
#endif

#define BOOST_VER "-1_69"
#define BOOST_POSTFIX BOOST_VS_VER BOOST_BUILD_MODE BOOST_BUILD_ARCH BOOST_VER ".lib"

#pragma comment(lib, BOOST_PREFIX "log"                      BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "wave"                     BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "graph"                    BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "regex"                    BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "fiber"                    BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "timer"                    BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "atomic"                   BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "locale"                   BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "chrono"                   BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "random"                   BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "system"                   BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "context"                  BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "thread"                   BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "contract"                 BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "container"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "coroutine"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "date_time"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "exception"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "iostreams"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "math_c99"                 BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "math_tr1"                 BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "math_c99f"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "math_tr1f"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "math_c99l"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "math_tr1l"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "log_setup"                BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "filesystem"               BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "type_erasure"             BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "serialization"            BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "wserialization"           BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "program_options"          BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "stacktrace_noop"          BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "prg_exec_monitor"         BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "test_exec_monitor"        BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "stacktrace_windbg"        BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "unit_test_framework"      BOOST_POSTFIX)
#pragma comment(lib, BOOST_PREFIX "stacktrace_windbg_cached" BOOST_POSTFIX)

#endif


#ifdef RAVID_MAIN_OPENCV_DIR
#include <opencv2/opencv.hpp>

#define OPENCV_PREFIX COMMONLIB_PREFIX RAVID_MAIN_OPENCV_DIR
#define OPENCV_POSTFIX LIB_DEBUG_POSTFIX

#pragma comment(lib, OPENCV_PREFIX "ippicvmt.lib")
#pragma comment(lib, OPENCV_PREFIX "libjpeg-turbo" OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "libprotobuf"   OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "ittnotify"     OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "libjasper"     OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "libtiff"       OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "libpng"        OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "IlmImf"        OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "ippiw"         OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "quirc"         OPENCV_POSTFIX)
#pragma comment(lib, OPENCV_PREFIX "zlib"          OPENCV_POSTFIX)

#define _CV_VERSION_STR  CVAUX_STR(CV_VERSION_MAJOR)  CVAUX_STR(CV_VERSION_MINOR)  CVAUX_STR(CV_VERSION_REVISION)
#define OPENCV_POSTFIX_WITH_VER _CV_VERSION_STR LIB_DEBUG_POSTFIX

#ifdef HAVE_OPENCV_CALIB3D 
#pragma comment(lib, OPENCV_PREFIX "opencv_calib3d"          OPENCV_POSTFIX_WITH_VER) 
#endif												         
#ifdef HAVE_OPENCV_FEATURES2D						         
#pragma comment(lib, OPENCV_PREFIX "opencv_features2d"       OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_DNN								         
#pragma comment(lib, OPENCV_PREFIX "opencv_dnn"              OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_FLANN							         
#pragma comment(lib, OPENCV_PREFIX "opencv_flann"            OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_HIGHGUI							         
#pragma comment(lib, OPENCV_PREFIX "opencv_highgui"          OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_IMGCODECS						         
#pragma comment(lib, OPENCV_PREFIX "opencv_imgcodecs"        OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_IMGPROC							         
#pragma comment(lib, OPENCV_PREFIX "opencv_imgproc"          OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_ML								         
#pragma comment(lib, OPENCV_PREFIX "opencv_ml"               OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_OBJDETECT						         
#pragma comment(lib, OPENCV_PREFIX "opencv_objdetect"        OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_PHOTO							         
#pragma comment(lib, OPENCV_PREFIX "opencv_photo"            OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_SHAPE							         
#pragma comment(lib, OPENCV_PREFIX "opencv_shape"            OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_STITCHING						         
#pragma comment(lib, OPENCV_PREFIX "opencv_stitching"        OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_SUPERRES							         
#pragma comment(lib, OPENCV_PREFIX "opencv_superres"         OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_VIDEO							         
#pragma comment(lib, OPENCV_PREFIX "opencv_video"            OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_VIDEOIO							         
#pragma comment(lib, OPENCV_PREFIX "opencv_videoio"          OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_VIDEOSTAB						         
#pragma comment(lib, OPENCV_PREFIX "opencv_videostab"        OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_VIZ								         
#pragma comment(lib, OPENCV_PREFIX "opencv_viz"              OPENCV_POSTFIX_WITH_VER)
#endif												         
#ifdef HAVE_OPENCV_ARUCO							         
#pragma comment(lib, OPENCV_PREFIX "opencv_aruco"            OPENCV_POSTFIX_WITH_VER)
#endif 												         
#ifdef HAVE_OPENCV_BGSEGM							         
#pragma comment(lib, OPENCV_PREFIX "opencv_bgsegm"           OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_BIOINSPIRED								 
#pragma comment(lib, OPENCV_PREFIX "opencv_bioinspired"      OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_CALIB3D									 
#pragma comment(lib, OPENCV_PREFIX "opencv_calib3d"          OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_CCALIB									 
#pragma comment(lib, OPENCV_PREFIX "opencv_ccalib"           OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_CORE										 
#pragma comment(lib, OPENCV_PREFIX "opencv_core"             OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_DATASETS									 
#pragma comment(lib, OPENCV_PREFIX "opencv_datasets"         OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_DNN										 
#pragma comment(lib, OPENCV_PREFIX "opencv_dnn"              OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_DNN_OBJDETECT							 
#pragma comment(lib, OPENCV_PREFIX "opencv_objdetect"        OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_DPM										 
#pragma comment(lib, OPENCV_PREFIX "opencv_dpm"              OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_FACE										 
#pragma comment(lib, OPENCV_PREFIX "opencv_face"             OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_FEATURES2D								 
#pragma comment(lib, OPENCV_PREFIX "opencv_features2d"       OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_FLANN									 
#pragma comment(lib, OPENCV_PREFIX "opencv_flann"            OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_FUZZY									 
#pragma comment(lib, OPENCV_PREFIX "opencv_fuzzy"            OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_GAPI										 
#pragma comment(lib, OPENCV_PREFIX "opencv_gapi"             OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_HFS										 
#pragma comment(lib, OPENCV_PREFIX "opencv_hfs"              OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_HIGHGUI									 
#pragma comment(lib, OPENCV_PREFIX "opencv_highgui"          OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_IMG_HASH									 
#pragma comment(lib, OPENCV_PREFIX "opencv_img_hash"         OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_IMGCODECS								 
#pragma comment(lib, OPENCV_PREFIX "opencv_imgcodecs"        OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_IMGPROC									 
#pragma comment(lib, OPENCV_PREFIX "opencv_imgproc"          OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_LINE_DESCRIPTOR							 
#pragma comment(lib, OPENCV_PREFIX "opencv_line_descriptor"  OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_ML										 
#pragma comment(lib, OPENCV_PREFIX "opencv_ml"               OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_OBJDETECT								 
#pragma comment(lib, OPENCV_PREFIX "opencv_objdetect"        OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_OPTFLOW									 
#pragma comment(lib, OPENCV_PREFIX "opencv_optflow"          OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_PHASE_UNWRAPPING							 
#pragma comment(lib, OPENCV_PREFIX "opencv_phase_unwrapping" OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_PHOTO									 
#pragma comment(lib, OPENCV_PREFIX "opencv_photo"            OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_PLOT										 
#pragma comment(lib, OPENCV_PREFIX "opencv_plot"             OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_REG										 
#pragma comment(lib, OPENCV_PREFIX "opencv_reg"              OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_RGBD										 
#pragma comment(lib, OPENCV_PREFIX "opencv_rgbd"             OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_SALIENCY									 
#pragma comment(lib, OPENCV_PREFIX "opencv_saliency"         OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_SHAPE									 
#pragma comment(lib, OPENCV_PREFIX "opencv_shape"            OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_STEREO									 
#pragma comment(lib, OPENCV_PREFIX "opencv_stereo"           OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_STITCHING								 
#pragma comment(lib, OPENCV_PREFIX "opencv_stitching"        OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_STRUCTURED_LIGHT							 
#pragma comment(lib, OPENCV_PREFIX "opencv_structured_light" OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_SUPERRES									 
#pragma comment(lib, OPENCV_PREFIX "opencv_superres"         OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_SURFACE_MATCHING							 
#pragma comment(lib, OPENCV_PREFIX "opencv_surface_matching" OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_TEXT										 
#pragma comment(lib, OPENCV_PREFIX "opencv_text"             OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_TRACKING									 
#pragma comment(lib, OPENCV_PREFIX "opencv_tracking"         OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_VIDEO									 
#pragma comment(lib, OPENCV_PREFIX "opencv_video"            OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_VIDEOIO									 
#pragma comment(lib, OPENCV_PREFIX "opencv_videoio"          OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_VIDEOSTAB								 
#pragma comment(lib, OPENCV_PREFIX "opencv_videostab"        OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_XFEATURES2D								 
#pragma comment(lib, OPENCV_PREFIX "opencv_xfeatures2d"      OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_XIMGPROC									 
#pragma comment(lib, OPENCV_PREFIX "opencv_ximgproc"         OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_XOBJDETECT								 
#pragma comment(lib, OPENCV_PREFIX "opencv_xobjdetect"       OPENCV_POSTFIX_WITH_VER)
#endif 														 
#ifdef HAVE_OPENCV_XPHOTO									 
#pragma comment(lib, OPENCV_PREFIX "opencv_xphoto"           OPENCV_POSTFIX_WITH_VER)
#endif
#endif