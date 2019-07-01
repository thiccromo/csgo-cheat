#pragma once

class collideable_t {
public:
	Vector3& mins( ) {
		using original_fn = Vector3 & ( __thiscall* )( void* );
		return ( *( original_fn** ) this ) [ 1 ]( this );
	}
	Vector3& maxs( ) {
		using original_fn = Vector3 & ( __thiscall* )( void* );
		return ( *( original_fn** ) this ) [ 2 ]( this );
	}
};