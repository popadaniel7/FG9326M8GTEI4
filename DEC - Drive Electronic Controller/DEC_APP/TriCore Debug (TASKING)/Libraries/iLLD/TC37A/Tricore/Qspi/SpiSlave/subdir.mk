################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.c" 

COMPILED_SRCS += \
"Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.src" 

C_DEPS += \
"./Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.d" 

OBJS += \
"Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.o" 


# Each subdirectory must supply rules for building sources it contributes
"Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.src":"../Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.c" "Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/Daniel/Desktop/FG9326M8GTEI4/DEC - Drive Electronic Controller/DEC_APP/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.o":"Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.src" "Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Qspi-2f-SpiSlave

clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Qspi-2f-SpiSlave:
	-$(RM) ./Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.d ./Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.o ./Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/IfxQspi_SpiSlave.src

.PHONY: clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Qspi-2f-SpiSlave

