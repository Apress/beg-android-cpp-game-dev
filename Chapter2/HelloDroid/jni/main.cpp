#include <jni.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

/**
 * Game Structures
 */
struct Paddle
{
	float x;
	float y;
};

static const float PADDLE_HALF_WIDTH = 0.15f;
static const float PADDLE_HALF_HEIGHT = 0.025f;
static const float PADDLE_START_X = 0.0f;
static const float PADDLE_START_Y = -0.775f;
static const float PADDLE_LEFT_BOUND = (-1.0f + PADDLE_HALF_WIDTH);
static const float PADDLE_RIGHT_BOUND = (1.0f - PADDLE_HALF_WIDTH);

static const float BALL_HALF_WIDTH = 0.025f;
static const float BALL_HALF_HEIGHT = 0.025f;
static const float BALL_START_X = 0.0f;
static const float BALL_START_Y = 0.0f;
static const float BALL_VELOCITY = 0.015f;
static const float BALL_LEFT_BOUND = (-1.0f + BALL_HALF_WIDTH);
static const float BALL_RIGHT_BOUND = (1.0f - BALL_HALF_WIDTH);
static const float BALL_TOP_BOUND = (1.0f - BALL_HALF_HEIGHT);
static const float BALL_BOTTOM_BOUND = (-1.0f + BALL_HALF_HEIGHT);

static const float BLOCK_HALF_WIDTH = 0.15f;
static const float BLOCK_WIDTH = BLOCK_HALF_WIDTH * 2.0f;
static const float BLOCK_HALF_HEIGHT = 0.05f;
static const float BLOCK_HEIGHT = BLOCK_HALF_HEIGHT * 2.0f;
static const float BLOCK_START_POSITION = -0.62f;
static const float BLOCK_HORIZONTAL_GAP = 0.01f;

static const int32_t NUM_BLOCKS = 15;
static const int32_t NUM_BLOCKS_ROW = 5;

static const int32_t VERTEX_SIZE = sizeof(GLfloat) * 7;
static const int32_t POSITION_PARAMETER_INDEX = 0;
static const int32_t POSITION_NUM_ELEMENTS = 3;
static const int32_t COLOR_PARAMETER_INDEX = 1;
static const int32_t COLOR_NUM_ELEMENTS = 4;

static const int32_t TRIANGLE_NUM_VERTICES = 3;
static const int32_t QUAD_NUM_VERTICES = 6;

struct block
{
	float x;
	float y;
	bool isActive;
};

/**
 * Shared state for our app.
 */
struct engine
{
	struct android_app* app;

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t programObject;
	float width;
	float height;
	float touchX;
	float touchY;
	float playerX;
	float playerY;
	float ballX;
	float ballY;
	float ballVelocityX;
	float ballVelocityY;
	bool touchIsDown;
	block blocks[NUM_BLOCKS];
};

static void engine_init_blocks(struct engine* engine)
{
	float blockYPos[] = { 0.8f, 0.675f, 0.55f };

	for (int32_t i=0; i<NUM_BLOCKS; ++i)
	{
		engine->blocks[i].x = BLOCK_START_POSITION + ((BLOCK_WIDTH + BLOCK_HORIZONTAL_GAP) * (i % NUM_BLOCKS_ROW));
		engine->blocks[i].y = blockYPos[i / NUM_BLOCKS_ROW];
		engine->blocks[i].isActive = true;
	}
}

