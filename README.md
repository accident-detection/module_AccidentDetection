# module_AccidentDetection
Module for monitoring, analysis and decision making about accident detection. Uses two ultrasonic sensors, front and back, to detect if an object is too close. That in combination with the Gyroscope determines if there has been an accident. The gyroscope sensitivity can be adjusted.
<a href="http://imgur.com/Sn8uJyG"><img src="http://i.imgur.com/Sn8uJyG.png" title="source: imgur.com" /></a>
Module returns the following codes based on the event:

### AccidentDetection related codes (2xx)

* 200 - OK
* 201 - Back distance sensor reacted. There is an object in the back. 
* 202 - Front distance sensor reacted. There is an object in the front.
* 203 - Both distance sensor reacted. There are objects in the front and back.
* 204 - Gyroscope reacted, but there are no objects around the vehicle.
* 205 - Gyroscope + Back distance sensor = Hit from back.
* 206 - Gyroscope + Front distance sensor = Hit in front.
* 207 - Gyroscope + Both sensors = Hit while surrounded.

### Simple automata picture
<a href="http://imgur.com/ZEctSZr"><img src="http://i.imgur.com/ZEctSZr.png" title="source: imgur.com" /></a>

### Full automata picture
<a href="http://imgur.com/kDyclCL"><img src="http://i.imgur.com/kDyclCL.png?1" title="source: imgur.com" /></a>
