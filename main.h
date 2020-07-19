
#ifndef MAIN_H
#define MAIN_H
                       
          
//LCD module connections             
#define LCD_RS_PIN      PIN_D0                               
#define LCD_RW_PIN      PIN_D1
#define LCD_ENABLE_PIN  PIN_D2                  
#define LCD_DATA4       PIN_D3                                                                            
#define LCD_DATA5       PIN_D4
#define LCD_DATA6       PIN_D5                                      
#define LCD_DATA7       PIN_D6                                 
//End LCD module connections   
               
               
#include <16F877A.h>                         
#fuses HS,NOWDT,NOPROTECT,NOLVP                       
#use delay(clock = 8MHz)                    
#include <lcd.c>

                                                         
//#FUSES FRC                                
                                                                                                                                
//#use delay(restart_wdt)                                                                  
                                       
                                  
                                
//#include <lcd.c>                                                        
                              
                                                                 
#define DS18B20_PIN PIN_B1                         // DS18B20 Data pin is connected to pin RB1
#define DS18B20_PIN PIN_B1                         // DS18B20 Data pin is connected to pin RB1
#define DS18B20_PIN1 PIN_B2                         // DS18B20 Data pin is connected to pin RB1
                          
#define Down                        PIN_B4                      // Temperature down button
#define Up                          PIN_B3                     // Temperature up button                                     
#define PWR                         PIN_B5                    // PWR pin to turn on a water pump                                
#define LCD_BACKLIGHT_ON            PIN_D7                   // Pin to turn on LCD backlight   
                                                                        

 
//Min and Max set temperatures
float min = 0;
float max = 100;                                  

 
signed int16 raw_temp;
signed int16 raw_temp1;    
                                                                             
float ButtonCounter = 10;                    // Starting point temperature set                    
                                           
float temp;                                          
float temp1;
float Old_temp;
int On_flag;   
int i;                                                                           
                              
                          
// DS18B20 sensor functions for full run                                                       
int1 ds18b20_start(){                             
  output_low(DS18B20_PIN);                         // Send reset pulse to the DS18B20 sensor
  output_drive(DS18B20_PIN);                       // Configure DS18B20_PIN pin as output
  delay_us(500);                                   // Wait 500 us 
  output_float(DS18B20_PIN);                       // Configure DS18B20_PIN pin as input
  delay_us(100);                                   //wait to read the DS18B20 sensor response
  if (!input(DS18B20_PIN)) {                                                                 
    delay_us(400);                                 // Wait 400 us  
    return TRUE;                                   // DS18B20 sensor is present
  }                              
  return FALSE; 
}                                      
                                                      
                                                                                                               
void ds18b20_write_bit(int1 value){                 
  output_low(DS18B20_PIN);                                                                                                                       
  output_drive(DS18B20_PIN);                       // Configure DS18B20_PIN pin as output
  delay_us(2);                                     // Wait 2 us
  output_bit(DS18B20_PIN, value);                        
  delay_us(80);                                    // Wait 80 us
  output_float(DS18B20_PIN);                       // Configure DS18B20_PIN pin as input
  delay_us(2);                                     // Wait 2 us                                                                     
}                     
                                                
 
void ds18b20_write_byte(int8 value){               
  int8 i;                
  for(i = 0; i < 8; i++)
    ds18b20_write_bit(bit_test(value, i));
}
        
                                                         
int1 ds18b20_read_bit(void) {                          
  int1 value;       
  output_low(DS18B20_PIN);
  output_drive(DS18B20_PIN);                       // Configure DS18B20_PIN pin as output
  delay_us(2);                                                                            
  output_float(DS18B20_PIN);                       // Configure DS18B20_PIN pin as input
  delay_us(5);                                     // Wait 5 us
  value = input(DS18B20_PIN);
  delay_us(100);                                   // Wait 100 us
  return value;           
}
                          
                        
int8 ds18b20_read_byte(void) {
  int8 i, value = 0;
  for(i = 0; i  < 8; i++)
    shift_right(&value, 1, ds18b20_read_bit());
  return value;                                                    
}                   
  
 
int1 ds18b20_read(int16 *raw_temp_value) {
  if (!ds18b20_start())                              // Send start pulse
    return FALSE;
  ds18b20_write_byte(0xCC);                          // Send skip ROM command
  ds18b20_write_byte(0x44);                          // Send start conversion command
  while(ds18b20_read_byte() == 0);                   // Wait for conversion complete
  if (!ds18b20_start())                              // Send start pulse
    return FALSE;                                    // Return 0 if error
  ds18b20_write_byte(0xCC);                          // Send skip ROM command
  ds18b20_write_byte(0xBE);                          // Send read command
  *raw_temp_value = ds18b20_read_byte();             // Read temperature LSB byte and store it on raw_temp_value LSB byte
  *raw_temp_value |= (int16)(ds18b20_read_byte()) << 8;     // Read temperature MSB byte and store it on raw_temp_value MSB byte
  return TRUE;                                              // OK --> return 1
}         
      
      
     int1 ds18b20_start1(){
  output_low(DS18B20_PIN1);                        // Send reset pulse to the DS18B20 sensor
  output_drive(DS18B20_PIN1);                      // Configure DS18B20_PIN pin as output
  delay_us(500);                                   // Wait 500 us
  output_float(DS18B20_PIN1);                      // Configure DS18B20_PIN pin as input
  delay_us(100);                                   // wait to read the DS18B20 sensor response
  if (!input(DS18B20_PIN1)) {                                                                           
    delay_us(400);                                 // Wait 400 us  
                      
    return TRUE;                                   // DS18B20 sensor is present
  }           
    output_high(DS18B20_PIN1);    
    return FALSE; 
}                                                     
       
     
void ds18b20_write_bit1(int1 value1){
  output_low(DS18B20_PIN1);
  output_drive(DS18B20_PIN1);                       // Configure DS18B20_PIN pin as output
  delay_us(2);                                      // Wait 2 us
  output_bit(DS18B20_PIN1, value1);
  delay_us(80);                                     // Wait 80 us
  output_float(DS18B20_PIN1);                       // Configure DS18B20_PIN pin as input
  delay_us(2);                                      // Wait 2 us
}                            
                 

