#include "preprocess.hpp"

using namespace cv;
using namespace std;

Mat Preprocess::prepareFrame(Mat input, int thresh1, int thresh2, Mat kernel)
{
    Mat grayFrame, blurred, eroded, output;
    cvtColor(input, grayFrame, COLOR_BGR2GRAY);
    erode(grayFrame, eroded, kernel);
    threshold(eroded, output, thresh1, thresh2, THRESH_BINARY);
    return output.clone();
}

Mat Preprocess::cutMatrix(Mat input, Rect dimensions, Rect coordinates)
{
    int x, y, width, height;
    x = coordinates.x;
    y = coordinates.y;
    width = dimensions.width;
    height = dimensions.height;
    Rect ROI(x, y, width, height);
    return input(ROI);
}

Point2f *Preprocess::convertRectToPoint2f(Rect rect)
{
    Point2f *points = new Point2f[4];
    float width = (float)rect.width;
    float height = (float)rect.height;
    float x = (float)rect.x;
    float y = (float)rect.y;
    points[0] = {x, y};
    points[1] = {x + width, y};
    points[2] = {x, y + height};
    points[3] = {x + width, y + height};
    return points;
}

Mat Preprocess::getWarpIdent(Mat inputImage, Rect rect)
{
    Point2f *identsPoints = convertRectToPoint2f(rect);
    float width = (float)rect.width;
    float height = (float)rect.height;
    Point2f destination[4] = {
        {0.0f, 0.0f},
        {width, 0.0f},
        {0.0f, height},
        {width, height}};
    Mat perspective = getPerspectiveTransform(identsPoints, destination);
    Mat outputImage;
    warpPerspective(inputImage, outputImage, perspective, Point(rect.width, rect.height));
    return outputImage;
}

vector<Rect> Preprocess::findIdents(vector<vector<Point>> contours, int numOfEdges, double aspectLow, double aspectHigh)
{
    vector<Rect> boundedRect(contours.size());
    vector<Rect> idents;
    size_t contoursSize = contours.size();
    for (int i = 0; i < contoursSize; i++)
    {
        if (contours[i].size() == numOfEdges)
        {
            boundedRect[i] = boundingRect(contours[i]);
            float aspectRatio = (float)boundedRect[i].width / (float)boundedRect[i].height;
            if (aspectRatio > aspectLow && aspectRatio < aspectHigh)
            {
                idents.push_back(boundedRect[i]);
            }
        }
    }
    return idents;
}

vector<vector<Point>> Preprocess::findAllClosedPolys(Mat input, Mat output, int area)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(input, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> conPoly(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        if (hierarchy[i][2] != -1)
        {
            double peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
        }
    }
    return conPoly;
}

void Preprocess::displayWarpedImages(Mat outputImage, vector<Rect> playerIdents, int thresh1, int thresh2)
{
    for (int i = 0; i < playerIdents.size(); i++)
    {
        Mat warpedImage = getWarpIdent(outputImage, playerIdents[i]);
        resize(warpedImage, warpedImage, Size(500, 800), i);
        cvtColor(warpedImage, warpedImage, COLOR_BGR2GRAY);
        threshold(warpedImage, warpedImage, thresh1, thresh2, THRESH_BINARY);
    }
}

int Preprocess::calculateAvgColorInRect(Mat rect)
{
    return mean(rect)(0);
}

void Preprocess::downscaleMovie(string movieName, int width, int height)
{
    VideoCapture toConvert(movieName);
    if (!toConvert.isOpened())
    {
        cout << "Nie znaleziono filmu do konwersji" << endl;
    }
    string convertedFileName = "converted_" + movieName;
    VideoWriter writer(convertedFileName, VideoWriter::fourcc('m', 'p', '4', 'v'), 30, Size(width, height));
    int i = 0;
    while (1)
    {
        Mat frame;
        toConvert >> frame;
        if (frame.empty())
        {
            break;
        }
        Mat resizedFrame;
        resize(frame, resizedFrame, cv::Size(width, height));
        writer.write(resizedFrame);
    }

    toConvert.release();
    writer.release();

    cout << "Done!" << endl;
}

int Preprocess::determineNumber(Mat wrappedImage, int width, int height)
{
    Point2f *points = new Point2f[4];
    bool *truths = new bool[4];
    int newWidth = width / 4;
    int newHeight = height / 4;
    float w1, w2, h1, h2;
    w1 = (float)newWidth;
    w2 = w1 * 2;
    h1 = (float)newHeight;
    h2 = h1 * 2;
    points[0] = {w1, h1};
    points[1] = {w2, h1};
    points[2] = {w1, h2};
    points[3] = {w2, h2};
    for (int i = 0; i < 4; i++)
    {
        Mat corner = getRectFromMatrix(wrappedImage, Rect(points[i].x, points[i].y, newWidth, newHeight));
        int avgColor = calculateAvgColorInRect(corner);
        truths[i] = isBlack(avgColor);
    }
    int playerIdentyficator = 0;
    for (int i = 0; i < 4; i++)
    {
        if (truths[i] == true)
        {
            playerIdentyficator += (int)pow(2.0, i);
        }
    }
    return playerIdentyficator;
}

bool Preprocess::isBlack(int avgColor)
{
    return avgColor < 150 ? true : false;
}

Mat Preprocess::getRectFromMatrix(Mat matrix, Rect rect)
{
    return matrix(rect);
}

bool Preprocess::canBeCompared(Mat first, Mat second)
{
    if (first.size() == second.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}
Mat Preprocess::getDifferenceMatrix(cv::Mat first, cv::Mat second)
{
    return abs(first - second);
}

double Preprocess::calculateDifferencePercentage(Mat first, Mat diff, int limit)
{
    int width = first.size().width;
    int height = first.size().height;
    int numOfPixels = width * height;
    double sum = 0;
    Mat outputArray;
    float range[] = {0, 256};
    const float *histRange[] = {range};
    int histSize = 256;
    cv::calcHist(&diff, 1, 0, Mat(), outputArray, 1, &histSize, histRange, true, true);
    for (int i = 0; i < outputArray.rows; i++)
    {
        if (i < limit)
        {
            sum += outputArray.at<float>(i);
        }
    }
    return (double)(sum / numOfPixels);
}
