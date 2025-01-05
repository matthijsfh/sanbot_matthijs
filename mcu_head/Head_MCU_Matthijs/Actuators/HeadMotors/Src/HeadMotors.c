#include "HeadMotors.h"
#include "stm32f1xx.h"
#include <main.h>
#include "Encoders.h"
#include <stdlib.h>  // For abs()
#include "RGBLeds_Head.h"

struct HeadMotor_State_Type HeadPan_State;
struct HeadMotor_State_Type HeadTilt_State;

void Generic_Head_Position_Setpoint(enum ENUM_BodyParts BodyPart, char HighByte, char LowByte)
{
	short combined = ((unsigned char)HighByte << 8) | (unsigned char)LowByte;

	if (BodyPart == HeadPan)
	{
		HeadPan_State.TargetPosition = combined;
		Generic_Head_HAL_Brake(False, BodyPart);
		HeadPan_State.MotionState = Motion_Moving;
	}

	if (BodyPart == HeadTilt)
	{
		HeadTilt_State.TargetPosition = combined;
		Generic_Head_HAL_Brake(False, BodyPart);
		HeadTilt_State.MotionState = Motion_Moving;
	}
}

void Generic_Head_HAL_Brake(enum ENUM_Booleans BrakeEnable, enum ENUM_BodyParts BodyPart)
{
	//	 Works inverted. High to release brake.
	if (BodyPart == HeadPan)
	{
		HAL_GPIO_WritePin(PanEnable_GPIO_Port, PanEnable_Pin, !BrakeEnable);
	}

	if (BodyPart == HeadTilt)
	{
//		HAL_GPIO_WritePin(TiltEnable_GPIO_Port, TiltEnable_Pin, !BrakeEnable);
	}
}

