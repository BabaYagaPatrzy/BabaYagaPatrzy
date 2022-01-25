#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "math.h"
#include "iostream"
#include "preprocess.hpp"
#include "Player.hpp"
#include "settings.hpp"
#include "GameController.hpp"

/**
 * @brief Klasa gry służąca do kontroli logiki oraz wyświetlaniu stanu gry na odpowiednich okienkach.
 */
class Game
{
public:
    /**
     * @brief Możliwe stany w jakich znajdować się może gra.
     */
    enum GameState
    {
        INIT,
        GREEN,
        YELLOW,
        RED
    };
    /**
     * @brief Pętla gry.
     */
    void gameStart();
    /**
     * @brief Konstruktor gry.
     */
    Game(int difficulty);

private:
    cv::VideoCapture cap;
    cv::Rect statusFrame;
    std::vector<cv::Rect> playerIdents;
    std::map<int, Player> players;
    cv::Mat frame, frameGray, blured, preparedFrame, tresholded;
    int detectedPlayers;
    int timeCounter;
    int stateLength;
    enum GameState gameState;
    int difficulty;
    GameController gameController;

    /**
     * @brief Pętla inicjalizująca graczy. Trwa dopóki nie znajdzie PLAYER_NUM graczy w obszarze klatki.
     * @param frameNumber ilość klatek
     * @return Zwraca true jeżeli odszuka wymaganą ilość graczy, false jeżeli nie.
     */
    bool initFrame(int frameNumber);

    /**
     * @brief Pętla inicjalizująca graczy. Trwa dopóki nie znajdzie PLAYER_NUM graczy w obszarze klatki.
     * @param frameNumber ilość klatek
     * @return Zwraca true jeżeli odszuka wymaganą ilość graczy, false jeżeli nie.
     */
    void greenFrame(int frameNumber);

    /**
     * @brief Pętla zapowiadająca sprawdzanie graczy. Zapisuje wykryty obrys gracza w celu porównania go w momencie wystąpienia fazy czerwonej.
     * @param frameNumber ilość klatek
     */
    void yellowFrame(int frameNumber);

    /**
     * @brief Pętla sprawdzająca graczy. Sprawdza czy, któryś z graczy się poruszył. I wykonuje obrys wokół każdego z graczy (czerwony w przypadku śmierci, zielony w przeciwnym wypadku).
     * @param frameNumber ilość klatek
     */
    void redFrame(int frameNumber);

    /**
     * @brief Inicjalizuje trackbary wykorzystywane do konfiguracji ustawień takich jak progowanie, wielkość pola, oraz rozmiary kernela.
     */
    void prepareTrackbars(void);

    /**
     * @brief Przygotowuje kamerę ustawiając konkretny rozmiar okna.
     */
    void prepareCamera(void);

    /**
     * @brief Przechodzi w pętli po liście graczy i rysuje każdego na zadanym output.
     * @param output klatka wyjściowa
     */
    void drawPlayers(cv::Mat output);

    /**
     * @brief Rysuje ramkę fazy gry wokół okna (zielony, żółty, czerwony).
     * @param output klatka wyjściowa
     * @param gameStatus aktualna faza gry
     */
    void drawStatusFrame(cv::Mat output, GameState gameStatus);

    /**
     * @brief Paruje konkretnego gracza z zadanym numerem identyfikatora.
     * @param playerId identyfikator gracza
     * @param playerIdent współrzędne plakietki gracza
     */
    void addPlayer(int playerId, cv::Rect playerIdent);

    /**
     * @brief Szuka gracza o konkretnym numerze id.
     * @param playerId zadany identyfikator gracza.
     * @param playerIdent współrzędne gracza
     * @return zwraca referencję do znalezionego gracza
     */
    std::map<int, Player>::iterator findPlayer(int playerId);

    /**
     * @brief Określa czy gracz się poruszył obliczając procent czarnych pikseli w stosunku do wszystkich pikseli.
     * @param playerId identyfikator gracza
     * @param diffFrame klatka różnicowa
     * @param stoppedFrame obecna klatka w fazie czerwonej
     * @return Zwraca true jeżeli gracz się poruszył, false jeżeli nie.
     */
    bool determineIfPlayerHasMoved(int playerId, cv::Mat diffFrame, cv::Mat stoppedFrame);

    /**
     * @brief Wyświetla ruch gracza w fazie czerwonej
     * @param playerId identyfikator gracza
     * @param diffFrame klatka różnicowa
     */
    void displayPlayerMovement(cv::Mat diffFrame, int playerId);
};