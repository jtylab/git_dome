/**
 * @file Referee_Protocol.h
 * @brief RM裁判系统通信通信协议 v1.6.1
 * @attention
 * @hardware
 * @version 1.6.1-alpha
 * @date 2024-03-24
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __Referee_Protocol_H
#define __Referee_Protocol_H

#include "main.h"

#define Ref_SOF (0xA5)  // 帧头固定值

#define Referee_CMDID_Len  (sizeof(((Ref_t *)0)->CMD_ID))
#define Referee_Header_Len (sizeof(Ref_Header_t) + Referee_CMDID_Len)
#define Referee_CRC16_Len  (2)

#define Ref_CMDID_GameStatus       (0x0001)  // Len=  11，比赛状态数据，固定以 1Hz 频率发送
#define Ref_CMDID_GameResult       (0x0002)  // Len=   1，比赛结果数据，比赛结束触发发送
#define Ref_CMDID_HP               (0x0003)  // Len=  32，机器人血量数据，固定以 3Hz 频率发送
// #define Ref_CMDID_DartStatus       (0x0004)  // Len=   3，飞镖发射状态，飞镖发射后发送  // 已删除
// #define Ref_CMDID_AI_Buff          (0x0005)  // Len=  11，人工智能挑战赛加成与惩罚状态，1Hz 周期发送  // 已删除
#define Ref_CMDID_Event            (0x0101)  // Len=   4，场地事件数据，固定以 1Hz 频率发送
#define Ref_CMDID_SupplyAction     (0x0102)  // Len=   4，补给站动作标识数据，补给站弹丸释放时触发发送
// #define Ref_CMDID_SupplyReq        (0x0103)  // Len=   2，请求补给站补弹数据，由参赛队发送，上限 10Hz。（RM 对抗赛尚未开放）  // 已删除
#define Ref_CMDID_Warning          (0x0104)  // Len=   3，裁判警告数据，己方判罚/判负时触发发送，其余时间以 1Hz 频率发送
#define Ref_CMDID_DartInfo         (0x0105)  // Len=   3，飞镖发射相关数据，固定以 1Hz 频率发送
#define Ref_CMDID_RobotStatus      (0x0201)  // Len=  13，机器人状态数据，10Hz 周期发
#define Ref_CMDID_PowerHeat        (0x0202)  // Len=  16，实时底盘功率和枪口热量数据，固定以 50Hz 频率发送
#define Ref_CMDID_RobotPos         (0x0203)  // Len=  16，机器人位置数据，固定以 1Hz 频率发送
#define Ref_CMDID_Buff             (0x0204)  // Len=   6，机器人增益数据，固定以 3Hz 频率发
#define Ref_CMDID_AirSupport       (0x0205)  // Len=   2，空中支援时间数据，固定以 1Hz 频率发送，仅空中机器人可接收
#define Ref_CMDID_GotDamage        (0x0206)  // Len=   1，伤害状态数据，伤害发生后发送
#define Ref_CMDID_Shooting         (0x0207)  // Len=   7，实时射击数据，弹丸发射后发送
#define Ref_CMDID_Allowance        (0x0208)  // Len=   6，允许发弹量，固定以 10Hz 频率发送
#define Ref_CMDID_RfidState        (0x0209)  // Len=   4，机器人 RFID 模块状态，固定以 3Hz 频率发送
#define Ref_CMDID_DartClientCmd    (0x020A)  // Len=   6，飞镖选手端指令数据，固定以 3Hz 频率发送
#define Ref_CMDID_GroundRobotPos   (0x020B)  // Len=  40，地面机器人位置数据，固定以 1Hz 频率发送
#define Ref_CMDID_RadarMarking     (0x020C)  // Len=   6，雷达标记进度数据，固定以 1Hz 频率发送
#define Ref_CMDID_SentryInfo       (0x020D)  // Len=   4，哨兵自主决策信息同步，固定以 1Hz 频率发送
#define Ref_CMDID_RadarInfo        (0x020E)  // Len=   1，雷达自主决策信息同步，固定以 1Hz 频率发送
#define Ref_CMDID_ExchData         (0x0301)  // Len<=128，机器人间交互数据，发送方触发发送，上限 10Hz
#define Ref_CMDID_VTM_CustomCtrler (0x0302)  // Len<= 30，自定义控制器与机器人交互数据，发送方触发发送，频率上限为 30Hz
#define Ref_CMDID_MapToRobot       (0x0303)  // Len=  15，选手端小地图交互数据，选手端触发发送
#define Ref_CMDID_VTM_KbdData      (0x0304)  // Len=  12，键盘、鼠标信息，通过图传串口发送
#define Ref_CMDID_RadarMapping     (0x0305)  // Len=  10，选手端小地图接收雷达数据，频率上限为 10Hz
// #define Ref_CMDID_CustomCtrlerToClient (0x0306)  // Len=  10，自定义控制器与选手端交互数据，发送方触发发送，频率上限为 30Hz，非机器人处理
#define Ref_CMDID_SentryMapping    (0x0307)  // Len= 103，选手端小地图接收哨兵数据，频率上限为 1Hz
#define Ref_CMDID_CustomInfo       (0x0308)  // Len=  34，选手端小地图接收机器人数据，频率上限为 3Hz

/* 机器人 ID 红方 */
#define Ref_RobotID_R_Hero       (1)   // 机器人 ID 红1 英雄
#define Ref_RobotID_R_Engineer   (2)   // 机器人 ID 红2 工程
#define Ref_RobotID_R_Infantry1  (3)   // 机器人 ID 红3 步兵1
#define Ref_RobotID_R_Infantry2  (4)   // 机器人 ID 红4 步兵2
#define Ref_RobotID_R_Infantry3  (5)   // 机器人 ID 红5 步兵3
#define Ref_RobotID_R_Drone      (6)   // 机器人 ID 红6 无人机
#define Ref_RobotID_R_Sentry     (7)   // 机器人 ID 红7 哨兵
#define Ref_RobotID_R_Radar      (9)   // 机器人 ID 红方 雷达
#define Ref_RobotID_R_Outpost    (10)  // 机器人 ID 红方 前哨站
#define Ref_RobotID_R_Base       (11)  // 机器人 ID 红方 基地
/* 机器人 ID 蓝方 */
#define Ref_RobotID_B_Hero       (101)  // 机器人 ID 蓝1 英雄
#define Ref_RobotID_B_Engineer   (102)  // 机器人 ID 蓝2 工程
#define Ref_RobotID_B_Infantry1  (103)  // 机器人 ID 蓝3 步兵1
#define Ref_RobotID_B_Infantry2  (104)  // 机器人 ID 蓝4 步兵2
#define Ref_RobotID_B_Infantry3  (105)  // 机器人 ID 蓝5 步兵3
#define Ref_RobotID_B_Drone      (106)  // 机器人 ID 蓝6 无人机
#define Ref_RobotID_B_Sentry     (107)  // 机器人 ID 蓝7 哨兵
#define Ref_RobotID_B_Radar      (109)  // 机器人 ID 蓝方 雷达
#define Ref_RobotID_B_Outpost    (110)  // 机器人 ID 蓝方 前哨站
#define Ref_RobotID_B_Base       (111)  // 机器人 ID 蓝方 基地
/* 客户端 ID 红方 */
#define Ref_ClientID_R_Hero      (0x0101)  // 客户端 ID 红1 英雄
#define Ref_ClientID_R_Engineer  (0x0102)  // 客户端 ID 红2 工程
#define Ref_ClientID_R_Infantry1 (0x0103)  // 客户端 ID 红3 步兵1
#define Ref_ClientID_R_Infantry2 (0x0104)  // 客户端 ID 红4 步兵2
#define Ref_ClientID_R_Infantry3 (0x0105)  // 客户端 ID 红5 步兵3
#define Ref_ClientID_R_Drone     (0x0106)  // 客户端 ID 红6 无人机
/* 客户端 ID 蓝方 */
#define Ref_ClientID_B_Hero      (0x0165)  // 客户端 ID 蓝1 英雄
#define Ref_ClientID_B_Engineer  (0x0166)  // 客户端 ID 蓝2 工程
#define Ref_ClientID_B_Infantry1 (0x0167)  // 客户端 ID 蓝3 步兵1
#define Ref_ClientID_B_Infantry2 (0x0168)  // 客户端 ID 蓝4 步兵2
#define Ref_ClientID_B_Infantry3 (0x0169)  // 客户端 ID 蓝5 步兵3
#define Ref_ClientID_B_Drone     (0x016A)  // 客户端 ID 蓝6 无人机

