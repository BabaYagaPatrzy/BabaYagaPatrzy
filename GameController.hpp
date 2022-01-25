#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "math.h"
#include "iostream"
#include "preprocess.hpp"
#include "Player.hpp"
#include "settings.hpp"

/**
 * @brief Klasa służąca do wykonywania niskopoziomowych operacji dla klasy Game związanych między innymi z preprocessingiem.
 */
class GameController
{
private:
    int area;
    int thresh1;
    int thresh2;
    int kernelWidth;
    int kernelHeight;
    cv::Mat kernel;

public:
    /**
     * @brief Konstruktora kontrolera gry.
     */
    GameController::GameController();

    /**
     * @brief Umożliwia wyszukiwanie
     * @return Zwraca wektor znalezionych współrzędnych plakietek.
     */
    std::vector<cv::Rect> findPlayerIdents(cv::Mat preparedFrame,
                                           int numOfEdges = RECTANGLES,
                                           double aspectLow = ASPECT_RATIO_LOW,
                                           double aspectHigh = ASPECT_RATIO_HIGH);

    /**
     * @brief Wykonuje preprocessing na zadanej klatce.
     * @param rawFrame nieprzetworzona klatka
     * @return Zwraca przetworzoną klatkę.
     */
    cv::Mat getPreparedFrame(cv::Mat rawFrame);

    /**
     * @brief Określa identyfikator gracza (numer 1 - 14).
     * @param preparedFrame przetworzona klatka
     * @param playerIdentCords współrzędne gracza
     * @return Zwraca znaleziony identyfikator
     */
    int determinePlayerIdentNumber(cv::Mat preparedFrame, cv::Rect playerIdentCords);

    /**
     * @brief Zwraca klatkę różnicową między zadanymi klatkami.
     * @param stopped klatka na stopie
     * @param current aktualna klatka
     * @return Klatka różnicowa
     */
    cv::Mat getDifferenceMatrix(cv::Mat stopped, cv::Mat current);

    /**
     * @brief Oblicza procent czarnych pikseli w stosunku do wszystkich pikseli.
     * @param stopped przetworzona klatka
     * @param diffMatrix klatka różnicowa
     * @return Stosunek procentowy czarnych pikseli / wszystkie piksele
     */
    double calcDiffBetweenMatrixesPercentage(cv::Mat stopped, cv::Mat diffMatrix);

    int *getThresh1();
    void setThresh1(int thresh1);

    int *getThresh2();
    void setThresh2(int thresh2);

    int *getArea();
    void setArea(int area);

    int *getKernelWidth();
    void setKernelWidth(int kernelWidth);

    int *getKernelHeight();
    void setKernelHeight(int kernelHeight);

    cv::Mat getKernel();
    void setKernel(cv::Mat kernel);
};