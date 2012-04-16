void main(vec3 & in position, vec4 & out color)
{
	vec4 a;
	float w0, w1, w2, w3, g0, g1, h0, h1, p;

	p = position.x;

	a.x = p * p * p;
	a.y = p * p;
	a.z = p;
	a.w = 1.0;

	w0 = dot(a, vec4(-1.0,  3.0, -3.0,  1.0)) / 6.0;
	w1 = dot(a, vec4(-3.0,  3.0,  3.0,  1.0)) / 6.0;
	w2 = dot(a, vec4( 3.0, -6.0,  0.0,  4.0)) / 6.0;
	w3 = dot(a, vec4( 1.0,  0.0,  0.0,  0.0)) / 6.0;

	g0 = w0 + w1;
	g1 = w2 + w3;

	h0 = 1.0 - (w1 / (w0 + w1)) + p;
	h1 = 1.0 + (w3 / (w2 + w3)) - p;

	color.r = h0;
	color.g = h1;
	color.b = g0;
	color.a = g1;
}

