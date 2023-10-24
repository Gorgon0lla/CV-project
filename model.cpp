#include "model.h"
#include <time.h>

#define BoardCOM 8
#define ArduinoCOM 6

Model::Model()
{
    this->Board = new Messenger();
    std::cout << "hhhhdssds\n";
    this->Arduino = new ArduinoCommunicator();
    std::cout << "hhhh\n";
    this->Czas = new Timer; //TODO czas to nie jest dobra nazwa
    this->Camera=new MicroscopeCamera();

    //this->Board->SetSpeed(200); // ustawienie pr�dko�ci plytki

    this->BoardError = false; //Ustawienie bledow jako false
    this->ArduinoError = false;

}

Model::~Model()
{
    delete Board;
    delete Arduino;
    delete Czas;
    delete Camera;

    may_end = true;
}

int Model::getX() {
    return this->x;
}
int Model::getY() {
    return this->y;
}
int Model::getZ() {
    return this->z;
}


void Model::Move(char mode, float X, bool X_M, float Y, bool Y_M, float Z, bool Z_M)
{
    std::cout<<"Odpalono ruch"<<std::endl;
    // Do zmiany funkcja wysylania komedny w klasie CommandCreator oraz w klasie Messenger
    int move_mode=0;
    switch(mode)
    {
    case 'A': //tryb absolutny
        move_mode=1;
        break;
    case 'I':
        move_mode=2;
        break;
    default:

        break;
    }
    std::cout<<"Przed wyslaniem ustawienia trybu"<<std::endl;
    // ustawnienie trybu
    if(!this->Board->SetMode(move_mode))
    {
       // return 1; // zle wyslanie komendy
    }
    std::cout<<"Ustawiono tryb"<<std::endl;
    Sleep(1); //oczekiwanie na odpowiedz

   if(!this->Board->AnswerTaker())
    {
       // return 2; //brak odebrania ok
    }

   if(!this->Board->SetSpeed(200))
   {
       //return 1;
       std::cout<<"Nie wyslano predkosci"<<std::endl;
   }

   std::cout<<"Wyslano predkosc"<<std::endl;

   Sleep(1);

   if(!this->Board->AnswerTaker())
   {
       //return 2;

   }
    // ruch
   if (!this->Board->TraditionalMove(X,Y,Z,0,0,0)) //
   {
      // return 1;
   }
   else
   {
       if(move_mode==1) // przypisanie przy trybie globalnym
       {
           if(X!=0)
           this->x=X;
           if(Y!=0)
           this->y=Y;
           if(Z!=0)
           this->z=Z;
       }
       else if(move_mode==2) // zmiana wartosci przy trybie inkrementalnym
       {
           this->x+=X;
           this->y+=Y;
           this->z+=Z;
       }
   }

   std::cout<<"Wyslano polecenie ruchu"<<std::endl;

   Sleep(1);

   if(!this->Board->AnswerTaker())
    {
       // return 2; //brak odebrania ok
    }

   std::cout<<"Koniec funkcji ruchu"<<std::endl;
  // return 0;
}

bool Model::IsOkForce()
{
   double Force;
   int Counter=0;

    std::cout<<"Rozpoczeto czytanie sily"<<std::endl;
    this->Board->SetMode(2);
    Sleep(20);

   while (true)
   {

       Counter++;
       Force = this->Arduino->AskForForce();
       if (Force >= this->InitialForce)
       {
           std::cout<<"Odrazu poprawna sila"<<std::endl;
          return true; //Czy warunek zosta� spe�niony

       }
       // std::cout<<"Zczytanie nr: "<<Counter<<std::endl;
       this->Board->TraditionalMove(InkrementDocisku, -InkrementDocisku, 0, 0, 0, 0); // dosciskanie probki do szlifierki
       this->x+=InkrementDocisku;
       this->y-=InkrementDocisku;

       Sleep(500);

       if (Counter == 7)
       {
          // std::cout<<"Nie uzyskano poprawnej sily"<<std::endl;
           return false;
           //break;

       }
      // std::cout<<"Zczytanie nr: "<<Counter<<" zakonczone"<<std::endl;
   }


}

