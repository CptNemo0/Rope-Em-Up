#ifndef SHADOWSMANAGER
#define SHADOWSMANAGER

#include "../Lights.h"
#include "../Shader.h"
#include "../typedef.h"

class ShadowsManager
{
public:
	static ShadowsManager* i_;
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO[MAX_LIGHTS + 1];
	unsigned int planeShadowMap;
	unsigned int cubeShadowMap[MAX_LIGHTS];
	s_ptr<Shader> depth_shader_;
	float near_plane = 1.0f, far_plane = 7.5f;

	PointLight pointLights[MAX_LIGHTS];
	float farPlane = 25.0f;

	DirectionalLight directionalLight;


	glm::mat4 lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
	//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix[MAX_LIGHTS + 1];

	ShadowsManager(s_ptr<Shader> depthShader);
	~ShadowsManager() = default;
	static void Initialize(s_ptr<Shader> depthShader);

	void SetUpPlaneShadowBuffers(int lightID);
	void SetUpCubeShadowBuffers(int lightID);

	void InitCubeShadowMap(int lightID);
	void InitPlaneShadowMap(int lightID);

	std::vector<glm::mat4> GetShadowTransforms(glm::vec3 lightPos, float farPlane);

	void SetLightSpaceMatrix(glm::vec3 lightPos, int lightID);

	void BindCubeShadowMap(s_ptr<Shader> shader, int lightID);
	void BindPlaneShadowMap(s_ptr<Shader> shader, int lightID);
	

	




};


#endif // !SHADOWSMANAGER