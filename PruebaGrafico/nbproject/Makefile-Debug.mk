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
	${OBJECTDIR}/main.o

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
LDLIBSOPTIONS=-Wl,-rpath /home/tokayo/Proyecto/pt/Grafico/dist/Debug/GNU-Linux-x86 -L/home/tokayo/Proyecto/pt/Grafico/dist/Debug/GNU-Linux-x86 -lGrafico -Wl,-rpath /home/tokayo/Proyecto/pt/Reglas/dist/Debug/GNU-Linux-x86 -L/home/tokayo/Proyecto/pt/Reglas/dist/Debug/GNU-Linux-x86 -lReglas -Wl,-rpath /home/tokayo/Proyecto/pt/Scripting/dist/Debug/GNU-Linux-x86 -L/home/tokayo/Proyecto/pt/Scripting/dist/Debug/GNU-Linux-x86 -lScripting

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/pruebagrafico

dist/Debug/GNU-Linux-x86/pruebagrafico: /home/tokayo/Proyecto/pt/Grafico/dist/Debug/GNU-Linux-x86/libGrafico.so

dist/Debug/GNU-Linux-x86/pruebagrafico: /home/tokayo/Proyecto/pt/Reglas/dist/Debug/GNU-Linux-x86/libReglas.so

dist/Debug/GNU-Linux-x86/pruebagrafico: /home/tokayo/Proyecto/pt/Scripting/dist/Debug/GNU-Linux-x86/libScripting.so

dist/Debug/GNU-Linux-x86/pruebagrafico: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pruebagrafico ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd /home/tokayo/Proyecto/pt/Grafico && ${MAKE}  -f Makefile CONF=Debug
	cd /home/tokayo/Proyecto/pt/Reglas && ${MAKE}  -f Makefile CONF=Debug
	cd /home/tokayo/Proyecto/pt/Scripting && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/pruebagrafico

# Subprojects
.clean-subprojects:
	cd /home/tokayo/Proyecto/pt/Grafico && ${MAKE}  -f Makefile CONF=Debug clean
	cd /home/tokayo/Proyecto/pt/Reglas && ${MAKE}  -f Makefile CONF=Debug clean
	cd /home/tokayo/Proyecto/pt/Scripting && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