/* 0x0301 子 ID */
#define Ref_ExchID_DelGraphic     (0x0100)  // Len=6+  2，选手端删除图形
#define Ref_ExchID_Draw1          (0x0101)  // Len=6+ 15，选手端绘制一个图形
#define Ref_ExchID_Draw2          (0x0102)  // Len=6+ 30，选手端绘制二个图形
#define Ref_ExchID_Draw5          (0x0103)  // Len=6+ 75，选手端绘制五个图形
#define Ref_ExchID_Draw7          (0x0104)  // Len=6+105，选手端绘制七个图形
#define Ref_ExchID_DrawChar       (0x0110)  // Len=6+ 45，选手端绘制字符图形
#define Ref_ExchID_SentryDecision (0x0120)  // Len=6+  4，哨兵自主决策指令
#define Ref_ExchID_RadarDecision  (0x0121)  // Len=6+  1，雷达自主决策指令
/* 0x0200~0x02FF x≤113 机器人之间通信 */

/* 图形操作 */
#define Ref_Graphic_OType_NULL     (0)  // 空操作
#define Ref_Graphic_OType_Add      (1)  // 增加
#define Ref_Graphic_OType_Modify   (2)  // 修改
#define Ref_Graphic_OType_Del      (3)  // 删除
/* 图形类型 */
#define Ref_Graphic_GType_Line     (0)  // 直线
#define Ref_Graphic_GType_Rect     (1)  // 矩形
#define Ref_Graphic_GType_Circle   (2)  // 整圆
#define Ref_Graphic_GType_Elliptic (3)  // 椭圆
#define Ref_Graphic_GType_Arc      (4)  // 圆弧
#define Ref_Graphic_GType_Float    (5)  // 浮点数
#define Ref_Graphic_GType_Int      (6)  // 整型数
#define Ref_Graphic_GType_Char     (7)  // 字符
/* 颜色 */
#define Ref_Graphic_Color_RedBule  (0)  // 红蓝主色
#define Ref_Graphic_Color_Yellow   (1)  // 黄色
#define Ref_Graphic_Color_Green    (2)  // 绿色
#define Ref_Graphic_Color_Orange   (3)  // 橙色
#define Ref_Graphic_Color_Purple   (4)  // 紫红色
#define Ref_Graphic_Color_Pink     (5)  // 粉色
#define Ref_Graphic_Color_Cyan     (6)  // 青色
#define Ref_Graphic_Color_Black    (7)  // 黑色
#define Ref_Graphic_Color_White    (8)  // 白色

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef struct {
		uint8_t SOF;   // 帧头，固定值 0xA5
		uint16_t Len;  // 数据帧中 data 的长度
		uint8_t Seq;   // 包序号
		uint8_t CRC8;  // 帧头 CRC8 校验
} Ref_Header_t;

