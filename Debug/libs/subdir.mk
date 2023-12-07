################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/audiobin.cpp \
../libs/features.cpp \
../libs/featureskey.cpp \
../libs/gkt_gui.cpp \
../libs/metadata.cpp \
../libs/navigator.cpp \
../libs/playback.cpp \
../libs/playbin.cpp \
../libs/videobin.cpp 

CPP_DEPS += \
./libs/audiobin.d \
./libs/features.d \
./libs/featureskey.d \
./libs/gkt_gui.d \
./libs/metadata.d \
./libs/navigator.d \
./libs/playback.d \
./libs/playbin.d \
./libs/videobin.d 

OBJS += \
./libs/audiobin.o \
./libs/features.o \
./libs/featureskey.o \
./libs/gkt_gui.o \
./libs/metadata.o \
./libs/navigator.o \
./libs/playback.o \
./libs/playbin.o \
./libs/videobin.o 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.cpp libs/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/atk-1.0/ -I/usr/include/gdk-pixbuf-2.0/ -I/usr/include/cairo/ -I/usr/include/harfbuzz/ -I/usr/include/pango-1.0/ -I/usr/include/gtk-3.0/ -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-libs

clean-libs:
	-$(RM) ./libs/audiobin.d ./libs/audiobin.o ./libs/features.d ./libs/features.o ./libs/featureskey.d ./libs/featureskey.o ./libs/gkt_gui.d ./libs/gkt_gui.o ./libs/metadata.d ./libs/metadata.o ./libs/navigator.d ./libs/navigator.o ./libs/playback.d ./libs/playback.o ./libs/playbin.d ./libs/playbin.o ./libs/videobin.d ./libs/videobin.o

.PHONY: clean-libs

