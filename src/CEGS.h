#ifndef __CEGS_H__
#define __CEGS_H__

enum class DemoGSType
{
	ModernOpenGL
};

class CEGS
{
public:
	CEGS(){}
	virtual ~CEGS(){}
	virtual void InitWindow(CEWindow* window)=0;
	virtual void SetActive(CEWindow* window)=0;
	virtual void SetClearColor(float red, float green, float blue, float alpha)=0;
	virtual void BeginDraw( bool clear_canvas )=0;
	virtual void EndDraw()=0;
};

#include "CEGSOpenGL.h"


#endif