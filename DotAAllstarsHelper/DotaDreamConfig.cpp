#include "Main.h"
#include <Tools.h>
#include <UISimpleFrame.h>
#include <Button.h>
#include <CheckBox.h>
#include <Label.h>
#include <ToolTip.h>
#include <HotkeyButton.h>
#include <Game.h>
#include <War3Window.h>
#include <KeyCode.h>
#include "IniReader.h"
#include "IniWriter.h"

Button * ShowConfigButton = NULL;

bool ConfigWindowHidden = true;
bool ConfigWindowInitialized = false;
UISimpleFrame * mainFrame = NULL;
UISimpleFrame * navigationFrame = NULL;
UISimpleFrame * contentFrame = NULL;

const float OPTIONMENU_WIDTH = 0.6f;
const float OPTIONMENU_HEIGHT = 0.375f;
const float OPTIONMENU_CENTER_X = 0.385f;
const float OPTIONMENU_CENTER_Y = 0.36f;
const float NAVIGATION_WIDTH = 0.16f;


UISimpleFrame * contentFrame1 = NULL;
UISimpleFrame * contentFrame2 = NULL;
UISimpleFrame * contentFrame3 = NULL;
UISimpleFrame * contentFrame4 = NULL;

Button * ShowConfigButton1 = NULL;
Button * ShowConfigButton2 = NULL;
Button * ShowConfigButton3 = NULL;
Button * ShowConfigButton4 = NULL;

Label * ConfigLabel1 = NULL;
Label * ConfigLabel2 = NULL;
Label * ConfigLabel3 = NULL;
Label * ConfigLabel4 = NULL;


bool IfNeedSkipAllKeyMessages()
{
	return  HotkeyButton::IsAnyHotkeyActive();
}


void ShowConfigWindowByID(int id)
{



}

void ConfigWindow_Show()
{
	mainFrame->show();
}

void ConfigWindow_Hide()
{
	mainFrame->hide();
}

void ShowCategory1(Button *button)
{
	contentFrame1->show();
	contentFrame2->hide();
	contentFrame3->hide();
	contentFrame4->hide();
	UI_Refresh();
}
void ShowCategory2(Button *button)
{
	contentFrame1->hide();
	contentFrame2->show();
	contentFrame3->hide();
	contentFrame4->hide();
	UI_Refresh();
}
void ShowCategory3(Button *button)
{
	contentFrame1->hide();
	contentFrame2->hide();
	contentFrame3->show();
	contentFrame4->hide();
	UI_Refresh();
}
void ShowCategory4(Button *button)
{
	contentFrame1->hide();
	contentFrame2->hide();
	contentFrame3->hide();
	contentFrame4->show();
	UI_Refresh();
}

void ToggleConfigWindowState(Button *btn)
{
	ConfigWindowHidden = !ConfigWindowHidden;

	if (ShowConfigButton)
	{
		if (ConfigWindowHidden)
		{
			ConfigWindow_Hide();
			ShowConfigButton->setText("Show dota config");
		}
		else
		{
			ConfigWindow_Show();
			ShowConfigButton->setText("Hide dota config");
		}
	}
	

}

void ConfigWindowKeyDown(const Event * evt)
{
	KeyboardEventData *data = evt->data<KeyboardEventData>();
	int keyCode = data->code;
	if (keyCode == KEYCODE::KEY_F7)
	{
		ToggleConfigWindowState(NULL);
	}
}

void InitConfigMenu1()
{

}

void InitConfigMenu2()
{

}

void InitConfigMenu3()
{

}
int SkillPanelHotkeys_NORMAL[12];
int SkillPanelHotkeys_QUICK[12];
int SkillPanelHotkeys_AUTO[12];





