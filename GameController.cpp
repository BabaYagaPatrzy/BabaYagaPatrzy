#include "GameController.hpp"

using namespace std;
using namespace cv;

GameController::GameController()
{
    this->thresh1 = 75;
    this->thresh2 = 255;
    this->area = 800;
    this->kernelWidth = 3;
    this->kernelHeight = 3;
    this->kernel = getStructuringElement(MORPH_RECT, Size(this->kernelWidth, this->kernelHeight));
}

vector<Rect> GameController::findPlayerIdents(Mat preparedFrame, int numOfEdges, double aspectLow, double aspectHigh)
{
    vector<vector<Point>> allRectPointsFound = Preprocess::findAllClosedPolys(preparedFrame, preparedFrame, this->area);
    return Preprocess::findIdents(allRectPointsFound, numOfEdges, aspectLow, aspectHigh);
}

Mat GameController::getPreparedFrame(Mat rawFrame)
{
    return Preprocess::prepareFrame(rawFrame, this->thresh1, this->thresh2, this->kernel);
}
Mat GameController::getDifferenceMatrix(Mat stopped, Mat current)
{
    int sizeX = std::min(current.size[0], stopped.size[0]);
    int sizeY = std::min(current.size[1], stopped.size[1]);
    resize(current, current, Size(sizeX, sizeY));
    resize(stopped, stopped, Size(sizeX, sizeY));
    return Preprocess::getDifferenceMatrix(stopped, current);
}

int GameController::determinePlayerIdentNumber(Mat preparedFrame, Rect playerIdentCords)
{
    Mat warpedImage = Preprocess::getWarpIdent(preparedFrame, playerIdentCords);
    resize(warpedImage, warpedImage, Size(500, 800), 3); //"i" deleted from func
    return Preprocess::determineNumber(warpedImage, 500, 800);
}


int *GameController::getThresh1() {
    return &(this -> thresh1);
}
int *GameController::getThresh2(){
    return &(this -> thresh2);
}
int *GameController::getArea(){
    return &(this -> area);
}
int *GameController::getKernelWidth(){
    return &(this -> kernelWidth);
}

int *GameController::getKernelHeight(){
    return &(this -> kernelHeight);
}

Mat GameController::getKernel()
{
    return this->kernel;
}

