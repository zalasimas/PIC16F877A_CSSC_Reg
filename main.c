
// by Simas Za             

                                                                                                                                          
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
#device *=16
#fuses HS,NOWDT,NOPROTECT,NOLVP                       
#use delay(crystal = 8000000)                        // using external oscillator 
#use delay(restart_wdt)                                                            
#use delay(RESTART_WDT) 
                      
                      
#include <lcd.c>                                                        

#define DS18B20_PIN PIN_B1                         // DS18B20 Data pin is connected to pin RB1
#define DS18B20_PIN1 PIN_B2                         // DS18B20 Data pin is connected to pin RB1

#define Down            PIN_B4                      // Temperature down button
#define Up              PIN_B0                      // Temperature up button
#define Set             PIN_B3                     // Temperature set mode button                                   
#define PWR             PIN_B5                     // PWR pin to turn on a transistor
                                                                        
               
//#INT_EXT
//#define Temperature_Up      pin_B4
//#define Temperature_Down    pin_B5

// Atkomentavus situos kintamuosius, kazkodel pradeda neveikti interrupt ir taip pat gerokai sumazeja ROM vieta.
// When these variables are uncommented, the interrupt service routine begins to fail for some reason. It also reduces ROM space.
                                                 
//signed int16 raw_temp;
//signed int16 raw_temp1;
//float temp;
//float temp1;
//float ButtonCounter;

/*
void ext_isr(void)
{
delay_ms(20);

//float ButtonCounter;

if(input(pin_B0) == 0 & input(pin_B4) == 1){

 //lcd_init();            
 clear_interrupt(INT_EXT);       // Clear external interrupt flag bit
 
 lcd_gotoxy(14,1);
 ButtonCounter = ButtonCounter+10;
 printf(lcd_putc, "%3.0f",ButtonCounter);
  
 }
if(input(pin_B0) == 0 & input(pin_B4) == 0){         

 clear_interrupt(INT_EXT);       // Clear external interrupt flag bit
 lcd_gotoxy(14,1);
 ButtonCounter = ButtonCounter-10;
 printf(lcd_putc, "%3.0f",ButtonCounter);
 }
  if (ButtonCounter == 100){
  ButtonCounter=0;
 }
 if (ButtonCounter == -10 || ButtonCounter == 0){
   ButtonCounter=10;
 }
}
*/    
          
// DS18B20 sensor functions to full run                                                       
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

