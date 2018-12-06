/*MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

/************************
 *   STATIC VARIABLES   *
 ************************/
os_timer_t blue_led_timer;
os_timer_t key_read_timer;

/************************
 *   STATIC FUNCTIONS   *
 ************************/

/**
 * LED定时反转函数，1s状态反转一次
 */
static void ICACHE_FLASH_ATTR
blue_led_timer_toggle(void){
	os_timer_disarm(&blue_led_timer);//取消定时器
	uint32 status = GPIO_INPUT_GET(GPIO_ID_PIN(2));//获取蓝灯管脚状态
	GPIO_OUTPUT_SET(GPIO_ID_PIN(2),!status);//取反实现蓝灯管脚电平反转，从而实现亮灭操作
	os_timer_arm(&blue_led_timer, 1000, true);//使能定时器
}

/**
 * 按键定时状态读取函数，10ms获取一次按键状态
 */
static void ICACHE_FLASH_ATTR
key_read_value(void){
	os_timer_disarm(&key_read_timer);//取消定时器
	if(GPIO_INPUT_GET(GPIO_ID_PIN(14)) == 0x00){//如果按键按下，低电平
		os_delay_us(20000);//延时20ms消抖
		if(GPIO_INPUT_GET(GPIO_ID_PIN(14)) == 0x00){//再次判断状态，防止误触
			uint32 status = GPIO_INPUT_GET(GPIO_ID_PIN(4));//获取当前管脚状态
			GPIO_OUTPUT_SET(GPIO_ID_PIN(4),!status);//取反实现管脚电平反转
			while(GPIO_INPUT_GET(GPIO_ID_PIN(14)));//等待按键松开
		}
	}
	os_timer_arm(&key_read_timer, 10, true);//使能定时器
}

/**
 *按键中断服务函数，高电平触发
 */
static void ICACHE_FLASH_ATTR
key_intr_handler(void){
	uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);//读取GPIO寄存器状态，获取中断信息
	uint8 level = 0;
	GPIO_REG_WRITE(GPIO_STATUS_ADDRESS,gpio_status);//清楚中断信息
	if(gpio_status & (BIT(15))){//判断是否是gpio15
		if(GPIO_INPUT_GET(15)){//如果是高电平
			GPIO_OUTPUT_SET(GPIO_ID_PIN(5),0);//熄灭红灯
			GPIO_OUTPUT_SET(GPIO_ID_PIN(12),1);//熄灭黄灯
			GPIO_OUTPUT_SET(GPIO_ID_PIN(4),0);//熄灭绿灯
			GPIO_OUTPUT_SET(GPIO_ID_PIN(2),0);//熄灭蓝灯
			os_timer_disarm(&key_read_timer);//取消按键定时器
			os_timer_disarm(&blue_led_timer);//取消蓝灯定时反转定时器
		}else{//如果是低电平
			//不做处理
		}
	}else{//如果不是gpio15
		//不做处理
	}
}

/************************
 *   GLOBAL FUNCTIONS   *
 ************************/
void ICACHE_FLASH_ATTR
gpio_test(void){

	gpio_init();//初始化GPIO

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,FUNC_GPIO5);//红灯
	GPIO_OUTPUT_SET(GPIO_ID_PIN(5),1);//高电平点亮，常亮

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12);//黄灯
	GPIO_OUTPUT_SET(GPIO_ID_PIN(12),0);//低电平点亮，常亮

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);//蓝灯
	os_timer_disarm(&blue_led_timer);
	os_timer_setfn(&blue_led_timer, (os_timer_func_t *) blue_led_timer_toggle,NULL);//定时回调函数
	os_timer_arm(&blue_led_timer, 1000, true);//设置时间为1s

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO4);//绿灯，按键触发，反转
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);//按键
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(14));//按键设置为输入模式
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTMS_U);//引脚上拉使能
	os_timer_disarm(&key_read_timer);
	os_timer_setfn(&key_read_timer, (os_timer_func_t *) key_read_value,NULL);//定时回调函数
	os_timer_arm(&key_read_timer, 10, true);//设置时间为10ms

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U,FUNC_GPIO15);//中断按键
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(15));//设置为输入模式
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTDO_U);//下拉使能
	ETS_GPIO_INTR_DISABLE();//禁止所有GPIO中断
	ETS_GPIO_INTR_ATTACH((void *)key_intr_handler,NULL);//注册GPIO中断处理函数
	gpio_pin_intr_state_set(GPIO_ID_PIN(15),GPIO_PIN_INTR_HILEVEL);//设置高电平触发中断
	ETS_GPIO_INTR_ENABLE();//使能所有GPIO中断

}


