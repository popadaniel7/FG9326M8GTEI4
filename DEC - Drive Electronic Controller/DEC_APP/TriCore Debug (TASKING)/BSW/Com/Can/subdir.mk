################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/Com/Can/Can.c" 

COMPILED_SRCS += \
"BSW/Com/Can/Can.src" 

C_DEPS += \
"./BSW/Com/Can/Can.d" 

OBJS += \
"BSW/Com/Can/Can.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/Com/Can/Can.src":"../BSW/Com/Can/Can.c" "BSW/Com/Can/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fC:/Users/Daniel/Desktop/FG9326M8GTEI4/DEC - Drive Electronic Controller/DEC_APP/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/Com/Can/Can.o":"BSW/Com/Can/Can.src" "BSW/Com/Can/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-Com-2f-Can

clean-BSW-2f-Com-2f-Can:
	-$(RM) ./BSW/Com/Can/Can.d ./BSW/Com/Can/Can.o ./BSW/Com/Can/Can.src

.PHONY: clean-BSW-2f-Com-2f-Can

