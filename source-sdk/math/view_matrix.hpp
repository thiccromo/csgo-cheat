#pragma once
#include "vector3d.hpp"

typedef float vec_t;

class view_matrix_t {
public:

	view_matrix_t();
	view_matrix_t(
		vec_t m00, vec_t m01, vec_t m02, vec_t m03,
		vec_t m10, vec_t m11, vec_t m12, vec_t m13,
		vec_t m20, vec_t m21, vec_t m22, vec_t m23,
		vec_t m30, vec_t m31, vec_t m32, vec_t m33
	);

	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	view_matrix_t(const Vector3& forward, const Vector3& left, const Vector3& up);

	// Construct from a 3x4 matrix
	view_matrix_t(const matrix_t& matrix3x4);

	// Set the values in the matrix.
	void		init(
		vec_t m00, vec_t m01, vec_t m02, vec_t m03,
		vec_t m10, vec_t m11, vec_t m12, vec_t m13,
		vec_t m20, vec_t m21, vec_t m22, vec_t m23,
		vec_t m30, vec_t m31, vec_t m32, vec_t m33
	);


	// Initialize from a 3x4
	void		init(const matrix_t& matrix3x4);

	// array access
	inline float* operator[](int i) {
		return m[i];
	}

	inline const float* operator[](int i) const {
		return m[i];
	}

	// Get a pointer to m[0][0]
	inline float* base() {
		return &m[0][0];
	}

	inline const float* base() const {
		return &m[0][0];
	}

	void		set_left(const Vector3& vLeft);
	void		set_up(const Vector3& vUp);
	void		set_forward(const Vector3& vForward);

	void		get_basis_vector_3d(Vector3& vForward, Vector3& vLeft, Vector3& vUp) const;
	void		set_basis_vector_3d(const Vector3& vForward, const Vector3& vLeft, const Vector3& vUp);

	// Get/set the translation.
	Vector3& get_translation(Vector3& vTrans) const;
	void		set_translation(const Vector3& vTrans);

	void		pre_translate(const Vector3& vTrans);
	void		post_translate(const Vector3& vTrans);

	matrix_t& as_matrix();
	const matrix_t& as_matrix() const;
	void		copy_from_matrix(const matrix_t& m3x4);
	void		set_matrix(matrix_t& matrix3x4) const;

	bool		operator==(const view_matrix_t& src) const;
	bool		operator!=(const view_matrix_t& src) const { return !(*this == src); }

	// Access the basis Vector3s.
	Vector3		get_left() const;
	Vector3		get_up() const;
	Vector3		get_forward() const;
	Vector3		get_translation() const;


	// Matrix->Vector3 operations.
public:
	// Multiply by a 3D Vector3 (same as operator*).
	void		vector_3d_multiply(const Vector3& vIn, Vector3& vOut) const;

	// Multiply by a 4D Vector3.
	//void		V4Mul( const Vector34D &vIn, Vector34D &vOut ) const;

	// Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
	Vector3		apply_rotation(const Vector3& vVec) const;

	// Multiply by a Vector3 (divides by w, assumes input w is 1).
	Vector3		operator*(const Vector3& vVec) const;

	// Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
	Vector3		vector_3d_multiply(const Vector3& vVec) const;

	// Apply the inverse (transposed) rotation (only works on pure rotation matrix)
	Vector3		vector_3d_transpose_rotation(const Vector3& vVec) const;

	// Multiply by the upper 3 rows.
	Vector3		vector_3d_multiply_upper(const Vector3& vVec) const;

	// Apply the inverse (transposed) transformation (only works on pure rotation/translation)
	Vector3		vector_3d_transpose(const Vector3& vVec) const;


	// Matrix->plane operations.
	//public:
	// Transform the plane. The matrix can only contain translation and rotation.
	//void		TransformPlane( const VPlane &inPlane, VPlane &outPlane ) const;

	// Just calls TransformPlane and returns the result.
	//VPlane		operator*(const VPlane &thePlane) const;

	// Matrix->matrix operations.
public:

	view_matrix_t& operator=(const view_matrix_t& mOther);

	// Multiply two matrices (out = this * vm).
	void		MatrixMul(const view_matrix_t& vm, view_matrix_t& out) const;

	// Add two matrices.
	const view_matrix_t& operator+=(const view_matrix_t& other);

	// Just calls MatrixMul and returns the result.	
	view_matrix_t		operator*(const view_matrix_t& mOther) const;

	// Add/Subtract two matrices.
	view_matrix_t		operator+(const view_matrix_t& other) const;
	view_matrix_t		operator-(const view_matrix_t& other) const;

	// Negation.
	view_matrix_t		operator-() const;

	// Matrix operations.
public:
	// Set to identity.
	void		identity();

	bool		is_identity() const;

	// Setup a matrix for origin and angles.
	void		setup_maitrx_orginal_angles(const Vector3& origin, const Vector3& vAngles);

	// Does a fast inverse, assuming the matrix only contains translation and rotation.
	void		inverse_tr(view_matrix_t& mRet) const;

	// Usually used for debug checks. Returns true if the upper 3x3 contains
	// unit Vector3s and they are all orthogonal.
	bool		is_rotation_matrix() const;

	// This calls the other InverseTR and returns the result.
	view_matrix_t		inverse_tr() const;

	// Get the scale of the matrix's basis Vector3s.
	Vector3		get_scale() const;

	// (Fast) multiply by a scaling matrix setup from vScale.
	view_matrix_t		scale(const Vector3& vScale);

	// Normalize the basis Vector3s.
	view_matrix_t		normalize_basis_vector_3d() const;

	// Transpose.
	view_matrix_t		transpose() const;

	// Transpose upper-left 3x3.
	view_matrix_t		transpose_3x3() const;

public:
	// The matrix.
	vec_t		m[4][4];
};