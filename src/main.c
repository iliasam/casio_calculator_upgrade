#include "stm32f10x.h"
#include "config_periph.h"
#include "ST7565R.h"
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "stm32f10x_pwr.h"
#include "keys_functions.h"
#include "keys_handling.h"
#include "delay_us_timer.h"
#include "lcd_worker.h"
#include "display_handling.h"
#include "menu_handling.h"
#include "formula_handling.h"

void Delay_ms(uint32_t ms);
volatile double test;

extern volatile uint16_t tmp_cnt;

ModeStateType mode_state = FORMULA_INPUT;
FormulaInputStateType formula_input_state = INPUT_MODE_BASIC;

extern char formula_text[FORMULA_MAX_LENGTH];
extern uint16_t formula_current_length;

CalcAnswerType calc_result;

float battery_voltage = 0.0f;

uint16_t current_key_state;

int main()
{
  char str[32];
  memset(str, 0, 32);
  uint16_t cnt = 0;
  
  init_all_hardware();
  initialize_keys_gpio();
  dwt_init();
  
  lcd_init_pins();
  Delay_ms(100);
  
  lcd_init();
  Delay_ms(100);
  
  lcd_full_clear();

  while(1)
  {
    lcd_clear_framebuffer();
    
    display_draw_input_handler();
    display_draw_menu_handler();
    
    current_key_state = keys_polling();
    process_key_state(current_key_state);
    
    battery_voltage = measure_battery_voltage();
    
    if (mode_state == FORMULA_INPUT)//debug
    {
      draw_status_line();
      //delay_ms(1000);
      /*
      sprintf(str,"CNT: %d\n", cnt);
      lcd_draw_string(str, 0, FONT_SIZE_8*4, FONT_SIZE_8, 0);
      
      sprintf(str,"test: %d", tmp_cnt);
      lcd_draw_string(str, 0, FONT_SIZE_8*6, FONT_SIZE_8, 0);
      
      lcd_gotoxy(0,4);
      sprintf(str,"key: %d    ", key_state);
      lcd_draw_string(str, 0, FONT_SIZE_8*7, FONT_SIZE_8, 0);
      */
    }
    
    lcd_update();
    
   // go_to_sleep_mode();
    
    cnt++;
  }
}

void solve_formula(void)
{
  calc_result = solve((uint8_t*)formula_text, formula_current_length);
}


int putchar(int c)
 {
   // Write a character to the USART

  //Loop until the end of transmission   
  return c;    
}


void Delay_ms(uint32_t ms)
{
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
  for (; nCount!=0; nCount--);
}

