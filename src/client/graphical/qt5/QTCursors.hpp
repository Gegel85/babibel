/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTCursor.hpp
*/

#ifndef QTCURSOR_HPP
#define QTCURSOR_HPP

#include <QtWidgets>
namespace BabelClient {
	enum cursors {
		ARROWCURSOR = Qt::ArrowCursor,
		UPARROWCURSOR = Qt::UpArrowCursor,
		CROSSCURSOR = Qt::CrossCursor,
		WAITCURSOR = Qt::WaitCursor,
		IBEAMCURSOR = Qt::IBeamCursor,
		SIZEVERCURSOR = Qt::SizeVerCursor,
		SIZEHORCURSOR = Qt::SizeHorCursor,
		SIZEBDIAGCURSOR = Qt::SizeBDiagCursor,
		SIZEFDIAGCURSOR = Qt::SizeFDiagCursor,
		SIZEALLCURSOR = Qt::SizeAllCursor,
		BLANKCURSOR = Qt::BlankCursor,
		SPLITVCURSOR = Qt::SplitVCursor,
		SPLITHCURSOR = Qt::SplitHCursor,
		POINTINGHANDCURSOR = Qt::PointingHandCursor,
		FORBIDDENCURSOR = Qt::ForbiddenCursor,
		OPENHANDCURSOR = Qt::OpenHandCursor,
		CLOSEHANDCURSOR = Qt::ClosedHandCursor,
		WHATDTHISCURSOR = Qt::WhatsThisCursor,
		BUSYCURSOR = Qt::BusyCursor
	};
}
#endif