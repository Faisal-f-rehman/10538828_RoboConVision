#include "vision.h"
#include "guicomms.h"
#include <unistd.h>

int main()
{
    int camID = 0;
    std::cout << "//\\//\\//\\=====>>>> Please enter camera ID <<<<=====//\\//\\//\\" << std::endl << "CAM ID: ";
    std::cin >> camID;
    std::cout << std::endl << "CAM ID entered: " << camID << std::endl;
    usleep(2000000);

    Vision vision(camID);
    GuiComms gui;
    gui.init();

    bool exitGameFlag1 = false;
    bool exitGameFlag2 = false;
    while(exitGameFlag2 == false){

        std::string gState = gui.checkState();
        std::string cState = gui.checkComms();
        std::cout << "guiState = " << gState << "    commsState = " << cState << std::endl;
        if (gState == "align" && cState == "newData"){
            gui.setBusyFlag();
            gState = gui.checkState();
            cState = gui.checkComms();
            std::cout << "guiState = " << gState << "    commsState = " << cState << std::endl;
            std::cout << "======>>> CLICK ON '" << WIN_NAME_EDGE_IMG << "' WINDOW AND PRESS 'q' ONCE ALIGNMENT IS COMPLETE <<<======"<< std::endl;
            vision.alignBoard();
            gui.clrBusyFlag();
            gui.setTaskCompleteFlag();
            gState = gui.checkState();
            cState = gui.checkComms();
            std::cout << "guiState = " << gState << "    commsState = " << cState << std::endl;
        }


        if (gState == "calibrate" && cState == "newData"){
            gui.setBusyFlag();
            gState = gui.checkState();
            cState = gui.checkComms();
            std::cout << "guiState = " << gState << "    commsState = " << cState << std::endl;
            std::cout << "======>>> CLICK ON '" << WIN_NAME_YELLOW_BLOB << "' or '" << WIN_NAME_RED_BLOB << "' WINDOW AND PRESS 'q' TO EXIT CALIBRATION <<<======"<< std::endl;
            vision.calibration();
            gui.clrBusyFlag();
            gui.setTaskCompleteFlag();
            gState = gui.checkState();
            cState = gui.checkComms();
            std::cout << "guiState = " << gState << "    commsState = " << cState << std::endl;
        }


        if (gState == "newGame" && cState == "newData"){
            gui.setBusyFlag();
            gState = gui.checkState();
            cState = gui.checkComms();
            std::cout << "guiState = " << gState << "    commsState = " << cState << std::endl;
            std::cout << "======>>> CLICK ON '" << WIN_NAME_YELLOW_BLOB << "' or '" << WIN_NAME_RED_BLOB << "' WINDOW AND PRESS 'q' TO EXIT CURRENT GAME <<<======"<< std::endl;
            //___Initiate Camera________________________________________________//
            cv::VideoCapture cap(vision.getCamID());                            //
            if (!cap.isOpened()){ //__Check if camera was successfully opened   //
                error_t err = -1;                                               //
                printf("Err %i: Camera failed to open",err);
                std::cout << std::endl << "CAM ID entered: " << vision.getCamID() << std::endl;
                usleep(5000000);                                                //
            }//-----------------------------------------------------------------//

            //___Main Routine Variables___//
            Vision::Mat img;              //
            char exitKey = ' ';           //
            //----------------------------//

            while (true){//----------------------------------------------------------//
                cap >> img; //__capture image                                        //
                vision.yellowDisk(&img);                                             //
                vision.redDisk(&img);                                                //

                std::string gState = gui.checkState();
                            cState = gui.checkComms();
                if (gState == "exit" && cState == "newData"){
                    exitGameFlag1 = true;
                }                                                                    //
                exitKey = static_cast<char>(cv::waitKey(10));                        //
                if ((exitKey == 'q') || (exitGameFlag1 == true)){                    //
                    cv::destroyAllWindows(); //__Destroy all image and video windows //
                    cap.release();           //__Close camera module                 //
                    break;                                                           //
                }//------------------------------------------------------------------//
            }
            exitGameFlag1 = false;
            gui.clrBusyFlag();
            gui.setTaskCompleteFlag();
            gState = gui.checkState();
            cState = gui.checkComms();
            std::cout << "guiState = " << gState << "    commsState = " << cState << std::endl;
        }

        gState = gui.checkState();
        cState = gui.checkComms();
        if (gState == "exit"  && cState == "newData"){
            gui.setBusyFlag();
            exitGameFlag2 = true;
            gui.clrBusyFlag();
            gui.setTaskCompleteFlag();
        }

        if (exitGameFlag2 == true){break;}
    }
    gui.exitSequence();
    std::cout << "------->> GOODBYE SEE YOU SOON <<-------" << std::endl;
    return 0;
}
