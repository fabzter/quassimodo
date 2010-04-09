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
	${OBJECTDIR}/Tablero.o \
	${OBJECTDIR}/EventReceiver.o \
	${OBJECTDIR}/Barrera.o \
	${OBJECTDIR}/Celda.o \
	${OBJECTDIR}/Antorcha.o \
	${OBJECTDIR}/Jugador.o \
	${OBJECTDIR}/Pieza.o

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
LDLIBSOPTIONS=-lIrrlicht /usr/lib/libGL.so /usr/lib/libXxf86vm.so -Wl,-rpath ../Reglas/dist/Debug/GNU-Linux-x86 -L../Reglas/dist/Debug/GNU-Linux-x86 -lReglas

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/grafico

dist/Release/GNU-Linux-x86/grafico: /usr/lib/libGL.so

dist/Release/GNU-Linux-x86/grafico: /usr/lib/libXxf86vm.so

dist/Release/GNU-Linux-x86/grafico: ../Reglas/dist/Debug/GNU-Linux-x86/libReglas.so

dist/Release/GNU-Linux-x86/grafico: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/grafico ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Tablero.o: nbproject/Makefile-${CND_CONF}.mk Tablero.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Tablero.o Tablero.cpp

${OBJECTDIR}/EventReceiver.o: nbproject/Makefile-${CND_CONF}.mk EventReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/EventReceiver.o EventReceiver.cpp

${OBJECTDIR}/Barrera.o: nbproject/Makefile-${CND_CONF}.mk Barrera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Barrera.o Barrera.cpp

${OBJECTDIR}/Celda.o: nbproject/Makefile-${CND_CONF}.mk Celda.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Celda.o Celda.cpp

${OBJECTDIR}/Antorcha.o: nbproject/Makefile-${CND_CONF}.mk Antorcha.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Antorcha.o Antorcha.cpp

${OBJECTDIR}/Jugador.o: nbproject/Makefile-${CND_CONF}.mk Jugador.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Jugador.o Jugador.cpp

${OBJECTDIR}/Pieza.o: nbproject/Makefile-${CND_CONF}.mk Pieza.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -I/usr/include/irrlicht -MMD -MP -MF $@.d -o ${OBJECTDIR}/Pieza.o Pieza.cpp

# Subprojects
.build-subprojects:
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/grafico

# Subprojects
.clean-subprojects:
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
