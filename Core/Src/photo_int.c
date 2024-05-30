#include "photo_int.h"
#include "def.h"
#include "servo_motor.h"

extern volatile int TIM2_servo_motor_count;
extern int  motor_state;
extern TIM_HandleTypeDef htim3;
extern uint8_t temp_servo_state;
/**
 * moved from Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
 * EXTI Callback함수임
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
  switch(GPIO_Pin){
  case GPIO_PIN_0: // PA0
	  printf("GPIO_PIN_0!!\n");
	  temp_servo_state = 1;
	  TIM2_servo_motor_count = 0;
	  break;
  case GPIO_PIN_1: // PA1
	  printf("GPIO_PIN_1!!\n");
	  HAL_GPIO_WritePin(GPIOB, 0x0F, GPIO_PIN_RESET);
	  delay_us(30000); // 30ms
	  HAL_GPIO_WritePin(GPIOB, 0xF0, GPIO_PIN_SET);
//	  motor_state = MOTOR_BACKWARD;
	  break;
  }
}
