void main(vec3 & in position, vec4 & out color)
{
	vec3 tmp;
	float l0, l1;

	tmp.r = position.x;
	tmp.g = position.y;
	tmp.b = position.z;

	l0 = dot(tmp, vec3(0.33f, 0.59f, 0.11f));
	l1 = dot(tmp, vec3(0.33f, 0.33f, 0.33f));

	tmp = mix(vec3(l1), tmp, pow(l0, 0.5f));

	tmp = pow(tmp, vec3(0.90f));

	color = vec4(tmp, 1.0f);
}