int1 ds18b20_start1(){
  output_low(DS18B20_PIN1);                        // Send reset pulse to the DS18B20 sensor
  output_drive(DS18B20_PIN1);                      // Configure DS18B20_PIN pin as output
  delay_us(500);                                   // Wait 500 us
  output_float(DS18B20_PIN1);                      // Configure DS18B20_PIN pin as input
  delay_us(100);                                   //wait to read the DS18B20 sensor response
  if (!input(DS18B20_PIN1)) {
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

void ds18b20_write_bit1(int1 value1){
  output_low(DS18B20_PIN1);
  output_drive(DS18B20_PIN1);                       // Configure DS18B20_PIN pin as output
  delay_us(2);                                      // Wait 2 us
  output_bit(DS18B20_PIN1, value1);
  delay_us(80);                                     // Wait 80 us
  output_float(DS18B20_PIN1);                       // Configure DS18B20_PIN pin as input
  delay_us(2);                                      // Wait 2 us
}
 
void ds18b20_write_byte(int8 value){               
  int8 i;
  for(i = 0; i < 8; i++)
    ds18b20_write_bit(bit_test(value, i));
}
 
 void ds18b20_write_byte1(int8 value1){
  int8 i;
  for(i = 0; i < 8; i++)
    ds18b20_write_bit1(bit_test(value1, i));
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
 
int8 ds18b20_read_byte(void) {
  int8 i, value = 0;
  for(i = 0; i  < 8; i++)
    shift_right(&value, 1, ds18b20_read_bit());
  return value;
}

int8 ds18b20_read_byte1(void) {
  int8 i, value1 = 0;
  for(i = 0; i  < 8; i++)
    shift_right(&value1, 1, ds18b20_read_bit1());
  return value1;
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

int1 ds18b20_read1(int16 *raw_temp_value1) {
  if (!ds18b20_start1())                                   // Send start pulse
    return FALSE;
  ds18b20_write_byte1(0xCC);                               // Send skip ROM command
  ds18b20_write_byte1(0x44);                               // Send start conversion command
  while(ds18b20_read_byte1() == 0);                        // Wait for conversion complete
  if (!ds18b20_start1())                                   // Send start pulse
    return FALSE;                                          // Return 0 if error
  ds18b20_write_byte1(0xCC);                               // Send skip ROM command
  ds18b20_write_byte1(0xBE);                               // Send read command
  *raw_temp_value1 = ds18b20_read_byte1();                 // Read temperature LSB byte and store it on raw_temp_value LSB byte
  *raw_temp_value1 |= (int16)(ds18b20_read_byte1()) << 8;     // Read temperature MSB byte and store it on raw_temp_value MSB byte
  return TRUE;                                                // OK --> return 1
}                               

//float ButtonCounter;          

void main() {
                       
//float ButtonCounter;
signed int16 raw_temp;
signed int16 raw_temp1;
 
float ButtonCounter = 0;  
float ButtonCounterDifference; 
float temp;                     
float temp1;

 lcd_init();                                    // Initialize LCD module
 // lcd_putc('\f');                                // Clear LCD
  //lcd_gotoxy(3, 1);                              // Go to column 3 row 1
  //set_tris_b(0xF0);
  //float ButtonCounter = 0;
 // printf(lcd_putc, "Temperature:");
   //output_low(PIN_C0);
 // ext_int_edge(H_TO_L);           // External interrupt edge from high to low
   //clear_interrupt(INT_EXT);       // Clear external interrupt flag bit
  // enable_interrupts(INT_EXT);     // Enable external interrupt
   //enable_interrupts(GLOBAL);      // Enable global interrupts
                                                                    
    // lcd_putc("\f");                            // Print 'C '
     lcd_gotoxy(11, 1);                // Print 'C '
     lcd_putc("Set:  ");                       // Print 'C '          
                                              
      lcd_gotoxy(11, 2);       
      lcd_putc("Pwr:  ");                            // Print 'C '
                                
      lcd_gotoxy(1, 1);                          // Go to column 5 row 2
      printf(lcd_putc, "V: ");      
                                                                     
      lcd_gotoxy(1, 2);  
      printf(lcd_putc, "A: ");// Go to column 5 row 2

 //ButtonCounterUpdated = ButtonCounter;  
                              
     while(TRUE){   
  // reset_cpu();             
        restart_wdt();
                                                                              
  float Bottom_Temp;   
  float Temperature_Treshold = 20;                
  float Temperature_Difference;                                           
                          
    if(ds18b20_read(&raw_temp)) {
      temp = (float)raw_temp / 16;               // Convert temperature raw value into degree Celsius (temp in �C = raw/16)
   //   lcd_gotoxy(1, 1);                          // Go to column 5 row 2
   //   printf(lcd_putc, "V: ");
      lcd_gotoxy(3, 1);      
      printf(lcd_putc, "%3.1f", temp);
      lcd_putc(223);                             // Print degree symbol ( � )
      lcd_putc("C; ");                            // Print 'C '
      //lcd_gotoxy(10, 1);    
      //lcd_putc("Set:  ");                            // Print 'C '  
    }
    
     if(ds18b20_read1(&raw_temp1)) {
      temp1 = (float)raw_temp1 / 16;               // Convert temperature raw value into degree Celsius (temp in �C = raw/16)
     // lcd_gotoxy(1, 2);  
     // printf(lcd_putc, "A: ");// Go to column 5 row 2
      lcd_gotoxy(3, 2);                                      
      printf(lcd_putc, "%3.1f", temp1);
      lcd_putc(223);                             // Print degree symbol ( � )
      lcd_putc("C; ");                            // Print 'C ' 
      
     // lcd_gotoxy(10, 2);  
    //  lcd_putc("Pwr:  ");                            // Print 'C '                                                     
    }                            
                                                         
    // In case of total CPU failure
    else {               
      lcd_putc('\f');                             // LCD clear
      lcd_gotoxy(5, 2);                          // Go to column 5 row 2
      printf(lcd_putc, " Klaida! ");
      delay_ms(3000);
      reset_cpu();        // Restart CPU  
    }                                                                               
      
    if(input(pin_B0) == 0) {                                            

 //lcd_init();                    
  
 lcd_gotoxy(14,1);
 ButtonCounter = ButtonCounter+10;
 printf(lcd_putc, "%2.0f",ButtonCounter);
                                      
 }                                                 
            
if(input(pin_B4) == 0) {         

 lcd_gotoxy(14,1);
 ButtonCounter = ButtonCounter-10;             
 printf(lcd_putc, "%2.0f",ButtonCounter);
 }
  if (ButtonCounter == 100){      
  ButtonCounter=0;                                                                      
 }
 if (ButtonCounter == -10 || ButtonCounter == 0){
   ButtonCounter=10;
 }                                              
 
 // ButtonCounterDifference = ButtonCounter - 10;        
  
    if (temp == ButtonCounter){          
     output_high(pin_B5) == 1;                   
    // delay_ms(5000);  
    }                                                                                      
    /*                                                                                  
    else{                               
    output_low(pin_B5) == 0;    
    }                                                                     
   /*                                                                                                                                  
    if (temp1 <= Temperature_Treshold)                  
    {                                     
    output_low(pin_B5) == 0;                                
    }                                     
     else{                                                                     
    output_high(pin_B5) == 1;                                
    }                                                                  
    */                                                              
    if (temp == temp1)            // Comparing two temperatures together. If upper temperature have reached set temperature,                            
    {                             // the water pump is switched on. After that, upper temperature is falling and bottom temperature                                                                           
       output_low(pin_B5) == 0;   // increasing. So, theoretically this two temperatures must face together.                                     
    }  
    delay_ms(1000);
 }                                // If this theoretical model will be wrong, then will need to set reference temperature
}                                 // (about 35 C). And compare bottom temperature with this reference. If bottom temperature will
                                  // reach reference - the water pump will be switched off. One more thing - reference temperature
                                  // must be set under real device working conditions.   