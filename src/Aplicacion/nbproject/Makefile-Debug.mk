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
	${OBJECTDIR}/Aplicacion.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/PartidaGrafica.o \
	${OBJECTDIR}/PartidaConsola.o \
	${OBJECTDIR}/ManejadorJuego.o \
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
LDLIBSOPTIONS=-L/usr/lib/irrlicht -L../lib -L./lib -Wl,-rpath . -Wl,-rpath ../Grafico/dist/Debug/GNU-Linux-x86 -L../Grafico/dist/Debug/GNU-Linux-x86 -lGrafico -Wl,-rpath ../Reglas/dist/Debug/GNU-Linux-x86 -L../Reglas/dist/Debug/GNU-Linux-x86 -lReglas -Wl,-rpath ../Scripting/dist/Debug/GNU-Linux-x86 -L../Scripting/dist/Debug/GNU-Linux-x86 -lScripting -Wl,-rpath ../Opciones/dist/Debug/GNU-Linux-x86 -L../Opciones/dist/Debug/GNU-Linux-x86 -lOpciones -lIrrlicht -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/aplicacion

dist/Debug/GNU-Linux-x86/aplicacion: ../Grafico/dist/Debug/GNU-Linux-x86/libGrafico.so

dist/Debug/GNU-Linux-x86/aplicacion: ../Reglas/dist/Debug/GNU-Linux-x86/libReglas.so

dist/Debug/GNU-Linux-x86/aplicacion: ../Scripting/dist/Debug/GNU-Linux-x86/libScripting.so

dist/Debug/GNU-Linux-x86/aplicacion: ../Opciones/dist/Debug/GNU-Linux-x86/libOpciones.so

dist/Debug/GNU-Linux-x86/aplicacion: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -Wl,-R../lib -Wl,-R./lib -lutil -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/aplicacion ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Aplicacion.o: Aplicacion.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Aplicacion.o Aplicacion.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/PartidaGrafica.o: PartidaGrafica.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/PartidaGrafica.o PartidaGrafica.cpp

${OBJECTDIR}/PartidaConsola.o: PartidaConsola.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/PartidaConsola.o PartidaConsola.cpp

${OBJECTDIR}/ManejadorJuego.o: ManejadorJuego.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ManejadorJuego.o ManejadorJuego.cpp

${OBJECTDIR}/EventReceiver.o: EventReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/EventReceiver.o EventReceiver.cpp

${OBJECTDIR}/Partida.o: Partida.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I.. -I/usr/include/irrlicht -I/usr/include/python2.6 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Partida.o Partida.cpp

# Subprojects
.build-subprojects:
	cd ../Grafico && ${MAKE}  -f Makefile CONF=Debug
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug
	cd ../Scripting && ${MAKE}  -f Makefile CONF=Debug
	cd ../Opciones && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/aplicacion

# Subprojects
.clean-subprojects:
	cd ../Grafico && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Reglas && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Scripting && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Opciones && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
