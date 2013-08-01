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
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Cabalar.o \
	${OBJECTDIR}/src/CabalarUtils.o \
	${OBJECTDIR}/src/Formula.o \
	${OBJECTDIR}/src/Formulas.o \
	${OBJECTDIR}/src/HengZhang.o \
	${OBJECTDIR}/src/NNFUtils.o \
	${OBJECTDIR}/src/Rule.o \
	${OBJECTDIR}/src/S2DLP.o \
	${OBJECTDIR}/src/Utils.o \
	${OBJECTDIR}/src/Vocabulary.o \
	${OBJECTDIR}/src/global.o \
	${OBJECTDIR}/src/lex.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/parse.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/alpha

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/alpha: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/alpha ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/Cabalar.o: src/Cabalar.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Cabalar.o src/Cabalar.cpp

${OBJECTDIR}/src/CabalarUtils.o: src/CabalarUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CabalarUtils.o src/CabalarUtils.cpp

${OBJECTDIR}/src/Formula.o: src/Formula.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Formula.o src/Formula.cpp

${OBJECTDIR}/src/Formulas.o: src/Formulas.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Formulas.o src/Formulas.cpp

${OBJECTDIR}/src/HengZhang.o: src/HengZhang.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HengZhang.o src/HengZhang.cpp

${OBJECTDIR}/src/NNFUtils.o: src/NNFUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/NNFUtils.o src/NNFUtils.cpp

${OBJECTDIR}/src/Rule.o: src/Rule.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Rule.o src/Rule.cpp

${OBJECTDIR}/src/S2DLP.o: src/S2DLP.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/S2DLP.o src/S2DLP.cpp

${OBJECTDIR}/src/Utils.o: src/Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Utils.o src/Utils.cpp

${OBJECTDIR}/src/Vocabulary.o: src/Vocabulary.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Vocabulary.o src/Vocabulary.cpp

${OBJECTDIR}/src/global.o: src/global.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/global.o src/global.cpp

${OBJECTDIR}/src/lex.o: src/lex.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/lex.o src/lex.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/parse.o: src/parse.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/parse.o src/parse.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/src/tests/TestHelloWorld.o ${TESTDIR}/src/tests/TestHelloWorldRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs`   


${TESTDIR}/src/tests/TestHelloWorld.o: src/tests/TestHelloWorld.cpp 
	${MKDIR} -p ${TESTDIR}/src/tests
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I. `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/src/tests/TestHelloWorld.o src/tests/TestHelloWorld.cpp


${TESTDIR}/src/tests/TestHelloWorldRunner.o: src/tests/TestHelloWorldRunner.cpp 
	${MKDIR} -p ${TESTDIR}/src/tests
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I. `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/src/tests/TestHelloWorldRunner.o src/tests/TestHelloWorldRunner.cpp


${OBJECTDIR}/src/Cabalar_nomain.o: ${OBJECTDIR}/src/Cabalar.o src/Cabalar.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Cabalar.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Cabalar_nomain.o src/Cabalar.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Cabalar.o ${OBJECTDIR}/src/Cabalar_nomain.o;\
	fi

${OBJECTDIR}/src/CabalarUtils_nomain.o: ${OBJECTDIR}/src/CabalarUtils.o src/CabalarUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CabalarUtils.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CabalarUtils_nomain.o src/CabalarUtils.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CabalarUtils.o ${OBJECTDIR}/src/CabalarUtils_nomain.o;\
	fi

${OBJECTDIR}/src/Formula_nomain.o: ${OBJECTDIR}/src/Formula.o src/Formula.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Formula.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Formula_nomain.o src/Formula.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Formula.o ${OBJECTDIR}/src/Formula_nomain.o;\
	fi

${OBJECTDIR}/src/Formulas_nomain.o: ${OBJECTDIR}/src/Formulas.o src/Formulas.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Formulas.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Formulas_nomain.o src/Formulas.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Formulas.o ${OBJECTDIR}/src/Formulas_nomain.o;\
	fi

${OBJECTDIR}/src/HengZhang_nomain.o: ${OBJECTDIR}/src/HengZhang.o src/HengZhang.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/HengZhang.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HengZhang_nomain.o src/HengZhang.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/HengZhang.o ${OBJECTDIR}/src/HengZhang_nomain.o;\
	fi

${OBJECTDIR}/src/NNFUtils_nomain.o: ${OBJECTDIR}/src/NNFUtils.o src/NNFUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/NNFUtils.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/NNFUtils_nomain.o src/NNFUtils.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/NNFUtils.o ${OBJECTDIR}/src/NNFUtils_nomain.o;\
	fi

${OBJECTDIR}/src/Rule_nomain.o: ${OBJECTDIR}/src/Rule.o src/Rule.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Rule.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Rule_nomain.o src/Rule.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Rule.o ${OBJECTDIR}/src/Rule_nomain.o;\
	fi

${OBJECTDIR}/src/S2DLP_nomain.o: ${OBJECTDIR}/src/S2DLP.o src/S2DLP.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/S2DLP.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/S2DLP_nomain.o src/S2DLP.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/S2DLP.o ${OBJECTDIR}/src/S2DLP_nomain.o;\
	fi

${OBJECTDIR}/src/Utils_nomain.o: ${OBJECTDIR}/src/Utils.o src/Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Utils.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Utils_nomain.o src/Utils.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Utils.o ${OBJECTDIR}/src/Utils_nomain.o;\
	fi

${OBJECTDIR}/src/Vocabulary_nomain.o: ${OBJECTDIR}/src/Vocabulary.o src/Vocabulary.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Vocabulary.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Vocabulary_nomain.o src/Vocabulary.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Vocabulary.o ${OBJECTDIR}/src/Vocabulary_nomain.o;\
	fi

${OBJECTDIR}/src/global_nomain.o: ${OBJECTDIR}/src/global.o src/global.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/global.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/global_nomain.o src/global.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/global.o ${OBJECTDIR}/src/global_nomain.o;\
	fi

${OBJECTDIR}/src/lex_nomain.o: ${OBJECTDIR}/src/lex.o src/lex.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/lex.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/lex_nomain.o src/lex.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/lex.o ${OBJECTDIR}/src/lex_nomain.o;\
	fi

${OBJECTDIR}/src/main_nomain.o: ${OBJECTDIR}/src/main.o src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main_nomain.o src/main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/main_nomain.o;\
	fi

${OBJECTDIR}/src/parse_nomain.o: ${OBJECTDIR}/src/parse.o src/parse.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/parse.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Wall -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/parse_nomain.o src/parse.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/parse.o ${OBJECTDIR}/src/parse_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/alpha

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