typedef struct {
		Ref_Header_t Header;
		uint16_t CMD_ID;  // 命令码 ID
		uint8_t Data[1];
} Ref_t;

typedef struct {
		uint8_t GameType : 4;
		uint8_t GameStage : 4;
		uint16_t StageRemainTime;
		uint64_t SyncTimeStamp;
} Ref_GameStatus_t;  // 0x0001

typedef struct {
		uint8_t Winner;
} Ref_GameResult_t;  // 0x0002

typedef struct {
		uint16_t R1_HP;
		uint16_t R2_HP;
		uint16_t R3_HP;
		uint16_t R4_HP;
		uint16_t R5_HP;
		uint16_t R7_HP;
		uint16_t R_Outpost_HP;
		uint16_t R_Base_HP;
		uint16_t B1_HP;
		uint16_t B2_HP;
		uint16_t B3_HP;
		uint16_t B4_HP;
		uint16_t B5_HP;
		uint16_t B7_HP;
		uint16_t B_Outpost_HP;
		uint16_t B_Base_HP;
} Ref_HP_t;  // 0x0003

typedef struct {
		uint32_t EventType;
} Ref_Event_t;  // 0x0101

typedef struct {
		uint8_t Reserved;  // Supply_Projectile_ID
		uint8_t Supply_Robot_ID;
		uint8_t Supply_Projectile_Step;
		uint8_t Supply_Projectile_Num;
} Ref_SupplyAction_t;  // 0x0102

