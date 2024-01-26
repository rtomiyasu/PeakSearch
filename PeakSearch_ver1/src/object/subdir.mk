
# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
$(ROOT)/ControlFile.cc \
$(ROOT)/OrthoPolynomial.cc \
$(ROOT)/ProfileData.cc \

OBJS += \
object/ControlFile.o \
object/OrthoPolynomial.o \
object/ProfileData.o \

DEPS += \
${addprefix object/, \
ControlFile.d \
OrthoPolynomial.d \
ProfileData.d \
}


# Each subdirectory must supply rules for building sources it contributes
object/%.o: $(ROOT)/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ $(CXXFLAGS) -o$@ $<
	@g++ $(CXXFLAGS) -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ $(PREFLAGS) $(CXXFLAGS) $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '
