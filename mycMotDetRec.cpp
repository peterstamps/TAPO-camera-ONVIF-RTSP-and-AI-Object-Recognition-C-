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

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/video.hpp>
#include "./INIReader.h"
#include <curl/curl.h>
#include <json/json.h>

using namespace cv;
using namespace std;
VideoWriter outputVideo;

// Structure to hold object detection result
struct DetectionResult {
    std::string label;
    cv::Rect boundingBox;
};


size_t callback(
        const char* in,
        size_t size,
        size_t num,
        string* out)
{
    const size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}


size_t write_data(void *ptr, size_t size, size_t nmemb, string *data) {
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

// Function to post the image to the server and parse the JSON response
vector<DetectionResult> postImageAndGetResponse(string& AIserverUrl, string& min_confidence, Mat& frame, string& show_AIResponse_message, string& show_AIObjDetectionResult) {
    // Define vector for the extraction of  object detection results
    vector<DetectionResult> detectionResults;    
    CURL *curl;
    CURLcode res;
    vector<uchar> buffer;
   // Mat the_frame = imread("test.jpg"); // Load your image here. Keep for test. However a frame is used!

    curl = curl_easy_init();
    if (curl) {
        imencode(".jpg", frame, buffer);

        string min_confidence = "0.4";
        // string AIserverUrl = AIserverUrl; // set in the config file and passsed by main()
        // string min_confidence = "0.4";  // set in the config file and passsed by main()
        string output_obj_detection_filename = "picture_for_obj_detection.jpg";

        stringstream image_stream;
        for (const auto &byte : buffer) {
            image_stream << byte;
        }

        curl_mime *mime;
        curl_mimepart *part;

        mime = curl_mime_init(curl);

        part = curl_mime_addpart(mime);
        curl_mime_name(part, "min_confidence");
        curl_mime_data(part, min_confidence.c_str(), CURL_ZERO_TERMINATED);

        part = curl_mime_addpart(mime);
        curl_mime_name(part, "typedfile");
        curl_mime_data(part, image_stream.str().c_str(), buffer.size());
        curl_mime_filename(part, output_obj_detection_filename.c_str());
        curl_mime_type(part, "image/jpeg");

        curl_easy_setopt(curl, CURLOPT_URL, AIserverUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            }    
        else {
            // Parsing JSON response
            Json::Value jsonData;
            Json::Value root {};
            Json::Reader jsonReader;
            if (jsonReader.parse(response_data, jsonData)) {
              if (show_AIResponse_message == "Yes") {
                  cout << "Successfully parsed JSON data" << endl;
                  cout << "\nJSON data received:" << endl;
                  cout << jsonData.toStyledString() << endl;
                //  const string aiPredictions(jsonData["predictions"].asString());
              }  // END  if (show_AIResponse_message == "Yes")
              if (show_AIObjDetectionResult == "Yes") {
                  const string aiMessage(jsonData["message"].asString());
                  cout << "Natively parsed:" << endl;
                  cout << "\tAI Message string: " << aiMessage << endl;
                  cout << endl;
                 // Extract object detection results
                  for (const auto& prediction : jsonData["predictions"]) {
                      DetectionResult result;
                      result.label = prediction["label"].asString();
                      result.boundingBox = Rect(prediction["x_min"].asInt(), prediction["y_min"].asInt(),
                                                     prediction["x_max"].asInt(), prediction["y_max"].asInt());
                      detectionResults.push_back(result);
                      }
              } // END  if (show_AIObjDetectionResult == "Yes")
            }
            else 
            {
                cout << "Could not parse HTTP data as JSON" << endl;
                cout << "HTTP data was:\n" << response_data << endl;
            } // END of if (jsonReader.parse(response_data, jsonData))
        } // END if (res != CURLE_OK)
      } // END if (curl)

      curl_easy_cleanup(curl);
      return detectionResults;
    }




int main() {
    INIReader reader("mycMotDetRec_config.ini");
    if (reader.ParseError() < 0) {
        cout << "Error: Cannot load config.ini" << endl;
        return 1;
    }

    // Read variables from the .ini file
    // IP camera URL
    string url = reader.Get("camera_url", "url", "");
    // Thresholds for motion detection contour area: this is the minimum numer of the changed pixels between frames
    int min_contour_area = reader.GetInteger("motion_detection", "min_contour_area", 500);
    // Background subtraction method (choose between "KNN" or "MOG2")
    string background_subtractor_method = reader.Get("motion_detection", "background_subtraction_method", "KNN");
    // Path to the mask file
    string mask_path = reader.Get("motion_detection", "mask_path", "");
    // Record duration in seconds
    int record_duration = reader.GetInteger("motion_detection", "record_duration", 10);
    // Time to check for new motion before the end of the set record duration
    int check_motion_time = reader.GetInteger("motion_detection", "check_motion_time", 3);
    // Extra time to add to the record duration if new motion is detected
    int extra_record_time = reader.GetInteger("motion_detection", "extra_record_time", 5);
    // Pre-motion recording duration in seconds
    int before_record_duration_is_passed = reader.GetInteger("motion_detection", "before_record_duration_is_passed", 3);
    // When No motion rectangles will be drawn on the screen around the moving objects
    string draw_motion_rectangles = reader.Get("motion_detection", "draw_motion_rectangles", "No");
    // Simulate a motion Default No. (used for test purposes)
    string simulate_a_motion = reader.Get("motion_detection", "simulate_a_motion", "No");
    // Show the display window with mask in a resized frame
    string show_display_window = reader.Get("motion_detection", "show_display_window", "No");
    // Show the display window with resized frame
    string show_display_window_with_mask = reader.Get("motion_detection", "show_display_window", "No");
    // Show indicator that motion has been detected on display window
    string show_motion_detected_msg_on_display_window = reader.Get("motion_detection", "show_motion_detected_msg_on_display_window", "No");
    // Show the fps and the date and time on the display window
    string show_motion_fps_date_msg_on_display_window = reader.Get("motion_detection", "show_motion_fps_date_msg_on_display_window", "No");
    // Show indicator that motion has been detected on display console (terminal window)
    string show_motion_detected_msg_on_display_console = reader.Get("motion_detection", "show_motion_detected_msg_on_display_console", "No");
    // Show the fps and the date and time on the display  console (terminal window)
    string show_motion_fps_date_msg_on_display_console = reader.Get("motion_detection", "show_motion_fps_date_msg_on_display_console", "No");
    // Output video parameters
    string output_video_path = reader.Get("output_video", "output_video_path", "./");
    // The Frames per second that your camera uses
    int fps = reader.GetInteger("output_video", "fps", 15);
    // The codec to be used for writing the videos
    string codecString = reader.Get("output_video", "codec", "XVID");
    // AI Object Detection Service URL
    string AIserverUrl = reader.Get("object_detection", "AIserverUrl", "http://localhost:80/v1/vision/detection");
    // Only when an AI object Detection service is installed Object Detection will happen
    string AI_object_detection_service = reader.Get("object_detection", "AI_object_detection_service", "No");
    // Thresholds for object detection: this is the minimum percentage (fraction) when an object signaled as recognised
    string min_confidence = reader.Get("motion_detection", "min_confidence", "0.4");    
    // Object detection will repeat after x seconds (default 5) when motion has been detected
    int object_detection_time = reader.GetInteger("object_detection", "object_detection_time", 3);
   // Output picture parameters
    string output_obj_picture_path = reader.Get("object_detection", "output_obj_picture_path", "./");
    // Show the full json response message from the AI Object Detection Service
    string show_AIResponse_message = reader.Get("object_detection", "show_AIResponse_message", "No");
    // Show the result(s) from the response message from the AI Object Detection Service
    string show_AIObjDetectionResult = reader.Get("object_detection", "show_AIObjDetectionResult", "No");    // When No motion rectangles will be drawn on the screen around the moving objects


    // Load the mask
    Mat mask = imread(mask_path, IMREAD_GRAYSCALE);
    if (mask.empty()) {
        cout << "Error: Cannot load the mask image." << endl;
        return -1;
    }
    // Invert the mask so that black becomes non-transparent and white becomes transparent
    mask = 255 - mask;    
    

    // Initialize camera
    VideoCapture cap(url);
    if (!cap.isOpened()) {
        cout << "Error: Cannot open the camera." << endl;
        return -1;
    }
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    
    Size frameSize = Size(frame_width, frame_height);
    char time_now_buf[21];
    time_t now;
    time(&now);
    strftime(time_now_buf, 21, "%Y_%m_%d_%H_%M_%S", gmtime(&now));
    cout << "Camera started @ " << time_now_buf << endl;
    cout << "Videos are saved @ " << output_video_path << endl;
    cout << "Pictures are saved @ " << output_obj_picture_path << endl;


    VideoWriter outputVideo;

    // Initialize background subtractor
    Ptr<BackgroundSubtractor> back_sub;
    if (background_subtractor_method == "KNN") {
        back_sub = createBackgroundSubtractorKNN();
    } else {
        back_sub = createBackgroundSubtractorMOG2();
    }

    // Initialize variables for recording
    time_t start_time = 0;
    time_t end_time = 0;
    int frameCounter = 0;
    int obj_detection_each_x_frames = fps * object_detection_time;
    
    bool recording_on = false;
    
    while (true) {
        // Read frame from the camera
        Mat frame;
        Mat frame_original;
        cap.read(frame);
        if (frame.empty()) {
            break;
        }
        frame_original = frame.clone();
        frameCounter += 1;
        string str_frameCounter = to_string(frameCounter);
         
        time(&now);
        strftime(time_now_buf, 21, "%Y_%m_%d_%H_%M_%S", gmtime(&now));

        // Apply the mask as an overlay
        for (int i = 0; i < frame.rows; ++i) {
            for (int j = 0; j < frame.cols; ++j) {
                if (mask.at<uchar>(i, j) != 0) {
                    frame.at<Vec3b>(i, j) = Vec3b(0, 0, 0); // Black
                }
            }
        }
        
        
        // Apply background subtraction
        Mat fg_mask;
        back_sub->apply(frame, fg_mask);

        // Apply morphological operations to clean up the mask
        erode(fg_mask, fg_mask, Mat(), Point(-1, -1), 2);
        dilate(fg_mask, fg_mask, Mat(), Point(-1, -1), 2);

        if (show_motion_fps_date_msg_on_display_window == "Yes") {
          // put frame number and time on screen
          putText(frame_original, '@'+str_frameCounter + " " + time_now_buf, Point(10,frame_height - 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,255,88),2);
        }

        if (show_motion_fps_date_msg_on_display_console == "Yes") {
          // put frame number and time on screen
          // [xK Erases part of the line. If n is 0 (or missing), 
          // clear from cursor to the end of the line. 
          // If n is 1, clear from cursor to beginning of the line. 
          // If n is 2, clear entire line. Cursor position does not change. 
          cout << '@'+str_frameCounter + " " + time_now_buf + "\033[K\r"; 
        }

        // Find contours in the mask
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        RNG rng(12345);
        findContours(fg_mask.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        bool motion_detected = false;
        // Check for motion after a warm up time to avoid initial writing of a file
        if (frameCounter > check_motion_time * fps) {
          vector<Rect>boundRect (contours.size());
          vector<vector<Point> > contours_poly( contours.size() );
          for (int i = 0; i < contours.size();i++) {
              if (contourArea(contours[i]) <  min_contour_area) {
                  continue;
              }
              motion_detected = true;
            if (show_motion_detected_msg_on_display_window == "Yes") {          
              //    cout << "Motion detected" << endl;
              // put frame number and time on screen
              putText(frame_original, "motion detected", Point(10, frame_height - 40), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,255,88),2);
            }
            if (show_motion_detected_msg_on_display_console== "Yes") {          
              //    cout << "Motion detected" << endl;
              // put frame number and time on screen
              cout << "motion detected                        \033[K\r";
            }
            if (draw_motion_rectangles == "Yes") {
              approxPolyDP( contours[i], contours_poly[i], 3, true );
              boundRect[i] = boundingRect( contours_poly[i] );
              Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
              rectangle(frame_original, boundRect[i].tl(), boundRect[i].br(), color, 2 );
            }
            else {
              break;
            }
          }
        }
        if (simulate_a_motion == "Yes") {
          if (frameCounter > 100 and frameCounter < 300  or frameCounter > 400 and frameCounter < 600) {motion_detected = true;} else {motion_detected = false;}
        }
        
        if (recording_on and AI_object_detection_service == "Yes") {
          // Initialize variables for a new object detection, object_detection_time is defined in config

            int modulo_result = frameCounter % obj_detection_each_x_frames;
            if (modulo_result == 0) {       
            // Post the image to the server and get the response
            auto detectionResults = postImageAndGetResponse(AIserverUrl, min_confidence, frame_original, show_AIResponse_message, show_AIObjDetectionResult);   
            for (const auto& result : detectionResults) {
               if (result.label == "car") {
                 imwrite(output_obj_picture_path + "Pic_" + time_now_buf + ".jpg", frame_original);  
                 cout << "Saved: " << output_obj_picture_path + "Pic_" + time_now_buf + ".jpg" << endl;
                }
            }

          }
        }
              
                      
        // If motion detected, start recording and set the record duration
        if (motion_detected and not recording_on) {
              recording_on = true;
              start_time = time(0);
              end_time = start_time + record_duration;
              cout << "Recording started @ " + string(time_now_buf) << endl;
              // set the codec and create VideoWriter object
              outputVideo.release();       
              outputVideo.open(output_video_path + "Vid_" + time_now_buf + ".avi",  VideoWriter::fourcc(codecString[0], codecString[1], codecString[2], codecString[3]), fps, frameSize, true);
              cout << "Saved: " << output_video_path + "Vid_" + time_now_buf + ".avi" << endl;

          }


        // If recording is on, then check if extra record time must be added and check if record duration is passed 
        if (recording_on) {
          if (motion_detected and time(0) >= end_time - before_record_duration_is_passed) {
                end_time += extra_record_time;
          }

          // Check if it's time to stop recording
          if (time(0) >= end_time) {
              cout << "Recording stopped @ " + string(time_now_buf) << endl;
              start_time = 0;
              end_time = 0;
              recording_on = false;
              // Release the VideoWriter object
              if (!outputVideo.isOpened()) {
                  cerr << "Could not open the output video file for release\n";
                  return -1;
              }    
              outputVideo.release();
          }
          else {
              
              // Write frame to the output video  
             // cout << "1 write frame ->" + str_frameCounter << endl;
              // out << frame; 
              if (!outputVideo.isOpened()) {
                  cerr << "Could not open the output video file for write\n";
                  return -1;
              }  
              outputVideo.write(frame_original);
          }
        }
        if (show_display_window == "Yes") {
          resize(frame_original, frame_original, Size(640,370));     
          // Display the resulting frame
          imshow("Motion Detection", frame_original);
        }
        if (show_display_window_with_mask == "Yes") {
          resize(frame, frame, Size(640,370));     
          // Display the resulting frame
          imshow("Motion Detection with Mask", frame);
        }

        // Check for key press to exit
        if (waitKey(1) == 'q') {
            break;
        }
    }

    // Release the camera
    cap.release();
    destroyAllWindows();

    return 0;
}