void ds18b20_write_byte1(int8 value1){
  int8 a;
  for(a = 0; a < 8; a++)                                   
    ds18b20_write_bit1(bit_test(value1, a));
}


int1 ds18b20_read_bit1(void) {                       
  int1 value1;
  output_low(DS18B20_PIN1);
  output_drive(DS18B20_PIN1);                       // Configure DS18B20_PIN pin as output
  delay_us(2);
  output_float(DS18B20_PIN1);                       // Configure DS18B20_PIN pin as input                 
  delay_us(5);                                      // Wait 5 us
  value1 = input(DS18B20_PIN1);
  delay_us(100);                                    // Wait 100 us
  return value1;      
}                           
                         
                   
int8 ds18b20_read_byte1(void) {       
  int8 a, value1 = 0;                             
  for(a = 0; a  < 8; a++)
    shift_right(&value1, 1, ds18b20_read_bit1());
  return value1;
}


int1 ds18b20_read1(int16 *raw_temp_value1) {
  if (!ds18b20_start1())                                   // Send start pulse         
  return FALSE; 
  ds18b20_write_byte1(0xCC);                               // Send skip ROM command
  ds18b20_write_byte1(0x44);                               // Send start conversion command
  while(ds18b20_read_byte1() == 0);                        // Wait for conversion complete
  if (!ds18b20_start1()) {                                  // Send start pulse     
       return FALSE;                                          // Return 0 if error
  }                                     
  ds18b20_write_byte1(0xCC);                               // Send skip ROM command
  ds18b20_write_byte1(0xBE);                               // Send read command
  *raw_temp_value1 = ds18b20_read_byte1();                 // Read temperature LSB byte and store it on raw_temp_value LSB byte
  *raw_temp_value1 |= (int16)(ds18b20_read_byte1()) << 8;     // Read temperature MSB byte and store it on raw_temp_value MSB byte
  return TRUE;                                                // OK --> return 1
}                               


void Show_User_Info(){
     
  // Print Set:
     lcd_gotoxy(10, 1);                                        
     lcd_putc("Set: ");
     lcd_gotoxy(14, 1);
     printf(lcd_putc, "%3.0f",ButtonCounter);   
     
  
  // Print Pwr:                                
     lcd_gotoxy(10, 2);                 
     lcd_putc("Pwr:");                            
                                                 
  // Print V:                             
      lcd_gotoxy(1, 1);                          
      printf(lcd_putc, "V: ");
   
  // Print A:                           
      lcd_gotoxy(1, 2);    
      printf(lcd_putc, "A: ");
}                                       

 
void User_Buttons(){

 if(input(pin_B0) == 0) {             
  
          //lcd_init();                          
         // lcd_gotoxy(14,1);
          lcd_gotoxy(6,2);   
          ButtonCounter = ButtonCounter+5;                                                                                                
          printf(lcd_putc, "%3.0f",ButtonCounter);    
         // output_high(PIN_D7);                     
         // delay_ms(2000);             
                                  
    if (ButtonCounter == max){   
            ButtonCounter = 10;                           
      }                           
 }                                          
                                                                                                                                                                                                                                                                                                                                                                                     
if(input(pin_B4) == 0) {          
                                      
         // lcd_gotoxy(14,1);
          lcd_gotoxy(6,2); 
          ButtonCounter = ButtonCounter-5;                                         
          printf(lcd_putc, "%3.0f",ButtonCounter);
         // output_high(PIN_D7);                      
         // delay_ms(2000);                    
   /*                              
     if (ButtonCounter <= min){                  
            ButtonCounter = 10;               
      }
      */
   } 
   /*
     if(ButtonCounter == 10 || ButtonCounter == 0){   
           output_low(pin_B5);                  
          // delay_ms(5000);            
           lcd_gotoxy(14, 2);                                  
           printf(lcd_putc, "Off ");      
    }                                          
    */
}                              

                                                            
void Compare_Upper_Temp(){

// Reset pump on flag          
On_flag == 0;
                      
       
if (temp1 >= ButtonCounter){          
        output_high(pin_B5);                   
       // delay_ms(5000);                           
        lcd_gotoxy(14, 2);            
        printf(lcd_putc, "On ");                          
        On_flag == 1;                       
    }     
                                      
      if (ButtonCounter <= min){                  
            //ButtonCounter = 10;                        
            ButtonCounter = 5;   
      }                                            
                        
      if(ButtonCounter == 10 || ButtonCounter == 5 || ButtonCounter == 0){    
           output_low(pin_B5);                  
          // delay_ms(5000);             
           lcd_gotoxy(14, 2);                               
           printf(lcd_putc, "Off ");                                           
           On_flag == 0;                                     
    }
}                                                
            
