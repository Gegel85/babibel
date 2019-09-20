/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Picture.hpp
*/

#ifndef PICTURE_HPP
#define PICTURE_HPP

#include "Widget.hpp"

namespace Babel {

class Picture : public Widget
{
protected:
	const std::string &_path;

public:
	Picture(const std::string &path);
};
}
#endif