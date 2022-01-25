#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

/**
 * @brief Reprezentacja gracza w grze.
 */
class Player
{
private:
    cv::Rect stoppedPlayerIdent;
    int id;
    bool alive;
    bool isStopFrameSaved;
    cv::Mat stoppedMat;
    cv::Mat currentMat;

public:
    /**
     * @brief Konstrukuje gracza gdy znamy jego id oraz współrzędne
     * @param id id gracz
     * @param stoppedPlayerIdent współrzędne gracza
     */
    Player(int id, cv::Rect &stoppedPlayerIdent);

    /**
     * @brief Konstruuje gracza gdy nie znamy jego danych.
     */
    Player();
    cv::Mat getStoppedMat() const;
    cv::Mat getCurrentMat() const;
    void setCurrentMat(const cv::Mat &currentMat);
    void setStoppedMat(const cv::Mat &stoppedMat);
    void setAlive(bool alive);
    cv::Rect getStoppedPlayerIdent() const;
    void setStoppedPlayerIdent(cv::Rect &stoppedPlayerIdent);

    /**
     * @brief Rysuje gracza na zadanej klatce.
     * @param output klatka wyjściowa
     */
    void drawPlayer(cv::Mat &output);

    /**
     * @brief Rysuje id gracza nad jego obrysem.
     * @param output klatka wyjściowa
     */
    void drawPlayerId(cv::Mat &output);
    cv::Mat getPlayerHitbox(cv::Mat &input);
};
