
# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
$(ROOT)/ver2/akima.cc \
$(ROOT)/ver2/ControlSmthData.cc \
$(ROOT)/ver2/Deconvolution2.cc \
$(ROOT)/ver2/main_peaksearch.cc \
$(ROOT)/ver2/mu_sig.cc \
$(ROOT)/ver2/PeakSearch.cc \
$(ROOT)/ver2/SavitzkyGolay.cc \

OBJS += \
object/ver2/akima.o \
object/ver2/ControlSmthData.o \
object/ver2/Deconvolution2.o \
object/ver2/main_peaksearch.o \
object/ver2/mu_sig.o \
object/ver2/PeakSearch.o \
object/ver2/SavitzkyGolay.o \

DEPS += \
${addprefix object/ver2/, \
akima.d \
ControlSmthData.d \
Deconvolution2.d \
main_peaksearch.d \
mu_sig.d \
PeakSearch.d \
SavitzkyGolay.d \
}


# Each subdirectory must supply rules for building sources it contributes
object/ver2/%.o: $(ROOT)/ver2/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ $(CXXFLAGS) -o$@ $<
	@g++ $(CXXFLAGS) -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ $(PREFLAGS) $(CXXFLAGS) $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


