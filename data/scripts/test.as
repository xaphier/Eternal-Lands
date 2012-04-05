void main(vec4 & in time, MaterialData & out material)
{
	mat2x3 matrix;

	matrix[0].z = fract(matrix[0].z + 0.1 * time.w);

	material.set_texture_matrix(matrix, 0);
}
