#include "servo_motor.h"

extern TIM_HandleTypeDef htim3;
extern volatile int TIM2_servo_motor_count;// this is 1ms count variable

uint8_t temp_servo_state = 0;
/*
 * TIM3 operating frequency : 84MHz
 * prescaler : 1680 -> TIM3 : 50KHz
 * Counter Period : 1000-1
 * frequency of PWM signal : 50Hz
 * T = 20ms
 * Servomotor 180 degrees : 2ms duty
 * Servomotor 90 degrees : 1.5ms duty
 * Servomotor 0 degrees : 1ms duty
 * */


void servo_motor_main(void) {
	static uint8_t servo_state = 0;

	if (TIM2_servo_motor_count >= 1000) {
		TIM2_servo_motor_count = 0;
		// every 1sec
		/* rotation order of dgrees : 180 --> 90 --> 0 */
		switch (servo_state) {
		case 0:
			// 1.rotation of 180 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 126); // 2ms
			break;
		case 1:
			// 1.rotation of 90 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 75);
			break;
		case 2:
			// 1.rotation of 0 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 28);
			break;
		}
		servo_state++;
		servo_state %= 3;

	}

}

void temp_servo_motor_run(void) {
		if(TIM2_servo_motor_count == 5000){
			temp_servo_state = 2;
		}
		// every 1sec
		/* rotation order of dgrees : 180 --> 90 --> 0 */
		switch (temp_servo_state) {
		case 0:
			// 1.rotation of 180 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 126); // 2ms
			break;
		case 1:
			// 1.rotation of 90 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 75);
			HAL_GPIO_WritePin(GPIOB, 0x0F, GPIO_PIN_SET);
			break;
		case 2:
			// 1.rotation of 0 dgrees
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 28);
			HAL_GPIO_WritePin(GPIOB, 0x0F, GPIO_PIN_RESET);
			break;
		}

}
