#own librariie dependacies
INCLUDE  += $(addsuffix /source,$(addprefix -I$(DIR_PROJECTS)/,$(OWN_LIBS_NAMES)))
OWN_LIBS := $(addsuffix .a,$(addprefix $(DIR_BINARY)/,$(OWN_LIBS_NAMES)))

#commands to be called
MAKE_COMPILER       := $(CPP)
MAKE_COMPILER_FLAGS := $(CPPFLAGS) $(INCLUDE)
MAKE_LINKER         := $(CPP)
MAKE_LINKER_FLAGS   := $(LDFLAGS) $(LIBS)
MAKE_ARCHIVER       := $(AR)
MAKE_ARCHIVER_FLAGS := $(ARFLAGS)

#input and output files
SRCEXT       := cpp
SRCDIR       := ./source
BUILDDIR     := $(DIR_BUILD)/$(TARGET_NAME)
TARGET       := $(TARGET_NAME).$(TARGET_EXT)
SOURCES      := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS      := $(patsubst $(SRCDIR)/%.o,$(BUILDDIR)/%.o,$(SOURCES:.$(SRCEXT)=.o))
