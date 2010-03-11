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
	${OBJECTDIR}/Grafo.o \
	${OBJECTDIR}/Tablero.o \
	${OBJECTDIR}/Partida.o \
	${OBJECTDIR}/Barrera.o \
	${OBJECTDIR}/Celda.o \
	${OBJECTDIR}/Excepciones.o \
	${OBJECTDIR}/Jugada.o \
	${OBJECTDIR}/Juez.o \
	${OBJECTDIR}/Jugador.o \
	${OBJECTDIR}/Pieza.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fpic
CXXFLAGS=-fpic

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath .

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/libReglas.so

dist/Release/GNU-Linux-x86/libReglas.so: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libReglas.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Grafo.o: nbproject/Makefile-${CND_CONF}.mk Grafo.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Grafo.o Grafo.cpp

${OBJECTDIR}/Tablero.o: nbproject/Makefile-${CND_CONF}.mk Tablero.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Tablero.o Tablero.cpp

${OBJECTDIR}/Partida.o: nbproject/Makefile-${CND_CONF}.mk Partida.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Partida.o Partida.cpp

${OBJECTDIR}/Barrera.o: nbproject/Makefile-${CND_CONF}.mk Barrera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Barrera.o Barrera.cpp

${OBJECTDIR}/Celda.o: nbproject/Makefile-${CND_CONF}.mk Celda.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Celda.o Celda.cpp

${OBJECTDIR}/Excepciones.o: nbproject/Makefile-${CND_CONF}.mk Excepciones.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Excepciones.o Excepciones.cpp

${OBJECTDIR}/Jugada.o: nbproject/Makefile-${CND_CONF}.mk Jugada.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Jugada.o Jugada.cpp

${OBJECTDIR}/Juez.o: nbproject/Makefile-${CND_CONF}.mk Juez.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Juez.o Juez.cpp

${OBJECTDIR}/Jugador.o: nbproject/Makefile-${CND_CONF}.mk Jugador.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Jugador.o Jugador.cpp

${OBJECTDIR}/Pieza.o: nbproject/Makefile-${CND_CONF}.mk Pieza.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Pieza.o Pieza.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/libReglas.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