typedef struct {
		uint8_t Level;
		uint8_t Foul_Robot_ID;
		uint8_t Count;
} Ref_Warning_t;  // 0x0104

typedef struct {
		uint8_t Dart_RemainingTime;
		uint16_t Dart_Info;
} Ref_DartInfo_t;  // 0x0105

typedef struct {
		uint8_t Robot_ID;
		uint8_t Robot_Level;
		uint16_t Current_HP;
		uint16_t Max_HP;
		uint16_t Shooter_Barrel_Cooling_Value;
		uint16_t Shooter_Barrel_Heat_Limit;
		uint16_t Chassis_Power_Limit;
		uint8_t PowerManagement_Gimbal_Output : 1;
		uint8_t PowerManagement_Chassis_Output : 1;
		uint8_t PowerManagement_Shooter_Output : 1;
} Ref_RobotStatus_t;  // 0x0201

typedef struct {
		uint16_t Chassis_Voltage;
		uint16_t Chassis_Current;
		float Chassis_Power;
		uint16_t Chassis_Power_Buffer;
		uint16_t Shooter_17mm_1_Barrel_Heat;
		uint16_t Shooter_17mm_2_Barrel_Heat;
		uint16_t Shooter_42mm_Barrel_Heat;
} Ref_PowerHeat_t;  // 0x0202

typedef struct {
		float X;
		float Y;
		float Angle;
} Ref_RobotPos_t;  // 0x0203

typedef struct {
		uint8_t Recovery_Buff;
		uint8_t Colling_Buff;
		uint8_t Defence_Buff;
		uint8_t Vulnerability_Buff;
		uint16_t Attack_Buff;
} Ref_Buff_t;  // 0x0204

typedef struct {
		uint8_t Airforce_Status;
		uint8_t Time_Remain;
} Ref_AirSupport_t;  // 0x0205

typedef struct {
		uint8_t Armor_ID : 4;
		uint8_t DamageType : 4;
} Ref_GotDamage_t;  // 0x0206

typedef struct {
		uint8_t Bullet_Type;
		uint8_t Shooter_ID;
		uint8_t Bullet_Freq;
		float Bullet_Speed;
} Ref_Shooting_t;  // 0x0207

typedef struct {
		uint16_t Bullet_17mm;
		uint16_t Bullet_42mm;
		uint16_t Coin;
} Ref_Allowance_t;  // 0x0208

typedef struct {
		uint32_t RfidStatus;
} Ref_RfidStatus_t;  // 0x0209

typedef struct {
		uint8_t Dart_launch_opening_status;
		uint8_t Reserved;  // Dart_attack_target
		uint16_t Target_change_time;
		uint16_t Operate_launch_cmd_time;
} Ref_DartClientCmd_t;  // 0x020A

typedef struct {
		float Hero_X;
		float Hero_Y;
		float Engineer_X;
		float Engineer_Y;
		float Standard_3_X;
		float Standard_3_Y;
		float Standard_4_X;
		float Standard_4_Y;
		float Standard_5_X;
		float Standard_5_Y;
} Ref_GroundRobotPos_t;  // 0x020B

typedef struct {
		uint8_t Marking_Hero;
		uint8_t Marking_Engineer;
		uint8_t Marking_Standard_3;
		uint8_t Marking_Standard_4;
		uint8_t Marking_Standard_5;
		uint8_t Marking_Sentry;
} Ref_RadarMarking_t;  // 0x020C

