# PIC16F877A Dual stage solar heater controller v1.1 Beta    
   
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
   
    Need to pay attention to:
      * Sensors placement (Thermal isolation, contact between sensor and contact substance);
      * Both sensors response time (how quickly they respond to heat). Need to avoid temperature coincidence above threshold temperatures.                          
