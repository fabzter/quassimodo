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
	${OBJECTDIR}/Aplicacion.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/PartidaGrafica.o \
	${OBJECTDIR}/AgenteThreads.o \
	${OBJECTDIR}/ManejadorJuego.o \
	${OBJECTDIR}/EventReceiver.o


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
LDLIBSOPTIONS=-L../lib -L./lib -Wl,-rpath ./lib -lIrrlicht -lboost_filesystem-mt -lboost_system-mt -lboost_thread-mt -lIrrKlang -lGrafico -lReglas -lOpciones -lAgentes

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/aplicacion

dist/Release/GNU-Linux-x86/aplicacion: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -Wl,-R../lib -Wl,-R./lib -lutil -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/aplicacion ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Aplicacion.o: Aplicacion.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Aplicacion.o Aplicacion.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/PartidaGrafica.o: PartidaGrafica.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/PartidaGrafica.o PartidaGrafica.cpp

${OBJECTDIR}/AgenteThreads.o: AgenteThreads.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/AgenteThreads.o AgenteThreads.cpp

${OBJECTDIR}/ManejadorJuego.o: ManejadorJuego.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ManejadorJuego.o ManejadorJuego.cpp

${OBJECTDIR}/EventReceiver.o: EventReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/EventReceiver.o EventReceiver.cpp

# Subprojects
.build-subprojects:
	cd ../Grafico && ${MAKE}  -f Makefile CONF=Release
	cd ../Opciones && ${MAKE}  -f Makefile CONF=Release
	cd ../Agentes && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/aplicacion

# Subprojects
.clean-subprojects:
	cd ../Grafico && ${MAKE}  -f Makefile CONF=Release clean
	cd ../Opciones && ${MAKE}  -f Makefile CONF=Release clean
	cd ../Agentes && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
