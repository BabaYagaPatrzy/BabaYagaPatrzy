#include "Game.hpp"
#include "camsettings.hpp"

using namespace std;
using namespace cv;

Game::Game(int difficulty)
{
    this->detectedPlayers = 0;
    this->difficulty = difficulty;
    this->gameController = GameController();
}

void Game::gameStart()
{
    this->prepareCamera();
    namedWindow("RGB FRAME");
    this->prepareTrackbars();

    int frameNumber = 0;
    bool gameReady = false;
    for (frameNumber; gameReady == false; frameNumber++)
    {
        gameReady = this->initFrame(frameNumber);
        waitKey(10);
    }

    for (;;)
    {
        cout << "Green" << endl;
        for (frameNumber = 0; frameNumber < GREEN_FRAMES; frameNumber++)
        {
            greenFrame(frameNumber);
            waitKey(10);
        }

        cout << "Yellow" << endl;
        for (frameNumber = 0; frameNumber < YELLOW_FRAMES; frameNumber++)
        {
            yellowFrame(frameNumber);
            waitKey(10);
        }

        cout << "Red" << endl;
        for (frameNumber = 0; frameNumber < (RED_FRAMES); frameNumber++)
        {
            redFrame(frameNumber);
            waitKey(10);
        }
    }
}

bool Game::initFrame(int frameNumber)
{
    cap >> frame;
    if (frame.empty())
    {
        return false;
    }

    preparedFrame = this->gameController.getPreparedFrame(frame);
    playerIdents = this->gameController.findPlayerIdents(preparedFrame);
    if (playerIdents.size() == PLAYERS_NUM)
    {
        for (int i = 0; i < PLAYERS_NUM; i++)
        {
            int playerId = this->gameController.determinePlayerIdentNumber(preparedFrame, playerIdents[i]);
            cout << "PLAYER : " << playerId << endl;
            if (playerId != 0 && playerId != 15)
            {
                this->addPlayer(playerId, playerIdents[i]);
            }
        }
    }

    if (players.size() == PLAYERS_NUM)
    {
        return true;
    }

    drawStatusFrame(preparedFrame, GameState::INIT);
    imshow("RGB FRAME", preparedFrame);
    return false;
}
void Game::greenFrame(int frameNumber)
{
    cap >> frame;
    if (frame.empty())
    {
        return;
    }

#if DEBUG_MODE == 1

    preparedFrame = this->gameController.getPreparedFrame(frame);
    playerIdents = this->gameController.findPlayerIdents(preparedFrame);

    for (size_t i = 0; i < playerIdents.size(); i++)
    {
        int playerId = this->gameController.determinePlayerIdentNumber(preparedFrame, playerIdents[i]);
        if (this->findPlayer(playerId) != players.end())
        {
            players[playerId].setStoppedPlayerIdent(playerIdents[i]);
            players[playerId].setStoppedMat(preparedFrame);
        }
    }
    drawPlayers(frame);

#endif
    drawStatusFrame(frame, GameState::GREEN);
    imshow("RGB FRAME", frame);
}

void Game::yellowFrame(int frameNumber)
{
    cap >> frame;
    if (frame.empty())
    {
        return;
    }

    preparedFrame = this->gameController.getPreparedFrame(frame);
    playerIdents = this->gameController.findPlayerIdents(preparedFrame);
    for (size_t i = 0; i < playerIdents.size(); i++)
    {
        int playerId = this->gameController.determinePlayerIdentNumber(preparedFrame, playerIdents[i]);
        if (this->findPlayer(playerId) != players.end())
        {
            players[playerId].setStoppedPlayerIdent(playerIdents[i]);
            players[playerId].setStoppedMat(preparedFrame);
        }
    }
    drawPlayers(frame);
    drawStatusFrame(frame, GameState::YELLOW);
    imshow("RGB FRAME", frame);
}

void Game::redFrame(int frameNumber)
{
    cap >> frame;
    if (frame.empty())
    {
        return;
    }

    preparedFrame = this->gameController.getPreparedFrame(frame);
    playerIdents = this->gameController.findPlayerIdents(preparedFrame);
    for (size_t i = 0; i < playerIdents.size(); i++)
    {
        int playerId = this->gameController.determinePlayerIdentNumber(preparedFrame, playerIdents[i]);
        if (this->findPlayer(playerId) != players.end())
        {
            players[playerId].setCurrentMat(preparedFrame);
        }
    }

    for (map<int, Player>::iterator it = players.begin(); it != players.end(); ++it)
    {
        Mat current = it->second.getCurrentMat();
        Mat stopped = it->second.getStoppedMat();

        if (current.empty() || stopped.empty())
        {
            continue;
        }

        Mat diff = this->gameController.getDifferenceMatrix(stopped, current);
        if (this->determineIfPlayerHasMoved(it->first, diff, stopped))
        {
            it->second.setAlive(false);
        }
        this->displayPlayerMovement(diff, it->first);
    }
    drawPlayers(frame);
    drawStatusFrame(frame, GameState::RED);
    imshow("RGB FRAME", frame);
}

void Game::drawStatusFrame(Mat output, GameState gameStatus)
{
    Scalar color;
    if (gameStatus == GameState::INIT)
    {
        color = Scalar(255, 0, 0); // Blue
    }
    if (gameStatus == GameState::GREEN)
    {
        color = Scalar(0, 255, 0); // Green
    }
    else if (gameStatus == GameState::YELLOW)
    {
        color = Scalar(0, 255, 255); // Yellow
    }
    else //(gameStatus == GameState::RED)
    {
        color = Scalar(0, 0, 255); // Red
    }
    Size outputSize = output.size();
    Rect borderFrame(0, 0, outputSize.width, outputSize.height);
    rectangle(output, borderFrame, color, 25);
}

void Game::prepareTrackbars()
{
    namedWindow("Trackbars", (640, 200));
    createTrackbar("Tresh1", "Trackbars", this->gameController.getThresh1(), 255);
    createTrackbar("Tresh2", "Trackbars", this->gameController.getThresh2(), 255);
    createTrackbar("area", "Trackbars", this->gameController.getArea(), 1500);
    createTrackbar("kWidth", "Trackbars", this->gameController.getKernelWidth(), 15);
    createTrackbar("kHeight", "Trackbars", this->gameController.getKernelHeight(), 15);
}

void Game::prepareCamera()
{
    VideoCapture cap(SELECTED_CAMERA);
    cap.set(CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(CAP_PROP_FRAME_HEIGHT, 1080);
    this->cap = cap;
}

void Game::drawPlayers(Mat output)
{
    for (map<int, Player>::iterator it = players.begin(); it != players.end(); ++it)
    {
        it->second.drawPlayer(output);
        it->second.drawPlayerId(output);
    }
}

void Game::addPlayer(int playerId, Rect playerIdents)
{
    players.insert(make_pair(playerId, Player(playerId, playerIdents)));
}

map<int, Player>::iterator Game::findPlayer(int playerId)
{
    return this->players.find(playerId);
}

bool Game::determineIfPlayerHasMoved(int playerId, Mat diffFrame, Mat stoppedFrame)
{
    double distance = Preprocess::calculateDifferencePercentage(stoppedFrame, diffFrame);
    double distanceLimit = 0.95 - 0.05 * difficulty;
    cout << playerId << "::" << distance << endl;
    return distance < distanceLimit;
}

void Game::displayPlayerMovement(Mat diffFrame, int playerId)
{
    resize(diffFrame, diffFrame, Size(), 4, 4, INTER_LINEAR);
    namedWindow(to_string(playerId), (800, 600));
    imshow(to_string(playerId), diffFrame);
}