int Model::MovingTime(char axis, float desired_position)
{
    int MoveTime=0;
    float axis_pos,distance;

    switch(axis)
    {
    case 'X':
        axis_pos=this->x;
        break;
    case 'Y':
        axis_pos=this->y;
        break;
    case 'Z':
        axis_pos=this->z;
        break;
    default:
        return 0;
        break;
    }

    distance=abs(desired_position-axis_pos);

    MoveTime=int(distance)*250; //czas w ms potrzebny na pokonanie 1mm

    return MoveTime;
}

void Model::MoveOnPosition(int pos_nr)
{
    int DirectZ=0;
    int CommunicationResult=0;


    std::cout<<"Odpalono ruch na pozycje"<<std::endl;

    switch(pos_nr) // wybor stanowiska do szlifowania
    {
    case 1:
        DirectZ = Z_Pierwsze_stanowisko;
        break;
    case 2:
        DirectZ = Z_Drugie_stanowisko;
        break;
    case 3:
        DirectZ = Z_Trzecie_stanowisko;
        break;
    default:
       // return 3; // zly case
        break;
    }

// Przemiesczenie sie nad wybrane stanowisko
    this->Board->Cleaner();
    this->Move('A',0,false,0,false,DirectZ,true);

    if(CommunicationResult!=0)
    {
     //   return CommunicationResult; // jesli zaszedl jakis blad w komunikacji zwracamy jego wartosc
    }

    std::cout<<"Ruszono Z"<<std::endl;
//Oczekiwanie zegara na przesuniecie

  // auto Z_sleepTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(this->MovingTime('Z',DirectZ));
  // std::this_thread::sleep_until(Z_sleepTime);
   // int T1=time(NULL);
    // Sleep(this->MovingTime('Z',DirectZ));
    //std::cout<<time(NULL)-T1<<std::endl;
   std::cout<<"Skonczono przemieszczac Z"<<std::endl;


// Wstepne podjechanie pod szlifierke
    this->Board->Cleaner();
    this->Move('A',10,true,50,true,0,false);

    if(CommunicationResult!=0)
    {
       // return CommunicationResult; // jesli zaszedl jakis blad w komunikacji zwracamy jego wartosc
    }
    std::cout<<"Ruszono X i Y"<<std::endl;

//Oczekiwanie zegara na przesuniecie
    //stary sposob na szlifowanie
     //int EarlyMoveTime=std::max(this->MovingTime('X',100),this->MovingTime('Y',20)); // czas potrzebny na wst�pne doci�ni�cie pr�bki
    int EarlyMoveTime=0; //czas potrzebny na wstępne docisniecie probki
     if(this->MovingTime('X',10)>this->MovingTime('Y',50))
     {
         EarlyMoveTime=this->MovingTime('X',10);
     }
     else
     {
         EarlyMoveTime=this->MovingTime('Y',50);
     }


    //Sleep(EarlyMoveTime);
    auto XY_sleepTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(EarlyMoveTime);
     std::this_thread::sleep_until(XY_sleepTime);

     std::cout<<"Skonczono ruszac X i Y"<<std::endl;

// Dociskanie probki
    //std::cout<<"SSSSSSSSSSSSSSSS\n";
    if(!this->IsOkForce())
    {
     //   return 3; // blad dociskania do plytki ,przekrocozna liczba iteracji
    }
    else
    {
      //  return 0; // probka gotowa do szlifowania
    }
    std::cout<<"Zrobiono z sila"<<std::endl;


}

