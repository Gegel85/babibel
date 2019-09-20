/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Picture.cpp
*/

#include "Picture.hpp"

namespace Babel {
	Picture::Picture(const std::string &path, Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0}) :
		Widget(position, size),
		_path(path)
	{
	}
}