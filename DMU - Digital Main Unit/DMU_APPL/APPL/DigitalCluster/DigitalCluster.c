#include "DigitalCluster.h"
#include "RevCam.h"
#include <stdio.h>
#include <string.h>
#include "rtc.h"
#include "FsmcH.h"
/* ICM messages */
__attribute__((section(".ccmram"))) char DigitalCluster_CheckControlMessage_String[DIGITALCLUSTER_CCM_MAXNUMBER][DIGITALCLUSTER_CCM_MAXLENGTH] =
{
		"NULL",
		DIGITALCLUSTER_CCM_MSGID000,
		DIGITALCLUSTER_CCM_MSGID001,
		DIGITALCLUSTER_CCM_MSGID013,
		DIGITALCLUSTER_CCM_MSGID025,
		DIGITALCLUSTER_CCM_MSGID026,
		DIGITALCLUSTER_CCM_MSGID027,
		DIGITALCLUSTER_CCM_MSGID028,
		DIGITALCLUSTER_CCM_MSGID029,
		DIGITALCLUSTER_CCM_MSGID030,
		DIGITALCLUSTER_CCM_MSGID031,
		DIGITALCLUSTER_CCM_MSGID032,
		DIGITALCLUSTER_CCM_MSGID033,
		DIGITALCLUSTER_CCM_MSGID034,
		DIGITALCLUSTER_CCM_MSGID035,
		DIGITALCLUSTER_CCM_MSGID037,
		DIGITALCLUSTER_CCM_MSGID040,
		DIGITALCLUSTER_CCM_MSGID053,
		DIGITALCLUSTER_CCM_MSGID058,
		DIGITALCLUSTER_CCM_MSGID059,
		DIGITALCLUSTER_CCM_MSGID060,
		DIGITALCLUSTER_CCM_MSGID061,
		DIGITALCLUSTER_CCM_MSGID062,
		DIGITALCLUSTER_CCM_MSGID063,
		DIGITALCLUSTER_CCM_MSGID064,
		DIGITALCLUSTER_CCM_MSGID065,
		DIGITALCLUSTER_CCM_MSGID066,
		DIGITALCLUSTER_CCM_MSGID067,
		DIGITALCLUSTER_CCM_MSGID068,
		DIGITALCLUSTER_CCM_MSGID069,
		DIGITALCLUSTER_CCM_MSGID070,
		DIGITALCLUSTER_CCM_MSGID071,
		DIGITALCLUSTER_CCM_MSGID072,
		DIGITALCLUSTER_CCM_MSGID073,
		DIGITALCLUSTER_CCM_MSGID074,
		DIGITALCLUSTER_CCM_MSGID075,
		DIGITALCLUSTER_CCM_MSGID076,
		DIGITALCLUSTER_CCM_MSGID077,
		DIGITALCLUSTER_CCM_MSGID078
};
/* Digital cluster design */
__attribute__((section(".ccmram"))) DigitalCluster_DisplayMode_t DigitalCluster_Display_ComfortEcoSport =
{
		.Time =
		{
				.TimeBox =
				{
						DIGITALCLUSTER_TIMEBOX_POSX_CSE,
						DIGITALCLUSTER_TIMEBOX_POSY_CSE,
						DIGITALCLUSTER_TIMEBOX_WIDTH_CSE,
						DIGITALCLUSTER_TIMEBOX_HEIGHT_CSE
				},
				.T_HourText =
				{
						DIGITALCLUSTER_THOURTEXT_POSX_CSE,
						DIGITALCLUSTER_THOURTEXT_POSY_CSE,
						DIGITALCLUSTER_THOURTEXT_TEXT_CSE
				},
				.T_DotDotText =
				{
						DIGITALCLUSTER_TDOTDOTTEXT_POSX_CSE,
						DIGITALCLUSTER_TDOTDOTTEXT_POSY_CSE,
						DIGITALCLUSTER_TDOTDOTTEXT_TEXT_CSE
				},
				.T_MinuteText =
				{
						DIGITALCLUSTER_TMINUTETEXT_POSX_CSE,
						DIGITALCLUSTER_TMINUTETEXT_POSY_CSE,
						DIGITALCLUSTER_TMINUTETEXT_TEXT_CSE
				},
		},
		.Temperature =
		{
				.TemperatureBox =
				{
						DIGITALCLUSTER_TEMPERATUREBOX_POSX_CSE,
						DIGITALCLUSTER_TEMPERATUREBOX_POSY_CSE,
						DIGITALCLUSTER_TEMPERATUREBOX_WIDTH_CSE,
						DIGITALCLUSTER_TEMPERATUREBOX_HEIGHT_CSE
				},
				.T_TempText =
				{
						DIGITALCLUSTER_TTEMPTEXT_POSX_CSE,
						DIGITALCLUSTER_TTEMPTEXT_POSY_CSE,
						DIGITALCLUSTER_TTEMPTEXT_TEXT_CSE
				},
		},
		.Gear =
		{
				.GearSelectedBox =
				{
						DIGITALCLUSTER_GEARSELECTEDBOX_POSX_CSE,
						DIGITALCLUSTER_GEARSELECTEDBOX_POSY_CSE,
						DIGITALCLUSTER_GEARSELECTEDBOX_WIDTH_CSE,
						DIGITALCLUSTER_GEARSELECTEDBOX_HEIGHT_CSE
				},
				.G_GselText =
				{
						DIGITALCLUSTER_GGSELTEXT_POSX_CSE,
						DIGITALCLUSTER_GGSELTEXT_POSY_CSE,
						DIGITALCLUSTER_GGSELTEXT_TEXT_CSE
				},
		},
		.CheckControlMessage =
		{
				.CheckControlMsgBox =
				{
						DIGITALCLUSTER_CHECKCONTROLMSGBOX_POSX_CSE,
						DIGITALCLUSTER_CHECKCONTROLMSGBOX_POSY_CSE,
						DIGITALCLUSTER_CHECKCONTROLMSGBOX_WIDTH_CSE,
						DIGITALCLUSTER_CHECKCONTROLMSGBOX_HEIGHT_CSE
				},
				.CCM_Text =
				{
						DIGITALCLUSTER_CCMTEXT_POSX_CSE,
						DIGITALCLUSTER_CCMTEXT_POSY_CSE,
						DIGITALCLUSTER_CCMTEXT_TEXT_CSE
				},
		},
		.DriverAssistanceMessage =
		{
				.DASBox =
				{
						DIGITALCLUSTER_DASBOX_POSX_CSE,
						DIGITALCLUSTER_DASBOX_POSY_CSE,
						DIGITALCLUSTER_DASBOX_WIDTH_CSE,
						DIGITALCLUSTER_DASBOX_HEIGHT_CSE
				},
				.DAS_Text =
				{
						DIGITALCLUSTER_DASTEXT_POSX_CSE,
						DIGITALCLUSTER_DASTEXT_POSY_CSE,
						DIGITALCLUSTER_DASTEXT_TEXT_CSE
				},
		},
		.DashboardLights =
		{
				.DashboardLightsBox =
				{
						DIGITALCLUSTER_DASHBOARDLIGHTBOX_POSX_CSE,
						DIGITALCLUSTER_DASHBOARDLIGHTBOX_POSY_CSE,
						DIGITALCLUSTER_DASHBOARDLIGHTBOX_WIDTH_CSE,
						DIGITALCLUSTER_DASHBOARDLIGHTBOX_HEIGHT_CSE
				},
				.DL_HB =
				{
						DIGITALCLUSTER_DLHB_POSX_CSE,
						DIGITALCLUSTER_DLHB_POSY_CSE,
						DIGITALCLUSTER_DLHB_TEXT_CSE
				},
				.DL_PS =
				{
						DIGITALCLUSTER_DLPS_POSX_CSE,
						DIGITALCLUSTER_DLPS_POSY_CSE,
						DIGITALCLUSTER_DLPS_TEXT_CSE
				},
		},
		.DashboardLights2 =
		{
				.DashboardLightsBox2 =
				{
						DIGITALCLUSTER_DASHBOARDLIGHTBOX2_POSX_CSE,
						DIGITALCLUSTER_DASHBOARDLIGHTBOX2_POSY_CSE,
						DIGITALCLUSTER_DASHBOARDLIGHTBOX2_WIDTH_CSE,
						DIGITALCLUSTER_DASHBOARDLIGHTBOX2_HEIGHT_CSE
				},
				.DL2_FLF =
				{
						DIGITALCLUSTER_DL2FLF_POSX_CSE,
						DIGITALCLUSTER_DL2FLF_POSY_CSE,
						DIGITALCLUSTER_DL2FLF_TEXT_CSE
				},
				.DL2_FLR =
				{
						DIGITALCLUSTER_DL2FLR_POSX_CSE,
						DIGITALCLUSTER_DL2FLR_POSY_CSE,
						DIGITALCLUSTER_DL2FLR_TEXT_CSE
				},
				.DL2_RLS =
				{
						DIGITALCLUSTER_DL2RLS_POSX_CSE,
						DIGITALCLUSTER_DL2RLS_POSY_CSE,
						DIGITALCLUSTER_DL2RLS_TEXT_CSE
				},
				.DL2_C =
				{
						DIGITALCLUSTER_DL2C_POSX_CSE,
						DIGITALCLUSTER_DL2C_POSY_CSE,
						DIGITALCLUSTER_DL2C_TEXT_CSE
				},
				.DL2_RT =
				{
						DIGITALCLUSTER_DL2RT_POSX_CSE,
						DIGITALCLUSTER_DL2RT_POSY_CSE,
						DIGITALCLUSTER_DL2RT_TEXT_CSE
				},
				.DL2_RTN =
				{
						DIGITALCLUSTER_DL2RTN_POSX_CSE,
						DIGITALCLUSTER_DL2RTN_POSY_CSE,
						DIGITALCLUSTER_DL2RTN_TEXT_CSE
				},
				.DL2_AUTO =
				{
						DIGITALCLUSTER_DL2AUTO_POSX_CSE,
						DIGITALCLUSTER_DL2AUTO_POSY_CSE,
						DIGITALCLUSTER_DL2AUTO_TEXT_CSE
				},
				.DL2_FV =
				{
						DIGITALCLUSTER_DL2FV_POSX_CSE,
						DIGITALCLUSTER_DL2FV_POSY_CSE,
						DIGITALCLUSTER_DL2FV_TEXT_CSE
				},
				.DL2_FVN =
				{
						DIGITALCLUSTER_DL2FVN_POSX_CSE,
						DIGITALCLUSTER_DL2FVN_POSY_CSE,
						DIGITALCLUSTER_DL2FVN_TEXT_CSE
				},
		},
		.TurnSignalsBox =
		{
				.TSBox =
				{
						DIGITALCLUSTER_TSBOX_POSX_CSE,
						DIGITALCLUSTER_TSBOX_POSY_CSE,
						DIGITALCLUSTER_TSBOX_WIDTH_CSE,
						DIGITALCLUSTER_TSBOX_HEIGHT_CSE
				},
		},
		.CenterScreenInformation =
		{
				.SL_KMH =
				{
						DIGITALCLUSTER_SLKMH_POSX_CSE,
						DIGITALCLUSTER_SLKMH_POSY_CSE,
						DIGITALCLUSTER_SLKMH_TEXT_CSE
				},
				.SL_RPM =
				{
						DIGITALCLUSTER_SLRPM_POSX_CSE,
						DIGITALCLUSTER_SLRPM_POSY_CSE,
						DIGITALCLUSTER_SLRPM_TEXT_CSE
				},
		},
		.SpeedBar =
		{
				.SpeedQuarter =
				{
						DIGITALCLUSTER_SPEEDQUARTER_POSX_CSE,
						DIGITALCLUSTER_SPEEDQUARTER_POSY_CSE,
						DIGITALCLUSTER_SPEEDQUARTER_POSX2_CSE,
						DIGITALCLUSTER_SPEEDQUARTER_POSY2_CSE
				},
		},
		.RpmBar =
		{
				.RpmQuarter =
				{
						DIGITALCLUSTER_RPMQUARTER_POSX_CSE,
						DIGITALCLUSTER_RPMQUARTER_POSY_CSE,
						DIGITALCLUSTER_RPMQUARTER_POSX2_CSE,
						DIGITALCLUSTER_RPMQUARTER_POSY2_CSE
				},
		},
		.KmTot =
		{
				.KMTOTBox =
				{
						DIGITALCLUSTER_KMTOTBOX_POSX_CSE,
						DIGITALCLUSTER_KMTOTBOX_POSY_CSE,
						DIGITALCLUSTER_KMTOTBOX_WIDTH_CSE,
						DIGITALCLUSTER_KMTOTBOX_HEIGHT_CSE
				},
				.KMTOT_Text =
				{
						DIGITALCLUSTER_KMTOTNUM_POSX_CSE,
						DIGITALCLUSTER_KMTOTNUM_POSY_CSE,
						DIGITALCLUSTER_KMTOTNUM_TEXT_CSE
				},
				.KMTOTStat_Text =
				{
						DIGITALCLUSTER_KMTOTTEXT_POSX_CSE,
						DIGITALCLUSTER_KMTOTTEXT_POSY_CSE,
						DIGITALCLUSTER_KMTOTTEXT_TEXT_CSE
				},
		},
		.KmDcy =
		{
				.KMDCYBox =
				{
						DIGITALCLUSTER_KMDCYBOX_POSX_CSE,
						DIGITALCLUSTER_KMDCYBOX_POSY_CSE,
						DIGITALCLUSTER_KMDCYBOX_WIDTH_CSE,
						DIGITALCLUSTER_KMDCYBOX_HEIGHT_CSE
				},
				.KMDCY_Text =
				{
						DIGITALCLUSTER_KMDCYNUM_POSX_CSE,
						DIGITALCLUSTER_KMDCYNUM_POSY_CSE,
						DIGITALCLUSTER_KMDCYNUM_TEXT_CSE
				},
		},
		.DisplayType = 0x01,
		.DisplayColor = TFT_LIMEGREEN,
		.DisplayRPMColor = TFT_ORCHID,
		.DisplayVehSpeedColor = TFT_ORANGE
};
/* Values declared in CCM RAM for access speed. */
__attribute__((section(".ccmram"))) uint8 DigitalCluster_ShutOffDisplayFlag = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_WelcomeAnimationFlag = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_LeavingAnimationFlag = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_ReadDisplayPowerMode_RegisterValue = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_LcdInit = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RetValInit = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_IgnitionStatus = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_VBat = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_Gear = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_HighBeamStatus = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_PowerSteeringStatus = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_VehicleSpeed = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_MotorRpm = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_TurnSignals = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_DisplayMode = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_CollisionWarning = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_ReverseCameraRequest = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_CheckControlMessageId = 0x00;
__attribute__((section(".ccmram"))) uint8 pCheckControlMessageId = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_OutsideTemperature = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_CalculatedHour = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_CalculatedMinutes = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_CalculatedSeconds = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_CalculatedDay = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_CalculatedMonth = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_CalculatedYear = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_CalculatedWeekDay = 0x00;
__attribute__((section(".ccmram"))) uint16 DigitalCluster_ReadDisplayStatus_RegisterValue = 0x00;
__attribute__((section(".ccmram"))) uint32 DigitalCluster_MainCounter = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_WelcomeAnimationCounter = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_LeavingAnimationCounter = 0x00;
__attribute__((section(".ccmram"))) uint16 DigitalCluster_WelcomeAuxColor = 0x00;
__attribute__((section(".ccmram"))) uint16 DigitalCluster_LeavingAuxColor = 0xFFFF;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_PreviousIgnStat = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_FsmcDmaErrorFlag = 0x00;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_IsSelfTestRequested = 0;
__attribute__((section(".ccmram"))) uint32 DigitalCluster_InitRetry = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_FogLightFront = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_FogLightRear = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_Rls = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_Recirculation = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_RequestedTemperature = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_AutoClimate = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_RxSig_FanValue = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_IsReverseCameraActive = 0;
__attribute__((section(".ccmram"))) uint8 DigitalCluster_BuzzerState = 0;
__attribute__((section(".ccmram"))) RTC_TimeTypeDef getCurrentTime = {0, 0, 0, 0, 0, 0, 0, 0};
__attribute__((section(".ccmram"))) RTC_DateTypeDef getCurrentDate = {0, 0, 0, 0};
__attribute__((section(".ccmram"))) uint32 CCM_Counter = 0x00;
__attribute__((section(".ccmram"))) uint32 TS_Counter = 0x00;
__attribute__((section(".ccmram"))) uint32 CAW_Counter = 0x00;
__attribute__((section(".ccmram"))) uint16 speedBarHeight = 0;
__attribute__((section(".ccmram"))) uint16 rpmBarHeight = 0;
__attribute__((section(".ccmram"))) uint16 pSpeedBarHeight = 0;
__attribute__((section(".ccmram"))) uint16 pRpmBarHeight = 0;
__attribute__((section(".ccmram"))) uint16 maxBarHeight = 180;
__attribute__((section(".ccmram"))) uint8 hasTimeHChanged = 0;
__attribute__((section(".ccmram"))) uint8 hasTimeMChanged = 0;
__attribute__((section(".ccmram"))) uint8 hbPrevState = 0;
__attribute__((section(".ccmram"))) uint8 psPrevState = 0;
__attribute__((section(".ccmram"))) uint8 tPrevState = 100;
__attribute__((section(".ccmram"))) uint8 gPrevState = 5;
__attribute__((section(".ccmram"))) uint8 dmPrevState = 0;
__attribute__((section(".ccmram"))) uint16 pVehSpeed = 331;
__attribute__((section(".ccmram"))) uint16 pRpm = 10000;
__attribute__((section(".ccmram"))) uint32 timestamp = 0;
__attribute__((section(".ccmram"))) uint8 collisionFlag = 0;
__attribute__((section(".ccmram"))) uint8 collisionFlagClear = 0;
__attribute__((section(".ccmram"))) uint8 ccmFlag = 0;
__attribute__((section(".ccmram"))) uint32 vehSpeedConv = 0;
__attribute__((section(".ccmram"))) uint32 rpmConv = 0;
__attribute__((section(".ccmram"))) uint8 localDM1 = 5;
__attribute__((section(".ccmram"))) uint8 localDM2 = 5;
__attribute__((section(".ccmram"))) uint8 localDM3 = 5;
__attribute__((section(".ccmram"))) uint8 localDM4 = 5;
__attribute__((section(".ccmram"))) uint8 clearBeforeLeave = 0;
__attribute__((section(".ccmram"))) uint8 clearBeforeWelcome = 0;
__attribute__((section(".ccmram"))) uint32 pKmTot = 0xFFFFFFFF;
__attribute__((section(".ccmram"))) uint32 pKmDcy = 0xFFFFFFFF;
__attribute__((section(".ccmram"))) uint8 pIsReverseCameraActive = 0;
__attribute__((section(".ccmram"))) uint8 pFogLightFront = 1;
__attribute__((section(".ccmram"))) uint8 pFogLightRear = 1;
__attribute__((section(".ccmram"))) uint8 pRls = 1;
__attribute__((section(".ccmram"))) uint8 pRecirculation = 1;
__attribute__((section(".ccmram"))) uint8 pRequestedTemperature = 99;
__attribute__((section(".ccmram"))) uint8 pAutoClimate = 1;
__attribute__((section(".ccmram"))) uint8 pFanValue = 9;
extern __attribute__((section(".ccmram"))) uint32 DataRecorder_CcmCounter[DIGITALCLUSTER_CCM_MAXNUMBER];
extern __attribute__((section(".ccmram"))) uint32 DataRecorder_KilometerTotal;
extern __attribute__((section(".ccmram"))) uint32 DataRecorder_KilometerPerDcy;
extern __attribute__((section(".ccmram"))) uint32 DataRecorder_RxSig_VehicleSpeed;