void GenericHead_HAL_Direction(enum ENUM_HeadMotorDirection Direction, enum ENUM_BodyParts BodyPart)
{
	if (BodyPart == HeadPan)
	{
		if (Direction == Move_Pos)
		{
			HAL_GPIO_WritePin(PanDirection_GPIO_Port, PanDirection_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(PanDirection_GPIO_Port, PanDirection_Pin, GPIO_PIN_SET);
		}
	}

	if (BodyPart == HeadTilt)
	{
		if (Direction == Move_Pos)
		{
//			HAL_GPIO_WritePin(RightArmUp_GPIO_Port, RightArmUp_Pin, GPIO_PIN_SET);
		}
		else
		{
//			HAL_GPIO_WritePin(RightArmUp_GPIO_Port, RightArmUp_Pin, GPIO_PIN_RESET);
		}
	}
}

void Generic_Head_HAL_PWM(int PWM, enum ENUM_BodyParts BodyPart)
{
	if (BodyPart == HeadPan)
	{
		__HAL_TIM_SET_COMPARE(HeadPan_State.TIM, HeadPan_State.TIM_CHANNEL, PWM);
	}

	if (BodyPart == HeadTilt)
	{
		__HAL_TIM_SET_COMPARE(HeadTilt_State.TIM, HeadTilt_State.TIM_CHANNEL, PWM);
	}
}

void Head_Update20Hz(struct Encoders_Data_Type *EncoderData)
{
	// Read limit switches
	int HeadPanLimitNeg = HAL_GPIO_ReadPin(PanNegSensor_GPIO_Port,PanNegSensor_Pin);
	int HeadPanLimitPos = HAL_GPIO_ReadPin(PanPosSensor_GPIO_Port, PanPosSensor_Pin);

	int PanSpeed = 30;

	//--------------------------------------------------------------------------------
	// Head pan
	//--------------------------------------------------------------------------------
	HeadPan_State.ActualPosition = EncoderData->Encoder[1];

	if (HeadPan_State.ActualPosition >= 32768)
	{
		HeadPan_State.ActualPosition = EncoderData->Encoder[1] - 66536;
	}

	if (HeadPan_State.HomeState < Homed)
	{
		if (HeadPan_State.HomeState == NotHomed)
		{
			HeadPan_State.HomeCounter = 0;
		}
		else if (HeadPan_State.HomeState == Homing)
		{
			Generic_Head_HAL_Brake(False, HeadPan);
			HeadPan_State.HomeCounter += 1;

			HeadPan_State.Direction = Move_Neg;
			HeadPan_State.PWM_Output = (100 - abs(30));

			if (HeadPanLimitNeg == 1)
			{
				// Set current encoder position to zero
				__HAL_TIM_SET_COUNTER(EncoderData->TIM[1], 0);

				// 600 counts
				Generic_Head_Position_Setpoint(HeadPan, 1, 170);
				HeadPan_State.HomeState = Homed;

				RGBLeds_SetAllColors(LeftHead, Green, LED_On);
			}
		}
	}
	else if (HeadPan_State.MotionState == Motion_Moving)
	{
		HeadPan_State.ErrorPosition = HeadPan_State.TargetPosition - HeadPan_State.ActualPosition;

		if (HeadPan_State.ErrorPosition > 10)
		{
			HeadPan_State.Direction = Move_Pos;
			HeadPan_State.PWM_Output = (100 - abs(PanSpeed));
			Generic_Head_HAL_Brake(False, HeadPan);
		}
		else if (HeadPan_State.ErrorPosition < -10)
		{
			HeadPan_State.Direction = Move_Neg;
			HeadPan_State.PWM_Output = (100 - abs(PanSpeed));
			Generic_Head_HAL_Brake(False, HeadPan);
		}
		else
		{
			HeadPan_State.PWM_Output = (100 - abs(0));
			Generic_Head_HAL_Brake(False, HeadPan);
			HeadPan_State.MotionState = Motion_Idle;
		}
	}
	else if (HeadPan_State.MotionState == Motion_Idle)
	{
		HeadPan_State.PWM_Output = (100 - abs(0));
		Generic_Head_HAL_Brake(False, HeadPan);
	}

	//--------------------------------------------------------------------------------
	// Right arm
	//--------------------------------------------------------------------------------
//	RightArm_State.ActualPosition = EncoderData->Encoder[4];

//	// Read limit switches
//	int RightLimitBack = HAL_GPIO_ReadPin(RightLimitBack_GPIO_Port, RightLimitBack_Pin);
//	int RightLimitUp = HAL_GPIO_ReadPin(RightLimitUp_GPIO_Port, RightLimitUp_Pin);
//
//	//--------------------------------------------------------------------------------
//	// Right arm
//	//--------------------------------------------------------------------------------
//	RightArm_State.ActualPosition = EncoderData->Encoder[4];
//
//	if (RightArm_State.HomeState == Arm_NotHomed)
//	{
//		RightArm_State.HomeCounter = 0;
//
//		RGBLeds_SetAllColors(RightArm, Red, LED_On);
//	}
//	else if (RightArm_State.HomeState == Arm_Homing)
//	{
//		GenericArms_HAL_Brake(False, RightArm);
//		RightArm_State.HomeCounter += 1;
//
//		if (RightArm_State.HomeCounter <= 1 * UPDATE_20HZ)
//		{
//			RGBLeds_SetAllColors(RightArm, Red, LED_Blink_Slow);
//			RightArm_State.Direction = Arm_Motion_MovingDown;
//			RightArm_State.PWM_Output = (100 - abs(20));
//
//			if (RightLimitUp == 1) { RightArm_State.HomeCounter = 10 * UPDATE_20HZ;}
//		}
//		else
//		{
//			RightArm_State.Direction = Arm_Motion_MovingUp;
//			RightArm_State.PWM_Output = (100 - abs(20));
//
//			if (RightLimitBack == 1)
//			{
//				EncoderData->Encoder[4] = 0;
//				Arm_PositionSetpoint(RightArm, 1, 00);
//				RightArm_State.HomeState = Arm_Homed;
//
//				RGBLeds_SetAllColors(RightArm, Green, LED_On);
//			}
//		}
//	}
//	else if (RightArm_State.HomeState == Arm_Homed)
//	{
//		RightArm_State.ErrorPosition = RightArm_State.TargetPosition - RightArm_State.ActualPosition;
//
//		RightSpeed = abs(RightArm_State.ErrorPosition/4);
//
//		if (RightSpeed < 15) {RightSpeed = 15;}
//		if (RightSpeed > 50) {RightSpeed = 50;}
//
//		if (RightArm_State.ErrorPosition > 15)
//		{
//			RightArm_State.Direction = Arm_Motion_MovingDown;
//			RightArm_State.PWM_Output = (100 - abs(35));
//			GenericArms_HAL_Brake(False, RightArm);
//		}
//		else if (RightArm_State.ErrorPosition < -15)
//		{
//			RightArm_State.Direction = Arm_Motion_MovingUp;
//			RightArm_State.PWM_Output = (100 - abs(35));
//			GenericArms_HAL_Brake(False, RightArm);
//		}
//		else
//		{
//			RightArm_State.PWM_Output = (100 - abs(0));
//			GenericArms_HAL_Brake(True, RightArm);
//		}
//	}

//	HAL_TIM_Base_Start(LeftArm_State.TIM );
//	HAL_TIM_PWM_Start(LeftArm_State.TIM , TIM_CHANNEL_1);

//	HAL_TIM_Base_Start(RightArm_State.TIM );
//	HAL_TIM_PWM_Start(RightArm_State.TIM , TIM_CHANNEL_2);

	GenericHead_HAL_Direction(HeadPan_State.Direction, HeadPan);
//	GenericHead_HAL_Direction(HeadTilt_State.Direction, HeadTilt);

	Generic_Head_HAL_PWM(HeadPan_State.PWM_Output, HeadPan);
//	Generic_Head_HAL_PWM(HeadTilt_State.PWM_Output, HeadTilt);
}

//----------------------------------------------------------------
//
//----------------------------------------------------------------
void Head_Pan_Init(TIM_HandleTypeDef *htim)
{
	HeadPan_State.MotionDirection = Move_Pos;
	HeadPan_State.MotionState = Motion_Idle;
	HeadPan_State.HomeState = NotHomed;

	// Encoder counter timer
	HeadPan_State.TIM = htim;
	HeadPan_State.TIM_CHANNEL = TIM_CHANNEL_4;

	HeadPan_State.TargetPosition = 0;

	// PWM generation timer
	HAL_TIM_Base_Start(htim);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4);

	// 100 = not moving
	__HAL_TIM_SET_COMPARE(HeadPan_State.TIM, HeadPan_State.TIM_CHANNEL, 100);
}

void Head_Pan_Home()
{
	// Move forward a litte
	HeadPan_State.HomeState = Homing;
	HeadPan_State.HomeCounter = 0;

	RGBLeds_SetAllColors(LeftHead, Red, LED_On);
}

void Head_Pan_SelfTest(enum ENUM_Booleans Enabled)
{
	// To do
}
//
////----------------------------------------------------------------
////
////----------------------------------------------------------------
//void RightArm_Init(TIM_HandleTypeDef *htim)
//{
//	RightArm_State.ArmDirection = Arm_Up;
//	RightArm_State.MotionState = Arm_Motion_Disabled;
//	RightArm_State.HomeState = Arm_NotHomed;
//
//	RightArm_State.TIM = htim;
//	RightArm_State.TIM_CHANNEL = TIM_CHANNEL_2;
//
//	RightArm_State.TargetPosition = 0;
//
//	HAL_TIM_Base_Start(htim);
//	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
//}
//
//void RightArm_Home()
//{
//	// Move forward a litte
//	RightArm_State.HomeState = Arm_Homing;
//	RightArm_State.HomeCounter = 0;
//}
//
//void RightArm_SelfTest(enum ENUM_Booleans Enabled)
//{
//	// To do
//}
