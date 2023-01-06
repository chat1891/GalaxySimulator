#include "SDLwindow.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>
#include "Helper.hpp"



glm::vec2 SDLWindow::GetWindowPos(GLfloat x, GLfloat y, GLfloat z)
{
	glm::vec3 pos = glm::vec3(x, y, z);
	glm::mat4 matModel = glm::mat4(1.0);
	glm::vec4 viewPort = glm::vec4(0.0f, 0.0f, (float)_width, (float)_height);
	glm::vec3 projected = glm::project(pos, matModel, projectionMatrix, viewPort);
	return glm::vec2(projected.x, projected.y);
}

SDLWindow::SDLWindow()
	: _event()
	, _fov(0)
	, _width(0)
	, _height(0)
	, _fps(0)
	, _camPos({ 0, 0, 2 })
	, _camLookAt({ 0, 0, 0 })
	, _camOrient({ 0, 1, 0 })
	, _pSdlWnd(nullptr)
	, _pSdlRenderer(nullptr)
	, run(true)
	, projectionMatrix()
	, viewMatrix()
	, _stopEventPolling(false)
{ }

SDLWindow::~SDLWindow()
{
	SDL_Quit();
}

void SDLWindow::Init(int width, int height, float axisLen, const std::string& caption)
{
	_fov = axisLen;
	_width = width;
	_height = height;

	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	_pSdlWnd = SDL_CreateWindow(
		caption.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_OPENGL);

	if (!_pSdlWnd)
		throw std::runtime_error(SDL_GetError());

	_pSdlRenderer = SDL_CreateRenderer(_pSdlWnd, -1, SDL_RENDERER_ACCELERATED);
	_sdcGlContext = SDL_GL_CreateContext(_pSdlWnd);

	glewInit();

	//testOpenGL();
	InitGL();
	InitSimulation();
}

void SDLWindow::ScaleAxis(float scale)
{
	_fov *= scale;
	AdjustCamera();
}

const glm::vec3& SDLWindow::GetCamPos() const
{
	return _camPos;
}

const glm::vec3& SDLWindow::GetCamOrient() const
{
	return _camOrient;
}

const glm::vec3& SDLWindow::GetCamLookAt() const
{
	return _camLookAt;
}

void SDLWindow::SetCameraOrientation(const glm::vec3& orient)
{
	_camOrient = orient;
	AdjustCamera();
}

void SDLWindow::SetCamera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& orient)
{
	_camOrient = orient;
	_camPos = pos;
	_camLookAt = lookAt;
	AdjustCamera();
}

void SDLWindow::AdjustCamera()
{
	GET_GLERROR;
	double l = _fov / 2.0;
	double aspect = (double)_width / _height;

	// new mvp matrices for glsl shaders via glm:
	projectionMatrix = glm::ortho(
		-l * aspect, l * aspect,
		-l, l,
		-l, l);

	//move to the right for 4000 units
	//glm::mat4 newView = glm::translate(glm::mat4(1.0f), glm::vec3(4000, 0, 0));
	//_matProjection *= newView;

	glm::dvec3 camPos(_camPos.x, _camPos.y, _camPos.z);
	glm::dvec3 camLookAt(_camLookAt.x, _camLookAt.y, _camLookAt.z);
	glm::dvec3 camOrient(_camOrient.x, _camOrient.y, _camOrient.z);
	viewMatrix = glm::lookAt(camPos, camLookAt, camOrient);
	GET_GLERROR;
	CameraZoomInOut();
}



double SDLWindow::GetFOV() const
{
	return _fov;
}

int SDLWindow::GetFPS() const
{
	return _fps;
}

void SDLWindow::MainLoop()
{
	int ct = 0;
	double dt = 0;
	time_t t1(time(nullptr)), t2;

	while (run)
	{
		//PollEvents();
		//glfwSetScrollCallback(_pSdlWnd, scroll_callback);
		//scroll_callback(_pSdlWnd, 1, 1);
;		CameraZoomInOut();
		UpdateSimulation();
		StartSimulation();

		if (!_stopEventPolling)
			PollEvents();

		++ct;

		t2 = time(nullptr);
		dt = difftime(t2, t1);
		if (dt > 1)
		{
			_fps = (int)((double)ct / dt);
			ct = 0;
			t1 = t2;
		}
	}
}

int zoomIn = -20;
int zoomOut = 20;
int zoomSpeed = zoomIn;
float initCameraZoom = 0;
void SDLWindow::CameraZoomInOut()
{
	//glm::mat4 newView = glm::translate(glm::mat4(1.0f), glm::vec3(initCameraZoom, 0, 0));
	//_matProjection *= newView;
	float aspect = (float)_width / _height;
	//glm::mat4 proj3 = glm::perspective(glm::radians(initCameraZoom), (float)aspect, 0.1f, 100.0f);
	glm::mat4 proj3 = glm::perspective(glm::radians(_fov), (float)aspect, 0.1f, 5000.0f);
	viewMatrix = proj3 * viewMatrix;
	
	if (_fov <= 0) zoomSpeed = zoomOut;
	else if(_fov>35000) zoomSpeed = zoomIn;
	_fov += zoomSpeed;
}

int SDLWindow::GetWidth() const
{
	return _width;
}

int SDLWindow::GetHeight() const
{
	return _height;
}

void SDLWindow::ExitMainLoop()
{
	run = false;
}

void SDLWindow::OnProcessEvents(Uint32 type)
{
}

void SDLWindow::PollEvents()
{
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			ExitMainLoop();
			break;

		default:
			OnProcessEvents(_event.type);
			break;
		} // switch event type
	}
}
