#include "controller.h"
#include <QGridLayout>
#include <QTextEdit>
#include <string>
#include <QString>
#include <iostream>
#include <QObject>
#include <QPushButton>
#include <QtConcurrent/QtConcurrent>

Controller::Controller(QWidget *parent) : QWidget(parent)
{
    this->ui.setupUi(this);

    this->model = new Model();

    for (int i=0;i<12;i++)
    {
      this->ui.boardComButton->addItem("COM"+QString::number(i));
      this->ui.arduinoComButton->addItem("COM"+QString::number(i));
    }

    //Standard baud rates include 110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000 and 256000 bits per second.

    this->ui.boardBaudButton->addItem("110");
    this->ui.boardBaudButton->addItem("300");
    this->ui.boardBaudButton->addItem("600");
    this->ui.boardBaudButton->addItem("1200");
    this->ui.boardBaudButton->addItem("4800");
    this->ui.boardBaudButton->addItem("9600");
    this->ui.boardBaudButton->addItem("14400");
    this->ui.boardBaudButton->addItem("19200");
    this->ui.boardBaudButton->addItem("38400");
    this->ui.boardBaudButton->addItem("57600");
    this->ui.boardBaudButton->addItem("115200");
    this->ui.boardBaudButton->addItem("128000");
    this->ui.boardBaudButton->addItem("256000");

    this->ui.arduinoBaudButton->addItem("110");
    this->ui.arduinoBaudButton->addItem("300");
    this->ui.arduinoBaudButton->addItem("600");
    this->ui.arduinoBaudButton->addItem("1200");
    this->ui.arduinoBaudButton->addItem("4800");
    this->ui.arduinoBaudButton->addItem("9600");
    this->ui.arduinoBaudButton->addItem("14400");
    this->ui.arduinoBaudButton->addItem("19200");
    this->ui.arduinoBaudButton->addItem("38400");
    this->ui.arduinoBaudButton->addItem("57600");
    this->ui.arduinoBaudButton->addItem("115200");
    this->ui.arduinoBaudButton->addItem("128000");
    this->ui.arduinoBaudButton->addItem("256000");




//    QObject::connect(model, &Model::resultReady, this, &Controller::update_ui);

   // QObject::connect(ui.btnHome,&QPushButton::clicked,model,&Model::autoHome_Button);
   // QObject::connect(ui.btnPos1,&QPushButton::clicked,model,&Model::FirstPosition_Button);
   // QObject::connect(ui.btnPos2,&QPushButton::clicked,model,&Model::SecondPosition_Button);
   // QObject::connect(ui.btnPos3,&QPushButton::clicked,model,&Model::ThirdPosition_Button);
}

Controller::~Controller() {
    delete model;
}

// alternative assignment of buttons
void Controller::on_btnHome_clicked()
{
    model->autoHome_Button();
}
void Controller::on_btnPos1_clicked()
{
    this->model->grind_time_1=std::stoi(ui.grindTime_1->text().toStdString());
    this->model->FirstPosition();

}
void Controller::on_btnPos2_clicked()
{
    this->model->grind_time_2=std::stoi(ui.grindTime_2->text().toStdString());
    model->SecondPosition();
}
void Controller::on_btnPos3_clicked()
{
    this->model->grind_time_3=std::stoi(ui.grindTime_3->text().toStdString());
    model->ThirdPosition();
}

void Controller::update_ui(QString position) {
    // TODO add parsing when position is not trivial(will be implemented)
    ui.x_dis->setText(position);
    ui.y_dis->setText(position);
    ui.z_dis->setText(position);
}

void Controller::on_connectButton_clicked()
{

    std::string strBoardPort=this->ui.boardComButton->currentText().toStdString();
    std::string strBoardBaudrate=this->ui.boardBaudButton->currentText().toStdString();

    std::string strArduinoPort=this->ui.arduinoComButton->currentText().toStdString();
    std::string strArduinoBaudrate=this->ui.arduinoBaudButton->currentText().toStdString();


    int boardPort=std::stoi(strBoardPort.substr(3,4));
    boardPort=boardPort-1;
    int boardBaudrate=std::stoi(strBoardBaudrate);

    int arduinoPort=std::stoi(strArduinoPort.substr(3,4));
    arduinoPort=arduinoPort-1;
    int arduinoBaudrate=std::stoi(strArduinoBaudrate);


    this->model->boardBaudRate=boardBaudrate;
    this->model->boardCOMport=boardPort;
    this->model->arduinoBaudRate=arduinoBaudrate;
    this->model->arduinoCOMport=arduinoPort;

    this->model->connect_Button();

}


void Controller::on_moveButton_clicked()
{
    //std::cout<<"llllllll";
    std::thread movingThread(std::bind(&Controller::moveThread,this));
    movingThread.detach();
}

void Controller::on_actCamera_clicked()
{
    //std::cout<<"Kliknieto przycisk kamery"<<std::endl;
    this->model->cameraOn();
}

void Controller::on_photoButton_clicked()
{
    this->model->takePhoto();
}

void Controller::moveThread()
{
    float x=this->ui.des_x->text().toFloat(); //konwetowanie z tekstu na typ float
    float y=this->ui.des_y->text().toFloat();
    float z=this->ui.des_z->text().toFloat();

    bool x_move=false;
    bool y_move=false;
    bool z_move=false;

    if(x!=model->getX())
    {
        x_move=true;
    }
    if(y!=model->getY())
    {
        y_move=true;
    }
    if(z!=model->getZ())
    {
        z_move=true;
    }

   int waitTimeX=model->MovingTime('X',x);
   int waitTimeY=model->MovingTime('Y',y);
   int waitTimeZ=model->MovingTime('Z',z);

   this->model->Move('A',x,x_move,y,y_move,z,z_move);


   int waitTime=std::max(waitTimeX,waitTimeY);
    waitTime=std::max(waitTimeZ,waitTime);

    std::cout<<waitTime<<std::endl;

    auto runTime=std::chrono::steady_clock::now()+std::chrono::milliseconds(waitTime);

    std::this_thread::sleep_until(runTime);

}
void Controller::on_pushButton_clicked()
{
    std::cout<<"kliknieto"<<std::endl;
    this->model->printThread();
}
void Controller::on_deactCamera_clicked()
{
    this->model->cameraOff();
}



