void main(vec3 & in position, vec4 & out color)
{
	color = vec4(pow(position, vec3(1.0f)), 1.0f);
}
