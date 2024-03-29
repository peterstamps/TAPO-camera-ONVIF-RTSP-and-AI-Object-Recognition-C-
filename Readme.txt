# TAPO-camera-ONVIF-RTSP-and-AI-Object-Recognition-C-
TAPO-camera-ONVIF-RTSP-and-AI-Object-Recognition in C++


You can run the compiled program on raspberry Pi 64 bit (e.g. bookworm). The compile Ubuntu version has been added as well and is called mycMotDetRecUbuntu (it was compiled on 64 bit Ubuntu 23.10)
1. Download mycMotDetRec and mycMotDetRec_config.ini and test_mask.png
   mycMotDetRec means: myc = my camera, Mot = Motion, Det = Detection, Rec = Recording.
2. Open a terminal in the directory where you saved these files.
3. DO NOT FORGET THIS: Edit / Adapt first the settings in mycMotDetRec_config.ini to your needs!
4. Run ./mycMotDetRec
5. You may see a few times this repeated message. Just ignore it: libpng warning: iCCP: known incorrect sRGB profile. 
   On Ubuntu you might see this message which can also be ignored: QSocketNotifier: Can only be used with threads started with QThread
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
 

