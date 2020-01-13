#include "vision.h"


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//___Constructor                        //
Vision::Vision(const uint8_t CameraID){ //
    _Cam._ID = CameraID;                //
    _calcGrid();                        //
    for (uint16_t i = 0;i < 42;i++){    //
        stateRec._total[i] = false;     //
        stateRec._yellow[i] = false;    //
        stateRec._red[i] = false;       //
    }                                   //
}//-------------------------------------//

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//___Destructor                         //
Vision::~Vision(){                      //
    cv::destroyAllWindows();            //
    //cap.release();                    //
}//-------------------------------------//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Grab camera ID stored in private member variable
uint8_t Vision::getCamID(){
    return _Cam._ID;
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//




//___Calibrates for light in the room
void Vision::calibration(){
    GuiComms gui;
    uint8_t hits = 0;
    bool exitGameFlag = false;
    //___Initiate Camera________________________________________________//
    cv::VideoCapture cap(getCamID());                                   //
    if (!cap.isOpened()){ //__Check if camera was successfully opened   //
        error_t err = -1;                                               //
        printf("Err %i: Camera failed to open",err);                    //
        usleep(5000000);                                                //
    }//-----------------------------------------------------------------//

    //___Main Routine Variables___//
    Vision::Mat img;              //
    char exitKey = ' ';           //
    bool converged = false;       //
    //----------------------------//

    std::cout << "Calibrating camera for yellow disks..." << std::endl;
    for (int i = 0; i < 255; i+=10){
        for (int j = 0; j < 255; j+=10){

            std::cout << "S: " << j << " V: " << i << "   "
                      << "Yellow HSV : " << _yellow_hsv[0]
                    << "    " << _yellow_hsv[1]
                    << "    " << _yellow_hsv[2]
                    << "    " << _yellow_hsv[3]
                    << "    " << _yellow_hsv[4]
                    << "    " << _yellow_hsv[5] << std::endl;

            cap >> img; //__capture image                                        //
            img = _findYellow(&img);
            img = _detectBlob(img,true,"yellow");                                //

            if ( (stateRec._yellow[0]  == true) && (stateRec._yellow[6]  == true) &&
                 (stateRec._yellow[10] == true) && (stateRec._yellow[14] == true) &&
                 (stateRec._yellow[20] == true) && (stateRec._yellow[24] == true) &&
                 (stateRec._yellow[28] == true) && (stateRec._yellow[34] == true) &&
                 (stateRec._yellow[38] == true) &&
                 (stateRec._yellow[1]  == false) && (stateRec._yellow[2]  == false) &&
                 (stateRec._yellow[8]  == false) && (stateRec._yellow[9]  == false) &&
                 (stateRec._yellow[15] == false) && (stateRec._yellow[16] == false) &&
                 (stateRec._yellow[22] == false) && (stateRec._yellow[23] == false) &&
                 (stateRec._yellow[29] == false) && (stateRec._yellow[30] == false) &&
                 (stateRec._yellow[36] == false) && (stateRec._yellow[37] == false) &&
                 (stateRec._yellow[4]  == false) && (stateRec._yellow[5]  == false) &&
                 (stateRec._yellow[11] == false) && (stateRec._yellow[12] == false) &&
                 (stateRec._yellow[18] == false) && (stateRec._yellow[19] == false) &&
                 (stateRec._yellow[25] == false) && (stateRec._yellow[26] == false) &&
                 (stateRec._yellow[32] == false) && (stateRec._yellow[33] == false) &&
                 (stateRec._yellow[39] == false) && (stateRec._yellow[40] == false)){
                
                
                if (hits >= CAL_HITS){
                    hits = 0;
                    _yellow_hsv[1] = j;
                    _yellow_hsv[2] = i;
                    converged = true;
                }
                else{
                    _yellow_hsv[1] = j;
                    _yellow_hsv[2] = i;
                    hits++;
                }
                break;
            }
            else{
                _yellow_hsv[1] = j;
                _yellow_hsv[2] = i;
            }
            
            std::string gState = gui.checkState();
            std::string cState = gui.checkComms();
                if (gState == "exit" && cState == "newData"){
                    exitGameFlag = true;
                }
            exitKey = static_cast<char>(cv::waitKey(10));//----------------------//
            if (exitKey == 'q' || exitGameFlag == true){                         //
                break;                                                           //
            }//------------------------------------------------------------------//
        }
        if (exitKey == 'q' || converged == true || exitGameFlag == true){
            exitGameFlag = false;
            break;
        }
    }

    if (converged == false){
        _yellow_hsv[0] = 15;
        _yellow_hsv[1] = 180;
        _yellow_hsv[2] = 60;
        _yellow_hsv[3] = 50;
        _yellow_hsv[4] = 255;
        _yellow_hsv[5] = 255;
    }
    cv::destroyAllWindows(); //__Destroy all image and video windows //
    converged = false;
    //============================================================================//

    std::cout << "Calibrating camera for red disks..." << std::endl;
    for (int i = 0; i < 255; i+=10){
        for (int j = 0; j < 255; j+=10){

            std::cout << "S: " << j << " V: " << i << "   "
                      << "RED HSV 1 : " << _red_hsv[0]
                    << "    " << _red_hsv[1]
                    << "    " << _red_hsv[2]
                    << "    " << _red_hsv[3]
                    << "    " << _red_hsv[4]
                    << "    " << _red_hsv[5] << "   "
                    << "RED HSV 2 : " << _red_hsv[6]
                    << "    " << _red_hsv[7]
                    << "    " << _red_hsv[8]
                    << "    " << _red_hsv[9]
                    << "    " << _red_hsv[10]
                    << "    " << _red_hsv[11] << std::endl;

            cap >> img; //__capture image                                        //
            img = _findRed(&img);
            img = _detectBlob(img,true,"red");                                //

            if ( (stateRec._red[3]  == true) && (stateRec._red[7]  == true) &&
                 (stateRec._red[13] == true) && (stateRec._red[17] == true) &&
                 (stateRec._red[21] == true) && (stateRec._red[27] == true) &&
                 (stateRec._red[31] == true) && (stateRec._red[35] == true) &&
                 (stateRec._red[41] == true) &&
                 (stateRec._red[1]  == false) && (stateRec._red[2]  == false) &&
                 (stateRec._red[8]  == false) && (stateRec._red[9]  == false) &&
                 (stateRec._red[15] == false) && (stateRec._red[16] == false) &&
                 (stateRec._red[22] == false) && (stateRec._red[23] == false) &&
                 (stateRec._red[29] == false) && (stateRec._red[30] == false) &&
                 (stateRec._red[36] == false) && (stateRec._red[37] == false) &&
                 (stateRec._red[4]  == false) && (stateRec._red[5]  == false) &&
                 (stateRec._red[11] == false) && (stateRec._red[12] == false) &&
                 (stateRec._red[18] == false) && (stateRec._red[19] == false) &&
                 (stateRec._red[25] == false) && (stateRec._red[26] == false) &&
                 (stateRec._red[32] == false) && (stateRec._red[33] == false) &&
                 (stateRec._red[39] == false) && (stateRec._red[40] == false) ){
                     
                
                
                if (hits >= CAL_HITS){
                    hits = 0;
                    _red_hsv[1] = j;
                    _red_hsv[2] = i;
                    _red_hsv[7] = j;
                    _red_hsv[8] = i;
                    converged = true;
                }
                else{
                    _red_hsv[1] = j;
                    _red_hsv[2] = i;
                    _red_hsv[7] = j;
                    _red_hsv[8] = i;
                    hits++;
                }
                break;

                break;
            }
            else{
                _red_hsv[1] = j;
                _red_hsv[2] = i;
                _red_hsv[7] = j;
                _red_hsv[8] = i;
            }

            std::string gState = gui.checkState();
            std::string cState = gui.checkComms();
                if (gState == "exit" && cState == "newData"){
                    exitGameFlag = true;
                }
            exitKey = static_cast<char>(cv::waitKey(10));//----------------------//
            if (exitKey == 'q' || exitGameFlag == true){                         //
                break;                                                           //
            }//------------------------------------------------------------------//
        }
        if (exitKey == 'q' || converged == true || exitGameFlag == true){
            exitGameFlag = false;
            break;
        }
    }

    if (converged == false){
        _red_hsv[0]  = 0;
        _red_hsv[1]  = 120;
        _red_hsv[2]  = 60;
        _red_hsv[3]  = 10;
        _red_hsv[4]  = 255;
        _red_hsv[5]  = 255;
        _red_hsv[6]  = 170;
        _red_hsv[7]  = 120;
        _red_hsv[8]  = 60;
        _red_hsv[9]  = 180;
        _red_hsv[10] = 255;
        _red_hsv[11] = 255;
    }
    cv::destroyAllWindows(); //__Destroy all image and video windows //
    converged = false;
    //============================================================================//

    cap.release();
    for (uint16_t i = 0;i < 42;i++){    //
        stateRec._total[i] = false;     //
        stateRec._yellow[i] = false;    //
        stateRec._red[i] = false;       //
    }
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





////___Manual calibration
//void Vision::_manCal(){
//    //___Main Routine Variables___//
//    Vision::Mat img;              //
////    char exitKey = ' ';           //
//    const uint8_t maxValH = 180;
//    const uint8_t maxVal = 254;

//    //___Initiate Camera________________________________________________//
//    cv::VideoCapture cap(getCamID());                            //
//    if (!cap.isOpened()){ //__Check if camera was successfully opened   //
//        error_t err = -1;                                               //
//        printf("Err %i: Camera failed to open",err);                    //
//    }//-----------------------------------------------------------------//

////    while (true){
//        cap >> img; //__capture image                                        //
//        img = _findYellow(&img);
//        img = _detectBlob(img,true,"yellow");                                //

//        // Trackbars to set thresholds for HSV values
//        cv::createTrackbar("Low H", WIN_NAME_YELLOW_BLOB, &_yellow_hsv[0], maxValH);
//        cv::createTrackbar("High H", WIN_NAME_YELLOW_BLOB, &_yellow_hsv[1], maxVal);
//        cv::createTrackbar("Low S", WIN_NAME_YELLOW_BLOB, &_yellow_hsv[2], maxVal);
//        cv::createTrackbar("High S", WIN_NAME_YELLOW_BLOB, &_yellow_hsv[3], maxValH);
//        cv::createTrackbar("Low V", WIN_NAME_YELLOW_BLOB, &_yellow_hsv[4], maxVal);
//        cv::createTrackbar("High V", WIN_NAME_YELLOW_BLOB, &_yellow_hsv[5], maxVal);

//        cv::waitKey(0);
//        //-------------------------------------------------------------------//
////        exitKey = static_cast<char>(cv::waitKey(10));                        //
////        if (exitKey == 'q'){                                                 //
////            cv::destroyAllWindows(); //__Destroy all image and video windows //
////            cap.release();           //__Close camera module                 //
////            break;                                                           //
////        }//------------------------------------------------------------------//
////    }
//}



//___Function to align the board
void Vision::alignBoard(){
    GuiComms gui;
    bool exitGameFlag = false;
    //___Local Variables___//
    Mat img;               //
    char exitKey = ' ';    //

    //---------------------//

    //___Initiate Camera________________________________________________//
    cv::VideoCapture cap(_Cam._ID);                                     //
    if (!cap.isOpened()){ //__Check if camera was successfully opened   //
        error_t err = -1;                                               //
        printf("Err %i: Camera failed to open",err);                    //
        usleep(5000000);                                                //
    }//-----------------------------------------------------------------//

    //___Loop until key pressed
    while(true){

        cap >> img;                         //__capture image
//        cv::imshow(WIN_NAME_ORG_IMG,img);   //___show original image (for debugging and development)

        img = _findBlue(&img);              //___Detect blue colour
//        cv::imshow(WIN_NAME_BLUE_IMG,img);  //___show blues detected (for debugging and development)

        img = _edgeDetection(&img);         //___Detect edges (Returns a coloured image)
        cv::imshow(WIN_NAME_EDGE_IMG,img);  //___show final

        //___For debugging only_______________________________//
        //for(uint8_t xi = 0; xi < _Grid.nOfLines_x; xi++ ){  //
        //    printf("X limits = %u\t",_Grid.xlim[xi]);       //
        //}printf("\n\r");                                    //
        //for(uint8_t yi = 0; yi < _Grid.nOfLines_y; yi++ ){  //
        //    printf("Y limits = %u\t",_Grid.ylim[yi]);       //
        //}printf("\n\r");                                    //
        //----------------------------------------------------//
        //-------------------------------------------------------------------//
        std::string gState = gui.checkState();
            std::string cState = gui.checkComms();
                if (gState == "exit" && cState == "newData"){
                    exitGameFlag = true;
                }
        exitKey = static_cast<char>(cv::waitKey(10));                        //
        if (exitKey == 'q' || exitGameFlag == true){                                                 //
            cv::destroyAllWindows(); //__Destroy all image and video windows //
            cap.release();           //__Close camera module                 //
            break;                                                           //
        }//------------------------------------------------------------------//
    }

//    //___Calibration________________________________________________________________//
//    puts("===============>>>> Calibration in progress... <<<<=============== ");    //
//    calibration();                                                                 //
//    puts("===============>>>> Calibration Complete <<<<=============== ");          //
//    //------------------------------------------------------------------------------//

}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Function to find yellow coins on the board
Vision::Mat Vision::yellowDisk(Mat* imgIn){
    //___Local Variables___//
    Mat img = *imgIn;      //
    //---------------------//
    img = _findYellow(&img);
    img = _detectBlob(img,true,"yellow");

    return img;
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Function to find red coins on the board
Vision::Mat Vision::redDisk(Mat* imgIn){
    //___Local Variables___//
    Mat img = *imgIn;      //
    //---------------------//

    img = _findRed(&img);
    img = _detectBlob(img,true,"red");

    return img;
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Detects edges in a given image
Vision::Mat Vision::_edgeDetection(Mat* img){
    //___Local variables
    int8_t minThreshold = 0;
    const int8_t maxThreshold = minThreshold * 5;
    const int8_t kernalSize = 5;
    Mat imgEdges;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::RNG rng(12345);
    imgEdges = *img;

    cv::Canny(imgEdges,imgEdges,minThreshold,maxThreshold,kernalSize);

    cv::findContours(imgEdges,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros( imgEdges.size(), CV_8UC3 );
    for( unsigned long i = 0; i < contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//        cv::Scalar color = cv::Scalar( 255, 0, 0 );
        drawContours( drawing, contours, int(i), color, 2, 8, hierarchy, 0 );
    }
    _drawGrid(&drawing);
    return drawing;
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Calculates grid lines positions
void Vision::_calcGrid(){
    //___Loop for number of vertical lines required_______________________________________//
    for(uint8_t xi = 0; xi < _Grid.nOfLines_x; xi++ ){

        if(xi == 0){ //___add a horizontal offset to the grid to fit the grid in the middle of the frame
            _Grid.x = _Grid.x + _Grid.gridDisX + (_Grid.gridDisX / 4) - 2;
        }
        else{ //___vertical lines with distance required between each line = drigDisX + last distance
            _Grid.x = _Grid.x + _Grid.gridDisX;
        }
        _Grid.xlim[xi] = _Grid.x;
    }
    //------------------------------------------------------------------------------------//

    //___Loop for number of horizontal lines required_______________________________________//
    for(uint8_t yi = 0; yi < _Grid.nOfLines_y; yi++ ){
        if(yi == 0){ //___add a vertical offset to the grid to fit the grid in the middle of the frame
            _Grid.y = _Grid.y + _Grid.gridDisY;
        }
        else{   //___horizontal lines with distance required between each line = drigDisY + last distance
            _Grid.y = _Grid.y + _Grid.gridDisY;
        }
        _Grid.ylim[yi] = _Grid.y;
    }
    //------------------------------------------------------------------------------------//
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//






//___Draws a checkered Grid on the image provided
void Vision::_drawGrid(Mat* Img){
    //___Loop for number of vertical lines required_______________________________________//
    for(uint8_t xi = 0; xi < _Grid.nOfLines_x; xi++ ){
        //___Draw vertical lines__________________________________________________________//
        cv::line(*Img,cv::Point(_Grid.xlim[xi],0),cv::Point(_Grid.xlim[xi],_Grid.width),                //
                 cv::Scalar(_Grid.lineColour[0],_Grid.lineColour[1],_Grid.lineColour[2]), //
                 _Grid.thickness, _Grid.lineType);                                        //
        //--------------------------------------------------------------------------------//
    }

    //___Loop for number of horizontal lines required_______________________________________//
    for(uint8_t yi = 0; yi < _Grid.nOfLines_y; yi++ ){
        //___Draw horizontal lines________________________________________________________//
        cv::line(*Img,cv::Point(0,_Grid.ylim[yi]),cv::Point(_Grid.height,_Grid.ylim[yi]),               //
                 cv::Scalar(_Grid.lineColour[0],_Grid.lineColour[1],_Grid.lineColour[2]), //
                 _Grid.thickness, _Grid.lineType);                                        //
        //--------------------------------------------------------------------------------//
    }
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//




//___
Vision::Mat Vision::_detectBlob(const Mat Img, bool showImage, cv::String colour){
    cv::Mat imgGray, blob, blob_keypoints;

    cv::bitwise_not(Img,imgGray);

    cv::SimpleBlobDetector::Params params;
    // cv::SimpleBlobDetector* detector;
    // detector = new SimpleBlobDetector(); 
    // detector->create("SimpleBlobDetector");

    params.minThreshold = Blob.minThreshold;
    params.maxThreshold = Blob.maxThreshold;

    params.filterByArea = true;
    params.minArea = Blob.minArea;
    //params.maxArea = Blob.maxArea;

    params.filterByCircularity = true;
    params.minCircularity = Blob.minCircularity;

    params.filterByConvexity = true;
    params.minConvexity = Blob.minConvexity;
    //params.maxConvexity = Blob.maxConvexity;

    params.filterByInertia = true;
    params.minInertiaRatio = Blob.minInertia;

    std::vector<cv::KeyPoint> keypoints;

    // Set up detector with params
    // cv::SimpleBlobDetector detector;
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

    // Detect blobs
    detector->detect(imgGray, keypoints);


    if (showImage == true){
        if (colour == "yellow"){
            try{
                cv::drawKeypoints(imgGray, keypoints, blob, cv::Scalar(0,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
                _keypoints2DiskLoc(colour,keypoints);
            }
            catch(...){
                puts("nothing found --> detectBlob()");
            }
            _drawGrid(&blob);
            cv::imshow(WIN_NAME_YELLOW_BLOB, blob);
        }
        else if(colour == "red"){
            try{
                cv::drawKeypoints(imgGray, keypoints, blob, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
                _keypoints2DiskLoc(colour,keypoints);
            }
            catch(...){
                puts("nothing found --> detectBlob()");
            }
            _drawGrid(&blob);
            cv::imshow(WIN_NAME_RED_BLOB, blob);
        }
        else{
            try{
                cv::drawKeypoints(imgGray, keypoints, blob, cv::Scalar(255,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
            }
            catch(...){
                puts("nothing found --> detectBlob()");
            }
            _drawGrid(&blob);
            cv::imshow(WIN_NAME_UNKNOWN_BLOB, blob);
        }

    }
    return blob;
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//







//___Filters out all colours except for blue (works in HSV space)
Vision::Mat Vision::_findBlue(Mat* imgOrg){
    cv::Mat mask;   //___declare local variable matrix
    cv::cvtColor(*imgOrg,_Cam._imgBlue,cv::COLOR_BGR2HSV); //___convert colour image into HSV image

    //___Filter image and keep only blue colour intensity___________________________________//
    inRange(_Cam._imgBlue, cv::Scalar(_blue_hsv[0], _blue_hsv[1], _blue_hsv[2]),            //
                               cv::Scalar(_blue_hsv[3], _blue_hsv[4], _blue_hsv[5]), mask); //
    //--------------------------------------------------------------------------------------//

    _Cam._imgBlue = mask; //___Copy local variable of filiterd image into member variable
    return _Cam._imgBlue; //___Return filtered image
};//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Filters out all colours except for yellow (works in HSV space)
Vision::Mat Vision::_findYellow(Mat* imgOrg){
    cv::Mat mask; //___declare local variable matrix
    cv::cvtColor(*imgOrg,_Cam._imgYellow,cv::COLOR_BGR2HSV); //___convert colour image into HSV image

    //___Filter image and keep only yellow colour intensity_______________________________________//
    inRange(_Cam._imgYellow, cv::Scalar(_yellow_hsv[0], _yellow_hsv[1], _yellow_hsv[2]),          //
                               cv::Scalar(_yellow_hsv[3], _yellow_hsv[4], _yellow_hsv[5]), mask); //
    //--------------------------------------------------------------------------------------------//

    _Cam._imgYellow = mask; //___Copy local variable of filiterd image into member variable
    return _Cam._imgYellow; //___Return filtered image
};//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Filters out all colours except for red (works in HSmV space)
Vision::Mat Vision::_findRed(Mat* imgOrg){
    cv::Mat mask1, mask2; //___declare local variable matrix
    cv::cvtColor(*imgOrg,_Cam._imgRed,cv::COLOR_BGR2HSV); //___convert colour image into HSV image

    //___Filter image and keep only red colour intensity______________________________//
    inRange(_Cam._imgRed, cv::Scalar(_red_hsv[0], _red_hsv[1], _red_hsv[2]),             //
                            cv::Scalar(_red_hsv[3], _red_hsv[4], _red_hsv[5]), mask1);   //
    inRange(_Cam._imgRed, cv::Scalar(_red_hsv[6], _red_hsv[7], _red_hsv[8]),             //
                            cv::Scalar(_red_hsv[9], _red_hsv[10], _red_hsv[11]), mask2); //
    //-----------------------------------------------------------------------------------//
    _Cam._imgRed = mask1 + mask2; //___Copy local variable of filiterd image into member variable
    return _Cam._imgRed;          //___Return filtered image
};//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//




Vision::StructDiskLoc Vision::_keypoints2DiskLoc(std::string colour, const std::vector<cv::KeyPoint> keypoints){
    uint16_t idx = 0;
    if(colour == "yellow"){
        DiskLoc.yellow_x.clear();
        DiskLoc.yellow_y.clear();
        for(uint16_t i = 0; i < keypoints.size(); i++){
            DiskLoc.yellow_x.push_back(keypoints[i].pt.x);
            DiskLoc.yellow_y.push_back(keypoints[i].pt.y);
            idx++;
        }
//        printf("number of yellow discs found = %u\n\r",idx); //for debugging only
    }
    else if (colour == "red"){
        DiskLoc.red_x.clear();
        DiskLoc.red_y.clear();
        for(uint16_t i = 0; i < keypoints.size(); i++){
            DiskLoc.red_x.push_back(keypoints[i].pt.x);
            DiskLoc.red_y.push_back(keypoints[i].pt.y);
            idx++;
        }
//        printf("number of red discs found = %u\n\r",idx);  //for debugging only
    }
    _stateLocator(&DiskLoc);
    return DiskLoc;
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Find what state each
void Vision::_stateLocator(StructDiskLoc* keyPoints){

    uint8_t col = 0;
    uint8_t row = 0;
    uint16_t stateIDX = 0;

    for (uint16_t i = 0;i < 42;i++){    //
        stateRec._total[i] = false;     //
        stateRec._yellow[i] = false;    //
        stateRec._red[i] = false;       //
    }

    for(uint16_t i = 0; i < keyPoints->yellow_x.size();i++){
        col = 0;
        row = 0;
        stateIDX = 0;
        if(keyPoints->yellow_x[i] <= _Grid.xlim[0]){ col = 1; }
        else if((keyPoints->yellow_x[i] > _Grid.xlim[0]) && (keyPoints->yellow_x[i] <= _Grid.xlim[1])){ col = 2; }
        else if((keyPoints->yellow_x[i] > _Grid.xlim[1]) && (keyPoints->yellow_x[i] <= _Grid.xlim[2])){ col = 3; }
        else if((keyPoints->yellow_x[i] > _Grid.xlim[2]) && (keyPoints->yellow_x[i] <= _Grid.xlim[3])){ col = 4; }
        else if((keyPoints->yellow_x[i] > _Grid.xlim[3]) && (keyPoints->yellow_x[i] <= _Grid.xlim[4])){ col = 5; }
        else if((keyPoints->yellow_x[i] > _Grid.xlim[4]) && (keyPoints->yellow_x[i] <= _Grid.xlim[5])){ col = 6; }
        else if(keyPoints->yellow_x[i] > _Grid.xlim[5]){ col = 7; }

        if(keyPoints->yellow_y[i] <= _Grid.ylim[0]){ row = 6; }
        else if((keyPoints->yellow_y[i] > _Grid.ylim[0]) && (keyPoints->yellow_y[i] <= _Grid.ylim[1])){ row = 5; }
        else if((keyPoints->yellow_y[i] > _Grid.ylim[1]) && (keyPoints->yellow_y[i] <= _Grid.ylim[2])){ row = 4; }
        else if((keyPoints->yellow_y[i] > _Grid.ylim[2]) && (keyPoints->yellow_y[i] <= _Grid.ylim[3])){ row = 3; }
        else if((keyPoints->yellow_y[i] > _Grid.ylim[3]) && (keyPoints->yellow_y[i] <= _Grid.ylim[4])){ row = 2; }
        else if(keyPoints->yellow_y[i] > _Grid.ylim[4]){ row = 1; }

        stateIDX = (col + (7 * (row - 1))) - 1; //convert row col to state
        stateRec._total[stateIDX] = true;
        stateRec._yellow[stateIDX] = true;
    }

    for(uint16_t i = 0; i < keyPoints->red_x.size();i++){
        col = 0;
        row = 0;
        stateIDX = 0;

        if(keyPoints->red_x[i] <= _Grid.xlim[0]){ col = 1; }
        else if((keyPoints->red_x[i] > _Grid.xlim[0]) && (keyPoints->red_x[i] <= _Grid.xlim[1])){ col = 2; }
        else if((keyPoints->red_x[i] > _Grid.xlim[1]) && (keyPoints->red_x[i] <= _Grid.xlim[2])){ col = 3; }
        else if((keyPoints->red_x[i] > _Grid.xlim[2]) && (keyPoints->red_x[i] <= _Grid.xlim[3])){ col = 4; }
        else if((keyPoints->red_x[i] > _Grid.xlim[3]) && (keyPoints->red_x[i] <= _Grid.xlim[4])){ col = 5; }
        else if((keyPoints->red_x[i] > _Grid.xlim[4]) && (keyPoints->red_x[i] <= _Grid.xlim[5])){ col = 6; }
        else if(keyPoints->red_x[i] > _Grid.xlim[5]){ col = 7; }

        if(keyPoints->red_y[i] <= _Grid.ylim[0]){ row = 6; }
        else if((keyPoints->red_y[i] > _Grid.ylim[0]) && (keyPoints->red_y[i] <= _Grid.ylim[1])){ row = 5; }
        else if((keyPoints->red_y[i] > _Grid.ylim[1]) && (keyPoints->red_y[i] <= _Grid.ylim[2])){ row = 4; }
        else if((keyPoints->red_y[i] > _Grid.ylim[2]) && (keyPoints->red_y[i] <= _Grid.ylim[3])){ row = 3; }
        else if((keyPoints->red_y[i] > _Grid.ylim[3]) && (keyPoints->red_y[i] <= _Grid.ylim[4])){ row = 2; }
        else if(keyPoints->red_y[i] > _Grid.ylim[4]){ row = 1; }

        stateIDX = (col + (7 * (row - 1))) - 1; //convert row col to state
        stateRec._total[stateIDX] = true;
        stateRec._red[stateIDX] = true;
    }

//    //___For debugging only_________________________________________________________//
//    for(uint16_t i = 0; i < 42;i++){                                                //
//        std::cout << i+1 << ")  Totoal states filled = " <<stateRec._total[i] <<    //
//            "   " << "Yellow states = " << stateRec._yellow[i] <<                   //
//            "   " << "Red states = " << stateRec._red[i] << std::endl;              //
//    }//-----------------------------------------------------------------------------//

}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//





//___Returns record of the current same status
Vision::StateRecord Vision::_getStateRecord(){
    return stateRec;
}//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
