all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(ECHO) $(AR) libraring $(TARGET)
	$(MAKE_ARCHIVER) $(MAKE_ARCHIVER_FLAGS) $(DIR_BINARY)/$(TARGET) $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(MKDIR) $(MKDIRFLAGS) $(BUILDDIR)
	$(ECHO) $(CPP) compiling $*.$(SRCEXT)
	$(MAKE_COMPILER) $< $(MAKE_COMPILER_FLAGS) -c -o $@

clean:
	$(ECHO) $(RM) removing all related .$(TARGET_EXT) files from $(realpath $(DIR_BUILD)) and $(realpath $(DIR_BINARY))/$(TARGET)
	$(RM) $(RMFLAGS) $(OBJECTS) $(DIR_BINARY)/$(TARGET)