GLuint LoadShader(const char *shaderSrc, GLenum type)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);
	if(shader != 0)
	{
		// Load the shader source
		glShaderSource(shader, 1, &shaderSrc, NULL);

		// Compile the shader
		glCompileShader(shader);
		// Check the compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if(!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

			if(infoLen > 1)
			{
				char* infoLog = new char[infoLen];
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				LOGW("Error compiling shader:\n%s\n", infoLog);
				delete[] infoLog;
			}
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
	// initialize OpenGL ES and EGL

	/*
	 * Here specify the attributes of the desired configuration.
	 * Below, we select an EGLConfig with at least 8 bits per color
	 * component compatible with on-screen windows
	 */
	const EGLint attribs[] =
	{
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, NULL, NULL);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);

	EGLint contextAttribs[] =
	{
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
	};
	context = eglCreateContext(display, config, NULL, contextAttribs);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->touchX = 0.0f;
	engine->touchY = 0.0f;
	engine->touchIsDown = false;
	engine->playerX = PADDLE_START_X;
	engine->playerY = PADDLE_START_Y;
	engine->ballX = BALL_START_X;
	engine->ballY = BALL_START_Y;
	engine->ballVelocityX = BALL_VELOCITY;
	engine->ballVelocityY = BALL_VELOCITY;

	// Initialize GL state.
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	char vShaderStr[] =
			"attribute vec4 a_vPosition;   \n"
			"attribute vec4 a_vColor;	   \n"
			"varying vec4 v_vColor;		   \n"
			"void main()                   \n"
			"{                             \n"
			"   gl_Position = a_vPosition; \n"
			"	v_vColor = a_vColor;       \n"
			"}                             \n";

	char fShaderStr[] =
			"precision mediump float;                   \n"
			"varying vec4 v_vColor;		 				\n"
			"void main()                                \n"
			"{                                          \n"
			"  gl_FragColor = v_vColor;					\n"
			"}                                          \n";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = LoadShader(vShaderStr, GL_VERTEX_SHADER);
	fragmentShader = LoadShader(fShaderStr, GL_FRAGMENT_SHADER);

	// Create the program object
	engine->programObject = glCreateProgram();
	if(engine->programObject == 0)
	{
		return -1;
	}

	// Attach shaders to program
	glAttachShader(engine->programObject, vertexShader);
	glAttachShader(engine->programObject, fragmentShader);

	// Bind a_vPosition to attribute 0 and a_vColor to 1
	glBindAttribLocation(engine->programObject, POSITION_PARAMETER_INDEX, "a_vPosition");
	glBindAttribLocation(engine->programObject, COLOR_PARAMETER_INDEX, "a_vColor");

	// Link the program
	glLinkProgram(engine->programObject);

	// Check the link status
	glGetProgramiv(engine->programObject, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(engine->programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1)
		{
			char* infoLog = new char[infoLen];
			glGetProgramInfoLog(engine->programObject, infoLen, NULL, infoLog);
			LOGW("Error linking program:\n%s\n", infoLog);

			delete[] infoLog;
		}

		glDeleteProgram(engine->programObject);
		return -1;
	}

	engine_init_blocks(engine);

	return 0;
}

