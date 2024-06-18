#ifndef LIGHTSMANAGER
#define LIGHTSMANAGER

#include "../Lights.h"
#include "../Shader.h"
#include "../typedef.h"

class LightsManager
{
public:
	static LightsManager* i_;
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO[MAX_LIGHTS*2];
	unsigned int planeShadowMap[MAX_LIGHTS];
	unsigned int cubeShadowMap[MAX_LIGHTS];
	s_ptr<Shader> depth_shader_;
	s_ptr<Shader> light_shader_;
	float near_plane = 1.0f, far_plane = 7.5f;


	glm::mat4 lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
	//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix[2*MAX_LIGHTS];

	LightsManager(s_ptr<Shader> depthShader, s_ptr<Shader> lightShader);
	~LightsManager() = default;
	static void Initialize(s_ptr<Shader> depthShader, s_ptr<Shader> lightShader);
	
	void RenderFromLightPov(int lightID);

	void InitCubeShadowMap(int lightID);
	void InitPlaneShadowMap(int lightID);
	void InitShadowMaps();

	void DepthToTexture(glm::vec3 lightPos, int lightID, bool isPointlight = false);

	void BindCubeShadowMap(s_ptr<Shader> shader, int lightID);
	void BindPlaneShadowMap(s_ptr<Shader> shader, int lightID);
	

	




};


#endif // !LIGHTSMANAGER