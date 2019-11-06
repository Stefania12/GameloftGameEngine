precision mediump float;

struct Light
{
	vec3 diffuseColor, specularColor, direction, position;
	float alphaAngle, smallLimit, bigLimit, Kc, Kl, Kq;
};

varying vec2 v_uv;
varying vec4 v_position;
uniform vec3 u_camPos;
uniform float u_minRadius;
uniform float u_maxRadius;
uniform vec3 u_fogColor;
uniform sampler2D u_texture0;

uniform Light u_directionalLights[5];
uniform Light u_pointLights[5];
uniform Light u_spotlightLights[5];
uniform vec3 u_ambientalColor;
uniform float u_ambRatio;
uniform float u_specPower;
varying vec3 v_norm;


void main()
{
	vec4 color = texture2D(u_texture0, v_uv);
	float d = distance(vec3(v_position.x, v_position.y, v_position.z), u_camPos);
	float alpha = (clamp(d, u_minRadius, u_maxRadius) - u_minRadius) / (u_maxRadius - u_minRadius);
	vec3 modified_color = vec3((1.0 - alpha) * color.x, (1.0 - alpha) * color.y, (1.0 - alpha) * color.z);
	color = vec4(alpha * u_fogColor + modified_color, color.a);
	
	vec3 ambientalComponent = color.rgb * u_ambientalColor, viewDirection = normalize(u_camPos - v_position.xyz);
	vec3 halfWayDirection, specularComponent = vec3(0.0, 0.0, 0.0), diffuseComponent = vec3(0.0, 0.0, 0.0), direction;
	float spec, betaAngle;

	//directional
	for (int i = 0; i < 1; i++)
	{
		halfWayDirection = normalize(-normalize(u_directionalLights[i].direction) + viewDirection);
		spec = pow(max(dot(normalize(v_norm), halfWayDirection), 0.0), u_specPower);
		specularComponent = specularComponent + u_directionalLights[i].specularColor * spec;

		diffuseComponent = diffuseComponent + color.rgb * u_directionalLights[i].diffuseColor * max(dot(normalize(v_norm), -normalize(u_directionalLights[i].direction)), 0.0);
	}

	//point
	
	for (int i = 0; i < 1; i++)
	{
		direction = normalize(v_position.xyz - u_pointLights[i].position);
		halfWayDirection = normalize(-direction + viewDirection);
		spec = pow(max(dot(normalize(v_norm), halfWayDirection), 0.0), u_specPower);

		
		float ity = max(dot(normalize(v_norm), -direction), 0.0);
		float d = distance(v_position.xyz, u_pointLights[i].position);
		float atten = 1.0 / (u_pointLights[i].Kc + d * u_pointLights[i].Kl + d * d * u_pointLights[i].Kq);
		float intensity = ity * clamp(atten, 0.0, 1.0);

		diffuseComponent = diffuseComponent + color.rgb * u_pointLights[i].diffuseColor * intensity;
		specularComponent = specularComponent + u_pointLights[i].specularColor * spec * intensity;
	}

	//spotlight
	for (int i = 0; i < 1; i++)
	{
		
		direction = normalize(v_position.xyz - u_spotlightLights[i].position);

		betaAngle = acos(dot(normalize(u_spotlightLights[i].direction), v_position.xyz - u_spotlightLights[i].position));
		
		halfWayDirection = normalize(-direction + viewDirection);
		spec = pow(max(dot(normalize(v_norm), halfWayDirection), 0.0), u_specPower);

		float ity = max(dot(normalize(v_norm), -direction), 0.0);
		float d = distance(v_position.xyz, u_spotlightLights[i].position);
		float atten = 1.0 / (u_spotlightLights[i].Kc + d * u_spotlightLights[i].Kl + d * d * u_spotlightLights[i].Kq);

		float DdotL = dot(normalize(u_spotlightLights[i].direction), normalize(v_position.xyz - u_spotlightLights[i].position));
		float intensity = smoothstep(u_spotlightLights[i].smallLimit, u_spotlightLights[i].bigLimit, DdotL) * ity * atten;

		specularComponent = specularComponent + u_spotlightLights[i].specularColor * spec * intensity;
		diffuseComponent = diffuseComponent + color.rgb * u_spotlightLights[i].diffuseColor * intensity;
	}

	color = vec4(u_ambRatio * ambientalComponent + (1.0 - u_ambRatio) * (diffuseComponent + specularComponent), color.a);

	if (color.a < 0.2)
		discard;

	gl_FragColor = color;
}
