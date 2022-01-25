#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

/**
 * @brief Grupuje wszystkie funkcje preprocessingu w naszym projekcie.
 */
class Preprocess
{
public:
    /**
     * @brief Dokonuje preprocessingu na zadanej klatce.
     * @param input klatka wejściowa
     * @param thresh1 pierwszy próg
     * @param thresh2 drugi próg
     * @param kernel kernle
     * @return zwraca przetworzoną klatkę.
     */
    static cv::Mat prepareFrame(cv::Mat input, int thresh1, int thresh2, cv::Mat kernel);

    /**
     * @brief wycina zadany obszar z klatki wejściowej.
     * @param input klatka wejściowa
     * @param dimensions wymiary wycięcia
     * @param coordinates współrzędne wycięcia
     * @return zwraca wycięty obszar w postaci macierzy
     */
    static cv::Mat cutMatrix(cv::Mat input, cv::Rect dimensions, cv::Rect coordinates);

    /**
     * @brief Konwertuje rect na point2f
     * @param rect współrzędne
     * @return konwertuje współrzędne do struktury Point2f
     */
    static cv::Point2f *convertRectToPoint2f(cv::Rect rect);

    /**
     * @brief Dokonuje perspektywizacji zadego obszaru.
     * @param inputImage obszar wejściowy
     * @param rect parametry obszaru
     * @return zwraca przetworzony obszar
     */
    static cv::Mat getWarpIdent(cv::Mat inputImage, cv::Rect rect);

    /**
     * @deprecated Używana wcześniej do debugowania
     * @brief Dokonuje perspektywizacji na podstawie zadanych współrzędnych.
     * @param input klatka wejściowa
     * @param idents współrzędne
     * @param thresh1 próg pierwszy
     * @param thresh2 próg drugi
     */
    static void displayWarpedImages(cv::Mat input, std::vector<cv::Rect> idents, int thresh1, int thresh2);

    /**
     * @brief Odfiltrowuje te identyfikatory, które spełniają zadane ograniczenia.
     * @param contours przeszukiwane kontury
     * @param numOfEdges ilość krawędzi 4 - prostokąt
     * @param aspectLow współczynnik startowy określający czy znaleziony identyfikator jest ustawiony pionowo.
     * @param aspectHigh współczynnik końcowy określający czy znaleziony identyfikator jest ustawiony pionowo.
     * @return zwraca przefiltrowane identyfikatory spełniające założenia
     */
    static std::vector<cv::Rect> findIdents(std::vector<std::vector<cv::Point>> contours, int numOfEdges, double aspectLow, double aspectHigh);

    /**
     * @brief Szuka wszystkich zamkniętych wielomianów w klatce wejściowej.
     * @param input macierz wejsciowa
     * @param output macierz wyjściowa
     * @param area
     * @return zwraca wektor wektorów punktów będących wszystkimi wielomiany do dalszej obróbki
     */
    static std::vector<std::vector<cv::Point>> findAllClosedPolys(cv::Mat input, /* TODO */ cv::Mat output, /* TODO */ int area);

    /**
     * @brief Dzieli zadanych obszar na 16 fragmentów. Determinuje dominujący kolor w podobszarach i określa na tej podstawie znaleziony numer. 0 - 15
     * @param wrappedImage klatka wejściowa
     * @param width szerokość
     * @param height wysokość
     * @return znaleziony numer
     */
    static int determineNumber(cv::Mat wrappedImage, int width, int height);

    /**
     * @brief Zwraca średni kolor w zadanym
     * @param rect obszar wejściowy
     * @return średni kolor w zadanym obszarze
     */
    static int calculateAvgColorInRect(cv::Mat rect);

    /**
     * @deprecated Używana do konwersji filmu.
     * @brief Zmniejsza rozdzielczość filmu i zapisuje go na dysku.
     * @param movieName nazwa pliku wejściowego -> wyjściowy ma przedrostek converted_
     * @param width nowa szerokość
     * @param height nowa wysokość
     */
    static void downscaleMovie(std::string movieName, int width, int height);

    /**
     * @deprecated Nie używana
     * @brief Funkcja okreslająca czy zadany kolor jest przez nas rozumiany jako bliższy kolorowi czarnemu / szaremu
     * @param avgColor
     * @return true jeżeli jest wartość piksela < 150 false jeżeli nie
     */
    static bool isBlack(int avgColor);

    /**
     * @brief Wycina mały obszar z dużego obszaru.
     * @param matrix klatka wejściowa
     * @param rect wielkość obszaru do wycięcia
     * @return wycięty obszar
     */
    static cv::Mat getRectFromMatrix(cv::Mat matrix, cv::Rect rect);

    /**
     * @deprecated Nie jest używana
     * @brief Określa czy rozmiary macierzy są równe
     * @param first pierwszy obszar
     * @param second drugi obszsr
     * @return true jęzeli są
     * @return false jeżeli nie są
     */
    static bool canBeCompared(cv::Mat first, cv::Mat second);

    /**
     * @brief Odejmuje macierze.
     * @param first pierwsza klatka
     * @param second druga klatka
     * @return wartość bezwględna różnicy dwóch macierzy
     */
    static cv::Mat getDifferenceMatrix(cv::Mat first, cv::Mat second);

    /**
     * @brief Tworzy histogram różnicy dwóch macierzy i zwraca procent czarnych pikseli / wszystkie piksele macierzy.
     * @param first klatka wejśćiowa
     * @param diff klatka wyjściowa
     * @param limit limit odpowiadający za określenie jak wiele pikseli traktujemy jako bliższe kolorowi czarnemu
     * @return procent różnicy.
     */
    static double calculateDifferencePercentage(cv::Mat first, cv::Mat diff, int limit = 10);
};
