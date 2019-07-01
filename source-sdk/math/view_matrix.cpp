
#include "view_matrix.hpp"

view_matrix_t::view_matrix_t() {
}

view_matrix_t::view_matrix_t(
	vec_t m00, vec_t m01, vec_t m02, vec_t m03,
	vec_t m10, vec_t m11, vec_t m12, vec_t m13,
	vec_t m20, vec_t m21, vec_t m22, vec_t m23,
	vec_t m30, vec_t m31, vec_t m32, vec_t m33) {
	init(
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
	);
}


view_matrix_t::view_matrix_t(const matrix_t& matrix3x4) {
	init(matrix3x4);
}


//-----------------------------------------------------------------------------
// Creates a matrix where the X axis = forward
// the Y axis = left, and the Z axis = up
//-----------------------------------------------------------------------------
view_matrix_t::view_matrix_t(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) {
	init(
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}


void view_matrix_t::init(
	vec_t m00, vec_t m01, vec_t m02, vec_t m03,
	vec_t m10, vec_t m11, vec_t m12, vec_t m13,
	vec_t m20, vec_t m21, vec_t m22, vec_t m23,
	vec_t m30, vec_t m31, vec_t m32, vec_t m33
) {
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}


//-----------------------------------------------------------------------------
// Initialize from a 3x4
//-----------------------------------------------------------------------------
void view_matrix_t::init(const matrix_t& matrix3x4) {
	memcpy(m, matrix3x4.base(), sizeof(matrix_t));

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
// Vector33DMultiplyPosition treats src2 as if it's a point (adds the translation)
//-----------------------------------------------------------------------------
// NJS: src2 is passed in as a full Vector3 rather than a reference to prevent the need
// for 2 branches and a potential copy in the body.  (ie, handling the case when the src2
// reference is the same as the dst reference ).
void vector_3d_multiply_position(const view_matrix_t& src1, const Vector3& src2, Vector3& dst) {
	dst[0] = src1[0][0] * src2.x + src1[0][1] * src2.y + src1[0][2] * src2.z + src1[0][3];
	dst[1] = src1[1][0] * src2.x + src1[1][1] * src2.y + src1[1][2] * src2.z + src1[1][3];
	dst[2] = src1[2][0] * src2.x + src1[2][1] * src2.y + src1[2][2] * src2.z + src1[2][3];
}

//-----------------------------------------------------------------------------
// Methods related to the basis Vector3s of the matrix
//-----------------------------------------------------------------------------

Vector3 view_matrix_t::get_forward() const {
	return Vector3(m[0][0], m[1][0], m[2][0]);
}

Vector3 view_matrix_t::get_left() const {
	return Vector3(m[0][1], m[1][1], m[2][1]);
}

Vector3 view_matrix_t::get_up() const {
	return Vector3(m[0][2], m[1][2], m[2][2]);
}

void view_matrix_t::set_forward(const Vector3& vForward) {
	m[0][0] = vForward.x;
	m[1][0] = vForward.y;
	m[2][0] = vForward.z;
}

void view_matrix_t::set_left(const Vector3& vLeft) {
	m[0][1] = vLeft.x;
	m[1][1] = vLeft.y;
	m[2][1] = vLeft.z;
}

void view_matrix_t::set_up(const Vector3& vUp) {
	m[0][2] = vUp.x;
	m[1][2] = vUp.y;
	m[2][2] = vUp.z;
}

void view_matrix_t::get_basis_vector_3d(Vector3& vForward, Vector3& vLeft, Vector3& vUp) const {
	vForward.init(m[0][0], m[1][0], m[2][0]);
	vLeft.init(m[0][1], m[1][1], m[2][1]);
	vUp.init(m[0][2], m[1][2], m[2][2]);
}

void view_matrix_t::set_basis_vector_3d(const Vector3& vForward, const Vector3& vLeft, const Vector3& vUp) {
	set_forward(vForward);
	set_left(vLeft);
	set_up(vUp);
}


//-----------------------------------------------------------------------------
// Methods related to the translation component of the matrix
//-----------------------------------------------------------------------------

Vector3 view_matrix_t::get_translation() const {
	return Vector3(m[0][3], m[1][3], m[2][3]);
}

Vector3& view_matrix_t::get_translation(Vector3& vTrans) const {
	vTrans.x = m[0][3];
	vTrans.y = m[1][3];
	vTrans.z = m[2][3];
	return vTrans;
}

void view_matrix_t::set_translation(const Vector3& vTrans) {
	m[0][3] = vTrans.x;
	m[1][3] = vTrans.y;
	m[2][3] = vTrans.z;
}


//-----------------------------------------------------------------------------
// appply translation to this matrix in the input space
//-----------------------------------------------------------------------------
void view_matrix_t::pre_translate(const Vector3& vTrans) {
	Vector3 tmp;
	vector_3d_multiply_position(*this, vTrans, tmp);
	m[0][3] = tmp.x;
	m[1][3] = tmp.y;
	m[2][3] = tmp.z;
}


//-----------------------------------------------------------------------------
// appply translation to this matrix in the output space
//-----------------------------------------------------------------------------
void view_matrix_t::post_translate(const Vector3& vTrans) {
	m[0][3] += vTrans.x;
	m[1][3] += vTrans.y;
	m[2][3] += vTrans.z;
}

const matrix_t& view_matrix_t::as_matrix() const {
	return *((const matrix_t*)this);
}

matrix_t& view_matrix_t::as_matrix() {
	return *((matrix_t*)this);
}

void view_matrix_t::copy_from_matrix(const matrix_t& m3x4) {
	memcpy(m, m3x4.base(), sizeof(matrix_t));
	m[3][0] = m[3][1] = m[3][2] = 0;
	m[3][3] = 1;
}

void view_matrix_t::set_matrix(matrix_t& matrix3x4) const {
	memcpy(matrix3x4.base(), m, sizeof(matrix_t));
}


//-----------------------------------------------------------------------------
// Matrix math operations
//-----------------------------------------------------------------------------
const view_matrix_t& view_matrix_t::operator+=(const view_matrix_t& other) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] += other.m[i][j];
		}
	}

	return *this;
}

