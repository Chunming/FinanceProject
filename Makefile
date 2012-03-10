#
# General Hash Function Algorithms Master MakeFile
# By Arash Partow - 2000
#
# URL: http://www.partow.net/programming/hashfunctions/index.html
#
# Copyright Notice:
# Free use of this library is permitted under the
# guidelines and in accordance with the most
# current version of the Common Public License.
# http://www.opensource.org/licenses/cpl1.0.php
#

COMPILER      = -c++
OPTIONS       = -ansi -pedantic -Wall -o
OPTIONS_LIBS  = -ansi -pedantic -Wall -c

all:  data

#GeneralHashFunctions.o: GeneralHashFunctions.cpp GeneralHashFunctions.h
#	$(COMPILER) $(OPTIONS_LIBS) GeneralHashFunctions.cpp

data: data.cpp
	$(COMPILER) $(OPTIONS) data data.cpp 

clean:
	rm -f core *.o *.bak *stackdump *#
  
#
# The End !
#
