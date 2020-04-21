IF( NOT Geant4_TAG )
    SET( Geant4_TAG "v10.6.1" )
ENDIF()

IF ( NOT GEANT4_INSTALL_DATA )
    SET( GEANT4_INSTALL_DATA "ON" )
ENDIF()

IF(WITH_GEANT4_UIVIS)
    SET(GEANT4_USE_QT "ON")
    SET(GEANT4_USE_OPENGL_X11 "ON")
ELSE()
    SET(GEANT4_USE_QT "OFF")
    SET(GEANT4_USE_OPENGL_X11 "OFF")
ENDIF()

EXTERNALPROJECT_ADD(
    Geant4

    GIT_REPOSITORY "https://github.com/Geant4/geant4"
    GIT_TAG ${Geant4_TAG}
    
    UPDATE_COMMAND ""
    PATCH_COMMAND ""

    SOURCE_DIR "${CMAKE_BINARY_DIR}/geant4"
    
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPENDENCY_INSTALL_DIR}/geant4 -DGEANT4_INSTALL_DATA=${GEANT4_INSTALL_DATA} -DGEANT4_USE_OPENGL_X11=${GEANT4_USE_OPENGL_X11} -DGEANT4_USE_QT=${GEANT4_USE_QT} -DGEANT4_USE_SYSTEM_EXPAT=OFF -DGEANT4_INSTALL_EXAMPLES=OFF
    
    BUILD_COMMAND make -j4
)

# FIXME: Install dir could be under lib or lib64 depending on Geant4 version.
SET( Geant4_DIR ${DEPENDENCY_INSTALL_DIR}/geant4/lib/Geant4-${Geant4_VERSION_NUMBER} CACHE PATH "Geant4 install dir" FORCE )
