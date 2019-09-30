/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main.cpp
*/

#include <stdio.h>
#include <iostream>
#include <thread>
#include <opus/opus.h>
#include <portaudio.h>
#include "graphical/GUIScreen.hpp"
#include "graphical/qt5/myQTWindow.hpp"
#include "graphical/qt5/QTApplication.hpp"

int main(int argc, char **argv)
{
	Babel::QTApplication app(argc, argv);
	Babel::myQTWindow myWindow({1000, 500});

	myWindow.window.show();
	return app.launch();
}

/*
int main(int argc, char **argv)
{
	Babel::TempWindow	window{{640, 480}};

	do {
		//Some stuff
	} while (window.refresh());
}
*/

/*
namespace Babel
{
	class TempWindow : public GUIScreen {
	private:
		Vector2<unsigned int> _size;

	public:
		TempWindow(Vector2<unsigned int> size) : GUIScreen(), _size{size} {};

		Vector2<unsigned int> getSize() const override
		{
			return this->_size;
		};

		virtual void setFullscreen(bool fullscreen = true) override
		{
			std::cout << "Fullscreen is now " << (fullscreen ? "on" : "off") << std::endl;
		};

		virtual void setSize(Vector2<unsigned int> size)
		{
			std::cout << "New window size (" << size.x << ", " << size.y << ")" << std::endl;
		};

		virtual bool refresh()
		{
			std::cout << "Rendering objects" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(17));
			return true;
		};
	};
}
*/