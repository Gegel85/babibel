/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Vector2.hpp
*/

template <typename T>
struct Vector2 {
	T x;
	T y;

	template <typename type>
	Vector2<T> &operator=(Vector2<type> &vec) {
		this->x = static_cast<type>(vec.x);
		this->y = static_cast<type>(vec.y);
		return *this;
	}

	template <typename type>
	operator Vector2<type>() {
		return {
			static_cast<type>(this->x),
			static_cast<type>(this->y)
		};
	}
};

template <typename T>
struct Vector4 {
	T x;
	T y;
	T w;
	T h;

	template <typename type>
	Vector4<T> &operator=(Vector4<type> &vec) {
		this->x = static_cast<type>(vec.x);
		this->y = static_cast<type>(vec.y);
		this->w = static_cast<type>(vec.w);
		this->h = static_cast<type>(vec.h);
		return *this;
	}

	template <typename type>
	operator Vector4<type>() {
		return {
			static_cast<type>(this->x),
			static_cast<type>(this->y),
			static_cast<type>(this->w),
			static_cast<type>(this->h)
		};
	}
};

typedef Vector2<double> Point;


#endif