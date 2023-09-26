# RoboMimicry

## Overview

#### Description

This project was created as a team in Maker Engineering class at Ohio Northern University. This controller utilizes a scaled down version of the main robotic arm to both control and motion map the robot using wireless communication. This allows for a better spatial representation of the robot for the programmar, as well as an ability to program it from farther away.
  
This won the 2023 IEEE ONU Student Branch Most Innovative Award - 1st Place.

#### Pictures

<img src="https://github.com/K-Wince/RoboMimicry/assets/89599862/831b65ad-c910-4d33-b029-a99b9a3fc181" width="25%" height="25%">
<img src="https://github.com/K-Wince/RoboMimicry/assets/89599862/151854e0-c085-42b7-997f-f66b8e880467" width="49%" height="100%">
<img src="https://github.com/K-Wince/RoboMimicry/assets/89599862/75fdbd0a-b8f7-4e7b-b71f-a66475a348a9" width="25%" height="25%">

#### Video

[<img src="https://i.ytimg.com/vi/TtZ6ATR1B-4/maxresdefault.jpg" width="50%">](https://www.youtube.com/watch?v=TtZ6ATR1B-4 "RoboMimicry Demonstration")

## Components

### Main Design

Both a controller and a robotic arm was constructed for this project. They are both controlled by an ESP32 that communicates with eachother wirelessly over the ESP-NOW protocol. This allows for the systems to be completely independent of each other and have a considerable range of communication.  

We utilized potentiometers to measure the angles of each joint of the scaled version in order to see what position each joint is in to send over to the robotic arm.

## Future Improvements

For future iterations of this design, the following improvements could be made:
- Use precise joint angle sensors rather than potentiometers as they add a lot of variability in the mimicry aspect
- Add a bearing on the bottom joint of the robotic arm to reduce frictional load
- Add a sophosticated motion control program so the robotic arm moves at a safe speed
