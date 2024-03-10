# TAPO-camera-ONVIF-RTSP-and-AI-Object-Recognition-C-
TAPO-camera-ONVIF-RTSP-and-AI-Object-Recognition in C++


You can run the compiled program on raspberry Pi 64 bit (e.g. bookworm).
1. Download mycMotDetRec and mycMotDetRec_config.ini and test_mask.png 
2. Open a terminal in the directory where you saved these files.
3. DO NOT FORGET THIS: EditT / Adapt first settings in mycMotDetRec_config.ini to your needs
4. Run ./mycMotDetRec
5. You may see a few times this repeated message. Just ignore it: libpng warning: iCCP: known incorrect sRGB profile
6. Use key q to close the program or CTRL-C in the console/terminal


To compile/Adapt the C++ program.
Download the repository and install the prerequisites.
There are a lot of comments in de C++ cpp program and the mycMotDetRec_config.ini file.
You may compile and change the mycMotDetRec.cpp as you wish.
The compiled version mycMotDetRec works on Raspberry Pi (4) with Bookworm 64 bit.
mycMotDetRec means: myc = my camera, Mot = Motion, Det = Detection, Rec = Recording.

There are multiple methods to create your own Mask file.
I used for the included test_mask.png the Gimp.
- Create a new file with the same pixel size as your camera stream produces e.g. 1280 x 720, you can also start (preferred) with a snapshot picture of your camera
- Draw a black colored layer over the whole picture. Black parts will become free of motion detection. Usually you draw a black figure/color over plants, streets with many cars etc. Things that create a lot of motion will give false triggers.
- Use a rectangle, circle or any other free form to cut out from the black layer the desired layout that should be visible for motion detection (cut-out-of-the black) and fill these gaps/parts in the black layer with white color. They will become transparant parts later on.
- When satisfied you delete the original snapshot picture from the background.
- Flatten hereafter all layers into a flat image (see menu image -> flatten image)
- Export now your Mask in a file e.g. mask_file.png 
- Specify that file / location in the mycMotDetRec_config.ini 

In principle this C++ version is almost similar to the Python version.

See the mycMotDetRec_config.ini for comments etc.

#put the folliwing in .bashrc in /home/<user>/.bashrc

PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH


/* PUT INIReader.h in same directory as this C++ program mycMotDetRec.cpp
 * You find INIReader.h at https://github.com/benhoyt/inih/blob/master/cpp/INIReader.h
 * 
 * PUT the MASK file "test_mask.png" (or your own mask file with any other name like "mask_stream2.png" also in the same directory
 * 
 * INSTALL the necessary compilation libraries for g++ and others
 * sudo apt update
 * sudo apt upgrade
 * sudo apt-get install build-essential cmake pkg-config libjpeg-dev libtiff5-dev libjasper-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libfontconfig1-dev libcairo2-dev libgdk-pixbuf2.0-dev libpango1.0-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran libhdf5-dev libhdf5-serial-dev libhdf5-103 python3-pyqt5 python3-dev -y
 * sudo apt-get install libcurl4-gnutls-dev
 * sudo apt-get install libjsoncpp-dev
 * sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json
 * sudo apt-get install libcurl4-openssl-dev
 * 
 * COMPILE NOW THIS PROGRAM WITH THIS COMMAND, ASSUMING YOU HAVE INSTALLED ALL LIBs op opencv, libcur and jsonccp
 * g++  mycMotDetRec.cpp -o mycMotDetRec   -I/usr/include/opencv4 -I/usr/include -lopencv_videoio -lopencv_video -lopencv_videostab -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_highgui -lopencv_imgcodecs -ljsoncpp  -lcurl
 *
*/
