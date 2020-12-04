################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.8.0.STS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ADC_Operation.obj" \
"./Init_Resonant.obj" \
"./Main_Resonant.obj" \
"./PWM_Operation.obj" \
"./f28004x_adc.obj" \
"./f28004x_codestartbranch.obj" \
"./f28004x_defaultisr.obj" \
"./f28004x_globalvariabledefs.obj" \
"./f28004x_gpio.obj" \
"./f28004x_piectrl.obj" \
"./f28004x_pievect.obj" \
"./f28004x_sysctrl.obj" \
"./f28004x_usdelay.obj" \
"./device/device.obj" \
"../280049C_RAM_lnk.cmd" \
"C:/ti/c2000/C2000Ware_3_03_00_00/driverlib/f28004x/driverlib/ccs/Debug/driverlib.lib" \
"../f28004x_headers_nonbios.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include device/subdir_vars.mk
-include subdir_rules.mk
-include device/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Resonant_Demo.out \

EXE_OUTPUTS__QUOTED += \
"Resonant_Demo.out" \

BIN_OUTPUTS += \
Resonant_Demo.hex \

BIN_OUTPUTS__QUOTED += \
"Resonant_Demo.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Resonant_Demo.out"

# Tool invocations
Resonant_Demo.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.8.0.STS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --opt_for_speed=1 --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"Resonant_Demo.map" --stack_size=0x200 --warn_sections -i"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.8.0.STS/lib" -i"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.8.0.STS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="Resonant_Demo_linkInfo.xml" --rom_model -o "Resonant_Demo.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Resonant_Demo.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.8.0.STS/bin/hex2000"  -o "Resonant_Demo.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "ADC_Operation.obj" "Init_Resonant.obj" "Main_Resonant.obj" "PWM_Operation.obj" "f28004x_adc.obj" "f28004x_codestartbranch.obj" "f28004x_defaultisr.obj" "f28004x_globalvariabledefs.obj" "f28004x_gpio.obj" "f28004x_piectrl.obj" "f28004x_pievect.obj" "f28004x_sysctrl.obj" "f28004x_usdelay.obj" "device\device.obj" 
	-$(RM) "ADC_Operation.d" "Init_Resonant.d" "Main_Resonant.d" "PWM_Operation.d" "f28004x_adc.d" "f28004x_defaultisr.d" "f28004x_globalvariabledefs.d" "f28004x_gpio.d" "f28004x_piectrl.d" "f28004x_pievect.d" "f28004x_sysctrl.d" "device\device.d" 
	-$(RM) "f28004x_codestartbranch.d" "f28004x_usdelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

