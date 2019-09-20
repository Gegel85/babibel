/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTButton.hpp
*/

#ifndef QTBUTTON_HPP
#define QTBUTOON_HPP

#include <QtWidgets/QPushButton>
#include "Cursors.hpp"
#include "data/Vector2.hpp"

class QTButton {
	public:
		Button(std::string text = "", Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0} , QWidget *parent = nullptr);
		~Button() = default;

		void setText(const std::string &text);
		const std::string getText();

		void setToolTip(const std::string &toolTip);
		const std::string getToolTip();

		void setIcon(const std::string &path);
		void setIcon(const QIcon &otherIcon);
		const QIcon getIcon();

		void setFont(std::string fontName, unsigned int size);

		void setCursor(enum cursors);
		const enum cursors getCursor();

		void setPosition(const Vector2<unsigned int> &position);
		const Vector2<unsigned int> getposition();
		void setGeometry(Vector4<unsigned int> geometry); //(abscisse, ordonnée, largeur, hauteur)
		const Vector4<unsigned int> getGeometry();


	private:
		std::string _text;
};

#endif