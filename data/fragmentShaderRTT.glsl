#version 330 core
in vec2 uv_out;
uniform sampler2D tex;
uniform sampler2D depth;
uniform int freichen_active;
uniform int gradient_active;
out vec4 color;
uniform mat3 G[9] = mat3[](
	1.0/(2.0*sqrt(2.0)) * mat3( 1.0, sqrt(2.0), 1.0, 0.0, 0.0, 0.0, -1.0, -sqrt(2.0), -1.0 ),
	1.0/(2.0*sqrt(2.0)) * mat3( 1.0, 0.0, -1.0, sqrt(2.0), 0.0, -sqrt(2.0), 1.0, 0.0, -1.0 ),
	1.0/(2.0*sqrt(2.0)) * mat3( 0.0, -1.0, sqrt(2.0), 1.0, 0.0, -1.0, -sqrt(2.0), 1.0, 0.0 ),
	1.0/(2.0*sqrt(2.0)) * mat3( sqrt(2.0), -1.0, 0.0, -1.0, 0.0, 1.0, 0.0, 1.0, -sqrt(2.0) ),
	1.0/2.0 * mat3( 0.0, 1.0, 0.0, -1.0, 0.0, -1.0, 0.0, 1.0, 0.0 ),
	1.0/2.0 * mat3( -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0 ),
	1.0/6.0 * mat3( 1.0, -2.0, 1.0, -2.0, 4.0, -2.0, 1.0, -2.0, 1.0 ),
	1.0/6.0 * mat3( -2.0, 1.0, -2.0, 1.0, 4.0, 1.0, -2.0, 1.0, -2.0 ),
	1.0/3.0 * mat3( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
);
const float blurH = 1.0/300.0;
const float blurV = 1.0/200.0;
const int numberPixels= 4;
vec4 degradado(vec2 coords);
vec4 freichen(sampler2D tex);
vec4 blur();

void main(){
	//color = freichen(tex);
	/*if(freichen_active>0){
		color = freichen(tex);
	}
	else if(gradient_active>0){
	color = texture(tex,uv_out)*degradado(uv_out);
	}
	else{
	color = texture(tex,uv_out);
	}*/
	vec3 imColor =  texture(tex,uv_out).rgb;
	float a  = texture(depth,uv_out).r;
	color = vec4(imColor*(1-a)+ imColor.g*a,1.0);

}
vec4 degradado(vec2 coords){
	vec4 result;
	result = vec4(coords.x,0,0,1);
	return result;
}
vec4 freichen(sampler2D texture){
	mat3 I;
	float cnv[9];
	vec3 sample;
	
	for (int i=0; i<3; i++)
	for (int j=0; j<3; j++) {
		sample = texelFetch( texture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
		I[i][j] = length(sample); 
	}
	
	
	for (int i=0; i<9; i++) {
		float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]);
		cnv[i] = dp3 * dp3; 
	}

	float M = (cnv[0] + cnv[1]) + (cnv[2] + cnv[3]);
	float S = (cnv[4] + cnv[5]) + (cnv[6] + cnv[7]) + (cnv[8] + M); 
	
	return vec4(sqrt(M/S));
}
vec4 blur(){
	vec4 result = vec4(0);
	for(int x =-numberPixels;x<=numberPixels;x++){
		for(int y=-numberPixels;y<=numberPixels;y++){
			result+= texture(tex,vec2(uv_out.x+x*blurH,uv_out.y+y*blurV))/81.0f;
		}
	}
	return result;
}