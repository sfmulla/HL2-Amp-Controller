This is firmware for the Hermes Lite 2 IO board designed by Saleh Almulla, 7Z1CY. 

  Saleh Almulla <sfmulla@hotmail.com>.


This firmware outputs the band Voltage to control any amplifier with icom ACC

the voltage output using pwm for fan control to J3 pin

   Band                Frq. MHz             Band Voltage

/	160			1.8				7.0 ~ 8.0V

/	80			3.5				6.0 ~ 7.0V

/	40			7				5.0 ~ 6.0V

/	20			14				4.0 ~ 5.0V

/	17&12		18 & 21			3.0 ~ 4.0V

/	12 & 10		24 & 28			2.0 ~ 3.0V

also it output 5.0v to GPIO10_Out5 (J4 pin 5) when PTT on (transsmition on) 
 as PTT output to key the amplifier

if you coneect your I/O board PICO to your PC using USB you can see the working Band
