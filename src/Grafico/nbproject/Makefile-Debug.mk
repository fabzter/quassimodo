#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Video.o \
	${OBJECTDIR}/ManejadorGUI.o \
	${OBJECTDIR}/Skin.o \
	${OBJECTDIR}/Exepciones.o \
	${OBJECTDIR}/SkinLoader.o \
	${OBJECTDIR}/Pieza.o \
	${OBJECTDIR}/Tablero.o \
	${OBJECTDIR}/CConfigMap.o \
	${OBJECTDIR}/CConfigReader.o \
	${OBJECTDIR}/Jugador.o \
	${OBJECTDIR}/Menu.o \
	${OBJECTDIR}/tokayoCamera.o \
	${OBJECTDIR}/Celda.o \
	${OBJECTDIR}/CGUIProgressBar.o \
	${OBJECTDIR}/Antorcha.o \
	${OBJECTDIR}/CImageGUISkin.o \
	${OBJECTDIR}/GUI.o \
	${OBJECTDIR}/Barrera.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath . -lIrrlicht /usr/lib/libGL.so /usr/lib/libXxf86vm.so -Wl,-rpath ../Reglas/dist/Debug/GNU-Linux-x86 -L../Reglas/dist/Debug/GNU-Linux-x86 -lReglas

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/libGrafico.so

dist/Debug/GNU-Linux-x86/libGrafico.so: /usr/lib/libGL.so

dist/Debug/GNU-Linux-x86/libGrafico.so: /usr/lib/libXxf86vm.so

dist/Debug/GNU-Linux-x86/libGrafico.so: ../Reglas/dist/Debug/GNU-Linux-x86/libReglas.so

dist/Debug/GNU-Linux-x86/libGrafico.so: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libGrafico.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Video.o: Video.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Video.o Video.cpp

${OBJECTDIR}/ManejadorGUI.o: ManejadorGUI.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/ManejadorGUI.o ManejadorGUI.cpp

${OBJECTDIR}/Skin.o: Skin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Skin.o Skin.cpp

${OBJECTDIR}/Exepciones.o: Exepciones.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Exepciones.o Exepciones.cpp

${OBJECTDIR}/SkinLoader.o: SkinLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SkinLoader.o SkinLoader.cpp

${OBJECTDIR}/Pieza.o: Pieza.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Pieza.o Pieza.cpp

${OBJECTDIR}/Tablero.o: Tablero.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Tablero.o Tablero.cpp

${OBJECTDIR}/CConfigMap.o: CConfigMap.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/CConfigMap.o CConfigMap.cpp

${OBJECTDIR}/CConfigReader.o: CConfigReader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/CConfigReader.o CConfigReader.cpp

${OBJECTDIR}/Jugador.o: Jugador.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Jugador.o Jugador.cpp

${OBJECTDIR}/Menu.o: Menu.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Menu.o Menu.cpp

${OBJECTDIR}/tokayoCamera.o: tokayoCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/tokayoCamera.o tokayoCamera.cpp

${OBJECTDIR}/Celda.o: Celda.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Celda.o Celda.cpp

${OBJECTDIR}/CGUIProgressBar.o: CGUIProgressBar.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/CGUIProgressBar.o CGUIProgressBar.cpp

${OBJECTDIR}/Antorcha.o: Antorcha.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Antorcha.o Antorcha.cpp

${OBJECTDIR}/CImageGUISkin.o: CImageGUISkin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/CImageGUISkin.o CImageGUISkin.cpp

${OBJECTDIR}/GUI.o: GUI.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/GUI.o GUI.cpp

${OBJECTDIR}/Barrera.o: Barrera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/irrlicht -I../ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Barrera.o Barrera.cpp

# Subprojects
.build-subprojects:
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/libGrafico.so

# Subprojects
.clean-subprojects:
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
