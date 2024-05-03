/*
 * Arms.h
 *
 *  Created on: May 3, 2024
 *      Author: matthijs
 */

#ifndef ARMS_INC_ARMS_H_
#define ARMS_INC_ARMS_H_

#include "RobotGlobals.h"
#include "stm32f2xx_hal.h"

enum ENUM_HomeState {
	NotHomed,
	Homed
};

enum ENUM_ArmDirection {
	Arm_Up,
	Arm_Down
};

enum ENUM_MotionState {
	Motion_Disabled,
	Motion_AtTarget,
	Motion_Error,
	Motion_MovingUp,
	Motion_MovingDown
};

struct Arm_State_Type {
	enum ENUM_HomeState Homed;
	enum ENUM_MotionState MotionState;
	enum ENUM_ArmDirection Direction;
	int Angle;
	int Timer;
	int SelTestRunning;
	int Speed;
	TIM_HandleTypeDef *TIM;
	};

//------------------------------------------------
void LeftArm_Init(TIM_HandleTypeDef *htim);

void LeftArm_SelfTest();

void LeftArm_MoveToAngle(int TargetAngle);

void LeftArm_Update10Hz();

void LeftArm_EnableBrake(enum ENUM_Booleans BrakeEnable);

//------------------------------------------------
void RightArm_Init(TIM_HandleTypeDef *htim);

void RightArm_SelfTest();

void RightArm_MoveToAngle(int TargetAngle);

void RightArm_Update10Hz();

void RightArm_EnableBrake(enum ENUM_Booleans BrakeEnable);


//------------------------------------------------
void GenericArm_Init(struct Arm_State_Type LeftArm_State);

#endif /* ARMS_INC_ARMS_H_ */


