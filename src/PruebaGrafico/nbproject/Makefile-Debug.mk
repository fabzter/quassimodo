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
	${OBJECTDIR}/Aplicacion.o \
	${OBJECTDIR}/ManejadorGUI.o \
	${OBJECTDIR}/AnimacionEnd.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/Menu.o \
	${OBJECTDIR}/tokayoCamera.o \
	${OBJECTDIR}/ManejadorJuego.o \
	${OBJECTDIR}/GUI.o \
	${OBJECTDIR}/EventReceiver.o \
	${OBJECTDIR}/Partida.o


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
LDLIBSOPTIONS=-L/usr/lib/irrlicht -L../lib -L./lib -Wl,-rpath . -Wl,-rpath ../Grafico/dist/Debug/GNU-Linux-x86 -L../Grafico/dist/Debug/GNU-Linux-x86 -lGrafico -Wl,-rpath ../Reglas/dist/Debug/GNU-Linux-x86 -L../Reglas/dist/Debug/GNU-Linux-x86 -lReglas -Wl,-rpath ../Scripting/dist/Debug/GNU-Linux-x86 -L../Scripting/dist/Debug/GNU-Linux-x86 -lScripting -lIrrlicht

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/pruebagrafico

dist/Debug/GNU-Linux-x86/pruebagrafico: ../Grafico/dist/Debug/GNU-Linux-x86/libGrafico.so

dist/Debug/GNU-Linux-x86/pruebagrafico: ../Reglas/dist/Debug/GNU-Linux-x86/libReglas.so

dist/Debug/GNU-Linux-x86/pruebagrafico: ../Scripting/dist/Debug/GNU-Linux-x86/libScripting.so

dist/Debug/GNU-Linux-x86/pruebagrafico: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -Wl,-R../lib -Wl,-R./lib -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pruebagrafico ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Video.o: Video.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Video.o Video.cpp

${OBJECTDIR}/Aplicacion.o: Aplicacion.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Aplicacion.o Aplicacion.cpp

${OBJECTDIR}/ManejadorGUI.o: ManejadorGUI.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/ManejadorGUI.o ManejadorGUI.cpp

${OBJECTDIR}/AnimacionEnd.o: AnimacionEnd.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/AnimacionEnd.o AnimacionEnd.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/Menu.o: Menu.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Menu.o Menu.cpp

${OBJECTDIR}/tokayoCamera.o: tokayoCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/tokayoCamera.o tokayoCamera.cpp

${OBJECTDIR}/ManejadorJuego.o: ManejadorJuego.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/ManejadorJuego.o ManejadorJuego.cpp

${OBJECTDIR}/GUI.o: GUI.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/GUI.o GUI.cpp

${OBJECTDIR}/EventReceiver.o: EventReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/EventReceiver.o EventReceiver.cpp

${OBJECTDIR}/Partida.o: Partida.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Partida.o Partida.cpp

# Subprojects
.build-subprojects:
	cd ../Grafico && ${MAKE}  -f Makefile CONF=Debug
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug
	cd ../Scripting && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/pruebagrafico

# Subprojects
.clean-subprojects:
	cd ../Grafico && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Scripting && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
