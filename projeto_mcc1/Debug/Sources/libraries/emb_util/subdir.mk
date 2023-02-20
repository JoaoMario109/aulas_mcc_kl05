################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/libraries/emb_util/emb_util.c 

OBJS += \
./Sources/libraries/emb_util/emb_util.o 

C_DEPS += \
./Sources/libraries/emb_util/emb_util.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/libraries/emb_util/%.o: ../Sources/libraries/emb_util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"../Includes" -std=c99 -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


