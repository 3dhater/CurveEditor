#ifndef __CEMS_H__
#define __CEMS_H__

class CEMainSystem{
protected:
	std::vector<CEWindow*> m_windows;
	float m_dt = 0.f;
public:

	CEMainSystem(){}
	virtual ~CEMainSystem()
	{
		for(auto w : m_windows)
		{
			delete w;
		}
	}
	void Quit(){gSystemRun = false;}
	float* GetDeltaTime(){return &m_dt;}
	unsigned long long GetTime()
	{
		static bool isInit = false;
		static unsigned long long baseTime;
		if( !isInit )
		{
			auto now = std::chrono::high_resolution_clock::now();
			baseTime = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
			isInit = true;
		}
		auto now = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

		return ms - baseTime;
	}

	virtual CEWindow * CreateWindow(int x, int y) = 0;

	bool Update()
	{
		UpdateOS();
		static unsigned long long t1 = 0u;
		unsigned long long t2 = this->GetTime();
		float m_tick = float(t2 - t1);
		t1 = t2;

		m_dt = m_tick / 1000.f;
		return gSystemRun;
	};
	
	virtual void UpdateOS()=0;

	virtual CEGS* CreateGS(DemoGSType type, CEWindow* mainWindow)
	{
		switch (type)
		{
		case DemoGSType::ModernOpenGL:
			return new CEGSModernOpenGL(mainWindow);
			break;
		default:
			fprintf(stderr,"%s\n","GS type not implement");
			break;
		}
		return nullptr;
	}
};

#ifdef KRGUI_PLATFORM_WINDOWS
class CEMainSystemWin32 : public CEMainSystem
{
public:
	CEMainSystemWin32(){}
	virtual ~CEMainSystemWin32(){}

	virtual CEWindow * CreateWindow(int x, int y)
	{
		CEWindowWin32 * new_window = new CEWindowWin32(x,y);
		m_windows.push_back(new_window);
		return new_window;
	}
	virtual void UpdateOS()
	{
		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}
};
CEMainSystem* CE_Create()
{
	return new CEMainSystemWin32;
}
#else
#error For Windows
#endif

#endif