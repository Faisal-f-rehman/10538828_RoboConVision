# **ROCO318 Coursework - RoboCon (Vision)**
### Vision system for Connect 4

<img src="https://github.com/Faisal-f-rehman/10538828_RoboConVision/blob/master/src/docs/connect4header.jpeg?raw=true" height="100%" width="100%"/>
(Ref 1 - Connect 4 Picture)

<br> <br>

-----------------------------------
## SECTION 1 - SYSTEM USED FOR DEVELOPMENT
-----------------------------------
**HARDWARE**
1) Laptop
2) Logitech USB camera
3) Connect 4 game

**SOFTWARE**
1) Linux Ubuntu 16.04 LTS
2) Standard C++ library
3) OpenCV 2.4.9
4) Python 2
5) PyQt4
6) Qt Designer
7) Gnome-terminal


<br> <br>

-----------------------------------
## SECTION 2 - SETUP
-----------------------------------

+ Pull repository 
```shell
$ git pull https://github.com/Faisal-f-rehman/10538828_RoboConVision.git
```

+ Change directory to c++ vision scripts, in RoboConVision folder
```shell
$ cd ~/..../10538828_RoboConVision-master/src/scripts/RoboConVision
```

+ Generate build files with Cmake
```shell
$ cmake .
```

+ Build files with make
```shell
$ make
```

+ Change directory back to where RoboCon.sh is located
```shell
$ cd ..
```

+ Execute bash script
```shell
$ ./RoboCon.sh
```

<br> <br>

-----------------------------------
## SECTION 3 - USING THE SOFTWARE
-----------------------------------
At startup the program will initiate a terminal and a GUI "MainWindow". Follow the instructions on the terminal and enter the ID number of your camera and hit enter. If the camera initiated successfully the terminal will start printing "complete" and "free" for guiState and commsState respectively. If the initiation was unsuccessful then please check the camera ID and try again.

<p align="center">
<img src="https://github.com/Faisal-f-rehman/10538828_RoboConVision/blob/master/src/docs/MainWindow_GUI.png?raw=true" height="30%" width="30%"/>
</p>

<br>

The GUI interface has four options, New Game, Align Camera, Calibrate Camera and Exit Game. 

+ New Game, starts the main sequence of the program where it detects the colour and position of the disks and draws them on the relevant images.

<br>

+ Aligning the camera option, helps aligning the game board with the camera frame. This is a necessary process and is achieved by placing and moveing the game board and camera until all circles of the board fits within the grid boxes. 

<br>

<p align="center">
<img src="https://github.com/Faisal-f-rehman/10538828_RoboConVision/blob/master/src/docs/RoboConVisionAlign.png?raw=true" height="50%" width="50%"/>
</p>

<br>

+ Calibrate camera fine tunes colour detection for the light in the room. To do so it simply iterates through all HSV values until the relevant disks AND empty slots have been located in the expected locations on the board. To calibrate the camera please slot the disks in the board as follows:

<p align="center">
<img src="https://github.com/Faisal-f-rehman/10538828_RoboConVision/blob/master/src/docs/CalibrationDiskLocations.jpg?raw=true" height="50%" width="50%"/>
</p>

<br>

                                          Calibration Video Demonstration
The following video shows camera performance without calibration first and then the improvement in the performance of the camera after calibration.                                             
                                          
[![](https://github.com/Faisal-f-rehman/10538828_RoboConVision/blob/master/src/docs/calibrateCameraDayAndYellowLight.png?raw=true)](https://youtu.be/wggh8jGW_Kw)

**Note: Before calibrating the camera please make sure the board is properly aligned with the camera**    

<br>

+ Exit Game option only becomes available when there are no other sequences in progress or running. Pressing this button will close the GUI window and game terminal.

<br> <br>

-----------------------------------
## SECTION 4 - TESTING
-----------------------------------
I ran tests on the system in 3 different lights and locations. This was done to find the effects on the colour detection performance of the system in different lights. It was observed that unless the light was extremely dim / dark or extremely bright, after calibrating the camera the results were consistent and satisfactory.


                      Light / Camera Calibration Test at Night with YELLOW LIGHT ONLY Video
                               
[![](https://github.com/Faisal-f-rehman/10538828_RoboConVision/blob/master/src/docs/CalibrationYoutubePic.png?raw=true)](https://youtu.be/lfmS6HFSoqU)

<br>

                            Light / Camera Calibration Test in DAY LIGHT ONLY Video
                               
[![](https://github.com/Faisal-f-rehman/10538828_RoboConVision/blob/master/src/docs/calibrateCameraDaylightOnly.png?raw=true)](https://youtu.be/0h1RfLp7IB8)                               
                               
<br>

                          Light / Camera Calibration Test in DAY AND YELLOW LIGHT Video

[![](https://github.com/Faisal-f-rehman/10538828_RoboConVision/blob/master/src/docs/calibrateCameraDayAndYellowLight.png?raw=true)](https://youtu.be/wggh8jGW_Kw)

<br> <br>

-----------------------------------
## SECTION 5 - REFERENCE
-----------------------------------

### Ref 1: Microsoft Website
https://store-images.s-microsoft.com/image/apps.41929.14246448385562777.7795b656-4cc6-43e1-897f-ecca74691263.ce6424bf-ec09-434a-9a0d-6d9507621cfa?mode=scale&q=90&h=1080&w=1920

### Ref 2: https://docs.opencv.org/2.4/
The link above was used as the main source for OpenCV research, reading and learning for this project.