void Bottom_Waiting_Animation(){
        
 delay_ms(50);       
 lcd_gotoxy(4, 2);                      
 printf(lcd_putc, ".");                               
 lcd_gotoxy(5, 2);                      
 printf(lcd_putc, ".");                      
                                                                         
                                                                          
 delay_ms(50);                              
 lcd_gotoxy(4, 2);      
 printf(lcd_putc, "");            
 lcd_gotoxy(5, 2);      
 printf(lcd_putc, "");                          
}   


void Read_Sensors(){
 
 // Starting to read temperatures from the sensors                                                             
    if(ds18b20_read(&raw_temp)) {
      temp == Old_temp;
      temp = (float)raw_temp / 16;               // Convert temperature raw value into degree Celsius (temp in ?C = raw/16) 
   //   lcd_gotoxy(1, 1);                          // Go to column 5 row 2
   //   printf(lcd_putc, "V: ");                                                          
      lcd_gotoxy(3, 2);            
      printf(lcd_putc, "%3.0f", temp);                                                                                                          
      lcd_putc(223);                             // Print degree symbol ( ? )
      lcd_putc("C; ");                            // Print 'C '
      //lcd_gotoxy(10, 1);    
      //lcd_putc("Set:  ");                            // Print 'C '         
    }                            
                                                                             
                                                                                                               
     if(ds18b20_read1(&raw_temp1)) {                                    
      temp1 = (float)raw_temp1 / 16;               // Convert temperature raw value into degree Celsius (temp in ?C = raw/16)   
     // lcd_gotoxy(1, 2);  
     // printf(lcd_putc, "A: ");// Go to column 5 row 2
      lcd_gotoxy(3, 1);                                      
      printf(lcd_putc, "%3.0f", temp1);
      lcd_putc(223);                             // Print degree symbol ( ? )
      lcd_putc("C; ");                            // Print 'C '                                   
                                                     
     // lcd_gotoxy(10, 2);                           
    //  lcd_putc("Pwr:  ");                            // Print 'C '                                                                                                                                    
     }                                           
        
     else    
   {              
                          
    //  An error "handler" is generated if V sensor is not present       
     // output_high(PIN_D7);
      lcd_putc('\f');                             // LCD clear
      lcd_gotoxy(5, 1);                          // Go to column 5 row 2
      printf(lcd_putc, "Davikliu");          
      lcd_gotoxy(5, 2);                                                 
      printf(lcd_putc, "klaida!");                         
      delay_ms(3000);                                           
      reset_cpu();        // Restart CPU    
   }                            
                          
    /*                                                                                                                     
       // In case of total CPU failure
    else {                                                         
      lcd_putc('\f');                             // LCD clear
      lcd_gotoxy(5, 2);                          // Go to column 5 row 2
      printf(lcd_putc, " Klaida! ");
      delay_ms(3000);                                           
      reset_cpu();        // Restart CPU    
    }                       
 */                                                                        

                                         
}                             


void User_Set_Mode(){                              
     
    // lcd_putc('\f');                             // LCD clear
     lcd_gotoxy(3, 1);                          // Go to column 5 row 2
     printf(lcd_putc, "Nustatykite: ");                     
     User_Buttons();
   //  delay_ms(200); 
     lcd_gotoxy(9, 2);                          // Go to column 5 row 2
     lcd_putc(223);                             // Print degree symbol ( ? )
     lcd_putc("C");                            // Print 'C '
     
   /*  
     lcd_gotoxy(8, 2);                          // Go to column 5 row 2
     printf(lcd_putc, "__");
     
     delay_ms(500);                
     lcd_gotoxy(8, 2);                          // Go to column 5 row 2
     printf(lcd_putc, "  ");
     
     delay_ms(500);
     lcd_gotoxy(8, 2);                          // Go to column 5 row 2
     printf(lcd_putc, "__");
  */   
     
}                                                   
                                                           
                              
#endif // MAIN_H                                     
                  
 
