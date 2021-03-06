SET(MDAL_SRCS
  qgsmdalprovider.cpp
  qgsmdaldataitems.cpp
)

IF (WITH_GUI)
  SET(MDAL_SRCS ${MDAL_SRCS}
    qgsmdalprovidergui.cpp
    qgsmdalsourceselect.cpp
  )
ENDIF ()

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/external/mdal/cmake ${CMAKE_MODULE_PATH})
IF(MSVC)
  # HDF5 with enabled thread safety (HDF5_ENABLE_THREADSAFE=ON) on Windows needs Threads::Threads
  FIND_PACKAGE(Threads)
ENDIF(MSVC)
FIND_PACKAGE(HDF5)
FIND_PACKAGE(NetCDF)
FIND_PACKAGE(LibXml2)

########################################################
# Compile internal MDAL
IF (WITH_INTERNAL_MDAL)

  ADD_DEFINITIONS(-DMDAL_STATIC)

  INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/external/mdal
    ${CMAKE_SOURCE_DIR}/external/mdal/api
  )

  SET(MDAL_LIB_SRCS
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_utils.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_data_model.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_datetime.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_memory_data_model.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_driver_manager.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_logger.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_logger.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_driver.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_2dm.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_ascii_dat.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_binary_dat.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_selafin.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_esri_tin.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_xms_tin.cpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_ply.cpp
  )

  SET(MDAL_LIB_HDRS
    ${CMAKE_SOURCE_DIR}/external/mdal/api/mdal.h
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_utils.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_data_model.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_datetime.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_memory_data_model.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/mdal_driver_manager.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_driver.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_2dm.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_ascii_dat.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_binary_dat.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_selafin.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_esri_tin.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_xms_tin.hpp
    ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_ply.hpp
  )

  IF(HDF5_FOUND)
    SET(MDAL_LIB_SRCS ${MDAL_LIB_SRCS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_hdf5.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_xmdf.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_hec2d.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_flo2d.cpp
    )
    SET(MDAL_LIB_HDRS ${MDAL_LIB_HDRS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_hdf5.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_xmdf.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_hec2d.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_flo2d.hpp
    )
    SET(MDAL_DEPS ${MDAL_DEPS} )
    SET (HAVE_HDF5 TRUE)
  ENDIF(HDF5_FOUND)

  IF(SQLITE3_FOUND)
     SET(HAVE_SQLITE3 TRUE)
  ENDIF(SQLITE3_FOUND)

  IF(SQLITE3_FOUND AND NETCDF_FOUND)
     SET(MDAL_LIB_SRCS ${MDAL_LIB_SRCS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_sqlite3.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_3di.cpp
    )
    SET(MDAL_LIB_HDRS ${MDAL_LIB_HDRS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_sqlite3.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_3di.hpp
    )
  ENDIF(SQLITE3_FOUND AND NETCDF_FOUND)

  IF(GDAL_FOUND)
    SET(MDAL_LIB_SRCS ${MDAL_LIB_SRCS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_gdal.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_gdal_grib.cpp
    )
    SET(MDAL_LIB_HDRS ${MDAL_LIB_HDRS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_gdal.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_gdal_grib.hpp
    )
    SET (HAVE_GDAL TRUE)
  ENDIF(GDAL_FOUND)

  IF(NETCDF_FOUND)
    SET(MDAL_LIB_SRCS ${MDAL_LIB_SRCS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_cf.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_netcdf.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_sww.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_ugrid.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_tuflowfv.cpp
    )
    SET(MDAL_LIB_HDRS ${MDAL_LIB_HDRS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_cf.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_netcdf.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_sww.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_ugrid.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_tuflowfv.hpp
    )
    SET (HAVE_NETCDF TRUE)
  ENDIF(NETCDF_FOUND)

  IF(GDAL_FOUND AND NETCDF_FOUND)
    SET(MDAL_LIB_SRCS ${MDAL_LIB_SRCS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_gdal_netcdf.cpp
    )
    SET(MDAL_LIB_HDRS ${MDAL_LIB_HDRS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_gdal_netcdf.hpp
    )
  ENDIF(GDAL_FOUND AND NETCDF_FOUND)

  IF(HDF5_FOUND AND LIBXML2_FOUND)
    SET(MDAL_LIB_SRCS ${MDAL_LIB_SRCS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_xml.cpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_xdmf.cpp
    )
    SET(MDAL_LIB_HDRS ${MDAL_LIB_HDRS}
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_xml.hpp
      ${CMAKE_SOURCE_DIR}/external/mdal/frmts/mdal_xdmf.hpp
    )
    SET (HAVE_XML TRUE)
  ENDIF(HDF5_FOUND AND LIBXML2_FOUND)


  # create mdal_config.h
  CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/external/mdal/cmake_templates/mdal_config.hpp.in ${CMAKE_BINARY_DIR}/mdal_config.hpp)
  INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

  UNSET(MDAL_LIBRARY)
  UNSET(MDAL_INCLUDE_DIR)

ELSE (WITH_INTERNAL_MDAL)

  INCLUDE_DIRECTORIES (SYSTEM
    ${MDAL_INCLUDE_DIR}
  )

ENDIF (WITH_INTERNAL_MDAL)

########################################################

INCLUDE_DIRECTORIES (
  ${CMAKE_SOURCE_DIR}/src/core
  ${CMAKE_SOURCE_DIR}/src/core/mesh
  ${CMAKE_SOURCE_DIR}/src/core/expression
  ${CMAKE_SOURCE_DIR}/src/core/geometry
  ${CMAKE_SOURCE_DIR}/src/core/metadata
  ${CMAKE_SOURCE_DIR}/src/core/symbology
  ${CMAKE_SOURCE_DIR}/src/gui
  ${CMAKE_SOURCE_DIR}/src/gui/auth
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/external/nlohmann

  ${CMAKE_BINARY_DIR}/src/core
  ${CMAKE_BINARY_DIR}/src/gui
  ${CMAKE_BINARY_DIR}/src/ui
)

ADD_LIBRARY (mdalprovider MODULE ${MDAL_SRCS} ${MDAL_LIB_SRCS} ${MDAL_LIB_HDRS})

TARGET_LINK_LIBRARIES (mdalprovider
  qgis_core
  ${MDAL_LIBRARY}
)

IF (WITH_GUI)
  TARGET_LINK_LIBRARIES (mdalprovider
    qgis_gui
  )
  ADD_DEPENDENCIES(mdalprovider ui)
ENDIF (WITH_GUI)

IF (HDF5_FOUND)
  TARGET_INCLUDE_DIRECTORIES(mdalprovider PRIVATE ${HDF5_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES(mdalprovider ${HDF5_C_LIBRARIES} )
ENDIF (HDF5_FOUND)

IF (GDAL_FOUND)
  TARGET_INCLUDE_DIRECTORIES(mdalprovider PRIVATE ${GDAL_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES(mdalprovider ${GDAL_LIBRARY} )
ENDIF (GDAL_FOUND)

IF (NETCDF_FOUND)
  TARGET_INCLUDE_DIRECTORIES(mdalprovider PRIVATE ${NETCDF_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES(mdalprovider ${NETCDF_LIBRARY} )
ENDIF (NETCDF_FOUND)

IF (LIBXML2_FOUND)
  TARGET_INCLUDE_DIRECTORIES(mdalprovider PRIVATE ${LIBXML2_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES(mdalprovider ${LIBXML2_LIBRARIES} )
  TARGET_COMPILE_DEFINITIONS(mdalprovider PRIVATE ${LIBXML2_DEFINITIONS})
ENDIF (LIBXML2_FOUND)

# clang-tidy
IF(CLANG_TIDY_EXE)
  SET_TARGET_PROPERTIES(
    mdalprovider PROPERTIES
    CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
  )
ENDIF(CLANG_TIDY_EXE)

INSTALL(TARGETS mdalprovider
  RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
  LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})

