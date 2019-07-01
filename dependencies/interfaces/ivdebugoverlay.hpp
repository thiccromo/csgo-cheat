#pragma once
#include "../../source-sdk/math/vector3d.hpp"

class iv_debug_overlay {
public:
	bool world_to_screen( const Vector3& in, Vector3& out ) {
		using original_fn = int( __thiscall* )( iv_debug_overlay*, const Vector3&, Vector3& );
		int return_value = ( *( original_fn** ) this ) [ 13 ]( this, in, out );
		return static_cast< bool >( return_value != 1 );
	}
};