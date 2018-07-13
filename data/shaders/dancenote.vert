#version 330 core

uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform int noteType;
uniform float hitAnim;
uniform float clock;
uniform float scale;
uniform vec2 position;

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTexCoord;
layout(location = 2) in vec3 vertNormal;
layout(location = 3) in vec4 vertColor;

out vData {
	vec3 lightDir;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vertex;

 mat4 scaleMat(in float sc) {
	return mat4(sc, 0, 0, 0,
				0, sc, 0, 0,
				0, 0, sc, 0,
				0, 0, 0, 1);
}

 mat4 rotMat(in float ang) {
	float ca = cos(ang);
	float sa = sin(ang);
	return mat4(ca, -sa, 0, 0,
				sa, ca, 0, 0,
				0,  0, 1, 0,
				0,  0, 0, 1);
}

void main() {
	vertex.texCoord = vertTexCoord;
	vertex.normal = normalMatrix * vertNormal;
	vertex.color = vertColor;
	vertex.lightDir = vec3(1,0,0);	
	mat4 trans = scaleMat(scale);

	// Cursor arrows
	if (noteType == 0) {
	trans *= scaleMat(1.0 - hitAnim * .5);

	// Regular arrows
	} else if (noteType == 1) {
	trans *= scaleMat(1.0 + hitAnim);

	// Holds
	} else if (noteType == 2) {
	trans *= scaleMat(1.0 + hitAnim);

	// Mines
	} else if (noteType == 3) {
	trans *= scaleMat(1.0 + hitAnim);
	 float r = radians(mod(clock*360.0, 360.0)); // They rotate!
	trans *= rotMat(r);
	}

	// Glow color for regular arrows or holds
	if (noteType == 1 || noteType == 2) {
	vertex.color = vec4(
	 min(vertColor.r + hitAnim *.5, 1.0),
	 min(vertColor.g + hitAnim *.5, 1.0),
	 min(vertColor.b + hitAnim *.5, 1.0),
	 max(vertColor.a - hitAnim, 0.0)
	);
	}

	gl_Position = projMatrix * mvMatrix * (vec4(position, 0, 0) + trans * vec4(vertPos, 1.0));
}

