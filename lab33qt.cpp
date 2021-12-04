#include "lab33qt.h"
#include "ui_lab33qt.h"
#include <qpainter.h>
#include <fstream>
#include <qlabel.h>
#include <qlayout.h>
#include <qdialog.h>
#include <QLineEdit>
#include <qpushbutton.h>
#include <qinputdialog.h>
#include <QTextEdit>

#include "Graph.h"
//#include "Graph.cpp"

#define PI 3.1415926535

lab33qt::lab33qt(QWidget *parent) //сделать деструктор g
    : QWidget(parent), ui(new Ui::lab33qtClass)
{
    ui->setupUi(this);
    
    g = new Graph();
    way = nullptr;
    neww = nullptr;
    r = 500;

    grid = new QGridLayout(this);
    QPushButton* ifile = new QPushButton();
    ifile->setText("FileInput");
    QPushButton* rand = new QPushButton();
    rand->setText("RandomGraph");
    QPushButton* pnt = new QPushButton();
    pnt->setText("Paint");
    QPushButton* way = new QPushButton();
    way->setText("ShortestWay");
    QPushButton* info = new QPushButton();
    info->setText("info");

    intWay = new QTextEdit();
    intWay->setFixedHeight(30);
    intWay->setFixedWidth(100);
    //intWay->setAlignment(Qt::AlignLeft);


    grid->addWidget(ifile,0,0);
    grid->addWidget(rand,1,0);
    grid->addWidget(pnt,2,0);
    grid->addWidget(way, 3, 0);
    grid->addWidget(info,5,0);
    grid->addWidget(intWay,4,0);

    grid->setAlignment(Qt::AlignLeft);
    setLayout(grid);


    connect(ifile, SIGNAL(clicked()), this, SLOT(inputFromFile()));
    connect(rand, SIGNAL(clicked()), this, SLOT(randomGeneration()));
    connect(pnt, SIGNAL(clicked()), this, SLOT(paint()));
    connect(way, SIGNAL(clicked()), this, SLOT(shortestWay()));
    connect(info, SIGNAL(clicked()), this, SLOT(showInfo()));
    //inputFromFile();
    //randomGeneration();
   // g->Paint();
   // shortestWay();
    
    //repaint();
}

void lab33qt::drawLine(int x1, int y1, int x2, int y2, bool shortest)
{
    std::ofstream outf("Output.txt");
    QPainter painter(this);
    if (shortest == false) painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    else painter.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawLine(x1, y1, x2, y2);
    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
    
    double k;
    if (x1 != x2) k = (double)(y2 -y1) / (double) (x2 - x1);
    else k = -1;
    int x3, y3;
    int dx, dy;
    if (x2 > x1) dx = 15;
    else
        if (x2 < x1) dx = -15;
        else
            if (y2 > y1) dy = 15;
            else dy = -15;

    if (x1 != x2)
    {
        y3 = y2 + round(k * dx);
        x3 = x2 + dx;
    }
    else if (y1 != y2)
    {
        x3 = x2;
        y3 = y2 + dy;
    }
    outf << k;
    painter.drawLine(x2, y2, x3, y3);
}

void lab33qt::drawCircle(int x1, int y1, int colour, int r)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    if (colour != 0)
    {
       //if (colour == 0) painter.setBrush(Qt::black); else
       if (colour == 1) painter.setBrush(Qt::red); else
       if (colour == 2) painter.setBrush(Qt::green); else
       if (colour == 3) painter.setBrush(Qt::blue); else
       if (colour == 4) painter.setBrush(Qt::cyan); else
       if (colour == 5) painter.setBrush(Qt::magenta); else
       if (colour == 6) painter.setBrush(Qt::yellow); else
       if (colour == 7) painter.setBrush(Qt::gray); else
       if (colour == 8) painter.setBrush(Qt::darkRed); else
       if (colour == 9) painter.setBrush(Qt::darkGreen); else
       if (colour == 10) painter.setBrush(Qt::darkBlue); else
       if (colour == 11) painter.setBrush(Qt::darkCyan); else
       if (colour == 12) painter.setBrush(Qt::darkMagenta); else
       if (colour == 13) painter.setBrush(Qt::darkYellow); else
       if (colour == 14) painter.setBrush(Qt::darkGray); else
           painter.setBrush(Qt::white);
    }
    painter.drawEllipse(x1-r/2, y1-r/2, r, r);
   
}

