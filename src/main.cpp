/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main.cpp
*/

#include <stdio.h>
#include <opus/opus.h>
#include <portaudio.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include "qt5/Cursors.hpp"

class Window : public QWidget
{
    public:
        Window() : QWidget()
        {
            this->setFixedSize(300, 150);

            this->_button = new QPushButton("Quitter", this);

            this->_button->setFont(QFont("Comic Sans MS", 14));
            this->_button->setCursor(Qt::PointingHandCursor);
            this->_button->setIcon(QIcon("./assets/images/02_grin.png"));
            this->_button->move(110, 50);
            setFixedSize(300, 150);

            // Connexion du clic du bouton à la fermeture de l'application
            QObject::connect(this->_button, SIGNAL(clicked()), qApp, SLOT(quit()));
            this->_lcd = new QLCDNumber(this);
            this->_lcd->setSegmentStyle(QLCDNumber::Flat);
            this->_lcd->move(50, 20);

            this->_slider = new QSlider(Qt::Horizontal, this);
            this->_slider->setGeometry(10, 90, 150, 20);
            QObject::connect(this->_slider, SIGNAL(valueChanged(int)), this->_lcd, SLOT(display(int)));
        };
        ~Window() {
            if (this->_button)
                delete (this->_button);
        };

    private:
        QPushButton *_button;
        QLCDNumber *_lcd;
        QSlider *_slider;
    };

int main(int argc, char **argv)
{
/*
    QApplication app (argc, argv);
    QWidget window;
    window.setFixedSize(300, 150);

    QPushButton button ("Hello world !", &window);
    button.setFont(QFont("Comic Sans MS", 25));
    button.setToolTip("C'est un bouton magique! *clin d'oeil*");
    button.setIcon(QIcon("./assets/images/02_grin.png"));
    button.setCursor(Qt::PointingHandCursor);
    button.setGeometry(10, 50, 180, 70);

    QPushButton otherButton("Autre bouton", &button);
    otherButton.move(50, 15);
    otherButton.setFont(QFont("", 11));

    window.show();
    return app.exec();
*/

    QApplication app(argc, argv);

    Window myWindow;
    myWindow.show();

    return app.exec();

}
