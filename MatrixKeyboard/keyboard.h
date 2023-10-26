#ifndef _KEY16_H
#define _KEY16_H

//#include "sys.h"
#include "usart.h"
#include "main.h"
#include "gpio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <string.h>

//定义行按键的引脚
#define key_row0_Pin GPIO_PIN_15//定义P15为行1
#define key_row1_Pin GPIO_PIN_12//定义P12为行2
#define key_row2_Pin GPIO_PIN_4//定义P4为行3
#define key_row3_Pin GPIO_PIN_5//定义P5为行4

//定义列按键的引脚
#define key_col0_Pin GPIO_PIN_14//定义P15为列1
#define key_col1_Pin GPIO_PIN_13//定义P12为列2
#define key_col2_Pin GPIO_PIN_12//定义P4为列3
#define key_col3_Pin GPIO_PIN_2//定义P5为列4

//行扫描函数、列扫描函数、初始化函数声明
void key_init();
char key_row_scan(void);
char key_scan(void);

//
char key_row[1]; // 声明一个长度为1的char数组

//定义列的低电平输出
#define KEY_COL0_OUT_LOW  HAL_GPIO_WritePin(GPIOB,key_col0_Pin,GPIO_PIN_RESET)
#define KEY_COL1_OUT_LOW  HAL_GPIO_WritePin(GPIOB,key_col1_Pin,GPIO_PIN_RESET)
#define KEY_COL2_OUT_LOW  HAL_GPIO_WritePin(GPIOB,key_col2_Pin,GPIO_PIN_RESET)
#define KEY_COL3_OUT_LOW  HAL_GPIO_WritePin(GPIOB,key_col3_Pin,GPIO_PIN_RESET)

//定义列的高电平输出
#define KEY_COL0_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,key_col0_Pin,GPIO_PIN_SET) 
#define KEY_COL1_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,key_col1_Pin,GPIO_PIN_SET)
#define KEY_COL2_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,key_col2_Pin,GPIO_PIN_SET)
#define KEY_COL3_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,key_col3_Pin,GPIO_PIN_SET)

//定义行的低电平输出
#define KEY_ROW0_OUT_LOW  HAL_GPIO_WritePin(GPIOB,key_row0_Pin,GPIO_PIN_RESET)
#define KEY_ROW1_OUT_LOW  HAL_GPIO_WritePin(GPIOB,key_row1_Pin,GPIO_PIN_RESET)
#define KEY_ROW2_OUT_LOW  HAL_GPIO_WritePin(GPIOB,key_row2_Pin,GPIO_PIN_RESET)
#define KEY_ROW3_OUT_LOW  HAL_GPIO_WritePin(GPIOB,key_row3_Pin,GPIO_PIN_RESET)

//定义行的高电平输出
#define KEY_ROW0_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,key_row0_Pin,GPIO_PIN_SET) 
#define KEY_ROW1_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,key_row1_Pin,GPIO_PIN_SET)
#define KEY_ROW2_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,key_row2_Pin,GPIO_PIN_SET)
#define KEY_ROW3_OUT_HIGH  HAL_GPIO_WritePin(GPIOB,key_row3_Pin,GPIO_PIN_SET)

void key_init(){

    GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pins : PB1 PB2 PB12 PB13
                           PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB15 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

char key_row_scan(void){
	
	key_row[0] = HAL_GPIO_ReadPin(GPIOB, key_row0_Pin)<<3;//读取PB15第1行
	key_row[0] = key_row[0] | (HAL_GPIO_ReadPin(GPIOA, key_row1_Pin)<<2);//读取PA12/第2行
	key_row[0] = key_row[0] | (HAL_GPIO_ReadPin(GPIOB, key_row2_Pin)<<1);//读取PD4/第3行
	key_row[0] = key_row[0] | (HAL_GPIO_ReadPin(GPIOB, key_row3_Pin));//读取PD5/第4行
	
	if(key_row[0] != 0x0f) //1111
	{
		HAL_Delay(10);
			if(key_row[0] != 0x0f)
			{
				HAL_Delay(30);
                if(key_row[0] != 0x0f)
                {
                    switch(key_row[0])
				    {
					    case 0x07:	//0111 第1行被按下
					    	return 1;	
					    case 0x0b:	//1011 第2行被按下
					    	return 2;	
				    	case 0x0d:	//1101 第3行被按下
				    		return 3;
				    	case 0x0e:	//1110 第4行被按下
				    		return 4;
				    	default	:
				    		return 0; //没有按键被按下
			    	}
                }		
			}else return 0;	
	}else return 0;	
} 

char key_scan(void){
	char key_num=0;       //1-16对应的按键数
    char key_row_num=0;        //行扫描结果记录
    
    KEY_COL0_OUT_LOW;        
    if( (key_row_num=key_row_scan()) != 0 )
    { 
        while(key_row_scan() != 0);  //消抖
        key_num = 0 + key_row_num;
    }
    KEY_COL0_OUT_HIGH;
    
    KEY_COL1_OUT_LOW;        
    if( (key_row_num=key_row_scan()) != 0 )
    { 
        while(key_row_scan() != 0);
        key_num = 4 + key_row_num;
        //printf("Key_Clo_2\r\n");
    }
    KEY_COL1_OUT_HIGH;
    
    KEY_COL2_OUT_LOW;    
    if( (key_row_num=key_row_scan()) != 0 )
    { 
        while(key_row_scan() != 0);
    key_num = 8 + key_row_num;
        //printf("Key_Clo_3\r\n");
    }
    KEY_COL2_OUT_HIGH;
    
    KEY_COL3_OUT_LOW;    
    if( (key_row_num=key_row_scan()) != 0 )
    {
        while(key_row_scan() != 0);
        key_num = 12 + key_row_num;
    }
    KEY_COL3_OUT_HIGH;
    
    return key_num;
}


#endif
