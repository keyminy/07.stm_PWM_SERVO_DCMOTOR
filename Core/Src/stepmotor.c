#include "stepmotor.h"
#include "def.h"
#include "button.h"

int  motor_state = MOTOR_IDLE; // default : MOTOR_IDLE

extern void delay_us(unsigned int us);
void set_rpm(int rpm);
void stepmotor_main(void);
void stepmotor_drive(int step);
void stop_motor(void);
void perform_motor_step_forward();
void perform_motor_step_backward();
/*
 * RPM(Revolutions Per Minutes) : 분 당 회전수
 * 1분 : 60sec : 1,000,000us(1sec) * 60 = 60,000,000us
 * 1초 : 1000ms --> 1ms(=1000us) * 1000ms ==> 1,000,000us
 * us단위로 모터를 제어해야함
 * 1바퀴 회전하는데 4096step 필요
 * 1sequence가 8step이다. 4096 / 8 = 512sequence 필요 : 360도 회전함
 * 1sequence(=8step) : 0.70312도
 * 0.70312도 * 512sequence = 360도
 * --- RPM 조정 함수 ---
 * 60,000,000us(1분) / 4096(1바퀴 도는데 필요한 STEP) / 13 ==> step과 step 간격 interval 1126us
 * 1126us * 4096(=1회전시 필요한 step) = 4,615,384us
 * 										= 4615ms
 * 										= 4.6초
 * 	1분에 몇바퀴 돌지?
 * 	60초/4.6초(1회전시 소요시간 초) = 13번 회전이 맞습니다(검증완료)
 * */

// rpm조절 함수 생성
void set_rpm(int rpm) {
	// 1분에 13번 회전할 수 있도록 interval주겠다.
	// 매개변수 rpm값은 1~13
	delay_us(60000000/4096/rpm);
	// 최대 speed기준(=13) :delay_us(1,126.8028846153);
}

void stepmotor_main(void) {
	switch (motor_state) {
	case MOTOR_IDLE:
		// When idle, ensure the motor is stopped
		stop_motor();
		break;
	case MOTOR_FORWARD:
		// Execute one step forward
		perform_motor_step_forward();
		break;
	case MOTOR_BACKWARD:
		// Execute one step backward
		perform_motor_step_backward();
		break;
	}
}

void perform_motor_step_forward() {
	static int step = 0; // This static variable retains its value between function calls
	stepmotor_drive(step); // Perform one step
	step = (step + 1) % 8; // Cycle through step sequence
	set_rpm(13); // 1,126.8028846153us delay
}

void perform_motor_step_backward() {
	static int step = 7; // This static variable retains its value between function calls
	stepmotor_drive(step); // Perform one step
	step = (step -1 + 8) % 8; // Cycle through step sequence
	// if(step <0) step = 7;
	set_rpm(13); // 1,126.8028846153us delay
}

void stepmotor_drive(int step){
	switch(step){
	case 0: // 1
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 1: // 2
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 2: // 3
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 3: // 4
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 4: // 5
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 5: // 6
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
		break;
	case 6:// 7
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
		break;
	case 7: // 8
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
		break;
	}
}

void stop_motor(void){
	HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
	HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
	HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
	HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
}
