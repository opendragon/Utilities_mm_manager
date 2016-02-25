# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=1.4.25" -D "JUCE_APP_VERSION_HEX=0x10419" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I /opt/test/include
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -L/opt/test/lib -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lYARP_OS -lYARP_init -lm+mCommon -lodLogging -lOGDF -lACE -ldns_sd 

  TARGET := m+m\ manager
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=1.4.25" -D "JUCE_APP_VERSION_HEX=0x10419" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I /opt/test/include
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -L/opt/test/lib -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lYARP_OS -lYARP_init -lm+mCommon -lodLogging -lOGDF -lACE -ldns_sd 

  TARGET := m+m\ manager
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/m+mCaptionedTextField_6648b578.o \
  $(OBJDIR)/m+mChannelContainer_8919ffae.o \
  $(OBJDIR)/m+mChannelEntry_3859971f.o \
  $(OBJDIR)/m+mCheckboxField_a8134ed1.o \
  $(OBJDIR)/m+mConfigurationWindow_f373d520.o \
  $(OBJDIR)/m+mContentPanel_19fbccfb.o \
  $(OBJDIR)/m+mEntitiesData_b39ee6fb.o \
  $(OBJDIR)/m+mEntitiesPanel_8032949d.o \
  $(OBJDIR)/m+mEntityData_bbfb3dd.o \
  $(OBJDIR)/m+mFormField_b1a30970.o \
  $(OBJDIR)/m+mFormFieldErrorResponder_7b38382a.o \
  $(OBJDIR)/m+mManagerApplication_abb6df13.o \
  $(OBJDIR)/m+mManagerMain_f38808a0.o \
  $(OBJDIR)/m+mManagerWindow_c4f886d7.o \
  $(OBJDIR)/m+mPeekInputHandler_b0a65b8b.o \
  $(OBJDIR)/m+mPortData_76190d3b.o \
  $(OBJDIR)/m+mRegistryLaunchThread_377125ea.o \
  $(OBJDIR)/m+mScannerThread_24261102.o \
  $(OBJDIR)/m+mServiceLaunchThread_6460918c.o \
  $(OBJDIR)/m+mSettingsWindow_74442b23.o \
  $(OBJDIR)/m+mTextValidator_a298b97f.o \
  $(OBJDIR)/m+mValidatingTextEditor_5e2c0d9d.o \
  $(OBJDIR)/m+mYarpLaunchThread_92995473.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_core_aff681cc.o \
  $(OBJDIR)/juce_data_structures_bdd6d488.o \
  $(OBJDIR)/juce_events_79b2840.o \
  $(OBJDIR)/juce_graphics_c8f1e7a4.o \
  $(OBJDIR)/juce_gui_basics_a630dd20.o \
  $(OBJDIR)/juce_gui_extra_7767d6a8.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking m+m manager
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning m+m manager
	@$(CLEANCMD)

strip:
	@echo Stripping m+m manager
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/m+mCaptionedTextField_6648b578.o: ../../Source/m+mCaptionedTextField.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mCaptionedTextField.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mChannelContainer_8919ffae.o: ../../Source/m+mChannelContainer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mChannelContainer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mChannelEntry_3859971f.o: ../../Source/m+mChannelEntry.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mChannelEntry.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mCheckboxField_a8134ed1.o: ../../Source/m+mCheckboxField.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mCheckboxField.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mConfigurationWindow_f373d520.o: ../../Source/m+mConfigurationWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mConfigurationWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mContentPanel_19fbccfb.o: ../../Source/m+mContentPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mContentPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mEntitiesData_b39ee6fb.o: ../../Source/m+mEntitiesData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mEntitiesData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mEntitiesPanel_8032949d.o: ../../Source/m+mEntitiesPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mEntitiesPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mEntityData_bbfb3dd.o: ../../Source/m+mEntityData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mEntityData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mFormField_b1a30970.o: ../../Source/m+mFormField.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mFormField.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mFormFieldErrorResponder_7b38382a.o: ../../Source/m+mFormFieldErrorResponder.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mFormFieldErrorResponder.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mManagerApplication_abb6df13.o: ../../Source/m+mManagerApplication.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mManagerApplication.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mManagerMain_f38808a0.o: ../../Source/m+mManagerMain.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mManagerMain.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mManagerWindow_c4f886d7.o: ../../Source/m+mManagerWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mManagerWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mPeekInputHandler_b0a65b8b.o: ../../Source/m+mPeekInputHandler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mPeekInputHandler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mPortData_76190d3b.o: ../../Source/m+mPortData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mPortData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mRegistryLaunchThread_377125ea.o: ../../Source/m+mRegistryLaunchThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mRegistryLaunchThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mScannerThread_24261102.o: ../../Source/m+mScannerThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mScannerThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mServiceLaunchThread_6460918c.o: ../../Source/m+mServiceLaunchThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mServiceLaunchThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mSettingsWindow_74442b23.o: ../../Source/m+mSettingsWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mSettingsWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mTextValidator_a298b97f.o: ../../Source/m+mTextValidator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mTextValidator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mValidatingTextEditor_5e2c0d9d.o: ../../Source/m+mValidatingTextEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mValidatingTextEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m+mYarpLaunchThread_92995473.o: ../../Source/m+mYarpLaunchThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m+mYarpLaunchThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_aff681cc.o: ../../JuceLibraryCode/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_bdd6d488.o: ../../JuceLibraryCode/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_79b2840.o: ../../JuceLibraryCode/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_c8f1e7a4.o: ../../JuceLibraryCode/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_a630dd20.o: ../../JuceLibraryCode/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_7767d6a8.o: ../../JuceLibraryCode/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