void DigitalCluster_SelfTest(void);
void DigitalCluster_WelcomeAnimationHandler(void);
void DigitalCluster_LeavingAnimationHandler(void);
void DigitalCluster_CSEHandler(void);
void DigitalCluster_GTEHandler(void);
void DigitalCluster_ShutOffDisplay(void);
void DigitalCluster_CalculateTime(DigitalCluster_DisplayMode_t *displayType);
void DigitalCluster_DisplayCheckControl(DigitalCluster_DisplayMode_t *displayType);
void DigitalCluster_Init(void);
void DigitalCluster_InitHandler(void);
void DigitalCluster_MainFunction(void);
void DigitalCluster_HandleCollisionWarning(DigitalCluster_DisplayMode_t *displayType);
void DigitalCluster_HandleTurnSignal(DigitalCluster_DisplayMode_t *displayType);
void DigitalCluster_DisplayInfoLights(DigitalCluster_DisplayMode_t *displayType);
void DigitalCluster_DisplayCenterSpeedRpm(DigitalCluster_DisplayMode_t *displayType);
void DigitalCluster_DisplaySideSpeedRpm(DigitalCluster_DisplayMode_t *displayType);
void DigitalCluster_DisplayKMTotalDcy(DigitalCluster_DisplayMode_t *displayType);
void DigitalCluster_InitMemory(void);