static void engine_update_frame(struct engine* engine)
{
	if (engine->touchIsDown)
	{
		if (engine->touchX < 0.15f && engine->touchY < 0.2f)
		{
			engine->playerX -= 0.015f;
			if (engine->playerX < PADDLE_LEFT_BOUND)
			{
				engine->playerX = PADDLE_LEFT_BOUND;
			}
		}
		else if (engine->touchX > 0.85f && engine->touchY < 0.2f)
		{
			engine->playerX += 0.015f;
			if (engine->playerX > PADDLE_RIGHT_BOUND)
			{
				engine->playerX = PADDLE_RIGHT_BOUND;
			}
		}
	}

	engine->ballX += engine->ballVelocityX;
	if (engine->ballX < BALL_LEFT_BOUND || engine->ballX > BALL_RIGHT_BOUND)
	{
		engine->ballVelocityX = -engine->ballVelocityX;
	}

	engine->ballY += engine->ballVelocityY;
	if (engine->ballY > BALL_TOP_BOUND)
	{
		engine->ballVelocityY = -engine->ballVelocityY;
	}

	if (engine->ballY < BALL_BOTTOM_BOUND)
	{
		// reset the ball
		if (engine->ballVelocityY < 0.0f)
		{
			engine->ballVelocityY = -engine->ballVelocityY;
		}

		engine->ballX = BALL_START_X;
		engine->ballY = BALL_START_Y;

		engine_init_blocks(engine);
	}

	float ballXPlusVelocity = engine->ballX + engine->ballVelocityX;
	float ballYPlusVelocity = engine->ballY + engine->ballVelocityY;

	const float ballLeft = ballXPlusVelocity - BALL_HALF_WIDTH;
	const float ballRight = ballXPlusVelocity + BALL_HALF_WIDTH;
	const float ballTop = ballYPlusVelocity + BALL_HALF_HEIGHT;
	const float ballBottom = ballYPlusVelocity - BALL_HALF_HEIGHT;
	const float paddleLeft = engine->playerX - PADDLE_HALF_WIDTH;
	const float paddleRight = engine->playerX + PADDLE_HALF_WIDTH;
	const float paddleTop = engine->playerY + PADDLE_HALF_HEIGHT;
	const float paddleBottom = engine->playerY - PADDLE_HALF_HEIGHT;
	if (!((ballRight < paddleLeft) ||
			(ballLeft > paddleRight) ||
			(ballBottom > paddleTop) ||
			(ballTop < paddleBottom)))
	{
		if (engine->ballVelocityY < 0.0f)
		{
			engine->ballVelocityY = -engine->ballVelocityY;
		}
	}

	bool anyBlockActive = false;
	for (int32_t i=0; i<NUM_BLOCKS; ++i)
	{
		block& currentBlock = engine->blocks[i];
		if (currentBlock.isActive)
		{
			const float blockLeft = currentBlock.x - BLOCK_HALF_WIDTH;
			const float blockRight = currentBlock.x + BLOCK_HALF_WIDTH;
			const float blockTop = currentBlock.y + BLOCK_HALF_HEIGHT;
			const float blockBottom = currentBlock.y - BLOCK_HALF_HEIGHT;
			if (!((ballRight < blockLeft) ||
					(ballLeft > blockRight) ||
					(ballTop < blockBottom) ||
					(ballBottom > blockTop)))
			{
				engine->ballVelocityY = -engine->ballVelocityY;

				if (ballLeft < blockLeft ||
						ballRight > blockRight)
				{
					engine->ballVelocityX = -engine->ballVelocityX;
				}

				currentBlock.isActive = false;
			}
			anyBlockActive = true;
		}
	}

	if (!anyBlockActive)
	{
		engine_init_blocks(engine);
	}
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine)
{
	if (engine->display == NULL)
	{
		// No display.
		return;
	}

	glViewport(0, 0, static_cast<int32_t>(engine->width), static_cast<int32_t>(engine->height));

	// Just fill the screen with a color.
	glClearColor(0.95f, 0.95f, 0.95f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Use the program object
	glUseProgram(engine->programObject);

	glEnableVertexAttribArray(POSITION_PARAMETER_INDEX);
	glEnableVertexAttribArray(COLOR_PARAMETER_INDEX);

	const float z = 0.0f;
	const float buttonColor[] = {0.25f, 0.25f, 0.25f, 1.0f};
	GLfloat leftButton[] = {-0.85f, 0.75f, z,
			buttonColor[0], buttonColor[1], buttonColor[2], buttonColor[3],
			-0.9f, 0.8f, z,
			buttonColor[0], buttonColor[1], buttonColor[2], buttonColor[3],
			-0.85f,  0.85f, z,
			buttonColor[0], buttonColor[1], buttonColor[2], buttonColor[3]};

	// Load the vertex data
	glVertexAttribPointer(POSITION_PARAMETER_INDEX, POSITION_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, leftButton);
	glVertexAttribPointer(COLOR_PARAMETER_INDEX, COLOR_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, &leftButton[3]);
	glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM_VERTICES);

	GLfloat rightButton[] = {0.85f,  0.75f, z,
			buttonColor[0], buttonColor[1], buttonColor[2], buttonColor[3],
			0.9f, 0.8f, z,
			buttonColor[0], buttonColor[1], buttonColor[2], buttonColor[3],
			0.85f,  0.85f, z,
			buttonColor[0], buttonColor[1], buttonColor[2], buttonColor[3]};

	glVertexAttribPointer(POSITION_PARAMETER_INDEX, POSITION_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, rightButton);
	glVertexAttribPointer(COLOR_PARAMETER_INDEX, COLOR_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, &rightButton[3]);
	glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM_VERTICES);

	float left = engine->playerX - PADDLE_HALF_WIDTH;
	float right = engine->playerX + PADDLE_HALF_WIDTH;
	float top = engine->playerY - PADDLE_HALF_HEIGHT;
	float bottom = engine->playerY + PADDLE_HALF_HEIGHT;
	const float paddleColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat paddle[] = {left, top, z,
			paddleColor[0], paddleColor[1], paddleColor[2], paddleColor[3],
			left, bottom, z,
			paddleColor[0], paddleColor[1], paddleColor[2], paddleColor[3],
			right, top, z,
			paddleColor[0], paddleColor[1], paddleColor[2], paddleColor[3],
			right, top, z,
			paddleColor[0], paddleColor[1], paddleColor[2], paddleColor[3],
			left, bottom, z,
			paddleColor[0], paddleColor[1], paddleColor[2], paddleColor[3],
			right, bottom, z,
			paddleColor[0], paddleColor[1], paddleColor[2], paddleColor[3]};

	glVertexAttribPointer(POSITION_PARAMETER_INDEX, POSITION_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, paddle);
	glVertexAttribPointer(COLOR_PARAMETER_INDEX, COLOR_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, &paddle[3]);
	glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM_VERTICES);

	left = engine->ballX - BALL_HALF_WIDTH;
	right = engine->ballX + BALL_HALF_WIDTH;
	top = engine->ballY - BALL_HALF_HEIGHT;
	bottom = engine->ballY + BALL_HALF_HEIGHT;
	const float ballColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat ball[] = {left,  top, z,
			ballColor[0], ballColor[1], ballColor[2], ballColor[3],
			left, bottom,  z,
			ballColor[0], ballColor[1], ballColor[2], ballColor[3],
			right,   top, z,
			ballColor[0], ballColor[1], ballColor[2], ballColor[3],
			right,   top, z,
			ballColor[0], ballColor[1], ballColor[2], ballColor[3],
			left, bottom,  z,
			ballColor[0], ballColor[1], ballColor[2], ballColor[3],
			right,  bottom, z,
			ballColor[0], ballColor[1], ballColor[2], ballColor[3]};

	glVertexAttribPointer(POSITION_PARAMETER_INDEX, POSITION_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, ball);
	glVertexAttribPointer(COLOR_PARAMETER_INDEX, COLOR_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, &ball[3]);
	glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM_VERTICES);

	GLfloat blockColors[][4] = { {0.0f, 1.0f, 0.0f, 1.0f},{0.0f, 0.0f, 1.0f, 1.0f} };
	for (int32_t i=0; i<NUM_BLOCKS; ++i)
	{
		block& currentBlock = engine->blocks[i];
		if (currentBlock.isActive)
		{
			const int32_t colorIndex = i % 2;
			const float r = blockColors[colorIndex][0];
			const float g = blockColors[colorIndex][1];
			const float b = blockColors[colorIndex][2];
			const float a = blockColors[colorIndex][3];
			const float left = currentBlock.x - BLOCK_HALF_WIDTH;
			const float right = currentBlock.x + BLOCK_HALF_WIDTH;
			const float top = currentBlock.y + BLOCK_HALF_HEIGHT;
			const float bottom = currentBlock.y - BLOCK_HALF_HEIGHT;
			GLfloat block[] = {left, top, z,
					r, g, b, a,
					left, bottom, z,
					r, g, b, a,
					right, top, z,
					r, g, b, a,
					right, top, z,
					r, g, b, a,
					left, bottom, z,
					r, g, b, a,
					right, bottom, z,
					r, g, b, a};

			glVertexAttribPointer(POSITION_PARAMETER_INDEX, POSITION_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, block);
			glVertexAttribPointer(COLOR_PARAMETER_INDEX, COLOR_NUM_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_SIZE, &block[3]);
			glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM_VERTICES);
		}
	}

	glDisableVertexAttribArray(POSITION_PARAMETER_INDEX);
	glDisableVertexAttribArray(COLOR_PARAMETER_INDEX);

	eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine)
{
	if (engine->display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		int32_t ret = 0;

		int32_t action = AMotionEvent_getAction(event);
		if (action == AMOTION_EVENT_ACTION_DOWN)
		{
			engine->touchIsDown = true;
			ret = 1;
		}
		else if (action == AMOTION_EVENT_ACTION_UP)
		{
			engine->touchIsDown = false;
			ret = 1;
		}

		if (ret)
		{
			engine->touchX = static_cast<float>(AMotionEvent_getRawX(event, 0)) / engine->width;
			engine->touchY = static_cast<float>(AMotionEvent_getRawY(event, 0)) / engine->height;
		}
		return ret;
	}
	return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
	struct engine* engine = static_cast<struct engine*>(app->userData);
	switch (cmd)
	{
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if (engine->app->window != NULL)
		{
			engine_init_display(engine);
			engine_draw_frame(engine);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		engine_term_display(engine);
		break;
	}
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state)
{
	struct engine engine;

	// Make sure glue isn't stripped.
	app_dummy();

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	// loop waiting for stuff to do.
	while (1) {
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident=ALooper_pollAll(0, NULL, &events, reinterpret_cast<void**>(&source))) >= 0)
		{
			// Process this event.
			if (source != NULL)
			{
				source->process(state, source);
			}

			// Check if we are exiting.
			if (state->destroyRequested != 0)
			{
				engine_term_display(&engine);
				return;
			}
		}

		// Done with events; draw next animation frame.
		// Drawing is throttled to the screen update rate, so there
		// is no need to do timing here.
		engine_update_frame(&engine);
		engine_draw_frame(&engine);
	}
}
