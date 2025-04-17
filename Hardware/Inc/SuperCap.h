/**
 * @file SuperCap.h
 * @brief 安合超级电容控制板
 * @attention
 * @hardware 
 * @version 0.1
 * @date 2024-03-24
 * @author he
 * @copyright Copyright (c) LNPU TCN
 *
 */
#ifndef __SuperCap_H
#define __SuperCap_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class SuperCap_t {
	private:
		uint8_t Prv_CAN_Port;
		bool isInit;
		uint16_t Set_Input_State;         // 输入 设置运行状态 0停机 1运行（输出负载开关断开） 2运行（输出负载开关打开）
		uint16_t Set_Input_State_Save;    // 输入 运行状态是否保存
		uint16_t Set_Input_Power;         // 输入 设置功率上限 单位 0.01W 默认 5000
		uint16_t Set_Input_Power_Save;    // 输入 功率上限是否保存
		uint16_t Set_Input_Voltage;       // 输入 设置输出电压 单位 0.01V 默认 2300
		uint16_t Set_Input_Voltage_Save;  // 输入 设置输出电压是否保存
		uint16_t Set_Input_Current;       // 输入 设置输出电流 单位 0.01A 默认 800
		uint16_t Set_Input_Current_Save;  // 输入 设置输出电流是否保存

	public:
		uint16_t Input_State;         // 输入 设置运行状态 0停机 1运行（输出负载开关断开） 2运行（输出负载开关打开）
		uint16_t Input_State_Save;    // 输入 运行状态是否保存
		uint16_t Input_Power;         // 输入 设置功率上限 单位 0.01W 默认 5000
		uint16_t Input_Power_Save;    // 输入 功率上限是否保存
		uint16_t Input_Voltage;       // 输入 设置输出电压 单位 0.01V 默认 2300
		uint16_t Input_Voltage_Save;  // 输入 设置输出电压是否保存
		uint16_t Input_Current;       // 输入 设置输出电流 单位 0.01A 默认 800
		uint16_t Input_Current_Save;  // 输入 设置输出电流是否保存
		uint16_t Output_State;        // 输出 模块状态 0就绪 1运行 2报警 3电源开关 4负载开关 5恒流 6恒压 7恒功率 8-14保留 15故障
		uint16_t Output_Fault;        // 输出 故障代码 0无故障 1输入欠压 2输入过压 3输入过流 4输入过功率 5过温保护 6低温保护 7输出过压 8输出过流 9输出过功率 10零漂过大 11反接错误 12失控故障 13通信故障 14EEPROM故障
		uint16_t Output_InPower;      // 输出 输入功率值 单位 0.01 W
		uint16_t Output_InVoltage;    // 输出 输入电压值 单位 0.01 W
		uint16_t Output_InCurrent;    // 输出 输入电流值 单位 0.01 A
		uint16_t Output_OutPower;     // 输出 输出功率值 单位 0.01 W
		uint16_t Output_OutVoltage;   // 输出 输出电压值 单位 0.01 W
		uint16_t Output_OutCurrent;   // 输出 输出电流值 单位 0.01 A
		uint16_t Output_Temp;         // 输出 温度 单位 0.1 摄氏度
		uint16_t Output_AddTime;      // 输出 累计运行时间 单位小时
		uint16_t Output_ThisTime;     // 输出 本次运行时间 单位分钟
		SuperCap_t(uint8_t CAN_Port) {
			Prv_CAN_Port = CAN_Port;

			isInit = 0;

			Set_Input_State = 2;         // 输入 设置运行状态 0停机 1运行（输出负载开关断开） 2运行（输出负载开关打开）
			Set_Input_State_Save = 0;    // 输入 运行状态是否保存
			Set_Input_Power = 5800;      // 输入 设置功率上限 单位 0.01W 默认 5000
			Set_Input_Power_Save = 0;    // 输入 功率上限是否保存
			Set_Input_Voltage = 2300;    // 输入 设置输出电压 单位 0.01V 默认 2300
			Set_Input_Voltage_Save = 0;  // 输入 设置输出电压是否保存
			Set_Input_Current = 3000;     // 输入 设置输出电流 单位 0.01A 默认 800
			Set_Input_Current_Save = 0;  // 输入 设置输出电流是否保存

			// Output_State = 0;       // 输出 模块状态 0就绪 1运行 2报警 3电源开关 4负载开关 5恒流 6恒压 7恒功率 8-14保留 15故障
			// Output_Fault = 0;       // 输出 故障代码 0无故障 1输入欠压 2输入过压 3输入过流 4输入过功率 5过温保护 6低温保护 7输出过压 8输出过流 9输出过功率 10零漂过大 11反接错误 12失控故障 13通信故障 14EEPROM故障
			// Output_InPower = 0;     // 输出 输入功率值 单位 0.01 W
			// Output_InVoltage = 0;   // 输出 输入电压值 单位 0.01 W
			// Output_InCurrent = 0;   // 输出 输入电流值 单位 0.01 A
			// Output_OutPower = 0;    // 输出 输出功率值 单位 0.01 W
			// Output_OutVoltage = 0;  // 输出 输出电压值 单位 0.01 W
			// Output_OutCurrent = 0;  // 输出 输出电流值 单位 0.01 A
			// Output_Temp = 0;        // 输出 温度 单位 0.1 摄氏度
			// Output_AddTime = 0;     // 输出 累计运行时间 单位小时
			// Output_ThisTime = 0;    // 输出 本次运行时间 单位分钟
		}
		void Init(void);
		uint8_t GetPort(void);
		void SetInputState(uint16_t State, bool save);
		void SetInputPower(uint16_t Power, bool save);
		void SetInputVoltage(uint16_t Voltage, bool save);
		void SetInputCurrent(uint16_t Current, bool save);
		friend void SuperCap_SendState(uint8_t ID);
		friend void SuperCap_SendAll(uint8_t ID);
		friend void SuperCap_CAN_RxCbHandler(uint32_t ID, uint8_t *Data);
};

SuperCap_t *Super_Point(void);
#endif

void SuperCap_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __SuperCap_H */

/*********************************END OF FILE*********************************/
