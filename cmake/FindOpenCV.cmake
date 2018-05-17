if(OPENCV_INCLUDE_DIR AND OPENCV_LIBRARIES)
	set(OPENCV_FIND_QUIETLY TRUE)
endif(OPENCV_INCLUDE_DIR AND OPENCV_LIBRARIES)

set(OPENCV_LIBS "opencv_calib3d" "opencv_core" "opencv_features2d" "opencv_flann" "opencv_highgui" "opencv_imgproc")

set(OPENCV_LIB_VER "249")

foreach(item ${OPENCV_LIBS})
	find_library(${item}_OPENCV_ITEM
			${item} lib${item} ${item}${OPENCV_LIB_VER} lib${item}${OPENCV_LIB_VER}
			PATHS ${PROJECT_BASE_DIR}/redist/OpenCV/lib_${SPEC})
	set(OPENCV_LIBRARIES ${OPENCV_LIBRARIES} ${${item}_OPENCV_ITEM})
endforeach()

find_path(OPENCV_INCLUDE_DIR 
	opencv
	PATHS ${PROJECT_BASE_DIR}/redist/OpenCV/sources/include)
	
find_path(OPENCV_MODULE_DIR 
	calib3d
	PATHS ${PROJECT_BASE_DIR}/redist/OpenCV/sources/modules)

set (OPENCV_INCLUDE_DIR
	${OPENCV_INCLUDE_DIR}
	${OPENCV_MODULE_DIR}/calib3d/include
	${OPENCV_MODULE_DIR}/contrib/include
	${OPENCV_MODULE_DIR}/core/include
	${OPENCV_MODULE_DIR}/features2d/include
	${OPENCV_MODULE_DIR}/flann/include
	${OPENCV_MODULE_DIR}/highgui/include
	${OPENCV_MODULE_DIR}/imgproc/include
	${OPENCV_MODULE_DIR}/legacy/include
	${OPENCV_MODULE_DIR}/ml/include
	${OPENCV_MODULE_DIR}/objdetect/include
	${OPENCV_MODULE_DIR}/photo/include
	${OPENCV_MODULE_DIR}/video/include)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OPENCV DEFAULT_MSG OPENCV_LIBRARIES OPENCV_INCLUDE_DIR)

MARK_AS_ADVANCED(
	OPENCV_INCLUDE_DIR
	OPENCV_LIBRARIES
)
