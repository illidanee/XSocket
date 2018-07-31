#-------------------------------------------------------------------------------- 
#        Compiler Setting 
#-------------------------------------------------------------------------------- 
#Compiler 
CC = g++ 

#Flag 
FLAG = -g -O0 -std=c++11 -I/usr/local/include/mariadb  -L/usr/local/lib/mariadb
#Library 
LIBS = -pthread -lmariadb 


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
ObjList = $(addsuffix .o, $(BaseList)) 
#ObjList = $(addsuffix .o, $(BaseList)) 

#-------------------------------------------------------------------------------- 
#        .d Ruler File Generate 
#-------------------------------------------------------------------------------- 
dFileList = $(addsuffix .d, $(BaseList)) 
#dFileList = $(addsuffix .d, $(BaseList)) 
-include $(dFileList) 

%.d : %.cpp
	@$(CC) $(FLAG) -MM $< > $@; 

%.o : %.cpp 
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
	rm -f *.d
	rm -f *.o
