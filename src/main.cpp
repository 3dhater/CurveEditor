#include "KrGui.h"
#include "krGuiOpenGL.h"

bool gSystemRun = true;
#include "CurveEditor.h"

using namespace Kr;

int main()
{
	CEMainSystem * cEditor = CE_Create();
	CEWindow * main_window = cEditor->CreateWindow(800,600);
	
	CEGS* gs = cEditor->CreateGS(DemoGSType::ModernOpenGL, main_window);
	gs->SetClearColor(0.304232f, 0.3041f, 0.348798f, 1.f);

	Gui::GuiSystem * gui = Gui::CreateSystem(Gui::GraphicsSystemType::OpenGL3, "../res/fonts/noto/", "notosans.txt" );
	if(!gui)
	{
		fprintf(stderr, "%s", "Can't init GUI\n");
		return 666;
	}
	Gui::Window guiWindow_main;
	guiWindow_main.OSWindow = main_window->GetHandle();

	auto defaultFont = gui->getCurrentFont();

	Gui::Font * iconFont = gui->createFontFromDDS_RGBA8(u"../res/icons.dds");
	Gui::Vec2f iconFontImageSize(100.f,100.f);
	iconFont->addCharacter(u'1', Gui::Vec4f(67.f, 29.f, 84.f, 47.f), 0, 0, 0, iconFontImageSize.x, iconFontImageSize.y );

	gui->setCurrentFont(defaultFont);

	float * dt = cEditor->GetDeltaTime();

	while(cEditor->Update())
	{
		gs->BeginDraw(true);
		gs->SetActive(main_window);
		
		gui->newFrame(&guiWindow_main, *dt );
		if( gui->menuBarBegin() )
		{
			if( gui->menuBarMenu(u"File") )
			{
				bool showMenu = true;
				if(gui->popupMenuBegin( &showMenu ))
				{
					if(gui->addMenuItem(u"Exit", u"Alt + F4", 0 ))
					{
						cEditor->Quit();
					}
					gui->popupMenuEnd();
				}
			}
			if( gui->menuBarMenu(u"TAbout") )
			{
			}

			gui->menuBarEnd();
		}
		gui->render();

		gui->endFrame();
		gs->EndDraw();
	}


	delete cEditor;
	return 0;
}