void SaveDotaConfig()
{
	CIniWriter DotaConfigIni(configfilename);

	DotaConfigIni.WriteString("HOTKEYS", "BindMove", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[0]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "BindStop", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[1]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "BindHold", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[2]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "BindAttack", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[3]).c_str());

	DotaConfigIni.WriteString("HOTKEYS", "BindPatrol", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[4]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "SkillSlot5", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[5]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "SkillSlot6", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[6]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "BindOpenHeroSkills", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[7]).c_str());

	DotaConfigIni.WriteString("HOTKEYS", "SkillSlot1", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[8]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "SkillSlot2", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[9]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "SkillSlot3", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[10]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "SkillSlot4", CovertKeyCodeToString(SkillPanelHotkeys_NORMAL[11]).c_str());


	DotaConfigIni.WriteString("HOTKEYS", "QuickCastSlot5", CovertKeyCodeToString(SkillPanelHotkeys_QUICK[5]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "QuickCastSlot6", CovertKeyCodeToString(SkillPanelHotkeys_QUICK[6]).c_str());

	DotaConfigIni.WriteString("HOTKEYS", "QuickCastSlot1", CovertKeyCodeToString(SkillPanelHotkeys_QUICK[8]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "QuickCastSlot2", CovertKeyCodeToString(SkillPanelHotkeys_QUICK[9]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "QuickCastSlot3", CovertKeyCodeToString(SkillPanelHotkeys_QUICK[10]).c_str());
	DotaConfigIni.WriteString("HOTKEYS", "QuickCastSlot4", CovertKeyCodeToString(SkillPanelHotkeys_QUICK[11]).c_str());


}


void NeedUpdateConfig(Button* button)
{
	SaveDotaConfig();
}

void InitConfigMenu4()
{
	int id = 0;
	for (int Y = 0; Y < 3; Y++)
	{
		for (int X = 0; X < 4; X++)
		{
			HotkeyButton*	BtnMenuHotkey;
			BtnMenuHotkey = new HotkeyButton(
				contentFrame4,
				0.11f, 0.035f,
				&SkillPanelHotkeys_NORMAL[id],
				SkillPanelHotkeys_NORMAL[id],
				0, 0, NeedUpdateConfig,true);
			BtnMenuHotkey->setRelativePosition(
				POS_UL,
				contentFrame4,
				POS_UL,
				0.02f + (X * 0.1f), -0.05f + (Y * -0.035f));
		
			id++;
		}
	}

	id = 0;
	for (int Y = 0; Y < 3; Y++)
	{
		for (int X = 0; X < 4; X++)
		{
			HotkeyButton*	BtnMenuHotkey;
			BtnMenuHotkey = new HotkeyButton(
				contentFrame4,
				0.11f, 0.035f,
				&SkillPanelHotkeys_QUICK[id],
				SkillPanelHotkeys_QUICK[id],
				0,0, NeedUpdateConfig,true);

			BtnMenuHotkey->setRelativePosition(
				POS_UL,
				contentFrame4,
				POS_UL,
				0.02f + (X * 0.1f), -0.18f + (Y * -0.035f));

			if ((id >= 5 && id <= 6)
				|| (id >= 8 && id <= 11))
			{
				;
			}
			else
			{
				BtnMenuHotkey->setText("");
				BtnMenuHotkey->enable(false);
			}
			id++;
		}
	}

}

const char * configfilename = ".\\config.dota.ini";


void LoadDotaConfig()
{
	CIniReader DotaConfigIni(configfilename);

	SkillPanelHotkeys_NORMAL[0] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "BindMove", "0"));
	SkillPanelHotkeys_NORMAL[1] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "BindStop", "0"));
	SkillPanelHotkeys_NORMAL[2] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "BindHold", "0"));
	SkillPanelHotkeys_NORMAL[3] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "BindAttack", "0"));

	SkillPanelHotkeys_NORMAL[4] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "BindPatrol", "0"));
	SkillPanelHotkeys_NORMAL[5] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "SkillSlot5", "0"));
	SkillPanelHotkeys_NORMAL[6] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "SkillSlot6", "0"));
	SkillPanelHotkeys_NORMAL[7] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "BindOpenHeroSkills", "0"));

	SkillPanelHotkeys_NORMAL[8] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "SkillSlot1", "0"));
	SkillPanelHotkeys_NORMAL[9] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "SkillSlot2", "0"));
	SkillPanelHotkeys_NORMAL[10] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "SkillSlot3", "0"));
	SkillPanelHotkeys_NORMAL[11] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "SkillSlot4", "0"));



	SkillPanelHotkeys_QUICK[5] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "QuickCastSlot5", "0"));
	SkillPanelHotkeys_QUICK[6] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "QuickCastSlot6", "0"));

	SkillPanelHotkeys_QUICK[8] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "QuickCastSlot1", "0"));
	SkillPanelHotkeys_QUICK[9] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "QuickCastSlot2", "0"));
	SkillPanelHotkeys_QUICK[10] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "QuickCastSlot3", "0"));
	SkillPanelHotkeys_QUICK[11] = CovertStringToKeyCode(DotaConfigIni.ReadString("HOTKEYS", "QuickCastSlot4", "0"));


}

