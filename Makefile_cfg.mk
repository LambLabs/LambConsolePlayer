#===================================================================================================================
#locations
DIR_PROJECTS := ../../projects
DIR_BUILD    := ../../tmp
DIR_BINARY   := ../../bin

#===================================================================================================================
#make
NUM_CORES ?= $(shell grep -c "vendor_id" /proc/cpuinfo)
MAKEFLAGS += --silent --jobs $(NUM_CORES)

#===================================================================================================================
#toolchain - GCC or LLVM
TOOLCHAIN  := GCC

#===================================================================================================================
#compiler
CPP        := $(if $(filter LLVM, $(TOOLCHAIN)), clang++, g++)
CPPFLAGS   := -DNDEBUG -std=c++17 -Ofast -Wall -m64 -march=native -mtune=native -flto 
#CPPFLAGS  += $(if $(filter GCC,  $(TOOLCHAIN)), , )  
CPPFLAGS   += $(if $(filter LLVM, $(TOOLCHAIN)), -stdlib=libstdc++ -O3, )
INCLUDE    := -I./source -I/usr/include/ -I../../external -I../../external/asio/asio/include

#===================================================================================================================
#linker
LD	       := $(if $(filter LLVM, $(TOOLCHAIN)), clang++, g++)
LDFLAGS	   := -m64 -L/usr/lib/ -L/usr/x86_64-linux-gnu/ $(if $(filter LLVM, $(TOOLCHAIN)), -stdlib=libstdc++, )
LIBS       := -lpthread 

#===================================================================================================================
#librarian
AR         := $(if $(filter LLVM, $(TOOLCHAIN)), llvm-ar, gcc-ar)
ARFLAGS    := rcs

#===================================================================================================================
#remover
RM	       := -rm
RMFLAGS    := -f

#===================================================================================================================
#other
CD         := cd
ECHO	     := echo
SHELL	     := /bin/sh
MKDIR      := mkdir
MKDIRFLAGS := -p
