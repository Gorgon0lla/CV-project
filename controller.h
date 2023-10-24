#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <ui_controller_form.h>
#include <model.h>

#include <QtConcurrent/QtConcurrent>


class Controller : public QWidget
{
    Q_OBJECT
public:
    explicit Controller(QWidget *parent = nullptr);
    ~Controller();

private slots:
    void on_btnHome_clicked();
    void on_btnPos1_clicked();
    void on_btnPos2_clicked();
    void on_btnPos3_clicked();
    void on_connectButton_clicked();
    void on_moveButton_clicked();
    void update_ui(QString position);
    void on_actCamera_clicked();
    void on_photoButton_clicked();
    void moveThread();
    void on_pushButton_clicked();
    void on_deactCamera_clicked();

private:
    Ui::controllerForm ui;
    Model *model;

};

#endif // CONTROLLER_H
