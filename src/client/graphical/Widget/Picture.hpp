/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Picture.hpp
*/

#ifndef PICTURE_HPP
#define PICTURE_HPP

#include "Widget.hpp"

namespace BabelClient
{
	class Picture : public Widget {
	protected:
		const std::string &_path;

	public:
		Picture(const std::string &path, Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
	};
}
#endif