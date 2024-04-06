# TAPO-camera-ONVIF-RTSP-and-AI-Object-Recognition-C-
TAPO-camera-ONVIF-RTSP-and-AI-Object-Recognition in C++


You can run the compiled program on raspberry Pi 64 bit (e.g. bookworm). The compiled Ubuntu version has been added as well and is called mycMotDetRecUbuntu (it was compiled on 64 bit Ubuntu 23.10)
1. Download mycMotDetRec and mycMotDetRec_config.ini and test_mask.png
   mycMotDetRec means: myc = my camera, Mot = Motion, Det = Detection, Rec = Recording.
2. Open a terminal in the directory where you saved these files.
3. DO NOT FORGET THIS: Edit / Adapt first the settings in mycMotDetRec_config.ini to your needs!
4. Run ./mycMotDetRec
5. You may see a few times this repeated message. Just ignore it: libpng warning: iCCP: known incorrect sRGB profile
6. Use key q to close the program or CTRL-C in the console/terminal

You get the best results on a Tapo camera using stream2 (1280 x 720 pixels) on a Raspberry pi 4 (4GB ram) 64 bit Bookworm.
Tapo Stream1 is 4 times! more pixels (2560x1440) to process and a little too heavy for the pi 4, maybe pi 5 will have no issues.

To compile/Adapt the C++ program.
Download the repository and install the prerequisites.
There are a lot of comments in de C++ cpp program and the mycMotDetRec_config.ini file.
You may compile and change the mycMotDetRec.cpp as you wish.
The compiled version mycMotDetRec works on Raspberry Pi (4) with Bookworm 64 bit.


There are multiple methods to create your own Mask file.
I used for the included test_mask.png the Gimp.
Tip: Read the file  "How to make quickly a Mask file for your camera stream.txt".

In principle this C++ version is almost similar to the Python version.

See the mycMotDetRec_config.ini for comments etc.

Next comments are for development only! If you want to develop further with C++ these instructions might be helpfull.

Put the following in .bashrc in /home/<user>/.bashrc

PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH

Put INIReader.h in same directory as this C++ program mycMotDetRec.cpp
You find INIReader.h at https://github.com/benhoyt/inih/blob/master/cpp/INIReader.h

Put the mak file "test_mask.png" (or your own mask file with any other name like "mask_stream2.png" also in the same directory
INSTALL the necessary compilation libraries for g++ and others
sudo apt update
sudo apt upgrade
sudo apt-get install build-essential cmake pkg-config libjpeg-dev libtiff5-dev libjasper-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libfontconfig1-dev libcairo2-dev libgdk-pixbuf2.0-dev libpango1.0-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran libhdf5-dev libhdf5-serial-dev libhdf5-103 python3-pyqt5 python3-dev -y
sudo apt-get install libopencv-dev python3-opencv
sudo apt-get install libcurl4-gnutls-dev
sudo apt-get install libjsoncpp-dev
sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json
sudo apt-get install libcurl4-openssl-dev
 
Compile now mycMotDetRec.cpp with the following command, assumeing you have configured all necessary libs and installed all dependencies.
 
g++  mycMotDetRec.cpp -o mycMotDetRec   -I/usr/include/opencv4 -I/usr/include -lopencv_videoio -lopencv_video -lopencv_videostab -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_highgui -lopencv_imgcodecs -ljsoncpp  -lcurl
 

FEATURES
========
Note that i have added some variants in one of my repositories.

The latest apps are doing the following once configured via a simple  .ini file and started in a terminal(console) on raspberry pi (or other linux e.g. Ubuntu)

1. Login into the configured Tapo camera (any ONVIF-S compatible camera will do)
 
 
2. Read the video stream.
 
 
3. Detect if a motion happened.
 
 
4. Record the motion and store the video at a configurable location on a disk.
 
 
4. Optional, when configured in your .ini file: Make and analyse every Xth second (is configurable) a frame, a snapshot of the video stream using an AI object recognition service running local on Rasberry Pi, like i have. You can also use a remote AI service, as it is just an URL you need to specify.
 
 
5. When one of the configured objects is recognized a jpeg picture will be saved at a configurable location on the disk.
 
 
6. Optional: the saved picture(s) can be mailed to you or the one you specify as a receiver. You just need to define an email account similar like you have on your phone mail. The mail function is completely included and simple configurable in the .ini file.
 
 
7. You can optionally exclude certain areas of the camera view using a mas. The instructions how to create a mask file is in the readme.txt. So only areas that you want will be used for motion detection. 
  
  
The latest App versions will use the motion areas defined on the camera and use a motion trigger message from the Tapo camera itself. That takes heavier load away from your Raspberry Pi. Other versions use a motion detection on the Raspberry Pi itself. Some camera's where no subscription to event messages is possible can use those  App versions. Read the Readme.txt file in each repository.
 
 
8. You can optionally view the resized video stream, the resized stream with the mask, a resized motion view and a full pixel view of the stream (large). An option is available (configurable) to view the motions and objects live in the video. The motion spots are marked and labelled live in the video and on the saved picture if you want that.
 
 
9. You can decide which objects you would recognise like a car, person cat, dog, bus, truck and many more when your AI object recognition service supports them.
 
 
10. Each video and picture is uniquely timestamped and in the picture file also the label is used. So jpeg pictures of persons are easy to find. The timestamp points you then quickly to the right video. Very simple.
 
 
11. You define subject and body text of the email. The picture can be optional embedded in the mail.
 
 
12. When you do not have/ want AI or mail, set in the .ini file a value to No and these functions will not be used.
 
 
13. The .ini file has clear explanations and default values.
 
 
14. You can record 4K video (2560x1440 pixels =stream1 of Tapo) with the C++ based apps on a Rasberry PI 4 with 4GB RAM 64bits,  and a disk attached or good USB 3.0 storage stick of about 128 or more GB.
 
 
15. The Python versions work better with the 720p HD format (1280x720 pixels =stream2 of Tapo) on a Raspberry Pi 4, although I got reasonable results with 4K. But only if you reduce all other loads, switch of WiFi, Bluetooth, Samba, etc.. 
 
 
16. My advice: Use stream2 with Python versions of the App.
 
 
17. My advice: Use stream1 with the C++versions of the App.
 
 
As I started with Python and later on detected that C++ was probably faster and better for 4K video I used the Python expertise to build further with C++. That is why one of the  latest C++ versions has a little more functionality like Mail included.
 
 
As I could not find an easy to use ONVIF library in C++, I decided to embed a Python runtime in the C++ App. An included Python program polls every Xth second the Tapo camera for ONVIF event (motion) messages. once motion is detected, recording starts.
 
 
I have to admit that this was a very, very complex exercise to get it all working. I am not a C++ programmer at all. 
 
 
Plenty of research and examples, trial and errors were needed to get it working.
 
 
Now I am done.... although never say never...
