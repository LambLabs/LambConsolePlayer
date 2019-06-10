#locations
DIR_PROJECTS := projects
DIR_BUILD    := tmp
DIR_BINARY   := bin

#make
NUM_CORES ?= $(shell grep -c "vendor_id" /proc/cpuinfo)
MAKEFLAGS += --silent

#tools
ECHO := echo

#libs and projects
OWN_LIBS_NAMES := lib_core
OWN_APPS_NAMES := console_player

OWN_LIBS := $(addprefix $(DIR_BINARY)/,$(addsuffix .a,$(OWN_LIBS_NAMES)))
OWN_APPS := $(addprefix $(DIR_BINARY)/,$(addsuffix .a,$(OWN_APPS_NAMES)))

.PHONY: all 
all: buld_libs build_apps

.PHONY: buld_libs
buld_libs: $(OWN_LIBS)

$(OWN_LIBS): 
	$(eval OWN_LIB_NAME := $(notdir $*))
	$(eval OWN_LIB_DIR  := $(addprefix $(DIR_PROJECTS)/,$(OWN_LIB_NAME)))
	$(ECHO) looking for $(OWN_LIB_NAME) from $(OWN_LIB_DIR)
	$(MAKE) $(MFLAGS) -C $(OWN_LIB_DIR)

.PHONY: build_apps
build_apps: $(OWN_APPS)

$(OWN_APPS):
	$(eval OWN_APP_NAME := $(notdir $*))
	$(eval OWN_APP_DIR  := $(addprefix $(DIR_PROJECTS)/,$(OWN_APP_NAME)))
	$(ECHO) looking for $(OWN_APP_NAME) from $(OWN_APP_DIR)
	$(MAKE) $(MFLAGS) -C $(OWN_APP_DIR)


#clean
CLEAN_LIBS = $(addsuffix .clean,$(OWN_LIBS))
CLEAN_APPS = $(addsuffix .clean,$(OWN_APPS))
.PHONY: clean $(CLEAN_LIBS) $(CLEAN_APPS)

clean: $(CLEAN_LIBS) $(CLEAN_APPS)

$(CLEAN_LIBS): %.clean:
	$(eval OWN_LIB_NAME := $(basename  $(notdir $*)))
	$(eval OWN_LIB_DIR  := $(addprefix $(DIR_PROJECTS)/,$(OWN_LIB_NAME)))
	$(ECHO) cleaning $(OWN_LIB_NAME) from $(OWN_LIB_DIR)
	$(MAKE) $(MFLAGS) -C $(OWN_LIB_DIR) clean

$(CLEAN_APPS): %.clean:
	$(eval OWN_APP_NAME := $(basename $(notdir $*)))
	$(eval OWN_APP_DIR  := $(addprefix $(DIR_PROJECTS)/,$(OWN_APP_NAME)))
	$(ECHO) cleaning $(OWN_APP_NAME) from $(OWN_APP_DIR)
	$(MAKE) $(MFLAGS) -C $(OWN_APP_DIR) clean
