#--------------------------------------------------------------------------------
#        Compiler Setting
#--------------------------------------------------------------------------------
#Compiler
CC = g++

#Flag
FLAG = -std=c++11 -DDEBUG -g

#Library
LIBS = -pthread

#--------------------------------------------------------------------------------
#        Project Setting
#--------------------------------------------------------------------------------
#Include Dir
IncDir = include

#LibDir
SrcDir = src

#Temp Dir
TmpDir = tmp

#--------------------------------------------------------------------------------
#        Project Setting
#--------------------------------------------------------------------------------
#Project Name
ProjectName = main

#Src List (etc: a.cpp b.cpp c.cpp …)
SrcList = $(shell ls *.cpp)

#Base Name List (etc: a b c …)
BaseList = $(basename $(SrcList))

#Obj List (etc: dir/a dir/b dir/c …)
ObjList = $(addsuffix .o, $(addprefix $(TmpDir)/,$(BaseList)))

#--------------------------------------------------------------------------------
#        .d Ruler File Generate
#--------------------------------------------------------------------------------
dFileList = $(addsuffix .d, $(addprefix $(TmpDir)/,$(BaseList)))
include $(dFileList)

$(TmpDir)/%.d : %.cpp
	@if test ! -d $(TmpDir); then\
		mkdir -p $(TmpDir);\
	fi;
	@set -e; rm -f $@;
	@$(CC) -MM $< > $@.$$$$;\
		sed 's/^\(.*\)\.o[ :]*/$(TmpDir)\/\1\.o $(TmpDir)\/\1\.d : /g' < $@.$$$$ > $@;\
		rm -f $@.$$$$;\

$(TmpDir)/%.o : %.cpp
	@if test ! -d $(TmpDir); then\
		mkdir -p $(TmpDir);\
	fi;
	$(CC) $(FLAG) -c $< -o $@

#--------------------------------------------------------------------------------
#        Cmd Setting
#--------------------------------------------------------------------------------
#Define Cmd
.PHONY : info all clean

info : 
	@echo $(dFileList)

#Compile All
all : $(ProjectName)
	@echo "-----------------------------------------------------------------"
	@echo "                      Good job!  ~(@^_^@)~"
	@echo "-----------------------------------------------------------------"

$(ProjectName) : $(ObjList)
	$(CC) $(FLAG) $(LIBS) $^ -o $@

#Clean
clean :
	rm -f  $(ProjectName)
	rm -rf $(TmpDir)
