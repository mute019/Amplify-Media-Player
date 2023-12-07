################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include libs/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Custom_Playbin
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: Custom_Playbin

# Tool invocations
Custom_Playbin: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/usr/lib/x86_64-linux-gnu/ -L/usr/lib/gstreamer-1.0 -o "Custom_Playbin" $(OBJS) $(USER_OBJS) $(LIBS) -lglib-2.0 -lgstpbutils-1.0 -lpthread -lgstvideo-1.0 -lcairo -lgdk-3 -lgtk-3 -lpangocairo-1.0 -lgstnet-1.0 -lgobject-2.0 -lgstbase-1.0 -lgstcheck-1.0 -lgstreamer-1.0 -lboost_filesystem -lgstcontroller-1.0 -lboost_system
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Custom_Playbin
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