void InitConfigWindow(const Event *)
{
	if (!ConfigWindowInitialized)
	{
		LoadDotaConfig();

		cout << "Load config";

		ShowConfigButton = new Button(
			UISimpleFrame::GetGameSimpleConsole(),
			0.09f, 0.027f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			&ToggleConfigWindowState,
			NULL,
			false,
			false,
			0.011f
		);

		ShowConfigButton->setText("Show dota config");
		ShowConfigButton->setAbsolutePosition(UILayoutFrame::POSITION_BOTTOM_RIGHT,
			0.79f, 0.155f);
		MainDispatcher()->listen(EVENT_KEY_DOWN, ConfigWindowKeyDown);


		mainFrame = UISimpleFrame::Create(UISimpleFrame::GetGameSimpleConsole());
		mainFrame->setBackground(Skin::getPathByName("EscMenuBackground"));
		mainFrame->setBorder(Skin::getPathByName("EscMenuBorder"));
		mainFrame->setBorderWidth(0.015f);
		mainFrame->setPadding(0.005f);
		mainFrame->applyTextureSettings();
		mainFrame->setWidth(OPTIONMENU_WIDTH);
		mainFrame->setHeight(OPTIONMENU_HEIGHT);
		mainFrame->setAbsolutePosition(UILayoutFrame::POSITION_CENTER, OPTIONMENU_CENTER_X, OPTIONMENU_CENTER_Y);
		mainFrame->applyPosition();
		mainFrame->show();
		

		navigationFrame = UISimpleFrame::Create(mainFrame);
		navigationFrame->setBackground("");
		navigationFrame->setBorder(Skin::getPathByName("EscMenuBorder"));
		navigationFrame->setBorderWidth(0.015f);
		navigationFrame->setPadding(0.005f);
		navigationFrame->applyTextureSettings();
		navigationFrame->setWidth(NAVIGATION_WIDTH);
		navigationFrame->setHeight(OPTIONMENU_HEIGHT);
		navigationFrame->setRelativePosition(POS_UL, mainFrame, POS_UL, 0, 0);
		navigationFrame->applyPosition();
		navigationFrame->show();

		contentFrame = UISimpleFrame::Create(mainFrame);
		contentFrame->setBackground("");
		contentFrame->setBorder(Skin::getPathByName("EscMenuBorder"));
		contentFrame->setBorderWidth(0.015f);
		contentFrame->setPadding(0.005f);
		contentFrame->applyTextureSettings();
		contentFrame->setWidth(OPTIONMENU_WIDTH - NAVIGATION_WIDTH);
		contentFrame->setHeight(OPTIONMENU_HEIGHT);
		contentFrame->setRelativePosition(UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0);
		contentFrame->applyPosition();
		contentFrame->show();
		contentFrame1 = UISimpleFrame::Create(mainFrame);
		contentFrame1->setWidth(OPTIONMENU_WIDTH - NAVIGATION_WIDTH);
		contentFrame1->setHeight(OPTIONMENU_HEIGHT);
		contentFrame1->setRelativePosition(UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0);
		contentFrame1->applyPosition();

		ConfigLabel1 = new Label(contentFrame1, "Settings menu 1", 0.02f);
		ConfigLabel1->setRelativePosition(
			POS_UL,
			contentFrame1,
			POS_UL,
			0.02f, -0.01f);


		InitConfigMenu1();

		contentFrame1->hide();

		contentFrame2 = UISimpleFrame::Create(mainFrame);
		contentFrame2->setWidth(OPTIONMENU_WIDTH - NAVIGATION_WIDTH);
		contentFrame2->setHeight(OPTIONMENU_HEIGHT);
		contentFrame2->setRelativePosition(UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0);
		contentFrame2->applyPosition();
		ConfigLabel2 = new Label(contentFrame2, "Settings menu 2", 0.02f);
		ConfigLabel2->setRelativePosition(
			POS_UL,
			contentFrame2,
			POS_UL,
			0.02f, -0.01f);

		InitConfigMenu2();

		contentFrame2->hide();

		contentFrame3 = UISimpleFrame::Create(mainFrame);
		contentFrame3->setWidth(OPTIONMENU_WIDTH - NAVIGATION_WIDTH);
		contentFrame3->setHeight(OPTIONMENU_HEIGHT);
		contentFrame3->setRelativePosition(UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0);
		contentFrame3->applyPosition();
		ConfigLabel3 = new Label(contentFrame3, "Settings menu 3", 0.02f);
		ConfigLabel3->setRelativePosition(
			POS_UL,
			contentFrame3,
			POS_UL,
			0.02f, -0.01f);


		InitConfigMenu3();

		contentFrame3->hide();


		contentFrame4 = UISimpleFrame::Create(mainFrame);
		contentFrame4->setWidth(OPTIONMENU_WIDTH - NAVIGATION_WIDTH);
		contentFrame4->setHeight(OPTIONMENU_HEIGHT);
		contentFrame4->setRelativePosition(UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0);
		contentFrame4->applyPosition();

		ConfigLabel4 = new Label(contentFrame4, "Settings menu 4", 0.02f);
		ConfigLabel4->setRelativePosition(
			POS_UL,
			contentFrame4,
			POS_UL,
			0.02f, -0.01f);


		InitConfigMenu4();

		contentFrame4->hide();



		ShowConfigButton1 = new Button(navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory1,
			0,
			true
		);
		ShowConfigButton1->setText("category 1");
		ShowConfigButton1->setRelativePosition(POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-(0.035f*0 + 0.03f)
		);
		ShowConfigButton1->applyPosition();

		ShowConfigButton2 = new Button(navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory2,
			0,
			true
		);
		ShowConfigButton2->setText("category 2");
		ShowConfigButton2->setRelativePosition(POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-(0.035f * 1 + 0.03f)
		);
		ShowConfigButton2->applyPosition();


		ShowConfigButton3 = new Button(navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory3,
			0,
			true
		);
		ShowConfigButton3->setText("category 3");
		ShowConfigButton3->setRelativePosition(POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-(0.035f * 2 + 0.03f)
		);
		ShowConfigButton3->applyPosition();


		ShowConfigButton4 = new Button(navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory4,
			0,
			true
		);
		ShowConfigButton4->setText("category 4");
		ShowConfigButton4->setRelativePosition(POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-(0.035f * 3 + 0.03f)
		);
		ShowConfigButton4->applyPosition();



		ShowCategory1(0);

		mainFrame->hide();

		ConfigWindowInitialized = true;
	}

}


void CleanupConfigWindow(const Event *)
{
	if (ConfigWindowInitialized)
	{
		UISimpleFrame::Destroy(contentFrame);
		UISimpleFrame::Destroy(contentFrame1);
		UISimpleFrame::Destroy(contentFrame2);
		UISimpleFrame::Destroy(contentFrame3);
		UISimpleFrame::Destroy(contentFrame4);

		UISimpleFrame::Destroy(navigationFrame);
		UISimpleFrame::Destroy(mainFrame);

		delete mainFrame;
		delete contentFrame;
		delete navigationFrame;
	}
	ConfigWindowInitialized = false;
}

void RegisterConfigWindow()
{
	MainDispatcher()->listen(EVENT_GAME_START, InitConfigWindow);
	MainDispatcher()->listen(EVENT_GAME_END, CleanupConfigWindow);
}


