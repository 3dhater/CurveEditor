#ifndef __CEGSOPENGL_H__
#define __CEGSOPENGL_H__

class CEGSModernOpenGL : public CEGS
{
#ifdef KRGUI_PLATFORM_WINDOWS
	HGLRC m_renderingContext = nullptr;
	HDC m_dc = nullptr;
#endif
public:

	CEGSModernOpenGL(CEWindow* mainWindow)
	{
		Kr::loadOpenGLProcs();
#ifdef KRGUI_PLATFORM_WINDOWS
		WNDCLASSEX wc;
		memset(&wc,0,sizeof(WNDCLASSEX));
		wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc   = DefWindowProc;
		wc.hInstance     = GetModuleHandle(0);
		wc.lpszClassName = L"OPENGL";
		wc.cbSize        = sizeof(WNDCLASSEX);
		RegisterClassEx(&wc);
		auto tmp_hwnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, L"q", WS_POPUP,0, 0, 128, 128, NULL, NULL, wc.hInstance, NULL);
		ShowWindow(tmp_hwnd, SW_HIDE);
		HDC dc = GetDC( tmp_hwnd );
		PIXELFORMATDESCRIPTOR pf;
		SetPixelFormat(dc, 1, &pf);
		auto rc = gwglCreateContext(dc);
		gwglMakeCurrent(dc, rc);
		Kr::loadOpenGLProcs();
		//int maxSamples = 0;
		//gglGetIntegerv( GL_MAX_SAMPLES, &maxSamples );
		gwglMakeCurrent(NULL, NULL);
		gwglDeleteContext(rc);
		ReleaseDC(tmp_hwnd, dc);
		DestroyWindow(tmp_hwnd);
		InitWindow(mainWindow);
		m_dc   = ((CEWindowWin32*)mainWindow)->m_dc;
		int v_maj = 4;
		int v_min = 6;
		while(true)
		{
			int attributeList[5];
			attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
			attributeList[1] = v_maj;
			attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
			attributeList[3] = v_min;
			attributeList[4] = 0;// Null terminate the attribute list.
			m_renderingContext = gwglCreateContextAttribsARB(m_dc, 0, attributeList);
			if(m_renderingContext)
				break;
			else
			{
				--v_min;
				if( v_min < 0 ){
					if( v_maj == 3 )
						break;
					v_min = 6;
					--v_maj;
				}
			}
		}
		fprintf(stdout, "Init OpenGL %i.%i\n", v_maj, v_min);
		SetActive(mainWindow);
		gwglSwapIntervalEXT(1);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		glViewport(0, 0, mainWindow->GetWindowWidth(), mainWindow->GetWindowHeight());
#else
#error For Windows
#endif
	}

	virtual ~CEGSModernOpenGL(){}

	virtual void InitWindow(CEWindow* window)
	{
#ifdef KRGUI_PLATFORM_WINDOWS
		CEWindowWin32 * w = (CEWindowWin32*)window;
		auto dc   = w->m_dc;
		if(!dc) return;
		int attributeListInt[] = 
		{
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,     32,
			WGL_DEPTH_BITS_ARB,     24,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_SWAP_METHOD_ARB,    WGL_SWAP_EXCHANGE_ARB,
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
			WGL_STENCIL_BITS_ARB,   8,
			//WGL_SAMPLE_BUFFERS_ARB, 1,
			//WGL_SAMPLES_ARB,        4,//maxSamples,
			0
		};
		int pixelFormat[1];
		unsigned int formatCount;
		auto result = gwglChoosePixelFormatARB(dc, attributeListInt, NULL, 1, pixelFormat, &formatCount);
		if(result != 1) return;
		PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
		result = SetPixelFormat(dc, pixelFormat[0], &pixelFormatDescriptor);
		if(result != 1) return;
#else
#error For Windows
#endif
	}

	virtual void SetActive(CEWindow* window)
	{
#ifdef KRGUI_PLATFORM_WINDOWS
		CEWindowWin32 * w = (CEWindowWin32*)window;
		m_dc   = w->m_dc;
		gwglMakeCurrent(m_dc, m_renderingContext);
#endif
	}

	virtual void SetClearColor(float red, float green, float blue, float alpha)
	{
		glClearColor( red, green, blue, alpha ); 
	}

	virtual void BeginDraw( bool clear_canvas )
	{
		if( clear_canvas )
		{
			GLbitfield mask = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
			glClear(mask);
		}
	}

	virtual void EndDraw()
	{
#ifdef KRGUI_PLATFORM_WINDOWS
		SwapBuffers( m_dc );
#endif
	}
};

#endif