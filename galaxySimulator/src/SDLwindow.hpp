#pragma once
#include <string>

#include <SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include <GLFW/glfw3.h>

class SDLWindow
{
public:
	void Init(int width, int height, float axisLen, const std::string& caption);

	void MainLoop();
	void ExitMainLoop();

	int GetWidth() const;
	int GetHeight() const;

protected:
	SDLWindow();
	virtual ~SDLWindow();

	virtual void StartSimulation() = 0;
	virtual void UpdateSimulation() = 0;

	virtual void InitGL() noexcept (false) = 0;
	virtual void InitSimulation() = 0;

	virtual void PollEvents();
	virtual void OnProcessEvents(Uint32 type);

	const glm::vec3& GetCamPos() const;
	const glm::vec3& GetCamOrient() const;
	const glm::vec3& GetCamLookAt() const;

	void SetCameraOrientation(const glm::vec3& orientation);
	void SetCamera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& orient);
	void AdjustCamera();
	int GetFPS() const;
	void ScaleAxis(float scale);
	double GetFOV() const;
	glm::vec2 GetWindowPos(GLfloat x, GLfloat y, GLfloat z);
	void scroll_callback(SDL_Window* window, double xoffset, double yoffset);
	void CameraZoomInOut();
	void printVersion();
	//SDL_Event event;

	SDL_Event _event;

	float _fov;

	std::string _caption;

	int _width;
	int _height;		
	int _fps;

	glm::vec3 _camPos;		
	glm::vec3 _camLookAt;	
	glm::vec3 _camOrient;	

	SDL_Window* _pSdlWnd;
	SDL_Renderer* _pSdlRenderer;
	SDL_GLContext _sdcGlContext;

	glm::mat4 _matProjection;
	glm::mat4 _matView;

	volatile bool _bRunning;
	bool _stopEventPolling;

	virtual int testOpenGL() = 0;
};