void lab33qt::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    //std::ofstream outf("Output.txt");

    for (int i = 0; i < g->GetSize(); i++)
    {
        int x = GetX(i);
        int y = GetY(i);
        drawCircle(x, y, g->GetColour(i));
    }

    for (int i = 0; i < g->GetSize(); i++)
    {
        ArrayVector<Link>* vec = g->GetLinks(i);
        for (int j = 0; j < vec->GetLength(); j++)
        {
            int x1, y1, x2, y2;
            x1 = GetX(i);
            y1 = GetY(i);
            x2 = GetX(vec->Get(j).num);
            y2 = GetY(vec->Get(j).num);
           //outf << i << " " << vec->Get(j).num  << "\n";
            drawLine(x1, y1, x2, y2);
        }
        vec = nullptr;
    }

    if (way) 
        for (int i = 0; i < way->GetLength() - 1; i++)
        {
            int x1 = GetX(way->Get(i));
            int y1 = GetY(way->Get(i));
            int x2 = GetX(way->Get(i + 1));
            int y2 = GetY(way->Get(i + 1));
            drawLine(x1, y1, x2, y2,true);
        }
}

int lab33qt::GetX(int k)
{
    double al = 2 * PI / g->GetSize();

    return r + 150 + (int)round(r * cos(al * k));
}

int lab33qt::GetY(int k)
{
    double al = 2 * PI / g->GetSize();

    return r + 50 + (int) round(r * sin(al * k));
}

void lab33qt::inputFromFile()
{
    if (g->GetSize() != 0) 
    {
        g->Clear();
        g = new Graph();
    }
    std::ifstream inf("Input.txt");
    int n, k, c, j;
    inf >> n;
    for (int i = 0; i < n; i++)
    {
        inf >> k;
        for (int f = 0; f < k; f++)
        {
            inf >> j;
            inf >> c;
            g->Add(i, j, c);
        }
    }
    if (way)
    {
        way->Clear();
        way = nullptr;
        intWay->clear();
    }

    if (neww)
    {
        //delete neww;
        //neww = nullptr;
    }
    neww = new Window(g);

    repaint();
}

void lab33qt::randomGeneration()
{
    if (g->GetSize() != 0) 
    {
        g->Clear();
        g = new Graph();
    }
    int v = QInputDialog::getInt(0, "Input", "Vertices:", QLineEdit::Normal);
    int e = QInputDialog::getInt(0, "Input", "Edges:", QLineEdit::Normal);
    g->GenerateGraph(v, e);
    if (way)
    {
        way->Clear();
        way = nullptr;
        intWay->clear();
    }

    if (neww)
    {
       // delete neww;
       // neww = nullptr;
    }
    neww = new Window(g);

    repaint();
}

void lab33qt::paint()
{
    g->Paint();
    repaint();
}

void lab33qt::shortestWay()
{
    if (way)
    {
        way->Clear();
        way = nullptr;
    }
    int num1 = QInputDialog::getInt(0, "Input", "From:", QLineEdit::Normal);
    int num2 = QInputDialog::getInt(0, "Input", "To:", QLineEdit::Normal);
    way = g->GetShortestWay(num1,num2,way);
    int wayInt = g->GetShortestWayValue(num1, num2);
    QString* s = new QString();
    QString value;
    value.setNum(num1);
    s->append(value);
    s->append("->");
    value.setNum(num2);
    s->append(value);
    s->append(": ");
    value.setNum(wayInt);
    s->append(value);

    intWay->setText(*s);
    //grid->addWidget(intWay, 4, 0);

    repaint();
}


void lab33qt::showInfo()
{
    if (neww) neww->show();
}

Window::Window(Graph* g, QWidget* parent)
    :QWidget(parent), ui(new Ui::lab33qtClass)
{
    QString* s = new QString();
    QString value;
    QTextEdit* text = new QTextEdit();
    s->append("numeration starts in 0-angle with x-axis and continues clockwise \n");
    for (int i = 0; i < g->GetSize(); i++)
    {
        value.setNum(i);
        s->append(value);
        s->append(":  ");
        ArrayVector<Link>* vec = g->GetLinks(i);
        for (int j = 0; j < vec->GetLength(); j++)
        {
            s->append("(");
            value.setNum(vec->Get(j).num);
            s->append(value);
            s->append(",");
            value.setNum(vec->Get(j).weight);
            s->append(value);
            s->append(") ");
        }
        s->append("\n");
    }
    text->setText(*s);
    QGridLayout* gr = new QGridLayout();
    gr->addWidget(text);

    setLayout(gr);
}