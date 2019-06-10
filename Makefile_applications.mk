all: $(TARGET)

$(TARGET): $(OBJECTS) $(OWN_LIBS)
	$(ECHO) $(LD) linking $(TARGET) against $(addsuffix .a,$(OWN_LIBS_NAMES))
	$(MAKE_LINKER) -o $(DIR_BINARY)/$(TARGET) $(MAKE_COMPILER_FLAGS) $^ $(MAKE_LINKER_FLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(MKDIR) $(MKDIRFLAGS) $(BUILDDIR)
	$(ECHO) $(CPP) compiling $*.$(SRCEXT)
	$(MAKE_COMPILER) $< $(MAKE_COMPILER_FLAGS) -c -o $@

$(OWN_LIBS): 
	$(eval OWN_LIB_NAME := $(notdir $*))
	$(eval OWN_LIB_DIR  := $(addprefix $(DIR_PROJECTS)/,$(OWN_LIB_NAME)))
	$(ECHO) looking for $(OWN_LIB_NAME) from $(OWN_LIB_DIR)
	$(MAKE) $(MFLAGS) -C $(OWN_LIB_DIR)

clean:
	$(ECHO) $(RM) removing all related .$(TARGET_EXT) files from $(realpath $(DIR_BUILD)) and $(realpath $(DIR_BINARY))/$(TARGET)
	$(RM) $(RMFLAGS) $(OBJECTS) $(DIR_BINARY)/$(TARGET)
  
run: all
	./$(TARGET)
