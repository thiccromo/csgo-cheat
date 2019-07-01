#pragma once
#include "../math/vector2d.hpp"
struct vertex_t {
	vertex_t( ) {}
	vertex_t( const Vector2&pos, const Vector2&coord = Vector2( 0, 0 ) ) {
		position = pos;
		tex_coord = coord;
	}
	void initialize( const Vector2 &pos, const Vector2 &coord = Vector2( 0, 0 ) ) {
		position = pos;
		tex_coord = coord;
	}

	Vector2	position;
	Vector2	tex_coord;
};