typedef struct {
		uint32_t Sentry_Info;
} Ref_SentryInfo_t;  // 0x020D

typedef struct {
		uint32_t Radar_Info;
} Ref_RadarInfo_t;  // 0x020E

typedef struct {
		uint16_t Data_CMD_ID;
		uint16_t Sender_ID;
		uint16_t Receiver_ID;
} Ref_ExchData_Header_t;  // 0x0301

typedef struct {
		uint8_t Operate_Type;
		uint8_t Layer;
} Ref_GraphicDelete_t;

#pragma anon_unions
typedef struct {
		uint8_t Graphic_Index[3];
		union {
				struct {
						uint32_t Operate_Type : 3;
						uint32_t Graphic_Type : 3;
						uint32_t Layer : 4;
						uint32_t Color : 4;
						uint32_t Details_A : 9;  // Start_Angle
						uint32_t Details_B : 9;  // End_Angle
				};
				uint32_t Config1;
		};
		union {
				struct {
						uint32_t Width : 10;
						uint32_t Start_X : 11;
						uint32_t Start_Y : 11;
				};
				uint32_t Config2;
		};
		union {
				struct {
						uint32_t Details_C : 10;
						uint32_t Details_D : 11;
						uint32_t Details_E : 11;
				};
				uint32_t Config3;
		};
} Ref_Graphic_t;

typedef struct {
		Ref_ExchData_Header_t ExchHeader;
		Ref_GraphicDelete_t DelGraphic;
} Ref_ExchData_DelGraphic_t;  // 0x0301 0x0100

typedef struct {
		Ref_ExchData_Header_t ExchHeader;
		Ref_Graphic_t Graphic;
} Ref_ExchData_Graphic_t;  // 0x0301 0x0101

typedef struct {
		Ref_ExchData_Header_t ExchHeader;
		Ref_Graphic_t Graphic[2];
} Ref_ExchData_Graphic2_t;  // 0x0301 0x0102

typedef struct {
		Ref_ExchData_Header_t ExchHeader;
		Ref_Graphic_t Graphic[5];
} Ref_ExchData_Graphic5_t;  // 0x0301 0x0103

typedef struct {
		Ref_ExchData_Header_t ExchHeader;
		Ref_Graphic_t Graphic[7];
} Ref_ExchData_Graphic7_t;  // 0x0301 0x0104

typedef struct {
		Ref_ExchData_Header_t ExchHeader;
		Ref_Graphic_t Graphic;
		uint8_t str[30];
} Ref_ExchData_String_t;  // 0x0301 0x0110

typedef struct {
		uint32_t SentryDecision;
} Ref_SentryDecision_t;  // 0x0301 0x0120

typedef struct {
		uint32_t RadarDecision;
} Ref_RadarDecision_t;  // 0x0301 0x0121

// 0x0303

typedef struct {
		uint16_t Target_RobotID;
		float Target_Pos_X;
		float Target_Pos_Y;
} Ref_RadarMaping_t;  // 0x0305

typedef struct {
		uint8_t Intention;
		uint16_t Start_Pos_X;
		uint16_t Start_Pos_Y;
		int8_t Delta_X[49];
		int8_t Delta_Y[49];
		uint16_t Sender_ID;
} Ref_SentryMaping_t;  // 0x0307

typedef struct {
		uint16_t Sender_ID;
		uint16_t Receiver_ID;
		uint8_t User_Data[30];
} Ref_CustomInfo_t;  // 0x0308

#pragma pack()

bool Ref_CheckCRC_Header(Ref_Header_t *header);
bool Ref_CheckCRC_Frame(Ref_t *frame);
void Ref_CalcCRC_Header(Ref_Header_t *header);
void Ref_CalcCRC_Frame(uint8_t *SOF, uint16_t Data_Len, uint16_t *CRC16);
uint16_t Ref_GetClientFromRobot(uint8_t RobotID);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __Referee_Protocol_H */

/*********************************END OF FILE*********************************/
