################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../testing_arena.cpp 

CPP_DEPS += \
./testing_arena.d 

OBJS += \
./testing_arena.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/atk-1.0/ -I/usr/include/gdk-pixbuf-2.0/ -I/usr/include/cairo/ -I/usr/include/harfbuzz/ -I/usr/include/pango-1.0/ -I/usr/include/gtk-3.0/ -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./testing_arena.d ./testing_arena.o

.PHONY: clean--2e-