void Model::FirstPosition()
{
    std::cout<<"Start"<<std::endl;
    int grind_time = this->grind_time_1*1000; // pobranie wartosci czasu szlifowania


    std::thread PosThread([this]{this->MoveOnPosition(1);});
    PosThread.join();
    std::cout<<"Skonczono przemieszczac na pozycje"<<std::endl;



    if(!this->Arduino->GrindStart('1'))
    {
//        return 1;
    }

    auto CzasSzlifu=std::chrono::steady_clock::now()+std::chrono::milliseconds(grind_time);

    std::this_thread::sleep_until(CzasSzlifu);



    if(!this->Arduino->GrindStop())
    {
  //      return 2; // niepoprawne wylaczenie
    }

   // return 0;
    std::cout<<"Koniec"<<std::endl;
}

void Model::SecondPosition()
{
    int grind_time = this->grind_time_2*1000; // pobranie wartosci czasu szlifowania

    this->MoveOnPosition(2);

    if(!this->Arduino->GrindStart('2'))
    {
      //  return 1;
    }

    auto CzasSzlifu=std::chrono::steady_clock::now()+std::chrono::milliseconds(grind_time);

    std::this_thread::sleep_until(CzasSzlifu);

    if(!this->Arduino->GrindStop())
    {
       // return 2; // niepoprawne wylaczenie
    }

   // return 0;

}

void Model::ThirdPosition()
{
    int grind_time = this->grind_time_3*1000; // pobranie wartosci czasu szlifowania

    this->MoveOnPosition(3);


    if(!this->Arduino->GrindStart('3'))
    {
       // return 1;
    }

    // TODO CzasSzlifu lepiej zast�pi� nazw� anigielsk�
    auto CzasSzlifu=std::chrono::steady_clock::now()+std::chrono::milliseconds(grind_time);

    std::this_thread::sleep_until(CzasSzlifu);

    if(!this->Arduino->GrindStop())
    {
       // return 2; // niepoprawne wylaczenie
    }

    //return 0;

}

 void Model::FirstPosition_Button()
 {
     std::cout<<"FirstButtonClicked"<<std::endl;
     std::thread FirstPositionThread(std::bind(&Model::FirstPosition, this)); // watek uruchamiajacy dzialanie stanowiska pierwszego
     FirstPositionThread.detach();

 }

 void Model::SecondPosition_Button()
 {
     std::thread SecondPositionThread(std::bind(&Model::SecondPosition, this)); // watek uruchamiajacy dzialanie stanowiska pierwszego
     SecondPositionThread.detach();
 }

 void Model::ThirdPosition_Button()
 {
     std::thread ThirdPositionThread(std::bind(&Model::ThirdPosition, this)); // watek uruchamiajacy dzialanie stanowiska pierwszego
     ThirdPositionThread.detach();
 }

void Model::autoHome_Button()
{
    this->Board->AutoHome();
}

void Model::connect_Button()
{
    bool connectBoard=this->Board->ConnectBoard(this->boardCOMport,this->boardBaudRate);
    //std::cout<<this->boardCOMport;
    bool connectArduino=this->Arduino->ConnectArduino(this->arduinoCOMport,this->arduinoBaudRate);


    if (connectBoard==false)
    {
        this->BoardError=true;
        std::cout<<"Nie połaczyla sie plyta od drukarki"<<std::endl;
    }
    else if (connectArduino==false)
    {
        this->ArduinoError=true;
        std::cout<<"Nie polaczylo sie arduino"<<std::endl;
    }
    else
    {
        std::cout<<"Polaczono"<<std::endl;
    }

}


void Model::cameraOn()
{
    std::cout<<"Wywolanie funkcji kamery"<<std::endl;
   this->Camera->CameraVisionActivate();
    //this->Camera->CameraVision();
}
void Model::cameraOff()
{
    this->Camera->CameraVisionDown();
}

void Model::takePhoto()
{
    this->Camera->TakePhoto("failedPhoto",true);
}
void Model::printThread()
{
    std::thread Watek(std::bind(&Model::print,this));
    Watek.detach();
}
void Model::print()
{
    this->Board->Cleaner();
}

