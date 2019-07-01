#pragma once
#include "../math/vector3d.hpp"

struct model_t;

struct model_render_info_t {
	Vector3 origin;
	Vector3 angles;
	char pad [ 0x4 ]; // added this
	void *renderable; // this
	const void *model; // and this
	const matrix_t* model_to_world;
	const matrix_t* lighting_offset;
	const Vector3* lighting_origin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	unsigned short instance;

	model_render_info_t( ) {
		model_to_world = NULL;
		lighting_offset = NULL;
		lighting_origin = NULL;
	}
};