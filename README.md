# module_AccidentDetection
Module for monitoring, analysis and decision making about accident detection.

Module returns the following codes based on the event:

200 - Everything ok!

201 - Back distance sensor reacted. There is an object in the back. 

202 - Front distance sensor reacted. There is an object in the front.

203 - Both distance sensor reacted. There are objects in the front and back.

204 - Gyroscope reacted, but there are no objects around the vehicle.

205 - Gyroscope + Back distance sensor = Hit from back.

206 - Gyroscope + Front distance sensor = Hit in front.

207 - Gyroscope + Both sensors = Hit while surrounded.