void DigitalCluster_Init(void)
{
	/* Re-init the variables with a known value.
	 * Useful for when the switching between different states
	 * of the LCD is done
	 * and we want to go back to previous state.
	 * */
	speedBarHeight = 180;
	rpmBarHeight = 180;
	pSpeedBarHeight = 180;
	pRpmBarHeight = 180;
	maxBarHeight = 180;
	hasTimeHChanged = 99;
	hasTimeMChanged = 99;
	hbPrevState = 99;
	psPrevState = 99;
	tPrevState = 100;
	gPrevState = 5;
	dmPrevState = 180;
	pVehSpeed = 331;
	pRpm = 10000;
	timestamp = 0;
	collisionFlag = 0;
	collisionFlagClear = 0;
	ccmFlag = 0;
	vehSpeedConv = 340;
	rpmConv = 10000;
	localDM1 = 99;
	localDM2 = 99;
	localDM3 = 99;
	localDM4 = 99;
	DigitalCluster_WelcomeAnimationCounter = 0x00;
	DigitalCluster_LeavingAnimationCounter = 0x00;
	DigitalCluster_WelcomeAuxColor = 0x00;
	DigitalCluster_LeavingAuxColor = 0xFFFF;
	DigitalCluster_PreviousIgnStat = 0x00;
	CCM_Counter = 0x00;
	TS_Counter = 0x00;
	CAW_Counter = 0x00;
	clearBeforeLeave = 0;
	clearBeforeWelcome = 0;
	pKmTot = 0xFFFFFFFF;
	pKmDcy = 0xFFFFFFFF;
	pFogLightFront = 99;
	pFogLightRear = 99;
	pRls = 99;
	pRecirculation = 99;
	pRequestedTemperature = 99;
	pAutoClimate = 99;
	pFanValue = 9;
	pCheckControlMessageId = 99;
	/* Initialize the ILI9341 via FSMC. */
	DigitalCluster_RetValInit = FsmcH_LcdInit();
	/* Delay to prevent wrong initialization. */
	HAL_Delay(10);
	/* If all good. */
	if(0x00 == DigitalCluster_RetValInit)
	{
		/* Read LCD register values to make sure the initialization sequence was done properly. */
		FsmcH_WriteCommand(DIGITALCLUSTER_READDISPLAYSTATUS_CMD);
		DigitalCluster_ReadDisplayStatus_RegisterValue = (uint16)FsmcH_ReadData();
		FsmcH_WriteCommand(DIGITALCLUSTER_READDISPLAYPOWERMODE_CMD);
		DigitalCluster_ReadDisplayPowerMode_RegisterValue = (uint16)FsmcH_ReadData();
		if(DIGITALCLUSTER_LCD_DISPLAYSTATUS_REGISTERVALUE == DigitalCluster_ReadDisplayStatus_RegisterValue && DIGITALCLUSTER_LCD_POWERMODE_REGISTERVALUE == DigitalCluster_ReadDisplayPowerMode_RegisterValue)
		{
			/* State that the LCD is initialized.
			 * Switch back-light on.
			 * Set background color to black. */
			DigitalCluster_LcdInit = 0x01;
			HAL_GPIO_WritePin(DIGITALCLUSTER_BACKLIGHT_GPIO_Port, DIGITALCLUSTER_BACKLIGHT_Pin, 1);
			FsmcH_FillRectangle(0, 0, 320, 240, TFT_BLACK);
		}
		else
		{
			/* Do nothing. */
		}
	}
}
void DigitalCluster_InitHandler(void)
{
	if(0x00 == DigitalCluster_LcdInit && 0x01 != DigitalCluster_FsmcDmaErrorFlag)
	{
		DigitalCluster_InitRetry++;
		DigitalCluster_Init();
	}
	else
	{
		/* Do nothing. */
	}
}
void DigitalCluster_DisplayKMTotalDcy(DigitalCluster_DisplayMode_t *displayType)
{
	if(pKmTot != DataRecorder_KilometerTotal)
	{
		pKmTot = DataRecorder_KilometerTotal;

		if(10 > DataRecorder_KilometerTotal)
		{
			FsmcH_FillRectangle(displayType->KmTot.KMTOTBox.position_x,
					displayType->KmTot.KMTOTBox.position_y,
					displayType->KmTot.KMTOTBox.width,
					displayType->KmTot.KMTOTBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmTot.KMTOT_Text.position_x,
					displayType->KmTot.KMTOT_Text.position_y,
					DataRecorder_KilometerTotal,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(displayType->KmTot.KMTOTStat_Text.position_x - 85,
					displayType->KmTot.KMTOTStat_Text.position_y,
					displayType->KmTot.KMTOTStat_Text.text,
					TFT_TAN,
					TFT_BLACK);
		}
		else if(100 > DataRecorder_KilometerTotal)
		{
			FsmcH_FillRectangle(displayType->KmTot.KMTOTBox.position_x,
					displayType->KmTot.KMTOTBox.position_y,
					displayType->KmTot.KMTOTBox.width,
					displayType->KmTot.KMTOTBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmTot.KMTOT_Text.position_x,
					displayType->KmTot.KMTOT_Text.position_y,
					DataRecorder_KilometerTotal,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(displayType->KmTot.KMTOTStat_Text.position_x - 68,
					displayType->KmTot.KMTOTStat_Text.position_y,
					displayType->KmTot.KMTOTStat_Text.text,
					TFT_TAN,
					TFT_BLACK);
		}
		else if(1000 > DataRecorder_KilometerTotal)
		{
			FsmcH_FillRectangle(displayType->KmTot.KMTOTBox.position_x,
					displayType->KmTot.KMTOTBox.position_y,
					displayType->KmTot.KMTOTBox.width,
					displayType->KmTot.KMTOTBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmTot.KMTOT_Text.position_x,
					displayType->KmTot.KMTOT_Text.position_y,
					DataRecorder_KilometerTotal,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(displayType->KmTot.KMTOTStat_Text.position_x - 51,
					displayType->KmTot.KMTOTStat_Text.position_y,
					displayType->KmTot.KMTOTStat_Text.text,
					TFT_TAN,
					TFT_BLACK);
		}
		else if(10000 > DataRecorder_KilometerTotal)
		{
			FsmcH_FillRectangle(displayType->KmTot.KMTOTBox.position_x,
					displayType->KmTot.KMTOTBox.position_y,
					displayType->KmTot.KMTOTBox.width,
					displayType->KmTot.KMTOTBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmTot.KMTOT_Text.position_x,
					displayType->KmTot.KMTOT_Text.position_y,
					DataRecorder_KilometerTotal,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(displayType->KmTot.KMTOTStat_Text.position_x - 34,
					displayType->KmTot.KMTOTStat_Text.position_y,
					displayType->KmTot.KMTOTStat_Text.text,
					TFT_TAN,
					TFT_BLACK);
		}
		else if(100000 > DataRecorder_KilometerTotal)
		{
			FsmcH_FillRectangle(displayType->KmTot.KMTOTBox.position_x,
					displayType->KmTot.KMTOTBox.position_y,
					displayType->KmTot.KMTOTBox.width,
					displayType->KmTot.KMTOTBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmTot.KMTOT_Text.position_x,
					displayType->KmTot.KMTOT_Text.position_y,
					DataRecorder_KilometerTotal,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(displayType->KmTot.KMTOTStat_Text.position_x - 17,
					displayType->KmTot.KMTOTStat_Text.position_y,
					displayType->KmTot.KMTOTStat_Text.text,
					TFT_TAN,
					TFT_BLACK);
		}
		else
		{
			FsmcH_FillRectangle(displayType->KmTot.KMTOTBox.position_x,
					displayType->KmTot.KMTOTBox.position_y,
					displayType->KmTot.KMTOTBox.width,
					displayType->KmTot.KMTOTBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmTot.KMTOT_Text.position_x,
					displayType->KmTot.KMTOT_Text.position_y,
					DataRecorder_KilometerTotal,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(displayType->KmTot.KMTOTStat_Text.position_x,
					displayType->KmTot.KMTOTStat_Text.position_y,
					displayType->KmTot.KMTOTStat_Text.text,
					TFT_TAN,
					TFT_BLACK);
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(pKmDcy != DataRecorder_KilometerPerDcy)
	{
		pKmDcy = DataRecorder_KilometerPerDcy;
		if(10 > DataRecorder_KilometerPerDcy)
		{
			FsmcH_FillRectangle(displayType->KmDcy.KMDCYBox.position_x,
					displayType->KmDcy.KMDCYBox.position_y,
					displayType->KmDcy.KMDCYBox.width,
					displayType->KmDcy.KMDCYBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmDcy.KMDCY_Text.position_x,
					displayType->KmDcy.KMDCY_Text.position_y,
					DataRecorder_KilometerPerDcy,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(203,
					displayType->KmDcy.KMDCY_Text.position_y,
					"KM",
					TFT_TAN,
					TFT_BLACK);
		}
		else if(100 > DataRecorder_KilometerPerDcy)
		{
			FsmcH_FillRectangle(displayType->KmDcy.KMDCYBox.position_x,
					displayType->KmDcy.KMDCYBox.position_y,
					displayType->KmDcy.KMDCYBox.width,
					displayType->KmDcy.KMDCYBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmDcy.KMDCY_Text.position_x,
					displayType->KmDcy.KMDCY_Text.position_y,
					DataRecorder_KilometerPerDcy,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(220,
					displayType->KmDcy.KMDCY_Text.position_y,
					"KM",
					TFT_TAN,
					TFT_BLACK);
		}
		else if(1000 > DataRecorder_KilometerPerDcy)
		{
			FsmcH_FillRectangle(displayType->KmDcy.KMDCYBox.position_x,
					displayType->KmDcy.KMDCYBox.position_y,
					displayType->KmDcy.KMDCYBox.width,
					displayType->KmDcy.KMDCYBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmDcy.KMDCY_Text.position_x,
					displayType->KmDcy.KMDCY_Text.position_y,
					DataRecorder_KilometerPerDcy,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(237,
					displayType->KmDcy.KMDCY_Text.position_y,
					"KM",
					TFT_TAN,
					TFT_BLACK);
		}
		else
		{
			FsmcH_FillRectangle(displayType->KmDcy.KMDCYBox.position_x,
					displayType->KmDcy.KMDCYBox.position_y,
					displayType->KmDcy.KMDCYBox.width,
					displayType->KmDcy.KMDCYBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->KmDcy.KMDCY_Text.position_x,
					displayType->KmDcy.KMDCY_Text.position_y,
					DataRecorder_KilometerPerDcy,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString8(254,
					displayType->KmDcy.KMDCY_Text.position_y,
					"KM",
					TFT_TAN,
					TFT_BLACK);
		}
	}
	else
	{
		/* Do nothing. */
	}
}
void DigitalCluster_CalculateTime(DigitalCluster_DisplayMode_t *displayType)
{
	HAL_RTC_GetTime(&hrtc, &getCurrentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &getCurrentDate, RTC_FORMAT_BIN);
	DigitalCluster_CalculatedHour = getCurrentTime.Hours;
	DigitalCluster_CalculatedMinutes = getCurrentTime.Minutes;
	DigitalCluster_CalculatedSeconds = getCurrentTime.Seconds;
	DigitalCluster_CalculatedDay = getCurrentDate.Date;
	DigitalCluster_CalculatedMonth = getCurrentDate.Month;
	DigitalCluster_CalculatedYear = getCurrentDate.Year;
	DigitalCluster_CalculatedWeekDay = getCurrentDate.WeekDay;
	if(hasTimeHChanged != DigitalCluster_CalculatedHour ||
			hasTimeMChanged != DigitalCluster_CalculatedMinutes)
	{
		hasTimeHChanged = DigitalCluster_CalculatedHour;
		hasTimeMChanged = DigitalCluster_CalculatedMinutes;

		if(10 > DigitalCluster_CalculatedHour)
		{
			FsmcH_FillRectangle(displayType->Time.TimeBox.position_x,
					displayType->Time.TimeBox.position_y,
					displayType->Time.TimeBox.width,
					displayType->Time.TimeBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->Time.T_HourText.position_x - 17,
					displayType->Time.T_HourText.position_y,
					DigitalCluster_CalculatedHour,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString(displayType->Time.T_DotDotText.position_x,
					displayType->Time.T_DotDotText.position_y,
					displayType->Time.T_DotDotText.text,
					TFT_TAN,
					TFT_BLACK);
		}
		else
		{
			FsmcH_FillRectangle(displayType->Time.TimeBox.position_x,
					displayType->Time.TimeBox.position_y,
					displayType->Time.TimeBox.width,
					displayType->Time.TimeBox.height,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->Time.T_HourText.position_x,
					displayType->Time.T_HourText.position_y,
					DigitalCluster_CalculatedHour,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawString(displayType->Time.T_DotDotText.position_x,
					displayType->Time.T_DotDotText.position_y,
					displayType->Time.T_DotDotText.text,
					TFT_TAN,
					TFT_BLACK);
		}
		if(10 > DigitalCluster_CalculatedMinutes)
		{
			FsmcH_DrawInteger(displayType->Time.T_MinuteText.position_x,
					displayType->Time.T_MinuteText.position_y,
					0,
					TFT_TAN,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->Time.T_MinuteText.position_x + 17,
					displayType->Time.T_MinuteText.position_y,
					DigitalCluster_CalculatedMinutes,
					TFT_TAN,
					TFT_BLACK);
		}
		else
		{
			FsmcH_DrawInteger(displayType->Time.T_MinuteText.position_x,
					displayType->Time.T_MinuteText.position_y,
					DigitalCluster_CalculatedMinutes,
					TFT_TAN,
					TFT_BLACK);
		}
	}
	else
	{
		/* Do nothing. */
	}
}
void DigitalCluster_DisplayCheckControl(DigitalCluster_DisplayMode_t *displayType)
{
	if(0 != DigitalCluster_RxSig_CheckControlMessageId)
	{
		if(12 > CCM_Counter)
		{
			if(2 > CCM_Counter && pCheckControlMessageId != DigitalCluster_RxSig_CheckControlMessageId) DigitalCluster_BuzzerState = 1;
			else if(DigitalCluster_RxSig_CollisionWarning != 2) DigitalCluster_BuzzerState = 0;
			else
			{
				/* Do nothing. */
			}
			CCM_Counter++;
			if(0 == ccmFlag)
			{
				ccmFlag = 1;
				FsmcH_FillRectangle(displayType->CheckControlMessage.CheckControlMsgBox.position_x,
						displayType->CheckControlMessage.CheckControlMsgBox.position_y,
						displayType->CheckControlMessage.CheckControlMsgBox.width,
						displayType->CheckControlMessage.CheckControlMsgBox.height,
						TFT_GRAY);
				displayType->CheckControlMessage.CCM_Text.text = DigitalCluster_CheckControlMessage_String[DigitalCluster_RxSig_CheckControlMessageId];
				FsmcH_DrawString8(displayType->CheckControlMessage.CCM_Text.position_x,
						displayType->CheckControlMessage.CCM_Text.position_y,
						displayType->CheckControlMessage.CCM_Text.text,
						TFT_WHITE,
						TFT_BLACK);
			}
			else
			{
				/* Do nothing. */
			}
		}
		else
		{
			pCheckControlMessageId = DigitalCluster_RxSig_CheckControlMessageId;
			DataRecorder_CcmCounter[DigitalCluster_RxSig_CheckControlMessageId]++;
			DigitalCluster_RxSig_CheckControlMessageId = 0;
			CCM_Counter = 0;
			ccmFlag = 0;
			FsmcH_FillRectangle(displayType->CheckControlMessage.CheckControlMsgBox.position_x,
					displayType->CheckControlMessage.CheckControlMsgBox.position_y,
					displayType->CheckControlMessage.CheckControlMsgBox.width,
					displayType->CheckControlMessage.CheckControlMsgBox.height,
					TFT_BLACK);
		}
	}
	else
	{
		CCM_Counter = 0;
	}
}
void DigitalCluster_HandleTurnSignal(DigitalCluster_DisplayMode_t *displayType)
{
	if(1 == DigitalCluster_RxSig_TurnSignals)
	{
		if(TS_Counter % 2 == 0 && TS_Counter != 0)
		{
			FsmcH_FillRectangle(displayType->TurnSignalsBox.TSBox.position_x,
					displayType->TurnSignalsBox.TSBox.position_y,
					displayType->TurnSignalsBox.TSBox.width - displayType->TurnSignalsBox.TSBox.width / 2,
					displayType->TurnSignalsBox.TSBox.height,
					TFT_GREEN);
			TS_Counter = 0;
		}
		else
		{
			TS_Counter++;
			FsmcH_FillRectangle(displayType->TurnSignalsBox.TSBox.position_x,
					displayType->TurnSignalsBox.TSBox.position_y,
					displayType->TurnSignalsBox.TSBox.width,
					displayType->TurnSignalsBox.TSBox.height,
					TFT_BLACK);
		}
	}
	else if(2 == DigitalCluster_RxSig_TurnSignals)
	{
		if(TS_Counter % 2 == 0  && TS_Counter != 0)
		{
			FsmcH_FillRectangle(displayType->TurnSignalsBox.TSBox.position_x + displayType->TurnSignalsBox.TSBox.width / 2,
					displayType->TurnSignalsBox.TSBox.position_y,
					displayType->TurnSignalsBox.TSBox.width / 2,
					displayType->TurnSignalsBox.TSBox.height,
					TFT_GREEN);
			TS_Counter = 0;
		}
		else
		{
			TS_Counter++;
			FsmcH_FillRectangle(displayType->TurnSignalsBox.TSBox.position_x,
					displayType->TurnSignalsBox.TSBox.position_y,
					displayType->TurnSignalsBox.TSBox.width,
					displayType->TurnSignalsBox.TSBox.height,
					TFT_BLACK);
		}
	}
	else if(3 == DigitalCluster_RxSig_TurnSignals)
	{
		if(TS_Counter % 2 == 0  && TS_Counter != 0)
		{
			FsmcH_FillRectangle(displayType->TurnSignalsBox.TSBox.position_x,
					displayType->TurnSignalsBox.TSBox.position_y,
					displayType->TurnSignalsBox.TSBox.width,
					displayType->TurnSignalsBox.TSBox.height,
					TFT_GREEN);
			TS_Counter = 0;
		}
		else
		{
			TS_Counter++;
			FsmcH_FillRectangle(displayType->TurnSignalsBox.TSBox.position_x,
					displayType->TurnSignalsBox.TSBox.position_y,
					displayType->TurnSignalsBox.TSBox.width,
					displayType->TurnSignalsBox.TSBox.height,
					TFT_BLACK);
		}
	}
	else
	{
		if(TS_Counter != 0)
		{
			FsmcH_FillRectangle(displayType->TurnSignalsBox.TSBox.position_x,
					displayType->TurnSignalsBox.TSBox.position_y,
					displayType->TurnSignalsBox.TSBox.width,
					displayType->TurnSignalsBox.TSBox.height,
					TFT_BLACK);
		}
		else
		{
			/* Do nothing. */
		}
		TS_Counter = 0;
	}
}
void DigitalCluster_HandleCollisionWarning(DigitalCluster_DisplayMode_t *displayType)
{
	if(1 == DigitalCluster_RxSig_CollisionWarning)
	{
		if(0 == collisionFlag)
		{
			collisionFlag = 1;
			collisionFlagClear = 0;
			FsmcH_FillRectangle(displayType->DriverAssistanceMessage.DASBox.position_x,
					displayType->DriverAssistanceMessage.DASBox.position_y,
					displayType->DriverAssistanceMessage.DASBox.width,
					displayType->DriverAssistanceMessage.DASBox.height,
					TFT_RED);
			FsmcH_DrawString(displayType->DriverAssistanceMessage.DAS_Text.position_x,
					displayType->DriverAssistanceMessage.DAS_Text.position_y,
					displayType->DriverAssistanceMessage.DAS_Text.text,
					TFT_WHITE,
					TFT_BLACK);
		}
		else
		{
			/* Do nothing. */
		}
	}
	else if(2 == DigitalCluster_RxSig_CollisionWarning)
	{
		collisionFlagClear = 0;
		if(CAW_Counter % 2 == 0 && CAW_Counter != 0)
		{
			FsmcH_FillRectangle(displayType->DriverAssistanceMessage.DASBox.position_x,
					displayType->DriverAssistanceMessage.DASBox.position_y,
					displayType->DriverAssistanceMessage.DASBox.width,
					displayType->DriverAssistanceMessage.DASBox.height,
					TFT_RED);
			FsmcH_DrawString(displayType->DriverAssistanceMessage.DAS_Text.position_x,
					displayType->DriverAssistanceMessage.DAS_Text.position_y,
					displayType->DriverAssistanceMessage.DAS_Text.text,
					TFT_WHITE,
					TFT_BLACK);
			CAW_Counter = 0;
			DigitalCluster_BuzzerState = 1;
		}
		else
		{
			CAW_Counter++;
			FsmcH_FillRectangle(displayType->DriverAssistanceMessage.DASBox.position_x,
					displayType->DriverAssistanceMessage.DASBox.position_y,
					displayType->DriverAssistanceMessage.DASBox.width,
					displayType->DriverAssistanceMessage.DASBox.height,
					TFT_BLACK);
			DigitalCluster_BuzzerState = 0;
		}
	}
	else
	{
		if(0 == collisionFlagClear)
		{
			collisionFlagClear = 1;
			FsmcH_FillRectangle(displayType->DriverAssistanceMessage.DASBox.position_x,
					displayType->DriverAssistanceMessage.DASBox.position_y,
					displayType->DriverAssistanceMessage.DASBox.width,
					displayType->DriverAssistanceMessage.DASBox.height,
					TFT_BLACK);
		}
		else
		{
			/* Do nothing. */
		}
		CAW_Counter = 0;
		collisionFlag = 0;
	}
}
void DigitalCluster_DisplayInfoLights(DigitalCluster_DisplayMode_t *displayType)
{
	if(pFogLightFront != DigitalCluster_RxSig_FogLightFront)
	{
		pFogLightFront = DigitalCluster_RxSig_FogLightFront;
		FsmcH_FillRectangle(displayType->DashboardLights2.DashboardLightsBox2.position_x,
				displayType->DashboardLights2.DashboardLightsBox2.position_y,
				51,
				17,
				TFT_BLACK);
		if(1 == DigitalCluster_RxSig_FogLightFront)
		{
			FsmcH_DrawString(displayType->DashboardLights2.DL2_FLF.position_x,
					displayType->DashboardLights2.DL2_FLF.position_y,
					displayType->DashboardLights2.DL2_FLF.text,
					TFT_GREEN,
					TFT_BLACK);

		}
		else
		{
			/* Do nothing. */
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(pFogLightRear != DigitalCluster_RxSig_FogLightRear)
	{
		pFogLightRear = DigitalCluster_RxSig_FogLightRear;
		FsmcH_FillRectangle(displayType->DashboardLights2.DashboardLightsBox2.position_x + 60,
				displayType->DashboardLights2.DashboardLightsBox2.position_y,
				51,
				17,
				TFT_BLACK);
		if(1 == DigitalCluster_RxSig_FogLightRear)
		{
			FsmcH_DrawString(displayType->DashboardLights2.DL2_FLR.position_x,
					displayType->DashboardLights2.DL2_FLR.position_y,
					displayType->DashboardLights2.DL2_FLR.text,
					TFT_YELLOW,
					TFT_BLACK);
		}
		else
		{
			/* Do nothing. */
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(pRls != DigitalCluster_RxSig_Rls)
	{
		pRls = DigitalCluster_RxSig_Rls;
		FsmcH_FillRectangle(displayType->DashboardLights2.DashboardLightsBox2.position_x + 120,
				displayType->DashboardLights2.DashboardLightsBox2.position_y,
				51,
				17,
				TFT_BLACK);
		if(1 == DigitalCluster_RxSig_Rls)
		{
			FsmcH_DrawString(displayType->DashboardLights2.DL2_RLS.position_x,
					displayType->DashboardLights2.DL2_RLS.position_y,
					displayType->DashboardLights2.DL2_RLS.text,
					TFT_GREEN,
					TFT_BLACK);
		}
		else
		{
			/* Do nothing. */
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(pRecirculation != DigitalCluster_RxSig_Recirculation)
	{
		pRecirculation = DigitalCluster_RxSig_Recirculation;
		FsmcH_FillRectangle(displayType->DashboardLights2.DashboardLightsBox2.position_x + 180,
				displayType->DashboardLights2.DashboardLightsBox2.position_y,
				17,
				17,
				TFT_BLACK);
		if(2 == DigitalCluster_RxSig_Recirculation)
		{
			FsmcH_DrawString(displayType->DashboardLights2.DL2_C.position_x,
					displayType->DashboardLights2.DL2_C.position_y,
					displayType->DashboardLights2.DL2_C.text,
					TFT_ORANGE,
					TFT_BLACK);
		}
		else
		{
			/* Do nothing. */
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(pAutoClimate != DigitalCluster_RxSig_AutoClimate)
	{
		pAutoClimate = DigitalCluster_RxSig_AutoClimate;
		FsmcH_FillRectangle(displayType->DashboardLights2.DashboardLightsBox2.position_x,
				displayType->DashboardLights2.DashboardLightsBox2.position_y + 30,
				68,
				17,
				TFT_BLACK);
		if(1 == DigitalCluster_RxSig_AutoClimate)
		{
			FsmcH_DrawString(displayType->DashboardLights2.DL2_AUTO.position_x,
					displayType->DashboardLights2.DL2_AUTO.position_y,
					displayType->DashboardLights2.DL2_AUTO.text,
					TFT_ORANGE,
					TFT_BLACK);
		}
		else
		{
			/* Do nothing. */
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(pRequestedTemperature != DigitalCluster_RxSig_RequestedTemperature)
	{
		pRequestedTemperature = DigitalCluster_RxSig_RequestedTemperature;
		FsmcH_FillRectangle(displayType->DashboardLights2.DashboardLightsBox2.position_x,
				displayType->DashboardLights2.DashboardLightsBox2.position_y + 60,
				51,
				17,
				TFT_BLACK);

		FsmcH_DrawInteger(displayType->DashboardLights2.DL2_RTN.position_x,
				displayType->DashboardLights2.DL2_RTN.position_y,
				DigitalCluster_RxSig_RequestedTemperature + 16,
				TFT_ORANGE,
				TFT_BLACK);

		FsmcH_DrawString(displayType->DashboardLights2.DL2_RT.position_x,
				displayType->DashboardLights2.DL2_RT.position_y,
				displayType->DashboardLights2.DL2_RT.text,
				TFT_ORANGE,
				TFT_BLACK);
	}
	else
	{
		/* Do nothing. */
	}
	if(pFanValue != DigitalCluster_RxSig_FanValue)
	{
		pFanValue = DigitalCluster_RxSig_FanValue;
		FsmcH_FillRectangle(displayType->DashboardLights2.DashboardLightsBox2.position_x + 80,
				displayType->DashboardLights2.DashboardLightsBox2.position_y + 30,
				51,
				17,
				TFT_BLACK);

		FsmcH_DrawInteger(displayType->DashboardLights2.DL2_FVN.position_x,
				displayType->DashboardLights2.DL2_FVN.position_y,
				DigitalCluster_RxSig_FanValue,
				TFT_ORANGE,
				TFT_BLACK);

		FsmcH_DrawString(displayType->DashboardLights2.DL2_FV.position_x,
				displayType->DashboardLights2.DL2_FV.position_y,
				displayType->DashboardLights2.DL2_FV.text,
				TFT_ORANGE,
				TFT_BLACK);
	}
	else
	{
		/* Do nothing. */
	}
	if(tPrevState != DigitalCluster_RxSig_OutsideTemperature)
	{
		tPrevState = DigitalCluster_RxSig_OutsideTemperature;
		if(100 <= DigitalCluster_RxSig_OutsideTemperature) DigitalCluster_RxSig_OutsideTemperature = 99;
		else
		{
			/* Do nothing. */
		}
		FsmcH_FillRectangle(displayType->Temperature.TemperatureBox.position_x,
				displayType->Temperature.TemperatureBox.position_y,
				displayType->Temperature.TemperatureBox.width,
				displayType->Temperature.TemperatureBox.height,
				TFT_BLACK);
		if(10 > DigitalCluster_RxSig_OutsideTemperature)
		{
			FsmcH_DrawInteger(displayType->Temperature.T_TempText.position_x + 17,
					displayType->Temperature.T_TempText.position_y,
					DigitalCluster_RxSig_OutsideTemperature,
					TFT_TAN,
					TFT_BLACK);
		}
		else
		{
			FsmcH_DrawInteger(displayType->Temperature.T_TempText.position_x,
					displayType->Temperature.T_TempText.position_y,
					DigitalCluster_RxSig_OutsideTemperature,
					TFT_TAN,
					TFT_BLACK);
		}
		FsmcH_DrawString(displayType->Temperature.T_TempText.position_x + 34,
				displayType->Temperature.T_TempText.position_y,
				"C",
				TFT_TAN,
				TFT_BLACK);
	}
	else
	{
		/* Do nothing. */
	}
	if(gPrevState != DigitalCluster_RxSig_Gear || dmPrevState != DigitalCluster_RxSig_DisplayMode)
	{
		gPrevState = DigitalCluster_RxSig_Gear;
		dmPrevState = DigitalCluster_RxSig_DisplayMode;
		FsmcH_DrawRectangle(displayType->DashboardLights.DashboardLightsBox.position_x,
				displayType->DashboardLights.DashboardLightsBox.position_y,
				displayType->DashboardLights.DashboardLightsBox.width,
				displayType->DashboardLights.DashboardLightsBox.height,
				TFT_BLACK);

		switch(DigitalCluster_RxSig_Gear)
		{
		case 0:
			FsmcH_DrawString(displayType->Gear.G_GselText.position_x,
					displayType->Gear.G_GselText.position_y,
					"P",
					displayType->DisplayColor,
					TFT_BLACK);
			break;
		case 1:
			FsmcH_DrawString(displayType->Gear.G_GselText.position_x,
					displayType->Gear.G_GselText.position_y,
					"N",
					displayType->DisplayColor,
					TFT_BLACK);
			break;
		case 2:
			FsmcH_DrawString(displayType->Gear.G_GselText.position_x,
					displayType->Gear.G_GselText.position_y,
					"D",
					displayType->DisplayColor,
					TFT_BLACK);
			break;
		case 3:
			FsmcH_DrawString(displayType->Gear.G_GselText.position_x,
					displayType->Gear.G_GselText.position_y,
					"R",
					displayType->DisplayColor,
					TFT_BLACK);
			break;
		default:
			break;
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(DigitalCluster_RxSig_HighBeamStatus == 1 && hbPrevState == 0)
	{
		hbPrevState = DigitalCluster_RxSig_HighBeamStatus;
		FsmcH_DrawString(displayType->DashboardLights.DL_HB.position_x,
				displayType->DashboardLights.DL_HB.position_y,
				displayType->DashboardLights.DL_HB.text,
				TFT_BLUE,
				TFT_BLACK);
	}
	else if(DigitalCluster_RxSig_HighBeamStatus == 0 && hbPrevState == 1)
	{
		hbPrevState = DigitalCluster_RxSig_HighBeamStatus;
		FsmcH_DrawString(displayType->DashboardLights.DL_HB.position_x,
				displayType->DashboardLights.DL_HB.position_y,
				"  ",
				TFT_BLUE,
				TFT_BLACK);
	}
	else
	{
		/* Do nothing. */
	}
	if(DigitalCluster_RxSig_PowerSteeringStatus == 1 && psPrevState == 0)
	{
		psPrevState = DigitalCluster_RxSig_PowerSteeringStatus;
		FsmcH_DrawString(displayType->DashboardLights.DL_PS.position_x,
				displayType->DashboardLights.DL_PS.position_y,
				displayType->DashboardLights.DL_PS.text,
				TFT_YELLOW,
				TFT_BLACK);
	}
	else if(DigitalCluster_RxSig_PowerSteeringStatus == 0 && psPrevState == 1)
	{
		psPrevState = DigitalCluster_RxSig_PowerSteeringStatus;
		psPrevState = DigitalCluster_RxSig_PowerSteeringStatus;
		FsmcH_DrawString(displayType->DashboardLights.DL_PS.position_x,
				displayType->DashboardLights.DL_PS.position_y,
				"  ",
				TFT_YELLOW,
				TFT_BLACK);
	}
	else
	{
		/* Do nothing. */
	}
}
void DigitalCluster_DisplayCenterSpeedRpm(DigitalCluster_DisplayMode_t *displayType)
{
	if(pVehSpeed != DigitalCluster_RxSig_VehicleSpeed || localDM1 != DigitalCluster_RxSig_DisplayMode)
	{
		localDM1 = DigitalCluster_RxSig_DisplayMode;
		pVehSpeed = DigitalCluster_RxSig_VehicleSpeed;
		vehSpeedConv = DigitalCluster_RxSig_VehicleSpeed;
		vehSpeedConv = vehSpeedConv * 330 / 255;

		if(10 > vehSpeedConv)
		{
			FsmcH_FillRectangle(0,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					102,
					20,
					TFT_BLACK);
			FsmcH_DrawString(displayType->CenterScreenInformation.SL_KMH.position_x,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					displayType->CenterScreenInformation.SL_KMH.text,
					displayType->DisplayColor,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->CenterScreenInformation.SL_KMH.position_x - 24,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					vehSpeedConv,
					displayType->DisplayColor,
					TFT_BLACK);
		}
		else if(100 > vehSpeedConv)
		{
			FsmcH_FillRectangle(0,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					102,
					20,
					TFT_BLACK);
			FsmcH_DrawString(displayType->CenterScreenInformation.SL_KMH.position_x,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					displayType->CenterScreenInformation.SL_KMH.text,
					displayType->DisplayColor,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->CenterScreenInformation.SL_KMH.position_x - 40,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					vehSpeedConv,
					displayType->DisplayColor,
					TFT_BLACK);
		}
		else
		{
			FsmcH_FillRectangle(0,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					102,
					20,
					TFT_BLACK);
			FsmcH_DrawString(displayType->CenterScreenInformation.SL_KMH.position_x,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					displayType->CenterScreenInformation.SL_KMH.text,
					displayType->DisplayColor,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->CenterScreenInformation.SL_KMH.position_x - 60,
					displayType->CenterScreenInformation.SL_KMH.position_y,
					vehSpeedConv,
					displayType->DisplayColor,
					TFT_BLACK);
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(pRpm != DigitalCluster_RxSig_MotorRpm || localDM2 != DigitalCluster_RxSig_DisplayMode)
	{
		pRpm = DigitalCluster_RxSig_MotorRpm;
		rpmConv = DigitalCluster_RxSig_MotorRpm;
		rpmConv = rpmConv * 9999 / 255;
		localDM2 = DigitalCluster_RxSig_DisplayMode;
		if(10 > rpmConv)
		{
			FsmcH_FillRectangle(190,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					190,
					20,
					TFT_BLACK);
			FsmcH_DrawString(displayType->CenterScreenInformation.SL_RPM.position_x,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					displayType->CenterScreenInformation.SL_RPM.text,
					displayType->DisplayColor,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->CenterScreenInformation.SL_RPM.position_x - 24,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					rpmConv,
					displayType->DisplayColor,
					TFT_BLACK);
		}
		else if(100 > rpmConv)
		{
			FsmcH_FillRectangle(190,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					190,
					20,
					TFT_BLACK);
			FsmcH_DrawString(displayType->CenterScreenInformation.SL_RPM.position_x,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					displayType->CenterScreenInformation.SL_RPM.text,
					displayType->DisplayColor,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->CenterScreenInformation.SL_RPM.position_x - 40,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					rpmConv,
					displayType->DisplayColor,
					TFT_BLACK);
		}
		else if(1000 > rpmConv)
		{
			FsmcH_FillRectangle(190,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					190,
					20,
					TFT_BLACK);
			FsmcH_DrawString(displayType->CenterScreenInformation.SL_RPM.position_x,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					displayType->CenterScreenInformation.SL_RPM.text,
					displayType->DisplayColor,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->CenterScreenInformation.SL_RPM.position_x - 60,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					rpmConv,
					displayType->DisplayColor,
					TFT_BLACK);
		}
		else
		{
			FsmcH_FillRectangle(190,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					190,
					20,
					TFT_BLACK);
			FsmcH_DrawString(displayType->CenterScreenInformation.SL_RPM.position_x,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					displayType->CenterScreenInformation.SL_RPM.text,
					displayType->DisplayColor,
					TFT_BLACK);
			FsmcH_DrawInteger(displayType->CenterScreenInformation.SL_RPM.position_x - 77,
					displayType->CenterScreenInformation.SL_RPM.position_y,
					rpmConv,
					displayType->DisplayColor,
					TFT_BLACK);
		}
	}
	else
	{
		/* Do nothing. */
	}
}
void DigitalCluster_DisplaySideSpeedRpm(DigitalCluster_DisplayMode_t *displayType)
{
	speedBarHeight = (DigitalCluster_RxSig_VehicleSpeed * maxBarHeight) / 255;
	rpmBarHeight = (DigitalCluster_RxSig_MotorRpm * maxBarHeight) / 255;
	if (pSpeedBarHeight != speedBarHeight || localDM3 != DigitalCluster_RxSig_DisplayMode)
	{
		localDM3 = DigitalCluster_RxSig_DisplayMode;
		pSpeedBarHeight = speedBarHeight;
		FsmcH_FillRectangle(displayType->SpeedBar.SpeedQuarter.position_x1,
				displayType->SpeedBar.SpeedQuarter.position_y2 - maxBarHeight,
				displayType->SpeedBar.SpeedQuarter.position_x2 - displayType->SpeedBar.SpeedQuarter.position_x1,
				maxBarHeight - speedBarHeight,
				TFT_BLACK);
		FsmcH_FillRectangle(displayType->SpeedBar.SpeedQuarter.position_x1,
				displayType->SpeedBar.SpeedQuarter.position_y2 - speedBarHeight,
				displayType->SpeedBar.SpeedQuarter.position_x2 - displayType->SpeedBar.SpeedQuarter.position_x1,
				speedBarHeight,
				displayType->DisplayVehSpeedColor);
	}
	else
	{
		/* Do nothing. */
	}
	if (pRpmBarHeight != rpmBarHeight || localDM4 != DigitalCluster_RxSig_DisplayMode)
	{
		localDM4 = DigitalCluster_RxSig_DisplayMode;
		pRpmBarHeight = rpmBarHeight;
		FsmcH_FillRectangle(displayType->RpmBar.RpmQuarter.position_x1,
				displayType->RpmBar.RpmQuarter.position_y2 - maxBarHeight,
				displayType->RpmBar.RpmQuarter.position_x2 - displayType->RpmBar.RpmQuarter.position_x1,
				maxBarHeight - rpmBarHeight,
				TFT_BLACK);
		FsmcH_FillRectangle(displayType->RpmBar.RpmQuarter.position_x1,
				displayType->RpmBar.RpmQuarter.position_y2 - rpmBarHeight,
				displayType->RpmBar.RpmQuarter.position_x2 - displayType->RpmBar.RpmQuarter.position_x1,
				rpmBarHeight,
				displayType->DisplayRPMColor);
	}
	else
	{
		/* Do nothing. */
	}
}
void DigitalCluster_CSEHandler(void)
{
	/* Display collision warning. */
	DigitalCluster_HandleCollisionWarning(&DigitalCluster_Display_ComfortEcoSport);
	/* Display info-cluster warnings. */
	DigitalCluster_DisplayCheckControl(&DigitalCluster_Display_ComfortEcoSport);
	/* Display speed and RPM values. */
	DigitalCluster_DisplayCenterSpeedRpm(&DigitalCluster_Display_ComfortEcoSport);
	/* Display fill-bars for speed and RPM values. */
	DigitalCluster_DisplaySideSpeedRpm(&DigitalCluster_Display_ComfortEcoSport);
	/* Display outside temperature
	 * climate automatic
	 * recirculation
	 * power steering warning
	 * high beam
	 * RLS
	 * fog lights
	 * requested temperature
	 * fan value */
	DigitalCluster_DisplayInfoLights(&DigitalCluster_Display_ComfortEcoSport);
	/* Display turn signals and hazard lights. */
	DigitalCluster_HandleTurnSignal(&DigitalCluster_Display_ComfortEcoSport);
	/* Display time with RTC. */
	DigitalCluster_CalculateTime(&DigitalCluster_Display_ComfortEcoSport);
	/* Display total KM and DCY KM. */
	DigitalCluster_DisplayKMTotalDcy(&DigitalCluster_Display_ComfortEcoSport);
	/* Buzzer will beep on CAW or on ICM. */
	HAL_GPIO_WritePin(BUZOUT_GPIO_Port, BUZOUT_Pin, DigitalCluster_BuzzerState);
}
void DigitalCluster_ShutOffDisplay(void)
{
	/* Shutdown sequence. */
	DigitalCluster_ShutOffDisplayFlag = 0x01;
	FsmcH_FillRectangle(0, 0, 320, 240 ,0);
	HAL_GPIO_WritePin(DIGITALCLUSTER_BACKLIGHT_GPIO_Port, DIGITALCLUSTER_BACKLIGHT_Pin, 0);
	HAL_GPIO_WritePin(BUZOUT_GPIO_Port, BUZOUT_Pin, 0);
	FsmcH_WriteCommand(0x28);
}
void DigitalCluster_WelcomeAnimationHandler(void)
{
	if(0 == clearBeforeWelcome)
	{
		clearBeforeWelcome = 1;
		FsmcH_FillRectangle(0, 0, 320, 240, TFT_BLACK);
	}
	else
	{
		/* Do nothing. */
	}
	if(DIGITALCLUSTER_WELCOMELEAVINGANIMATIONCOUNTER_VALUE > DigitalCluster_WelcomeAnimationCounter)
	{
		DigitalCluster_WelcomeAnimationCounter++;
		if(0xFFFF - 11000 > DigitalCluster_WelcomeAuxColor) DigitalCluster_WelcomeAuxColor += 11000;
		else
		{
			/* Do nothing. */
		}
		FsmcH_DrawString(100, 100, "WELCOME\0", DigitalCluster_WelcomeAuxColor, TFT_BLACK);
		FsmcH_DrawString(125, 125, "USER\0", DigitalCluster_WelcomeAuxColor, TFT_BLACK);
	}
	else
	{
		FsmcH_FillRectangle(100, 100, 140, 20, TFT_BLACK);
		FsmcH_FillRectangle(125, 125, 80, 20, TFT_BLACK);
		DigitalCluster_WelcomeAnimationFlag = 0x01;
	}
}
void DigitalCluster_LeavingAnimationHandler(void)
{
	DigitalCluster_WelcomeAnimationFlag = 0;

	if(0 == clearBeforeLeave)
	{
		/* Set variables to a known state for potential transition to run state. */
		clearBeforeLeave = 1;
		FsmcH_FillRectangle(0, 0, 320, 240, TFT_BLACK);
		speedBarHeight = 180;
		rpmBarHeight = 180;
		pSpeedBarHeight = 180;
		pRpmBarHeight = 180;
		maxBarHeight = 180;
		hasTimeHChanged = 180;
		hasTimeMChanged = 180;
		hbPrevState = 180;
		psPrevState = 180;
		tPrevState = 100;
		gPrevState = 5;
		dmPrevState = 180;
		pVehSpeed = 331;
		pRpm = 10000;
		timestamp = 0;
		collisionFlag = 0;
		collisionFlagClear = 0;
		ccmFlag = 0;
		vehSpeedConv = 331;
		rpmConv = 10000;
		localDM1 = 180;
		localDM2 = 180;
		localDM3 = 180;
		localDM4 = 180;
		DigitalCluster_WelcomeAnimationCounter = 0x00;
		DigitalCluster_LeavingAnimationCounter = 0x00;
		DigitalCluster_WelcomeAuxColor = 0x00;
		DigitalCluster_LeavingAuxColor = 0xFFFF;
		CCM_Counter = 0x00;
		TS_Counter = 0x00;
		CAW_Counter = 0x00;
		clearBeforeWelcome = 0;
		pKmTot = 0xFFFFFFFF;
		pKmDcy = 0xFFFFFFFF;
		pFogLightFront = 99;
		pFogLightRear = 99;
		pRls = 99;
		pFogLightFront = 99;
		pRecirculation = 99;
		pRequestedTemperature = 99;
		pAutoClimate = 99;
		pFanValue = 9;
		pCheckControlMessageId = 0;
	}
	else
	{
		/* Do nothing. */
	}
	if(DIGITALCLUSTER_WELCOMELEAVINGANIMATIONCOUNTER_VALUE > DigitalCluster_LeavingAnimationCounter)
	{
		DigitalCluster_LeavingAnimationCounter++;
		if(11000 < DigitalCluster_LeavingAuxColor) DigitalCluster_LeavingAuxColor -= 11000;
		else
		{
			/* Do nothing. */
		}
		FsmcH_DrawString(100,100,"Leaving\0",DigitalCluster_LeavingAuxColor,TFT_BLACK);
	}
	else
	{
		DigitalCluster_LeavingAnimationFlag = 0x02;
		DigitalCluster_ShutOffDisplay();
	}
}
void DigitalCluster_SelfTest(void)
{
	/* Traditional RGB LCD self-test. */
	if(0 == timestamp)
	{
		timestamp = DigitalCluster_MainCounter;
		FsmcH_FillRectangle(0, 0, 320, 240, TFT_BLACK);
		FsmcH_FillRectangle(0, 0, 320, 240, TFT_RED);
	}
	else
	{
		/* Do nothing. */
	}
	if((DigitalCluster_MainCounter - timestamp) == 200) FsmcH_FillRectangle(0, 0, 320, 240, TFT_GREEN);
	else if((DigitalCluster_MainCounter - timestamp) == 400) FsmcH_FillRectangle(0, 0, 320, 240, TFT_BLUE);
	else
	{
		/* Do nothing. */
	}
	if(DigitalCluster_MainCounter - timestamp > 600)
	{
		/* Restore LCD to previous state. */
		FsmcH_FillRectangle(0, 0, 320, 240, TFT_BLACK);
		DigitalCluster_IsSelfTestRequested = 0;
		timestamp = 0;
		DigitalCluster_InitMemory();
	}
	else
	{
		/* Do nothing. */
	}
}
void DigitalCluster_InitMemory(void)
{
	CCM_Counter = 0x00;
	TS_Counter = 0x00;
	CAW_Counter = 0x00;
	speedBarHeight = 180;
	rpmBarHeight = 180;
	pSpeedBarHeight = 180;
	pRpmBarHeight = 180;
	maxBarHeight = 180;
	hasTimeHChanged = 99;
	hasTimeMChanged = 99;
	hbPrevState = 99;
	psPrevState = 99;
	tPrevState = 100;
	gPrevState = 5;
	dmPrevState = 99;
	pVehSpeed = 331;
	pRpm = 10000;
	timestamp = 0;
	collisionFlag = 0;
	collisionFlagClear = 0;
	ccmFlag = 0;
	vehSpeedConv = 340;
	rpmConv = 10000;
	localDM1 = 99;
	localDM2 = 99;
	localDM3 = 99;
	localDM4 = 99;
	clearBeforeLeave = 0;
	clearBeforeWelcome = 0;
	pKmTot = 0xFFFFFFFF;
	pKmDcy = 0xFFFFFFFF;
	pFogLightFront = 99;
	pFogLightRear = 99;
	pRls = 99;
	pRecirculation = 99;
	pRequestedTemperature = 99;
	pAutoClimate = 99;
	pFanValue = 99;
	pCheckControlMessageId = 0;
}
void DigitalCluster_MainFunction(void)
{
	/* Make sure to keep buzzer low when needed. */
	if(DigitalCluster_RxSig_CheckControlMessageId == 0 && DigitalCluster_RxSig_CollisionWarning != 2)
	{
		HAL_GPIO_WritePin(BUZOUT_GPIO_Port, BUZOUT_Pin, 0);
		DigitalCluster_BuzzerState = 0;
	}
	else
	{
		/* Do nothing. */
	}
	/* Execute logic only when initialization is succesful. */
	if(0x01 == DigitalCluster_LcdInit)
	{
		/* Voltage supply is above 3V. */
		if(29 < DigitalCluster_RxSig_VBat)
		{
			/* Ignition is above 1. */
			if(0x01 <= DigitalCluster_RxSig_IgnitionStatus)
			{
				DigitalCluster_PreviousIgnStat = DigitalCluster_RxSig_IgnitionStatus;
				/* Execute reverse camera functionality only after welcoming animation is executed. */
				if(0x01 == DigitalCluster_WelcomeAnimationFlag) RevCam_MainFunction();
				else
				{
					/* Do nothing. */
				}
				if(0 == DigitalCluster_IsReverseCameraActive)
				{
					if(pIsReverseCameraActive != DigitalCluster_IsReverseCameraActive)
					{
						/* Restore previous state after reverse camera. */
						pIsReverseCameraActive = DigitalCluster_IsReverseCameraActive;
						DigitalCluster_InitMemory();
						FsmcH_FillRectangle(0, 0, 320, 240 ,0);
					}
					else
					{
						/* Do nothing. */
					}
					/* If shutdown was executed, re-initialize. */
					if(0x01 == DigitalCluster_ShutOffDisplayFlag)
					{
						DigitalCluster_ShutOffDisplayFlag = 0x00;
						DigitalCluster_WelcomeAnimationFlag = 0x00;
						DigitalCluster_LeavingAnimationFlag = 0x01;
						DigitalCluster_LcdInit = 0x00;
						DigitalCluster_InitHandler();
					}
					else
					{
						/* Do nothing. */
					}
					/* Execute welcoming animation. */
					if(0x00 == DigitalCluster_WelcomeAnimationFlag)
					{
						/* 4 times per second refresh rate. */
						if(DigitalCluster_MainCounter % 50 == 0 && DigitalCluster_MainCounter != 0)
						{
							DigitalCluster_WelcomeAnimationHandler();
							DigitalCluster_LeavingAnimationFlag = 0x01;
						}
						else
						{
							/* Do nothing. */
						}
					}
					else
					{
						/* If no self test is requested and the welcoming animation is finished.
						 * Update display mode. */
						if(0 == DigitalCluster_IsSelfTestRequested)
						{
							DigitalCluster_Display_ComfortEcoSport.DisplayType = DigitalCluster_RxSig_DisplayMode;
							if(0 == DigitalCluster_Display_ComfortEcoSport.DisplayType)
							{
								DigitalCluster_Display_ComfortEcoSport.DisplayColor = TFT_IVORY;
								DigitalCluster_Display_ComfortEcoSport.DisplayVehSpeedColor = TFT_FIREBRICK;
								DigitalCluster_Display_ComfortEcoSport.DisplayRPMColor = TFT_PALEGOLDENROD;
							}
							else if(1 == DigitalCluster_Display_ComfortEcoSport.DisplayType)
							{
								DigitalCluster_Display_ComfortEcoSport.DisplayColor = TFT_WHITESMOKE;
								DigitalCluster_Display_ComfortEcoSport.DisplayVehSpeedColor = TFT_ORANGE;
								DigitalCluster_Display_ComfortEcoSport.DisplayRPMColor = TFT_RED;
							}
							else if(2 == DigitalCluster_Display_ComfortEcoSport.DisplayType)
							{
								DigitalCluster_Display_ComfortEcoSport.DisplayColor = TFT_ALICEBLUE;
								DigitalCluster_Display_ComfortEcoSport.DisplayVehSpeedColor = TFT_SKYBLUE;
								DigitalCluster_Display_ComfortEcoSport.DisplayRPMColor = TFT_AQUA;
							}
							else
							{
								/* Do nothing. */
							}
							/* 4 times per second refresh displayable information. */
							if(DigitalCluster_MainCounter % 50 == 0 && DigitalCluster_MainCounter != 0) DigitalCluster_CSEHandler();
							else
							{
								/* Do nothing. */
							}
						}
						else DigitalCluster_SelfTest();
					}
				}
				else
				{
					/* Keep track of previous reverse camera state. */
					if(pIsReverseCameraActive != DigitalCluster_IsReverseCameraActive)
					{
						pIsReverseCameraActive = DigitalCluster_IsReverseCameraActive;
						FsmcH_FillRectangle(0, 0, 320, 240 ,0);
					}
					else
					{
						/* Do nothing. */
					}
				}
			}/* Prepare for shutdown and leaving animation. */
			else if(0x00 == DigitalCluster_RxSig_IgnitionStatus && 0x00 != DigitalCluster_PreviousIgnStat)
			{
				DigitalCluster_IsReverseCameraActive = 0;
				if(0x01 == DigitalCluster_LeavingAnimationFlag)
				{
					if(DigitalCluster_MainCounter % 50 == 0 && DigitalCluster_MainCounter != 0) DigitalCluster_LeavingAnimationHandler();
					else
					{
						/* Do nothing. */
					}
				}
				else  if(0x02 == DigitalCluster_LeavingAnimationFlag)
				{
					if(0x00 == DigitalCluster_ShutOffDisplayFlag) DigitalCluster_ShutOffDisplay();
					else
					{
						/* Do nothing. */
					}
				}
				else
				{
					/* Do nothing. */
				}
			}
			else
			{
				/* Do nothing. */
			}
		}
		else
		{
			if(0x00 == DigitalCluster_ShutOffDisplayFlag) DigitalCluster_ShutOffDisplay();
			else
			{
				/* Do nothing. */
			}
		}
	}
	else DigitalCluster_InitHandler();
	DigitalCluster_MainCounter++;
}