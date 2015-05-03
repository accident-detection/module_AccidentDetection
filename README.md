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
In the simplified version of the automata, following things are not present in the picture:
* Connections between all the states - to make the graph more readable only basic transitions between states were drawn.
* The automata can actually move from any state Q0-Q4 into any other state. This is because all the three parameters (gyroscope, front and back sensor) are polled in a row and the next state is determined according to their values combined.
<a href="http://imgur.com/ZEctSZr"><img src="http://i.imgur.com/ZEctSZr.png" title="source: imgur.com" /></a>

### Full automata picture
<a href="http://imgur.com/kDyclCL"><img src="http://i.imgur.com/kDyclCL.png?1" title="source: imgur.com" /></a>
