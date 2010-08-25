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
	${OBJECTDIR}/AgenteMinimax2.o \
	${OBJECTDIR}/ManejadorAgentes.o \
	${OBJECTDIR}/AgenteAstar.o \
	${OBJECTDIR}/AgenteHumano.o \
	${OBJECTDIR}/AgenteCaminaAdelante.o \
	${OBJECTDIR}/AgenteMinimax4.o


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
LDLIBSOPTIONS=-Wl,-rpath ../Reglas/dist/Release/GNU-Linux-x86 -L../Reglas/dist/Release/GNU-Linux-x86 -lReglas

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/libAgentes.so

dist/Release/GNU-Linux-x86/libAgentes.so: ../Reglas/dist/Release/GNU-Linux-x86/libReglas.so

dist/Release/GNU-Linux-x86/libAgentes.so: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libAgentes.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/AgenteMinimax2.o: AgenteMinimax2.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AgenteMinimax2.o AgenteMinimax2.cpp

${OBJECTDIR}/ManejadorAgentes.o: ManejadorAgentes.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/ManejadorAgentes.o ManejadorAgentes.cpp

${OBJECTDIR}/AgenteAstar.o: AgenteAstar.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AgenteAstar.o AgenteAstar.cpp

${OBJECTDIR}/AgenteHumano.o: AgenteHumano.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AgenteHumano.o AgenteHumano.cpp

${OBJECTDIR}/AgenteCaminaAdelante.o: AgenteCaminaAdelante.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AgenteCaminaAdelante.o AgenteCaminaAdelante.cpp

${OBJECTDIR}/AgenteMinimax4.o: AgenteMinimax4.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AgenteMinimax4.o AgenteMinimax4.cpp

# Subprojects
.build-subprojects:
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/libAgentes.so

# Subprojects
.clean-subprojects:
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
