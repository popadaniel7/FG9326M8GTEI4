################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.c" \
"../Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.c" 

COMPILED_SRCS += \
"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.src" \
"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.src" 

C_DEPS += \
"./Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.d" \
"./Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.d" 

OBJS += \
"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.o" \
"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.o" 


# Each subdirectory must supply rules for building sources it contributes
"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.src":"../Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.c" "Libraries/iLLD/TC37A/Tricore/Smu/Std/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fC:/Users/Daniel/Desktop/FG9326M8GTEI4/DEC - Drive Electronic Controller/DEC_APP/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.o":"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.src" "Libraries/iLLD/TC37A/Tricore/Smu/Std/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.src":"../Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.c" "Libraries/iLLD/TC37A/Tricore/Smu/Std/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fC:/Users/Daniel/Desktop/FG9326M8GTEI4/DEC - Drive Electronic Controller/DEC_APP/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.o":"Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.src" "Libraries/iLLD/TC37A/Tricore/Smu/Std/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Smu-2f-Std

clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Smu-2f-Std:
	-$(RM) ./Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.d ./Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.o ./Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmu.src ./Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.d ./Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.o ./Libraries/iLLD/TC37A/Tricore/Smu/Std/IfxSmuStdby.src

.PHONY: clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Smu-2f-Std

