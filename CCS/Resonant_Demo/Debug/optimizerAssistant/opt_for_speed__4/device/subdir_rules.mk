################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.8.0.STS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --opt_for_speed=4 --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/driverlib/f28004x/driverlib" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f28004x/headers/include" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f28004x/common/include" --include_path="C:/Users/rober/Downloads/Resonant_Demo" --include_path="C:/Users/rober/Downloads/Resonant_Demo/driverlib" --include_path="C:/Users/rober/Downloads/Resonant_Demo/device" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.8.0.STS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Users/rober/Downloads/Resonant_Demo/Debug__opt_for_speed__4/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


