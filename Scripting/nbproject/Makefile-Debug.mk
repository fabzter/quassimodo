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
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Excepciones.o \
	${OBJECTDIR}/InterpretePython.o \
	${OBJECTDIR}/ModuloPython.o \
	${OBJECTDIR}/UtilsPython.o \
	${OBJECTDIR}/Manejador.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-ftemplate-depth-100 -DBOOST_PYTHON_STATIC_LIB -isystem -I.
CXXFLAGS=-ftemplate-depth-100 -DBOOST_PYTHON_STATIC_LIB -isystem -I.

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/python2.6/config -Wl,-rpath . -lboost_python -lpython2.6 -lutil

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/libScripting.so

dist/Debug/GNU-Linux-x86/libScripting.so: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libScripting.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Excepciones.o: nbproject/Makefile-${CND_CONF}.mk Excepciones.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/lib/python2.6/config -I/usr/include/python2.6 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Excepciones.o Excepciones.cpp

${OBJECTDIR}/InterpretePython.o: nbproject/Makefile-${CND_CONF}.mk InterpretePython.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/lib/python2.6/config -I/usr/include/python2.6 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/InterpretePython.o InterpretePython.cpp

${OBJECTDIR}/ModuloPython.o: nbproject/Makefile-${CND_CONF}.mk ModuloPython.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/lib/python2.6/config -I/usr/include/python2.6 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/ModuloPython.o ModuloPython.cpp

${OBJECTDIR}/UtilsPython.o: nbproject/Makefile-${CND_CONF}.mk UtilsPython.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/lib/python2.6/config -I/usr/include/python2.6 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/UtilsPython.o UtilsPython.cpp

${OBJECTDIR}/Manejador.o: nbproject/Makefile-${CND_CONF}.mk Manejador.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/lib/python2.6/config -I/usr/include/python2.6 -I.. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Manejador.o Manejador.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/libScripting.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
