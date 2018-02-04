#--------------------------------------------------------------------------------
#        Compiler Setting
#--------------------------------------------------------------------------------
#Compiler
CC = g++

#Flag
FLAG_COM = -std=c++11 -DDEBUG -g

#Library
LIBS = -pthread

#--------------------------------------------------------------------------------
#        Project Setting
#--------------------------------------------------------------------------------
#Include Dir
hDir = include

#LibDir
sDir = src

#Temp Dir
TempDir = tmp

#Project Name
ProjectName = Demo

#Src List (etc: a.cpp b.cpp c.cpp …)
SrcList = $(shell ls *.cpp)

#Base Name List (etc: a b c …)
BaseList = $(basename $(SrcList))

#Obj List (etc: dir/a dir/b dir/c …)
ObjList = $(addsuffix .o, $(addprefix $(TempDir)/,$(BaseList)))

#--------------------------------------------------------------------------------
#        Cmd Setting
#--------------------------------------------------------------------------------
#Define Cmd
.PHONY : info all clean 

info : 
	@echo $(BaseList)


#Compile All
all : $(ProjectName)
	@echo "-----------------------------------------------------------------"
	@echo "                      Good job!  ~(@^_^@)~"
	@echo "-----------------------------------------------------------------"

$(ProjectName) : $(ObjList)
	$(CC) $(FLAG_COM) $(LIBS) $^ -o $@

$(TempDir)/%.o : %.cpp
	@if test ! -d $(TempDir); then\
		mkdir -p $(TempDir);\
	fi;
	$(CC) $(FLAG_COM) -c $< -o $@

#Clean
clean :
	rm -f  $(ProjectName)
	rm -rf $(TempDir)
