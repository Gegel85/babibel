/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Picture.cpp
*/

#include "Picture.hpp"

namespace Babel::Client {
	Picture::Picture(const std::string &path, Vector2<int> position, Vector2<unsigned> size) :
		Widget(position, size),
		_path(path)
	{
	}
}