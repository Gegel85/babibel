//
// EPITECH PROJECT, 2019
// babel
// File description:
//
//

#include <stdio.h>
#include <opus/opus.h>
#include <portaudio.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    QPushButton button ("Hello world !");

    button.show();
    return app.exec();
}
