###############################################################################
# $Id: Makefile 1723 2008-01-16 17:46:33Z mellinger $
# File: Makefile
#
# Author: juergen.mellinger@uni-tuebingen.de
#
# Description: Makefile for the ParallelSwitch application.
#
# (C) 2000-2008, BCI2000 Project
# http://www.bci2000.org
###############################################################################
AND = &&
OR  = ||

TARGETS = ParallelSwitch.exe
          
OBJ = obj\ParallelSwitch.obj obj\SockStream.obj 
          
BCIROOT = ..\..\..
PATHOBJ = .\obj
PATHCPP = .;${BCIROOT}\shared\utils
INCLUDEPATH = ${PATHCPP};${BCIROOT}\shared\config;${BCB}\include;${BCB}\include\vcl
LIBPATH = ${BCB}\lib\obj;${BCB}\lib

DEFINES = _DEBUG;BCI_TOOL;NO_STRICT;_NO_VCL
CFLAGS = -Od -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- -c -tW -tWM -w-par

CPP = ${CC} ${CFLAGS} -D${DEFINES} -I${INCLUDEPATH}
LD  = bcc32 -L${LIBPATH}
RM  = del 

.autodepend
.path.cpp=${PATHCPP}

ParallelSwitch.exe: ${OBJ}
	${LD} ${OBJ}

.cpp.obj:
	if not exist ${PATHOBJ} mkdir ${PATHOBJ}
	${CPP} -c -o${PATHOBJ}\${*F}.obj $<

all: ${TARGETS}

clean:
	-${RM} ${PATHOBJ}\*.obj ${PATHOBJ}\*.lib ${PATHOBJ}\*.BAK *.tds

distclean: clean
	-${RM} *.exe *.~* *.il*
