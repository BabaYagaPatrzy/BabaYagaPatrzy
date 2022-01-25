#include "Player.hpp"
#include "preprocess.hpp"
using namespace cv;
using namespace std;

Player::Player(int id, Rect &stoppedPlayerIdent)
{
    this->id = id;
    this->alive = true;
    this->stoppedPlayerIdent = stoppedPlayerIdent;
    this->isStopFrameSaved = false;
}

Player::Player() {}

Mat Player::getStoppedMat() const
{
    return stoppedMat;
}

Mat Player::getCurrentMat() const
{
    return currentMat;
}

void Player::setCurrentMat(const Mat &currentMat)
{
    this->currentMat = getPlayerHitbox(currentMat.clone());
}

void Player::setStoppedMat(const Mat &stoppedMat)
{
    this->isStopFrameSaved = true;
    this->stoppedMat = getPlayerHitbox(stoppedMat.clone());
}

void Player::setAlive(bool alive)
{
    this->alive = alive;
}

Rect Player::getStoppedPlayerIdent() const
{
    return stoppedPlayerIdent;
}

void Player::drawPlayer(Mat &output)
{
    int innerPosX = stoppedPlayerIdent.x;
    int innerPosY = stoppedPlayerIdent.y;
    int width = stoppedPlayerIdent.width;
    int height = stoppedPlayerIdent.height;

    Point2i innerTopLeft(innerPosX, innerPosY);
    Point2i innerBottomRight(innerPosX + width, innerPosY + height);
    Scalar innerColor(255, 0, 0);
    rectangle(output, innerTopLeft, innerBottomRight, innerColor, LINE_4);
    int outterPosX = innerPosX - width / 2;
    int outterPosY = innerPosY - height;
    Point2i outterTopLeft(outterPosX, outterPosY);
    Point2i outterBottomRight(outterPosX + 2 * width, outterPosY + 5 * height);
    Scalar outterColor;
    if (this->alive == true)
    {
        outterColor = Scalar(0, 255, 0); // Player is alive ( Green )
    }
    else
    {
        outterColor = Scalar(0, 0, 255); // Player is dead ( Red )
    }
    rectangle(output, outterTopLeft, outterBottomRight, outterColor, LINE_4);
}
void Player::drawPlayerId(Mat &output)
{
    int posX, posY;
    posX = stoppedPlayerIdent.x - 30;
    posY = stoppedPlayerIdent.y - 100;
    String idString = "Player ";
    idString += to_string(this->id);
    Point pos(posX, posY);
    if (alive)
    {
        putText(output, idString, pos, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 5, false);
    }
    else
    {
        putText(output, idString, pos, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 5, false);
    }
    putText(output, idString, pos, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 0), 3, false);
    putText(output, idString, pos, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255), 2, false);
}

void Player::setStoppedPlayerIdent(Rect &stoppedPlayerIdent)
{
    this->stoppedPlayerIdent = stoppedPlayerIdent;
}

Mat Player::getPlayerHitbox(Mat &input)
{
    int w = stoppedPlayerIdent.width;
    int h = stoppedPlayerIdent.height;
    int x = stoppedPlayerIdent.x - w / 2;
    int y = stoppedPlayerIdent.y - h;

    x = max(0, min(input.cols - 2 * w, x));
    y = max(0, min(input.rows - 2 * h, y));

    int xx = min(input.cols, x + 2 * w);
    int yy = min(input.rows, y + 5 * h);

    Range colR(x, xx);
    Range rowR(y, yy);
    return input(rowR, colR);
}