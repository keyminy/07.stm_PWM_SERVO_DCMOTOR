#include "dc_motor.h"
#include "button.h"

uint16_t CCR_value = 0; //PWM control value
extern TIM_HandleTypeDef htim1;
void dc_motor_pwm_control(void);
void UpdateLEDBasedOnPWM();

static uint8_t pwm_start_flag=0;

void dc_motor_pwm_control(void)
{
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0)== BUTTON_PRESS){
		// speed up+10(increase duty ratio 0.1ms)
		CCR_value = __HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_1); // PWM 설정값을 읽어온다.
		CCR_value += 10;
		if(CCR_value >= 100) CCR_value = 100;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, CCR_value);
	}
	if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON1)== BUTTON_PRESS){
		// speed down-10(decrease duty ratio 0.1ms)
		CCR_value = __HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_1); // PWM 설정값을 읽어온다.
		CCR_value -= 10;
		if(CCR_value <= 60) CCR_value = 60;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, CCR_value);
	}
	if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, BUTTON2)== BUTTON_PRESS){
		// pwm start-- pwm stop
		if(!pwm_start_flag){
			// begin start
			HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
			pwm_start_flag = 1;
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		}else{
			pwm_start_flag = 0;
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 60);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		}
	}
}

void UpdateLEDBasedOnPWM()
{
	uint8_t pattern = 0xF0;
	if (CCR_value <= 60 ) {
		HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, pattern, GPIO_PIN_SET);
	} else if (CCR_value <= 70 ) {
		 pattern |= 0x08; // pattern = 0xF0 | 0x08
		HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, pattern, GPIO_PIN_SET);
	} else if (CCR_value <= 80 ) {
		pattern |= 0x0C; // pattern = 0xF0 | 0x0C
		HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, pattern, GPIO_PIN_SET);
	} else if (CCR_value <= 90 ) {
		 pattern |= 0x0E; // pattern = 0xF0 | 0x0E
		HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, pattern, GPIO_PIN_SET);
	} else if (CCR_value <= 100) {
		 pattern = 0xFF; // All bits set
		HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, pattern, GPIO_PIN_SET);
	}
}
