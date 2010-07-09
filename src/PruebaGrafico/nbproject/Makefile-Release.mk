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
CND_CONF=Release
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
	${OBJECTDIR}/PartidaGrafica.o \
	${OBJECTDIR}/Menu.o \
	${OBJECTDIR}/tokayoCamera.o \
	${OBJECTDIR}/PartidaConsola.o \
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
LDLIBSOPTIONS=-L/usr/lib/irrlicht -L../lib -L./lib -Wl,-rpath . -Wl,-rpath ../Grafico/dist/Debug/GNU-Linux-x86 -L../Grafico/dist/Debug/GNU-Linux-x86 -lGrafico -Wl,-rpath ../Reglas/dist/Debug/GNU-Linux-x86 -L../Reglas/dist/Debug/GNU-Linux-x86 -lReglas -Wl,-rpath ../Scripting/dist/Debug/GNU-Linux-x86 -L../Scripting/dist/Debug/GNU-Linux-x86 -lScripting -lIrrlicht -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/pruebagrafico

dist/Release/GNU-Linux-x86/pruebagrafico: ../Grafico/dist/Debug/GNU-Linux-x86/libGrafico.so

dist/Release/GNU-Linux-x86/pruebagrafico: ../Reglas/dist/Debug/GNU-Linux-x86/libReglas.so

dist/Release/GNU-Linux-x86/pruebagrafico: ../Scripting/dist/Debug/GNU-Linux-x86/libScripting.so

dist/Release/GNU-Linux-x86/pruebagrafico: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -Wl,-R../lib -Wl,-R./lib -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pruebagrafico ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Video.o: Video.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Video.o Video.cpp

${OBJECTDIR}/Aplicacion.o: Aplicacion.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Aplicacion.o Aplicacion.cpp

${OBJECTDIR}/ManejadorGUI.o: ManejadorGUI.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/ManejadorGUI.o ManejadorGUI.cpp

${OBJECTDIR}/AnimacionEnd.o: AnimacionEnd.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/AnimacionEnd.o AnimacionEnd.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/PartidaGrafica.o: PartidaGrafica.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/PartidaGrafica.o PartidaGrafica.cpp

${OBJECTDIR}/Menu.o: Menu.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Menu.o Menu.cpp

${OBJECTDIR}/tokayoCamera.o: tokayoCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/tokayoCamera.o tokayoCamera.cpp

${OBJECTDIR}/PartidaConsola.o: PartidaConsola.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/PartidaConsola.o PartidaConsola.cpp


${OBJECTDIR}/ManejadorJuego.o: ManejadorJuego.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/ManejadorJuego.o ManejadorJuego.cpp

${OBJECTDIR}/GUI.o: GUI.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/GUI.o GUI.cpp

${OBJECTDIR}/EventReceiver.o: EventReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/EventReceiver.o EventReceiver.cpp

${OBJECTDIR}/Partida.o: Partida.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Partida.o Partida.cpp

# Subprojects
.build-subprojects:
	cd ../Grafico && ${MAKE}  -f Makefile CONF=Debug
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug
	cd ../Scripting && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/pruebagrafico

# Subprojects
.clean-subprojects:
	cd ../Grafico && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Scripting && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