view_matrix_t& view_matrix_t::operator=(const view_matrix_t& mOther) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = mOther.m[i][j];
		}
	}

	return *this;
}

view_matrix_t view_matrix_t::operator+(const view_matrix_t& other) const {
	view_matrix_t ret;
	for (int i = 0; i < 16; i++) {
		((float*)ret.m)[i] = ((float*)m)[i] + ((float*)other.m)[i];
	}
	return ret;
}

view_matrix_t view_matrix_t::operator-(const view_matrix_t& other) const {
	view_matrix_t ret;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ret.m[i][j] = m[i][j] - other.m[i][j];
		}
	}

	return ret;
}

view_matrix_t view_matrix_t::operator-() const {
	view_matrix_t ret;
	for (int i = 0; i < 16; i++) {
		((float*)ret.m)[i] = -((float*)m)[i];
	}
	return ret;
}

//-----------------------------------------------------------------------------
// Vector3 transformation
//-----------------------------------------------------------------------------


Vector3 view_matrix_t::operator*(const Vector3& vVec) const {
	Vector3 vRet;
	vRet.x = m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z + m[0][3];
	vRet.y = m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z + m[1][3];
	vRet.z = m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z + m[2][3];

	return vRet;
}

Vector3 view_matrix_t::vector_3d_multiply(const Vector3& vVec) const {
	Vector3 vResult;
	vector_3d_multiply_position(*this, vVec, vResult);
	return vResult;
}


Vector3 view_matrix_t::vector_3d_transpose(const Vector3& vVec) const {
	Vector3 tmp = vVec;
	tmp.x -= m[0][3];
	tmp.y -= m[1][3];
	tmp.z -= m[2][3];

	return Vector3(
		m[0][0] * tmp.x + m[1][0] * tmp.y + m[2][0] * tmp.z,
		m[0][1] * tmp.x + m[1][1] * tmp.y + m[2][1] * tmp.z,
		m[0][2] * tmp.x + m[1][2] * tmp.y + m[2][2] * tmp.z
	);
}

Vector3 view_matrix_t::vector_3d_multiply_upper(const Vector3& vVec) const {
	return Vector3(
		m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z,
		m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z,
		m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z
	);
}

Vector3 view_matrix_t::vector_3d_transpose_rotation(const Vector3& vVec) const {
	return Vector3(
		m[0][0] * vVec.x + m[1][0] * vVec.y + m[2][0] * vVec.z,
		m[0][1] * vVec.x + m[1][1] * vVec.y + m[2][1] * vVec.z,
		m[0][2] * vVec.x + m[1][2] * vVec.y + m[2][2] * vVec.z
	);
}


void view_matrix_t::vector_3d_multiply(const Vector3& vIn, Vector3& vOut) const {
	vec_t rw;

	rw = 1.0f / (m[3][0] * vIn.x + m[3][1] * vIn.y + m[3][2] * vIn.z + m[3][3]);
	vOut.x = (m[0][0] * vIn.x + m[0][1] * vIn.y + m[0][2] * vIn.z + m[0][3]) * rw;
	vOut.y = (m[1][0] * vIn.x + m[1][1] * vIn.y + m[1][2] * vIn.z + m[1][3]) * rw;
	vOut.z = (m[2][0] * vIn.x + m[2][1] * vIn.y + m[2][2] * vIn.z + m[2][3]) * rw;
}

//-----------------------------------------------------------------------------
// Other random stuff
//-----------------------------------------------------------------------------
void view_matrix_t::identity() {
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}


bool view_matrix_t::is_identity() const {
	return
		m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f && m[0][3] == 0.0f &&
		m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f && m[1][3] == 0.0f &&
		m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f && m[2][3] == 0.0f &&
		m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f && m[3][3] == 1.0f;
}

Vector3 view_matrix_t::apply_rotation(const Vector3& vVec) const {
	return vector_3d_multiply(vVec);
}
