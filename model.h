#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QThread>
#include <atomic>

#include "V1/SterowanieKonsola/ArduinoCommunicator.h"
#include "V1/SterowanieKonsola/Messenger.h"
#include "V1/SterowanieKonsola/timer.h"
#include "V1/SterowanieKonsola/MicroscopeCamera.h"

// Ustawienie pozycji Z stanowisk
#define Z_Pierwsze_stanowisko 100
#define Z_Drugie_stanowisko 200
#define Z_Trzecie_stanowisko 300

// Ustawienie inkrementacji docisku
#define InkrementDocisku 5
class Model
{

private:
    int x{}, y{}, z{};

    float current_force;
    double InitialForce = 20; //Minimalna sila docisku

    bool may_end{false};

    bool BoardError;
    bool ArduinoError;

    bool OkForce;


    ArduinoCommunicator* Arduino;
    Messenger * Board;
    MicroscopeCamera* Camera;
    Timer* Czas;

public:
    Model();
    ~Model();

    int getX();
    int getY();
    int getZ();

    int grind_time_1, grind_time_2, grind_time_3;

    int boardBaudRate;
    int boardCOMport;
    int arduinoBaudRate;
    int arduinoCOMport;

    bool IsOkForce();

    void Move(char mode,float X,bool X_M,float Y,bool Y_M,float Z,bool Z_M);
    /*
     * Opis zwracanych wartosci:
     * 0 - wszystko przebieglo pomyslnie
     * 1 - blad wyslania komendy od plytki
     * 2 - blad odebrania komendy od plytki
     *
     */


    void MoveOnPosition(int pos_nr); // numer stanowiska nad ktore ruszamy durkarke
    /*
     * Opis zwracanych wartosci:
     * 0 - wszystko przebieglo pomyslnie
     * 1 - blad wyslania komendy od plytki
     * 2 - blad odebrania komendy od plytki
     * 3 - blad dociskania do plytki
     *
     */

    void FirstPosition(); // funckcje odpalane za pomocą wątków
    void SecondPosition();
    void ThirdPosition();


    int MovingTime(char axis,float desired_position); // zwraca czas w jakim maszyna przemiesci probke na dana pozycje


    // Funkcje pod przyciski--------------------------------------

    void FirstPosition_Button();
    void SecondPosition_Button();
    void ThirdPosition_Button();
    void autoHome_Button();
    void connect_Button();
    void cameraOn();
    void cameraOff();
    void takePhoto();
    //-----------------------------------------------------------
//    void resultReady(QString &result);

    void print();
    void printThread();

};

#endif // MODEL_H
