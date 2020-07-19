            
  /* 
  Dual stage solar heater controller v1.1 Beta    
  
   
  Comparing two temperatures together. If upper temperature have reached set temperature,                            
  the water pump is switched on. After that, upper temperature is falling and bottom temperature                                                                           
  increasing. So, theoretically this two temperatures must face together.     
    
   If this theoretical model will be wrong, then will need to set reference temperature
   (about 35 C). And compare bottom temperature with this reference. If bottom temperature will
   reach reference - the water pump will be switched off. One more thing - reference temperature
   must be set under real device working conditions.
   
   by Simas Za 
   
  =============                                      
   
   Versions: 
   
   V1.0:
   * Working upper temperature pump control;                                
   * Working user buttons (because of lack of separation of the measurement process buttons response is very slowly);
   * Working LCD backlight according to user button presses;           
   * Increasing and decreasing set temperature every 10 degrees celsius.   
              
   V1.1(current - need to test in real working conditions):
   * Working upper and bottom temperature pump control. The temperatures of each stage can response to its threshold;
   * Added user set temperature mode after turning on the device. For this reason buttons became convenient and fast response;       
   * Removed LCD backlight manipulation according to the button presses (now backlight running continuously);
   * Added debounce in some "warning" places.
   
   Possible issues in beta version
      need to pay attention to:
      * Sensors placement (Thermal isolation, contact between sensor and contact substance);
      * Both sensors response time (how quickly they respond to heat). Need to avoid temperature coincidence above threshold temperatures.   
                                                       
   */                                                                                       
                                                                         
                                                                     
                                                                     
#include "main.h"                                                                
                                    
// "Flag", indicating that the pump is switched on    
//extern int On_flag;                                                                                                                                                  
                                                  
void main() {                         

//Turn on LCD backlight                    
output_high(PIN_D7);

// Initialize LCD module
lcd_init();
                        
                                
// Keep the pump off, until the user doesn't set the temperature!!!                     
output_low(PIN_B5);           
                                                      
float Min_Threshold_Value = 41;       
                                             
                                
// Start doing user temperature set mode routine.                                                                                
do{                                                      
 User_Set_Mode();                                 
 delay_ms(300);                  // Debounce feature for the buttons
}                                                                        
                                                                      
while(input(pin_B0) == 1 || input(pin_B4) == 1);                                         
                                                                     
break;
continue;
   
Show_User_Info();                                                
delay_ms(10);                                    
                                                          
lcd_gotoxy(14, 2);                                  
printf(lcd_putc, "Off ");                                                            
 
                                    
while(TRUE){                                                                                 
  //Turn on LCD backlight                              
  //output_high(PIN_D7);   

                                            
  Read_Sensors();              


// Starting to read user button values 
//User_Buttons();    

//delay_ms(20);               // Minimum amount of time to read user button  values    


// Starting to compare user set temperature value and upper sensor temperature  read value.                                                                                              
//Compare_Upper_Temp();   
//delay_ms(20);               // Minimum amount of time to  compare user set temperature value and upper sensor temperature  read value.                                                               
                                 
 
       

                                                                                                                                                                    
//================================     
                                                                                                                                                                                          
// If the pump is switched on, we starting to make actions according to the bottom temperature value.                                                                                              
         
      // Checking, if the MCU pin connected to pump is high. If yes - do the bottom animation                                            
       // if(input(PIN_B5)){       
     // Gal whailui reikia didesnio delay tarp komandu vykdymo, nes ne visada issijungia siurblys...? 
          while(temp < Min_Threshold_Value){           
            // Starting to compare user set temperature value and upper sensor temperature  read value.                                                                                              
            Compare_Upper_Temp();
            delay_ms(500);  
            Read_Sensors();                                           
}
          
          while(temp > Min_Threshold_Value){     
                                                                                                         
           Read_Sensors();          
                                            
          }                                                                                             
         
            delay_ms(1000);
            Read_Sensors();                                                          
         
         if(temp < Min_Threshold_Value){
                                                   
            output_low(pin_B5);                  // Switch pump off                    
            // delay_ms(5000);                          
            lcd_gotoxy(14, 2);                                  
            printf(lcd_putc, "Off ");                        
                                                    
}                                                               
        //    {                                       
         //   output_high(pin_B5);                 // Switch pump on             
           // Bottom_Waiting_Animation();
         //   delay_ms(2000);                                  
           // Read_Sensors();                 
  /*                                   
            if(temp < Min_Threshold_Value){                          
                                           
             output_low(pin_B5);                  // Switch pump off                    
            // delay_ms(5000);                          
            lcd_gotoxy(14, 2);                                  
            printf(lcd_putc, "Off ");                                                        
            }                 
  }                         
*/                                             
         //   }                                       
           // break;                
          //  continue;                     
                        
      /*                                     
        if(input(PIN_B5)){                          
                                                                                      
          while(temp < Min_Threshold_Value)                
            {                             
            output_high(pin_B5);                 // Switch pump on                                
            Bottom_Waiting_Animation(); 
            }                         
           // break; 
           // continue;         
}                         
                                                                              
   */  
   /*
 // Control and last one check: If the set temp is less than threshold - turn the pump off.
 if(temp < Min_Threshold_Value){
        
      input(PIN_B5) == 0;              
}                                           
     
                                                        
      /*                                  
            output_low(pin_B5);                  // Switch pump off                    
            // delay_ms(5000);                          
            lcd_gotoxy(14, 2);                                 
            printf(lcd_putc, "Off ");        
        */                                     
                                                                        
     // Check, if bottom temperature is higher than minimal threshold value. If no - turn the pump off.
     /*
        if(temp < 40){                    
                            
              output_low(pin_B5);                 // Switch pump off    
            // delay_ms(5000);                          
             lcd_gotoxy(14, 2);                                 
             printf(lcd_putc, "Off ");        
                                                                          
         } 
         */                                                          
                                      
     
       /*                                            
          if(temp1 < 25){ 
                
              output_low(pin_B5);                     
            // delay_ms(5000);                          
             lcd_gotoxy(14, 2);                                 
             printf(lcd_putc, "Off ");        
                                        
             }            
             */                                       
            
 // reset_cpu(); 
                          
       }                                                                                                                                                                                  
}
 
    // End of code
                                                                                                 
