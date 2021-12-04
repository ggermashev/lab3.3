#pragma once

#include <QtWidgets/QWidget>
#include "ui_lab33qt.h"
#include <QGraphicsTextItem>
#include <qmainwindow.h>
#include "Graph.h"
#include <qtextedit.h>
#include <qgridlayout.h>


class Window : public QWidget
{
    Q_OBJECT
public:
    Window(Graph* g, QWidget* parent = Q_NULLPTR);
private:
    Ui::lab33qtClass* ui;
};


class lab33qt : public QWidget
{
    Q_OBJECT

public:
    lab33qt(QWidget *parent = Q_NULLPTR);
    void drawLine(int x1, int y1, int x2, int y2, bool shortest = false);
    void drawCircle(int x1, int y1,int colour = -1, int r = 15);
private:
    Ui::lab33qtClass* ui;
    
    Graph* g;
    int r;
    ListVector<int>* way;
    QTextEdit* intWay;
    QGridLayout* grid;
    Window* neww;

    int GetX(int n);
    int GetY(int n);
public slots:
    void inputFromFile();
    void randomGeneration();
    void paint();
    void shortestWay();
    void showInfo();

protected:
    void paintEvent(QPaintEvent* event);
};

