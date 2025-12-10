#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <string>
#include <iomanip>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

double luminance(const Scalar& s) {
    return 0.2126 * s[2] + 0.7152 * s[1] + 0.0722 * s[0];
}

void showROIs(Mat& frame, const Rect& roi_red, const Rect& roi_green,
    const Rect& roi_blue, const Rect& roi_yellow, int idx) {
    Mat preview = frame.clone();

    rectangle(preview, roi_red, Scalar(0, 0, 255), 3);      // Red (Top-left)
    rectangle(preview, roi_green, Scalar(0, 255, 0), 3);    // Green (Top-right)
    rectangle(preview, roi_blue, Scalar(255, 0, 0), 3);     // Blue (Bottom-left)
    rectangle(preview, roi_yellow, Scalar(0, 255, 255), 3); // Yellow (Bottom-right)

    putText(preview, "Red", Point(roi_red.x, roi_red.y - 10),
        FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
    putText(preview, "Green", Point(roi_green.x, roi_green.y - 10),
        FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
    putText(preview, "Blue", Point(roi_blue.x, roi_blue.y - 10),
        FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
    putText(preview, "Yellow", Point(roi_yellow.x, roi_yellow.y - 10),
        FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 2);

    resize(preview, preview, Size(), 0.5, 0.5);
    imshow("check ROI (video " + to_string(idx) + ")", preview);

    cout << "Press any key to start analysis\n";
    waitKey(0);
    destroyWindow("check ROI (video " + to_string(idx) + ")");
}

void analyzeVideo(const string& videoPath, const string& outputPath) {
    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Fail to open the video: " << videoPath << "\n";
        return;
    }

    double fps = cap.get(CAP_PROP_FPS);
    int totalFrames = static_cast<int>(cap.get(CAP_PROP_FRAME_COUNT));
    int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    int maxFrames = min(totalFrames, static_cast<int>(fps * 15));

    cout << "\nStart analyzing: " << videoPath << "\n";

    cout << left << setw(20) << "FPS:" << fps << "\n"
        << left << setw(20) << "Resolution:" << width << "x" << height << "\n"
        << left << setw(20) << "Total frames:" << totalFrames << "\n"
        << left << setw(20) << "Frames to process:" << maxFrames << "\n";

    // videos\video2.mp4
    regex pattern(R"(video(\d+)\.mp4)");
    smatch match;
    string number;
    int idx = 0;

    if (regex_search(videoPath, match, pattern)) {
        number = match[1];
        idx = stoi(number);
    }

    // Check ROI (Hard-coded)
    Rect roi_red, roi_green, roi_blue, roi_yellow;
    switch (idx) {
    case 1:
        roi_red = Rect(610, 220, 220, 215);
        roi_green = Rect(835, 220, 220, 215);
        roi_blue = Rect(610, 445, 220, 215);
        roi_yellow = Rect(835, 445, 220, 215);
        break;

    case 2:
    case 3:
        roi_red = Rect(595, 180, 220, 215);
        roi_green = Rect(820, 180, 220, 215);
        roi_blue = Rect(595, 410, 220, 215);
        roi_yellow = Rect(820, 410, 220, 215);
        break;

    case 4:
    case 5:
        roi_red = Rect(455, 195, 220, 200);
        roi_green = Rect(690, 200, 220, 200);
        roi_blue = Rect(465, 430, 220, 200);
        roi_yellow = Rect(705, 420, 215, 195);
        break;

    case 6:
    case 7:
    case 8:
        roi_red = Rect(455, 195, 220, 200);
        roi_green = Rect(690, 200, 220, 200);
        roi_blue = Rect(465, 430, 220, 200);
        roi_yellow = Rect(700, 420, 215, 195);
        break;

    case 9:
        roi_red = Rect(455, 195, 220, 200);
        roi_green = Rect(690, 200, 220, 200);
        roi_blue = Rect(465, 430, 220, 200);
        roi_yellow = Rect(695, 420, 215, 195);
        break;

    case 10:
    case 11:
    case 12:
        roi_red = Rect(560, 190, 220, 215);
        roi_green = Rect(785, 190, 220, 215);
        roi_blue = Rect(570, 415, 215, 215);
        roi_yellow = Rect(795, 410, 215, 215);
        break;

    case 13:
    case 14:
        roi_red = Rect(770, 210, 220, 215);
        roi_green = Rect(995, 210, 220, 215);
        roi_blue = Rect(770, 435, 215, 215);
        roi_yellow = Rect(995, 440, 215, 215);
        break;

    case 15:
    case 16:
    case 17:
        roi_red = Rect(770, 210, 220, 215);
        roi_green = Rect(1000, 210, 220, 215);
        roi_blue = Rect(770, 435, 215, 215);
        roi_yellow = Rect(995, 440, 215, 215);
        break;

    case 18:
    case 19:
        roi_red = Rect(770, 170, 220, 215);
        roi_green = Rect(1000, 165, 220, 215);
        roi_blue = Rect(775, 398, 215, 215);
        roi_yellow = Rect(995, 395, 215, 215);
        break;

    default:
        roi_red = Rect(490, 100, 400, 390);
        roi_green = Rect(890, 100, 400, 390);
        roi_blue = Rect(490, 490, 400, 390); //*
        roi_yellow = Rect(890, 490, 400, 390);
    }
   
    
    Mat firstFrame;
    cap >> firstFrame;
    if (!firstFrame.empty()) {
        showROIs(firstFrame, roi_red, roi_green, roi_blue, roi_yellow, idx);
    }
    cap.set(CAP_PROP_POS_FRAMES, 0); // back to frame 0
    

    ofstream csvFile(outputPath);
    if (!csvFile.is_open()) {
        cerr << "Fail to create CSV file: " << outputPath << endl;
        return;
    }
    csvFile << "Frame,lumRed,lumYellow,lumGreen,lumBlue,N_w(red+yellow),N_b(green+blue)\n";

    Vec3d sumRed(0, 0, 0), sumGreen(0, 0, 0), sumBlue(0, 0, 0), sumYellow(0, 0, 0);
    int frameCount = 0;
    double N_w = 0, N_b = 0;
    bool firstFrameDone = false;

    Mat img;
    while (true) {
        cap >> img;
        if (img.empty()) break;
        if (frameCount >= maxFrames) break;

        Scalar meanRed = mean(img(roi_red));
        Scalar meanGreen = mean(img(roi_green));
        Scalar meanBlue = mean(img(roi_blue));
        Scalar meanYellow = mean(img(roi_yellow));

        double lumRed = luminance(meanRed);
        double lumYellow = luminance(meanYellow);
        double lumGreen = luminance(meanGreen);
        double lumBlue = luminance(meanBlue);

        double lumW = (lumRed + lumYellow) / 2.0;
        double lumB = (lumGreen + lumBlue) / 2.0;

        csvFile << frameCount + 1 << ","
            << lumRed << ","
            << lumYellow << ","
            << lumGreen << ","
            << lumBlue << ","
            << lumW << ","
            << lumB << "\n";

        if (!firstFrameDone) {
            N_w = lumW;
            N_b = lumB;
            firstFrameDone = true;
        }

        sumRed += Vec3d(meanRed[0], meanRed[1], meanRed[2]);
        sumGreen += Vec3d(meanGreen[0], meanGreen[1], meanGreen[2]);
        sumBlue += Vec3d(meanBlue[0], meanBlue[1], meanBlue[2]);
        sumYellow += Vec3d(meanYellow[0], meanYellow[1], meanYellow[2]);
        frameCount++;
    }

    if (frameCount > 0) {
        Vec3d avgRed = sumRed / frameCount;
        Vec3d avgGreen = sumGreen / frameCount;
        Vec3d avgBlue = sumBlue / frameCount;
        Vec3d avgYellow = sumYellow / frameCount;

        Scalar sRed(avgRed[0], avgRed[1], avgRed[2]);
        Scalar sGreen(avgGreen[0], avgGreen[1], avgGreen[2]);
        Scalar sBlue(avgBlue[0], avgBlue[1], avgBlue[2]);
        Scalar sYellow(avgYellow[0], avgYellow[1], avgYellow[2]);

        double lumRed = luminance(sRed);
        double lumYellow = luminance(sYellow);
        double lumGreen = luminance(sGreen);
        double lumBlue = luminance(sBlue);

        double N_wp = (lumRed + lumYellow) / 2.0;
        double N_bp = (lumGreen + lumBlue) / 2.0;
        double Opacity = (1.0 - ((N_wp - N_bp) / (N_w - N_b))) * 100.0;

        csvFile << "AVG,,,,," << N_wp << "," << N_bp << "\n";
        csvFile << "Opacity(%)" << ",,,,,," << Opacity << "\n";

        cout << "Analysis done: " << outputPath << endl;
        cout << left << setw(20) << "N_w:" << N_w << "\n"
            << left << setw(20) << "N_b:" << N_b << "\n" 
            << left << setw(20) << "N_wp:" << N_wp << "\n"
            << left << setw(20) << "N_bp:" << N_bp << "\n"
            << left << setw(20) << "Opacity:" << Opacity << "%\n";
    }

    csvFile.close();
}

int main(void) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    string videoDir = "videos";
    string outputDir = "analysis";

    if (!fs::exists(outputDir))
        fs::create_directory(outputDir);

    int count = 0;
    for (const auto& entry : fs::directory_iterator(videoDir)) {
        cout << "==============================================\n";
        string path = entry.path().string();
        if (entry.is_regular_file() && path.find(".mp4") != string::npos) {
            count++;
            string stem = entry.path().stem().string();
            string outputPath = outputDir + "/" + stem + "_analysis.csv";
            
            analyzeVideo(path, outputPath);
        }
        cout << "\n==============================================\n";
    }

    cout << "\nProcessed videos: " << count << endl;
    return 0;
}