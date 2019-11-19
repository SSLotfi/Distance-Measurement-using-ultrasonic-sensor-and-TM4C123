# Distance-Measurement-using-ultrasonic-sensor-and-TM4C123
This project is a distance measuring tool , using an ultrasonic sensor and a Tiva C board with 7-segment displays to display the distance

# How the ultrasonic sensor works
The ultrasonic sensor has 2 main pins the trigger and the echo 	
Trigger pin is an Input pin. This pin has to be kept high for 10us to initialize measurement by sending US wave
Echo pin is an Output pin. This pin goes high for a period of time which will be equal to the time taken for the US wave to return back to the sensor.

# C code explanation
Two timers are intialized to control both the trigger and the echo pins, the first timer is in periodic mode used to set the trigger pin high for 10us , the second timer is in capture mode that captures the moment the echo is received.

The distance is then calculated by subtracting the low and high edges at which the echo arrived and then multiplies this value by the processor frequence and by a constant derived from the speed of sound.

the distance calculated is then divided to hundreds , tens and units and each number is displayed on a 7-segment display.

