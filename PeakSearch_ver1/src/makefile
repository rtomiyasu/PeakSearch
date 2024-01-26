# Makefile
# 2007/5/1

ROOT := src

RM := rm -rf

DEBUG	= # -g3 -DDEBUG -O0 
RELEASE	= -O3 -DNDEBUG
LDFLAGS	= -static
PREFLAGS = -MM -MG -P -w
CXXFLAGS = $(RELEASE) $(DEBUG) -Wall -c -fmessage-length=0

# All of the sources participating in the build are defined here

-include sources.mk
-include $(SUBDIRS:%=%/subdir.mk)
-include objects.mk

# 2007/5/1
# OBJS_PEAKSEARCH
# .o files linked to PeakSearch.exe
OBJS_PEAKSEARCH := 
-include link_peaksearch.mk

OBJS_COMMON := $(OBJS_PEAKSEARCH)

ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: PeakSearch.exe

# Tool invocations
PeakSearch.exe: $(OBJS_PEAKSEARCH) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	@echo g++ $(LDFLAGS) -o PeakSearch.exe $(OBJS_PEAKSEARCH) $(USER_OBJS) $(LIBS)
	@g++ $(LDFLAGS) -o PeakSearch.exe $(OBJS_PEAKSEARCH) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	rm -rf PeakSearch.exe
	rm -rf object/*.d object/*.o object/*/*.d object/*/*.o object/*/*/*.d object/*/*/*.o object/*/*/*/*.d object/*/*/*/*.o
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
