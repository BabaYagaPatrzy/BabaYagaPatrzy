# Dokumentacja projektu

Aby wyświetlić dokumentację projektu musimy wejść w folder html. W nim włączamy plik index.html. Powinna ukazać nam się dokumentacja wygenerowana za pomocą doxygena.

# Uruchomienie projektu

Aby uruchomic projekt w programie visual studio 2019 przechodzimy do folderu build i uruchamiamy plik BabaYaga.sln.
Z poziomu visual studio musimy ustawić dwie rzeczy:

- po pierwsze w eksploratorze rozwiązań klikamy prawy przycisk myszy na BabaYaga i ustawiamy jako projekt startowy
- po drugie klikamy na BabaYaga -> właściwości -> włąściwości konfiguracji -> debugowanie -> Katalog roboczy -> przeglądaj i wskazujemy na folder Resources naszego projektu
  Program włączy się w trybie filmu jednakże projekt działa też w czasie rzeczywistym do czego potrzebna jest kamera z laptopa / telefon zparowany z komputerem za pomocą aplikacji iriun webcam, oczywiśćie potrzebne są odpowiednie plakietki graczy.

# OpenCV

Aby program zadziałał powinniśmy mieć pobraną na komputer bibliotekę open cv w wersji 4.5.4
Znaleźć ją możemy pod linkiem: https://opencv.org/releases/
W pliku CMakeLists.txt mamy ustawienie
set(OpenCV_DIR "C:/.opencv/build")
"C:/.opencv/build" zastępujemy lokacją gdzie znajduje się rozpakowany open cv
