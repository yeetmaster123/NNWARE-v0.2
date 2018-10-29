#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CInputSystem.h"
#include "../UTILS/render.h"
#include "../SDK/ConVar.h"

#include "../InGameLogger.h"

#include "TMenu.h"

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(UTILS::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 8D 58") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39"));
	return (T*)find_hud_element(pThis, name);
}

void KnifeApplyCallbk()
{

	static auto clear_hud_weapon_icon_fn =
		reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
			UTILS::FindSignature("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C"));

	auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0x9c);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = clear_hud_weapon_icon_fn(hud_weapons, i);

	typedef void(*ForceUpdate) (void);
	ForceUpdate FullUpdate = (ForceUpdate)UTILS::FindSignaturenew("engine.dll", "FullUpdate", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
	FullUpdate();
}


int AutoCalc(int va)
{
	if (va == 1)
		return va * 35;
	else if (va == 2)
		return va * 34;
	else
		return va * 25 + 7.5;
}

#define RANDOMIZECOL CColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1)

namespace TGUI
{
	namespace MENU
	{
		void Do()
		{
			auto SetColor = []() -> void
			{
				const auto theme_color = HELPERS::TGUI_Color::LightBlue();

				tgui_colors[TGUI_COLOR_WINDOW_BODY] = HELPERS::TGUI_Color(40, 40, 40, 246);
				tgui_colors[TGUI_COLOR_WINDOW_BODY_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 100);
				tgui_colors[TGUI_COLOR_WINDOW_TITLE_BAR] = HELPERS::TGUI_Color(50, 50, 50, 246);
				tgui_colors[TGUI_COLOR_WINDOW_TITLE_BAR_TEXT] = theme_color;
				tgui_colors[TGUI_COLOR_WINDOW_TITLE_BAR_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_WINDOW_TITLE_BAR_BACKGROUND_TEXT] = HELPERS::TGUI_Color(19, 160, 216, 30);

				tgui_colors[TGUI_COLOR_CHECKBOX_UNSELECTED_BODY] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_CHECKBOX_UNSELECTED_BODY_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_CHECKBOX_SELECTED_BODY] = theme_color;
				tgui_colors[TGUI_COLOR_CHECKBOX_SELECTED_BODY_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_CHECKBOX_TEXT] = HELPERS::TGUI_Color(200, 200, 200, 255);
				tgui_colors[TGUI_COLOR_CHECKBOX_HOVERED_TEXT] = HELPERS::TGUI_Color::White();

				tgui_colors[TGUI_COLOR_BUTTON_BODY] = HELPERS::TGUI_Color(80, 80, 80, 246);
				tgui_colors[TGUI_COLOR_BUTTON_BODY_CLICKED] = HELPERS::TGUI_Color(100, 100, 100, 246);
				tgui_colors[TGUI_COLOR_BUTTON_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_BUTTON_TEXT] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_BUTTON_TEXT_CLICKED] = theme_color;

				tgui_colors[TGUI_COLOR_GROUPBOX_BODY] = HELPERS::TGUI_Color(50, 50, 50, 246);
				tgui_colors[TGUI_COLOR_GROUPBOX_BODY_HOVERED] = HELPERS::TGUI_Color(50, 50, 50, 246);
				tgui_colors[TGUI_COLOR_GROUPBOX_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_GROUPBOX_TEXT] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_GROUPBOX_TEXT_HOVERED] = HELPERS::TGUI_Color::White();

				tgui_colors[TGUI_COLOR_SLIDER_BODY] = HELPERS::TGUI_Color(80, 80, 80, 246);
				tgui_colors[TGUI_COLOR_SLIDER_BODY_SELECTED] = theme_color;
				tgui_colors[TGUI_COLOR_SLIDER_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_SLIDER_TEXT_NAME] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_SLIDER_TEXT_NAME_HOVERED] = HELPERS::TGUI_Color::White();
				tgui_colors[TGUI_COLOR_SLIDER_VALUE_TEXT] = HELPERS::TGUI_Color::White();

				tgui_colors[TGUI_COLOR_SEPARATOR_BODY] = HELPERS::TGUI_Color(100, 100, 100, 246);
				tgui_colors[TGUI_COLOR_SEPARATOR_TEXT] = HELPERS::TGUI_Color::White();

				tgui_colors[TGUI_COLOR_KEYBIND_BODY] = HELPERS::TGUI_Color(80, 80, 80, 246);
				tgui_colors[TGUI_COLOR_KEYBIND_BODY_SELECTED] = HELPERS::TGUI_Color(110, 110, 110, 246);
				tgui_colors[TGUI_COLOR_KEYBIND_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_KEYBIND_TEXT_NAME] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_KEYBIND_TEXT_NAME_HOVERED] = HELPERS::TGUI_Color::White();
				tgui_colors[TGUI_COLOR_KEYBIND_TEXT_VALUE] = HELPERS::TGUI_Color::White();

				tgui_colors[TGUI_COLOR_TEXT_INPUT_BODY] = HELPERS::TGUI_Color(80, 80, 80, 246);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_BODY_SELECTED] = HELPERS::TGUI_Color(110, 110, 110, 246);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_TEXT_VALUE] = HELPERS::TGUI_Color::White();
				tgui_colors[TGUI_COLOR_TEXT_INPUT_TEXT_NAME] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_TEXT_HOVERED] = HELPERS::TGUI_Color::White();

				tgui_colors[TGUI_COLOR_COMBOBOX_BODY_SELECTED] = HELPERS::TGUI_Color(80, 80, 80, 246);
				tgui_colors[TGUI_COLOR_COMBOBOX_OUTLINE_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_COMBOBOX_BODY_ITEM] = HELPERS::TGUI_Color(50, 50, 50, 246);
				tgui_colors[TGUI_COLOR_COMBOBOX_OUTLINE_ITEM] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_COMBOBOX_SELECTED_TEXT_ITEM] = theme_color;
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_ITEM] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_SELECTED] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_SELECTED_HOVERED] = HELPERS::TGUI_Color::White();
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_NAME] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_NAME_HOVERED] = HELPERS::TGUI_Color::White();

				tgui_colors[TGUI_COLOR_COLOR_PICKER_BODY_COLOR] = HELPERS::TGUI_Color(50, 50, 50, 246);
				tgui_colors[TGUI_COLOR_COLOR_PICKER_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);
				tgui_colors[TGUI_COLOR_COLOR_PICKER_TEXT] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_COLOR_PICKER_TEXT_HOVERED] = HELPERS::TGUI_Color::White();

				tgui_colors[TGUI_COLOR_TAB_BODY] = HELPERS::TGUI_Color(40, 40, 40, 246);
				tgui_colors[TGUI_COLOR_TAB_BODY_SELECTED] = HELPERS::TGUI_Color(70, 70, 70, 246);
				tgui_colors[TGUI_COLOR_TAB_TEXT] = HELPERS::TGUI_Color(255, 255, 255, 255);
				tgui_colors[TGUI_COLOR_TAB_TEXT_SELECTED] = theme_color;
				tgui_colors[TGUI_COLOR_TAB_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 150);

				tgui_colors[TGUI_COLOR_TOOL_TIP_BODY] = HELPERS::TGUI_Color(200, 200, 200, 246);
				tgui_colors[TGUI_COLOR_TOOL_TIP_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 246);
				tgui_colors[TGUI_COLOR_TOOL_TIP_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 246);
			};
			auto SetAttributes = []() -> void
			{
				tgui_attributes[TGUI_ATTRIBUTE_PADDING_HEIGHT] = 12;
				tgui_attributes[TGUI_ATTRIBUTE_PADDING_WIDTH] = 12;
				tgui_attributes[TGUI_ATTRIBUTE_SCROLL_SPEED] = 10;

				tgui_attributes[TGUI_ATTRIBUTE_WINDOW_TITLE_BAR_HEIGHT] = 30;

				tgui_attributes[TGUI_ATTRIBUTE_CHECKBOX_HEIGHT] = 10;
				tgui_attributes[TGUI_ATTRIBUTE_CHECKBOX_WIDTH] = 10;

				tgui_attributes[TGUI_ATTRIBUTE_BUTTON_HEIGHT] = 16;
				tgui_attributes[TGUI_ATTRIBUTE_BUTTON_WIDTH] = 100;
				tgui_attributes[TGUI_ATTRIBUTE_BUTTON_CLICK_DURATION] = 0.15f;

				tgui_attributes[TGUI_ATTRIBUTE_SLIDER_HEIGHT] = 6;
				tgui_attributes[TGUI_ATTRIBUTE_SLIDER_WIDTH] = 200;

				tgui_attributes[TGUI_ATTRIBUTE_KEYBIND_HEIGHT] = 14;
				tgui_attributes[TGUI_ATTRIBUTE_KEYBIND_WIDTH] = 100;

				tgui_attributes[TGUI_ATTRIBUTE_TEXT_INPUT_HEIGHT] = 14;
				tgui_attributes[TGUI_ATTRIBUTE_TEXT_INPUT_WIDTH] = 200;

				tgui_attributes[TGUI_ATTRIBUTE_COMBOBOX_WIDTH] = 200;
				tgui_attributes[TGUI_ATTRIBUTE_COMBOBOX_HEIGHT] = 14;
				tgui_attributes[TGUI_ATTRIBUTE_COMBOBOX_MAX_ITEMS_DISPLAYED] = 8;

				tgui_attributes[TGUI_ATTRIBUTE_COLOR_PICKER_WIDTH] = 30;
				tgui_attributes[TGUI_ATTRIBUTE_COLOR_PICKER_HEIGHT] = 10;
				tgui_attributes[TGUI_ATTRIBUTE_COLOR_PICKER_OPEN_WIDTH] = 250;
				tgui_attributes[TGUI_ATTRIBUTE_COLOR_PICKER_OPEN_HEIGHT] = 150;
				tgui_attributes[TGUI_ATTRIBUTE_COLOR_PICKER_SLIDER_WIDTH] = 14;
				tgui_attributes[TGUI_ATTRIBUTE_COLOR_PICKER_PADDING] = 8;
				tgui_attributes[TGUI_ATTRIBUTE_COLOR_PICKER_PIXELATION] = 6;

				tgui_attributes[TGUI_ATTRIBUTE_TAB_HEIGHT] = 16;
				tgui_attributes[TGUI_ATTRIBUTE_TAB_ITEM_PADDING] = 10;

				tgui_attributes[TGUI_ATTRIBUTE_TOOL_TIP_LINE_HEIGHT] = 16;
			};
			auto SetFonts = []()
			{
				tgui_fonts[TGUI_FONT_WINDOW_NAME] = FONTS::menu_window_font;
				tgui_fonts[TGUI_FONT_WINDOW_NAME_BEHIND] = FONTS::menu_window_blurr_font;

				tgui_fonts[TGUI_FONT_GROUPBOX] = FONTS::menu_groupbox_font;

				tgui_fonts[TGUI_FONT_SEPARATOR] = FONTS::menu_separator_font;

				tgui_fonts[TGUI_FONT_CHECKBOX] = FONTS::menu_checkbox_font;

				tgui_fonts[TGUI_FONT_BUTTON] = FONTS::menu_button_font;

				tgui_fonts[TGUI_FONT_SLIDER_NAME] = FONTS::menu_slider_font;
				tgui_fonts[TGUI_FONT_SLIDER_VALUE] = FONTS::menu_slider_value_font;

				tgui_fonts[TGUI_FONT_KEYBIND_NAME] = FONTS::menu_keybind_name_font;
				tgui_fonts[TGUI_FONT_KEYBIND_VALUE] = FONTS::menu_keybind_value_font;

				tgui_fonts[TGUI_FONT_TEXT_INPUT_NAME] = FONTS::menu_text_input_name_font;
				tgui_fonts[TGUI_FONT_TEXT_INPUT_VALUE] = FONTS::menu_text_input_value_font;

				tgui_fonts[TGUI_FONT_COMBOBOX_NAME] = FONTS::menu_combobox_name_font;
				tgui_fonts[TGUI_FONT_COMBOBOX_SELECTED_ITEM] = FONTS::menu_combobox_value_font;
				tgui_fonts[TGUI_FONT_COMBOBOX_ITEM_NAME] = FONTS::menu_combobox_value_font;

				tgui_fonts[TGUI_FONT_TAB_SELECTED_ITEM_NAME] = FONTS::menu_tab_font;
				tgui_fonts[TGUI_FONT_TAB_ITEM_NAME] = FONTS::menu_tab_font;

				tgui_fonts[TGUI_FONT_TOOL_TIP] = FONTS::menu_tool_tip_font;

				tgui_fonts[TGUI_FONT_COLOR_PICKER] = FONTS::menu_colorpicker_font;
			};

			auto SetDebugColor = []()
			{
				tgui_colors[TGUI_COLOR_WINDOW_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_WINDOW_BODY_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_WINDOW_TITLE_BAR] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_WINDOW_TITLE_BAR_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_WINDOW_TITLE_BAR_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_WINDOW_TITLE_BAR_BACKGROUND_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_CHECKBOX_UNSELECTED_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_CHECKBOX_UNSELECTED_BODY_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_CHECKBOX_SELECTED_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_CHECKBOX_SELECTED_BODY_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_CHECKBOX_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_CHECKBOX_HOVERED_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_BUTTON_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_BUTTON_BODY_CLICKED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_BUTTON_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_BUTTON_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_BUTTON_TEXT_CLICKED] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_GROUPBOX_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_GROUPBOX_BODY_HOVERED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_GROUPBOX_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_GROUPBOX_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_GROUPBOX_TEXT_HOVERED] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_SLIDER_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_SLIDER_BODY_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_SLIDER_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_SLIDER_TEXT_NAME] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_SLIDER_TEXT_NAME_HOVERED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_SLIDER_VALUE_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_SEPARATOR_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_SEPARATOR_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_KEYBIND_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_KEYBIND_BODY_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_KEYBIND_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_KEYBIND_TEXT_NAME] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_KEYBIND_TEXT_NAME_HOVERED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_KEYBIND_TEXT_VALUE] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_TEXT_INPUT_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_BODY_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_TEXT_VALUE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_TEXT_NAME] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TEXT_INPUT_TEXT_HOVERED] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_COMBOBOX_BODY_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_OUTLINE_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_BODY_ITEM] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_OUTLINE_ITEM] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_SELECTED_TEXT_ITEM] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_ITEM] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_SELECTED_HOVERED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_NAME] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COMBOBOX_TEXT_NAME_HOVERED] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_COLOR_PICKER_BODY_COLOR] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COLOR_PICKER_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COLOR_PICKER_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_COLOR_PICKER_TEXT_HOVERED] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_TAB_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TAB_BODY_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TAB_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TAB_TEXT_SELECTED] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TAB_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);

				tgui_colors[TGUI_COLOR_TOOL_TIP_BODY] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TOOL_TIP_OUTLINE] = HELPERS::TGUI_Color(0, 0, 0, 0);
				tgui_colors[TGUI_COLOR_TOOL_TIP_TEXT] = HELPERS::TGUI_Color(0, 0, 0, 0);
			};

			///SetDebugColor();
			SetColor();
			SetAttributes();
			SetFonts();

			/// Start
			TGUI_BeginFrame();

			/// Menu toggle
			static bool menu_open = false;
			if (menu.GetInput().DidClickKey(VK_INSERT))
			{
				menu_open = !menu_open;
				INTERFACES::InputSystem->EnableInput(!menu_open);
			}

			if (!menu_open)
				return;

			/// Draw mouse
			TGUI_DrawMouse();

			if (UTILS::INPUT::input_handler.GetKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.flip_int)) & 1)
				SETTINGS::settings.flip_bool = !SETTINGS::settings.flip_bool;

			if (UTILS::INPUT::input_handler.GetKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.quickstopkey)) & 1)
				SETTINGS::settings.stop_flip = !SETTINGS::settings.stop_flip;

			if (UTILS::INPUT::input_handler.GetKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.overridekey)) & 1)
				SETTINGS::settings.overridething = !SETTINGS::settings.overridething;

			TGUI_BeginWindow("Nnware Alpha", HELPERS::TGUI_Vector2D(200, 200), HELPERS::TGUI_Vector2D(800, 650), 2);
			{
				static char aim_mode[][TGUI_MAX_STRING_LENGTH] = { "rage", "legit" };
				static char acc_mode[][TGUI_MAX_STRING_LENGTH] = { "head", "body", "hit-scan" };
				static char hitscan_items[][TGUI_MAX_STRING_LENGTH] = { "head", "neck", "chest", "stomach", "arms", "legs", "feet" };
				static char chams_mode[][TGUI_MAX_STRING_LENGTH] = { "none", "visible", "invisible" };
				static char aa_pitch[][TGUI_MAX_STRING_LENGTH] = { "none", "emotion", "fake down", "fake up", "fake zero" };
				static char aa_mode[][TGUI_MAX_STRING_LENGTH] = { "none", "backwards", "sideways", "backjitter", "lowerbody", "legit troll", "rotational", "freestanding" };
				static char aa_fake[][TGUI_MAX_STRING_LENGTH] = { "none", "backjitter", "random", "local view", "opposite", "rotational" };
				static char configs[][TGUI_MAX_STRING_LENGTH] = { "default", "legit", "autos", "scouts", "pistols", "awps", "nospread" };
				static char box_style[][TGUI_MAX_STRING_LENGTH] = { "none", "full", "debug" };
				static char media_style[][TGUI_MAX_STRING_LENGTH] = { "perfect", "random" };
				static char delay_shot[][TGUI_MAX_STRING_LENGTH] = { "off", "lag compensation" };
				static char fakelag_mode[][TGUI_MAX_STRING_LENGTH] = { "factor", "adaptive" };
				static char hitmarker[][TGUI_MAX_STRING_LENGTH] = { "None", "Nnware" ,"Your mad", "Skeet", "Shrek" };
				static char antiaimmode[][TGUI_MAX_STRING_LENGTH] = { "standing", "moving", "jumping" };
				static char glow_styles[][TGUI_MAX_STRING_LENGTH] = { "regular", "pulsing", "outline" };
				static char local_chams[][TGUI_MAX_STRING_LENGTH] = { "None","Sim Fakelag: Normal", "Non Sim Fakelag", "Sim Fakelag: Color", "Rainbow Regular" };
				static char chams_type[][TGUI_MAX_STRING_LENGTH] = { "metallic", "basic" };
				static char team_select[][TGUI_MAX_STRING_LENGTH] = { "enemy", "team" };
				static char crosshair_select[][TGUI_MAX_STRING_LENGTH] = { "none", "static", "recoil" };
				static char override_method[][TGUI_MAX_STRING_LENGTH] = { "set", "key-press" };
				static char clan_tag_types[][TGUI_MAX_STRING_LENGTH] = { "static", "marquee", "shift" };
				static char Box_tpye[][TGUI_MAX_STRING_LENGTH] = { "None", "Full", "Corners" };
				static char weptype[][TGUI_MAX_STRING_LENGTH] = { "Weapon", "Weapon + Text Ammo", "Weapon Icon" };
				static char KnifeModel[][TGUI_MAX_STRING_LENGTH] = { "Bayonet",
					"Flip Knife",
					"Gut Knife",
					"Karambit",
					"M9 Bayonet",
					"Huntsman Knife",
					"Butterfly Knife",
					"Falchion Knife",
					"Shadow Daggers",
					"Bowie Knife",
					"Navaja Knife",
					"Stiletto Knife",
					"Ursus Knife",
					"Talon Knife" };
				static char M4A4[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Asiimov",
					"Howl",
					"Dragon King",
					"Poseidon",
					"Daybreak",
					"Royal Paladin",
					"BattleStar",
					"Desolate Space",
					"Buzz Kill",
					"Bullet Rain",
					"Hell Fire",
					"Evil Daimyo",
					"Griffin",
					"Zirka",
					"Radiation Harzard", };
				static char knifeskins[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Crimson Web",
					"Bone Mask",
					"Fade",
					"Night",
					"Blue Steel",
					"Stained",
					"Case Hardened",
					"Slaughter",
					"Safari Mesh",
					"Boreal Forest",
					"Ultraviolet",
					"Urban Masked",
					"Scorched",
					"Rust Coat",
					"Tiger Tooth",
					"Damascus Steel",
					"Damascus Steel",
					"Marble Fade",
					"Rust Coat",
					"Doppler Ruby",
					"Doppler Sapphire",
					"Doppler Blackpearl",
					"Doppler Phase 1",
					"Doppler Phase 2",
					"Doppler Phase 3",
					"Doppler Phase 4",
					"Gamma Doppler Phase 1",
					"Gamma Doppler Phase 2",
					"Gamma Doppler Phase 3",
					"Gamma Doppler Phase 4",
					"Gamma Doppler Emerald",
					"Lore",
					"Black Laminate",
					"Autotronic",
					"Freehand",
					"Whiteout"};
				static char ak47[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Fire Serpent",
					"Fuel Injector",
					"Bloodsport",
					"Vulcan",
					"Case Hardened",
					"Hydroponic",
					"Aquamarine Revenge",
					"Frontside Misty",
					"Point Disarray",
					"Neon Revolution",
					"Red Laminate",
					"Redline",
					"Jaguar",
					"Jet Set",
					"Wasteland Rebel",
					"The Empress",
					"Elite Build",
					"Neon Rider",
					"Whiteout", };
				static char GloveModel[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Bloodhound",
					"Sport",
					"Driver",
					"Wraps",
					"Moto",
					"Specialist",
					"Whiteout" };
				static char xdshit[][TGUI_MAX_STRING_LENGTH] = { "kit1",
					"kit2",
					"kit3",
					"kit4" };
				static char Duals[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Panther",
					"Dualing Dragons",
					"Cobra Strike",
					"Royal Consorts",
					"Duelist",
					"Whiteout" };
				static char M4A1[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Decimator",
					"Knight",
					"Chantico's Fire",
					"Golden Coi",
					"Hyper Beast",
					"Master Piece",
					"Hot Rod",
					"Mecha Industries",
					"Cyrex",
					"Icarus Fell",
					"Flashback",
					"Flashback",
					"Hyper Beast",
					"Atomic Alloy",
					"Guardian",
					"Briefing",
					"Whiteout" };
				static char Usp[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Neo-Noir",
					"Cyrex",
					"Orion",
					"Kill Confirmed",
					"Overgrowth",
					"Caiman",
					"Serum",
					"Guardian",
					"Road Rash",
					"Whiteout" };
				static char Glock[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Fade",
					"Dragon Tattoo",
					"Twilight Galaxy",
					"Wasteland Rebel",
					"Water Elemental",
					"Off World",
					"Weasel",
					"Royal Legion",
					"Grinder",
					"Steel Disruption",
					"Brass",
					"Ironwork",
					"Bunsen Burner",
					"Reactor",
					"Whiteout" };
				static char Deagle[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Blaze",
					"Kumicho Dragon",
					"Oxide Blaze",
					"Golden Koi",
					"Cobalt Disruption",
					"Directive",
					"Whiteout" };
				static char Five7[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Monkey Business",
					"Hyper Beast",
					"Fowl Play",
					"Triumvirate",
					"Retrobution",
					"Capillary",
					"Violent Daimyo",
					"Whiteout" };
				static char Aug[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Bengal Tiger",
					"Hot Rod",
					"Chameleon",
					"Akihabara Accept",
					"Whiteout" };
				static char Famas[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Djinn",
					"Styx",
					"Neural Net",
					"Survivor Z",
					"Whiteout" };
				static char G3sg1Skin[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Hunter",
					"The Executioner",
					"Terrace",
					"Whiteout" };
				static char Galil[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Chatterbox",
					"Crimson Tsunami",
					"Sugar Rush",
					"Eco",
					"Cerberus",
					"Whiteout" };
				static char M249[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Nebula Crusader",
					"System Lock",
					"Magma",
					"Whiteout" };
				static char Mac10[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Neon Rider",
					"Last Dive",
					"Curse",
					"Rangeen",
					"Rangeen",
					"Whiteout" };
				static char Ump45[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Blaze",
					"Minotaur's Labyrinth",
					"Pandora's Box",
					"Primal Saber",
					"Exposure",
					"Whiteout" };
				static char XM1014[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Seasons",
					"Traveler",
					"Ziggy",
					"Whiteout" };
				static char Cz75[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Red Astor",
					"Pole Position",
					"Victoria",
					"Xiangliu",
					"Whiteout" };
				static char PPBizon[][TGUI_MAX_STRING_LENGTH] = { "none",
					"High Roller",
					"Judgement of Anubis",
					"Fuel Rod",
					"Whiteout" };
				static char Mag7[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Bulldozer",
					"Heat",
					"Petroglyph",
					"Whiteout" };
				static char Awp[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Asiimov",
					"Dragon Lore",
					"Fever Dream",
					"Medusa",
					"HyperBeast",
					"Boom",
					"Lightning Strike",
					"Pink DDpat",
					"Corticera",
					"Redline",
					"Manowar",
					"Graphite",
					"Electric Hive",
					"Sun in Leo",
					"Hyper Beast",
					"Pit viper",
					"Phobos",
					"Elite Build",
					"Worm God",
					"Oni Taiji",
					"Fever Dream",
					"Whiteout" };
				static char negev[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Power Loader",
					"Loudmouth",
					"Man-o'-war",
					"Whiteout" };
				static char Sawedoff[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Wasteland Princess",
					"The Kraken",
					"Yorick",
					"Whiteout" };
				static char tec9[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Nuclear Threat",
					"Red Quartz",
					"Blue Titanium",
					"Titanium Bit",
					"Sandstorm",
					"Isaac",
					"Toxic",
					"Re-Entry",
					"Fuel Injector",
					"Whiteout" };
				static char P2000[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Handgun",
					"Fade",
					"Corticera",
					"Ocean Foam",
					"Fire Elemental",
					"Asterion",
					"Pathfinder",
					"Imperial",
					"Oceanic",
					"Imperial Dragon",
					"Whiteout" };
				static char Mp7[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Nemesis",
					"Impire",
					"Special Delivery",
					"Whiteout" };
				static char Mp9[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Rose Iron",
					"Ruby Poison Dart",
					"Airlock",
					"Whiteout" };
				static char Nova[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Hyper Beast",
					"Koi",
					"Antique",
					"Whiteout" };
				static char P250[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Whiteout",
					"Crimson Kimono",
					"Mint Kimono",
					"Wingshot",
					"Asiimov",
					"See Ya Later",
					"Whiteout" };
				static char SCAR20[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Splash Jam",
					"Storm",
					"Contractor",
					"Carbon Fiber",
					"Sand Mesh",
					"Palm",
					"Crimson Web",
					"Cardiac",
					"Army Sheen",
					"Cyrex",
					"Grotto",
					"Bloodsport",
					"Whiteout" };
				static char Sg553[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Tiger Moth",
					"Cyrex",
					"Pulse",
					"Fallout Warning",
					"Whiteout" };
				static char SSG08[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Lichen Dashed",
					"Dark Water",
					"Blue Spruce",
					"Sand Dune",
					"Palm",
					"Mayan Dreams",
					"Blood in the Water",
					"Tropical Storm",
					"Acid Fade",
					"Slashed",
					"Detour",
					"Abyss",
					"Big Iron",
					"Necropos",
					"Ghost Crusader",
					"Dragonfire",
					"Whiteout" };
				static char Revolver[][TGUI_MAX_STRING_LENGTH] = { "none",
					"Llama Cannon",
					"Fade",
					"Crimson Web",
					"Whiteout" };
				static char skinstabs[][TGUI_MAX_STRING_LENGTH] = { "Knife", "Weapons", "Glove" };
				static char ResolverOptions[][TGUI_MAX_STRING_LENGTH] = { "None", "LBY based", "Resolvy.us" };
				static char nadesbuyy[][TGUI_MAX_STRING_LENGTH]{"HEgrenade","Flashbang", "Smokegrenade", "Molotov/Incgrenade", "Decoy" };
				static char Primayguns[][TGUI_MAX_STRING_LENGTH]{"None", "Auto rifle", "Scout", "Awp", "Ak/m4"};
				static char secondaryguns[][TGUI_MAX_STRING_LENGTH]{ "None", "Glock-18 / P2000 / USP-S", "P250", "Dualies", "Deagle/Revolver", "Five Seven / Cz-75 / Tec 9" };
				static char wirefram[][TGUI_MAX_STRING_LENGTH]{ "None", "Sleeves", "Hands" };
				std::string config;

				/// Main tabs
				static char main_tabs[][TGUI_MAX_STRING_LENGTH] =
				{
					"Ragebot",
					"Visuals",
					"Miscellaneous",
					"Settings",
					"Skins"
				};

				static int selected_main_tab = 4;
				TGUI_SetColumn(0);
				switch (TGUI_Tab(5, main_tabs, selected_main_tab))
				{
					/* Aimbot */
				case 0: {

					TGUI_SetColumn(1);

					TGUI_Checkbox("Enable aimbot", SETTINGS::settings.aim_type);
					if (SETTINGS::settings.aim_type)
					{
						

						if (SETTINGS::settings.aim_type)
						{
							TGUI_BeginGroupbox("hitbox selection", AutoCalc(1));
							{
								TGUI_Combobox("hitbox", 3, acc_mode, SETTINGS::settings.acc_type); TGUI_ToolTip("What to aim for.");
			
							}
							TGUI_EndGroupbox();

							TGUI_BeginGroupbox("damage control", AutoCalc(2));
							{
								TGUI_Slider("minimum damage", "dmg", 1, 100, SETTINGS::settings.damage_val); TGUI_ToolTip("Shot only if can deal x dmg.");
								TGUI_Slider("minimum a.wall damage", "dmg", 1, 100, SETTINGS::settings.awall_damage_val); TGUI_ToolTip("Shot only if can deal x dmg through walls.");
							}
							TGUI_EndGroupbox();

							TGUI_BeginGroupbox("accuracy", AutoCalc(5));
							{
								TGUI_Checkbox("backtrack", SETTINGS::settings.backtrack_bool); TGUI_ToolTip("Shoots at old backtrack-able ticks.");

								TGUI_Slider("minimum hit-chance", "%", 0, 100, SETTINGS::settings.chance_val); TGUI_ToolTip("Only shot if accuracy is lower than hit-chance.");
								TGUI_Checkbox("multipoint", SETTINGS::settings.multi_bool);
								if (SETTINGS::settings.multi_bool)
								{
									TGUI_Slider("head scale", "°", 0, 1, SETTINGS::settings.point_val);
									TGUI_Slider("body scale", "°", 0, 1, SETTINGS::settings.body_val);
								}
								else
								{
									TGUI_AlphaModulate(0.2f);
									TGUI_Disable();
									TGUI_Slider("head scale", "°", 0, 1, SETTINGS::settings.point_val);
									TGUI_AlphaModulate(0.2f);
									TGUI_Disable();
									TGUI_Slider("body scale", "°", 0, 1, SETTINGS::settings.body_val);
								}
							}
							TGUI_EndGroupbox();

							TGUI_BeginGroupbox("corrections", AutoCalc(6));
							{
								TGUI_Combobox("Resolver",3, ResolverOptions, SETTINGS::settings.ResolverEnable); TGUI_ToolTip("Its a free paste stop complaining");
								TGUI_Checkbox("override enable", SETTINGS::settings.overrideenable); TGUI_ToolTip("Override enemy's angles.");
								TGUI_SameLine();
								TGUI_Keybind("override key", SETTINGS::settings.overridekey); TGUI_ToolTip("Override on what key.");
								TGUI_Combobox("override method", 2, override_method, SETTINGS::settings.overridemethod); TGUI_ToolTip("What the override key will do when clicked or held down.");
								TGUI_Combobox("delay shot", 2, delay_shot, SETTINGS::settings.delay_shot); TGUI_ToolTip("Can delay shot untill it is certin it can hit.");
							}
							TGUI_EndGroupbox();
						}
					}

					TGUI_SetColumn(2);
					TGUI_Checkbox("Enable antiaim", SETTINGS::settings.aa_bool);

					if (SETTINGS::settings.aa_bool)
					{
						TGUI_BeginGroupbox("antiaim", AutoCalc(4));
						{
							TGUI_Combobox("antiaim mode", 3, antiaimmode, SETTINGS::settings.aa_mode);
							switch (SETTINGS::settings.aa_mode)
							{
							case 0:
							{
								TGUI_Combobox("antiaim pitch - standing", 5, aa_pitch, SETTINGS::settings.aa_pitch_type);
								TGUI_Combobox("antiaim real - standing", 8, aa_mode, SETTINGS::settings.aa_real_type);
								TGUI_Combobox("antiaim fake - standing", 6, aa_fake, SETTINGS::settings.aa_fake_type);
							}break;
							
							case 1:
							{
								TGUI_Combobox("antiaim pitch - moving", 5, aa_pitch, SETTINGS::settings.aa_pitch1_type);
								TGUI_Combobox("antiaim real - moving", 8, aa_mode, SETTINGS::settings.aa_real1_type);
								TGUI_Combobox("antiaim fake - moving", 6, aa_fake, SETTINGS::settings.aa_fake1_type);
							}break;
							case 2:
							{
								TGUI_Combobox("antiaim pitch - jumping", 5, aa_pitch, SETTINGS::settings.aa_pitch2_type);
								TGUI_Combobox("antiaim real - jumping", 8, aa_mode, SETTINGS::settings.aa_real2_type);
								TGUI_Combobox("antiaim fake - jumping", 6, aa_fake, SETTINGS::settings.aa_fake2_type);
							}break;
							}

						}
						TGUI_EndGroupbox();

								TGUI_BeginGroupbox("additional options", AutoCalc(8));
								{
									TGUI_Keybind("flip key", SETTINGS::settings.flip_int); TGUI_ToolTip("Flips you aa if using sideways aa.");
									//TGUI_Checkbox("fake angle chams", SETTINGS::settings.aa_fakeangchams_bool);
									TGUI_Checkbox("Anti-Aim arrows", SETTINGS::settings.rifk_arrow); TGUI_ToolTip("Shows you where your real angle and fake angle are.");
									TGUI_Slider("Anti-Aim arrow size","", 10, 200, SETTINGS::settings.aasize); TGUI_ToolTip("Size of aa arrows.");
									switch (SETTINGS::settings.aa_mode)
									{
									case 0:
										TGUI_Slider("real additive", "°", -180, 180, SETTINGS::settings.aa_realadditive_val);
										TGUI_Slider("fake additive", "°", -180, 180, SETTINGS::settings.aa_fakeadditive_val);
										TGUI_Slider("lowerbodyyaw delta", "°", -119.9, 119.9, SETTINGS::settings.delta_val);
										break; //TGUI_Checkbox("lby flick up", SETTINGS::settings.lbyflickup);
									case 1:
										TGUI_Slider("real additive", "°", -180, 180, SETTINGS::settings.aa_realadditive1_val);
										TGUI_Slider("fake additive", "°", -180, 180, SETTINGS::settings.aa_fakeadditive1_val);
										TGUI_Slider("lowerbodyyaw delta", "°", -119.9, 119.9, SETTINGS::settings.delta1_val);
										break; //TGUI_Checkbox("lby flick up", SETTINGS::settings.lbyflickup1);
									case 2:
										TGUI_Slider("real additive", "°", -180, 180, SETTINGS::settings.aa_realadditive2_val);
										TGUI_Slider("fake additive", "°", -180, 180, SETTINGS::settings.aa_fakeadditive2_val);
										TGUI_Slider("lowerbodyyaw delta", "°", -119.9, 119.9, SETTINGS::settings.delta2_val);
										break; //TGUI_Checkbox("lby flick up", SETTINGS::settings.lbyflickup2);
									}

								}
								TGUI_EndGroupbox();

								TGUI_BeginGroupbox("rotate", AutoCalc(4));
								{

									TGUI_Slider("rotate fake - angle", "°", 0, 180, SETTINGS::settings.spinanglefake);
									TGUI_Slider("rotate fake - speed", "%", 0, 100, SETTINGS::settings.spinspeedfake);

									switch (SETTINGS::settings.aa_mode)
									{
									case 0:
										TGUI_Slider("rotate angle - standing", "°", 0, 180, SETTINGS::settings.spinangle);
										TGUI_Slider("rotate speed - standing", "%", 0, 100, SETTINGS::settings.spinspeed);
										break;
									case 1:
										TGUI_Slider("rotate angle - moving", "°", 0, 180, SETTINGS::settings.spinangle1);
										TGUI_Slider("rotate speed - moving", "%", 0, 100, SETTINGS::settings.spinspeed1);
										break;
									case 2:
										TGUI_Slider("rotate angle - jumping", "°", 0, 180, SETTINGS::settings.spinangle2);
										TGUI_Slider("rotate speed - jumping", "%", 0, 100, SETTINGS::settings.spinspeed2);
										break;
									}

								}
								TGUI_EndGroupbox();
					}
				} break;
					/* Visuals */
				case 1: {
					TGUI_SetColumn(1);
					TGUI_Checkbox("Enable visuals", SETTINGS::settings.esp_bool);
					if (SETTINGS::settings.esp_bool)
					{
						TGUI_BeginGroupbox("Players", AutoCalc(8));
						{
							TGUI_Combobox("Team selection", 2, team_select, SETTINGS::settings.espteamselection);
							if (SETTINGS::settings.espteamselection == 0)
							{
								TGUI_Combobox("Draw enemy box", 3, Box_tpye, SETTINGS::settings.box_bool);
								TGUI_Checkbox("Draw enemy name", SETTINGS::settings.name_bool); TGUI_ToolTip("Shows the name of the enemys.");
								TGUI_Checkbox("Draw Enemy Weapon", SETTINGS::settings.weap_bool); TGUI_ToolTip("Shows the current weapon of the enemys.");
								TGUI_Checkbox("Draw enemy flags", SETTINGS::settings.info_bool); TGUI_ToolTip("Shows armor and if they are scoped or not on the enemys.");
								TGUI_Checkbox("Draw enemy health", SETTINGS::settings.health_bool); TGUI_ToolTip("Shows how much health the enemys have.");
								TGUI_Checkbox("Draw enemy money", SETTINGS::settings.money_bool); TGUI_ToolTip("Shows how much money the enemys have.");
								TGUI_Checkbox("Draw enemy fov arrows", SETTINGS::settings.fov_bool); TGUI_ToolTip("Shows where the enemys are if you cannot see them (people are reporting bugs with this feature.");
							}
							else if (SETTINGS::settings.espteamselection == 1)
							{
								TGUI_Combobox("Draw team box", 3, Box_tpye, SETTINGS::settings.boxteam);
								TGUI_Checkbox("Draw team name", SETTINGS::settings.nameteam); TGUI_ToolTip("Shows the name of your teammates.");
								TGUI_Checkbox("Draw team weapon", SETTINGS::settings.weaponteam); TGUI_ToolTip("Shows the current weapon of your teammates.");
								TGUI_Checkbox("Draw team flags", SETTINGS::settings.flagsteam); TGUI_ToolTip("Shows armor and if they are scoped or not on your teammates.");
								TGUI_Checkbox("Draw team health", SETTINGS::settings.healthteam); TGUI_ToolTip("Shows how much health you teammates have.");
								TGUI_Checkbox("Draw team money", SETTINGS::settings.moneyteam); TGUI_ToolTip("Shows how much money your teammates have.");
								TGUI_Checkbox("Draw team fov arrows", SETTINGS::settings.arrowteam); TGUI_ToolTip("Shows where your teammates are if you cannot see them (people are reporting bugs with this feature.");
							}
						}
						TGUI_EndGroupbox();
						

						TGUI_BeginGroupbox("Chams", AutoCalc(2));
						{
							TGUI_Combobox("Model team selection", 2, team_select, SETTINGS::settings.chamsteamselection);
							if (SETTINGS::settings.chamsteamselection == 0)
								TGUI_Combobox("Enemy coloured models", 3, chams_mode, SETTINGS::settings.chams_type);
							else if (SETTINGS::settings.chamsteamselection == 1)
								TGUI_Combobox("Team coloured models", 3, chams_mode, SETTINGS::settings.chamsteam);
						}
						TGUI_EndGroupbox();
						
						TGUI_BeginGroupbox("World", AutoCalc(12));
						{
							TGUI_Checkbox("Night mode", SETTINGS::settings.night_mode); TGUI_ToolTip("The world will appear like its night time.");
							TGUI_Checkbox("Gravitational Ragdoll", SETTINGS::settings.ragdoll_bool); TGUI_ToolTip("When a player dies what happens to their dead body.");
							TGUI_Slider("Gravitational Ragdoll Amount", "", -100, 100, SETTINGS::settings.radgollslider); TGUI_ToolTip("How the ragdoll looks on players deaths.");
							TGUI_Checkbox("Sky color changer", SETTINGS::settings.sky_enabled); TGUI_ToolTip("How the sky color looks (When you change the color disable and re enable this feature for it to appear).");
							TGUI_Checkbox("World Color Changer", SETTINGS::settings.wolrd_enabled); TGUI_ToolTip("How the world color looks.");
							TGUI_Checkbox("Bullet tracers", SETTINGS::settings.beam_bool); TGUI_ToolTip("When you shoot a bullet a 'beam' follows where your bullet is going.");
							TGUI_Checkbox("Rainbow bullet tracers ", SETTINGS::settings.gradientbullet); TGUI_ToolTip("Bullet tracers come out rainbow and change color from shot to shot based on the rainbow bullet tracer speed.");
							TGUI_Slider("Rainbow bullet tracers speed","", 0.001, 0.01, SETTINGS::settings.gradienttracerspeed, 3); TGUI_ToolTip("How fast the rainbow bullet tracers change color from each shot.");
							TGUI_Checkbox("Bullet Impacts", SETTINGS::settings.impacts); TGUI_ToolTip("Shows where your bullet impacted.");
							TGUI_Checkbox("Wireframe Smoke", SETTINGS::settings.smoke_bool); TGUI_ToolTip("Smoke greandes (on lading) appear wire frame making it easier to see through.");
							TGUI_Checkbox("Damage Indicator", SETTINGS::settings.dmg_bool); TGUI_ToolTip("Shows on the enemy how much damage you did.");
							TGUI_Checkbox("Player Hitmarkers", SETTINGS::settings.hitmarker); TGUI_ToolTip("AimWare hit marker looks kewl.");
							if (!SETTINGS::settings.matpostprocessenable)
								TGUI_Checkbox("Postprocessing Disabled", SETTINGS::settings.matpostprocessenable);
							else
								TGUI_Checkbox("Postprocessing Enabled", SETTINGS::settings.matpostprocessenable);
						}
						TGUI_EndGroupbox();
						
						TGUI_SetColumn(2);

						TGUI_BeginGroupbox("Glow", AutoCalc(3));
						{
							TGUI_Combobox("Glow team selection", 3, team_select, SETTINGS::settings.glowteamselection);
							if (SETTINGS::settings.glowteamselection == 0)
							{
								TGUI_Checkbox("Enemy glow enable", SETTINGS::settings.glowenable); TGUI_ToolTip("Enables glow for enemys.");
							}
							else if (SETTINGS::settings.glowteamselection == 1)
							{
								TGUI_Checkbox("Team glow enable", SETTINGS::settings.glowteam); TGUI_ToolTip("Enables glow for teammates.");
							}
							TGUI_Combobox("Glow style", 3, glow_styles, SETTINGS::settings.glowstyle);
						}
						TGUI_EndGroupbox();

						

						TGUI_BeginGroupbox("Local player", AutoCalc(22));
						{
							TGUI_Checkbox("Draw zeus range", SETTINGS::settings.zeusrange); TGUI_ToolTip("Very buggy, decreases fps alot.");
							TGUI_Combobox("Wire frame goodies", 3, wirefram, SETTINGS::settings.wiresleeve_bool); TGUI_ToolTip("Very b1g p2c feature (made so you can see you gloves).");
							TGUI_Checkbox("Local glow", SETTINGS::settings.glowlocal); TGUI_ToolTip("Local player glows.");
							TGUI_Combobox("Local Glow Style", 3 ,glow_styles, SETTINGS::settings.glowstylelocal);
							TGUI_Checkbox("Rainbow local glow", SETTINGS::settings.gradientglow); TGUI_ToolTip("Local player will glow rainbow.");
							TGUI_Slider("Rainbow local glow speed","", 0.001, 0.01, SETTINGS::settings.gradientglowspeed, 3);
							TGUI_Checkbox("Render spread", SETTINGS::settings.spread_bool); TGUI_ToolTip("Shows a spread circle, showing your weapons spread.");
							TGUI_Checkbox("Fix zoom sensitivity", SETTINGS::settings.fixscopesens); TGUI_ToolTip("No slow down when moving cursor while scoped.");
							TGUI_Combobox("Crosshair", 3, crosshair_select, SETTINGS::settings.xhair_type);
							TGUI_Checkbox("Remove scope", SETTINGS::settings.scope_bool); TGUI_ToolTip("Removes black circle while scoping to have better vision.");
							TGUI_Checkbox("Remove zoom", SETTINGS::settings.removescoping); TGUI_ToolTip("Removes zooming on scope so you dont get the stupid zoom shit.");
							TGUI_Checkbox("Thirdperson", SETTINGS::settings.tp_bool);
							TGUI_SameLine();
							TGUI_Keybind("bind##Thirdperson key", SETTINGS::settings.thirdperson_int);
							TGUI_Slider("Scope trans", "", 0, 100, SETTINGS::settings.transparency_when_scoped, 50); TGUI_ToolTip("How mush of your player you can see in third person.");
							TGUI_Slider("Thirdperson fov", "", 0, 179, SETTINGS::settings.THIRDPERSONFOV); TGUI_ToolTip("How far away the third person camera is.");
							TGUI_Checkbox("Force crosshair", SETTINGS::settings.forcehair); TGUI_ToolTip("Forces your own crosshair.");
							TGUI_Combobox("Local chams", 5, local_chams, SETTINGS::settings.localchams);
							TGUI_Slider("Rainbow local chams speed","", 0.001, 0.01, SETTINGS::settings.gradientchamsspeed, 3); TGUI_ToolTip("Local player rainbow color speed.");
							TGUI_Slider("Render fov", "", 0, 179, SETTINGS::settings.fov_val, 0); TGUI_ToolTip("Field of view.");
							TGUI_Slider("Viewmodel fov", "", 0, 179, SETTINGS::settings.viewfov_val, 0); TGUI_ToolTip("How far away the gun appears from the screen.");
							TGUI_Combobox("Hitmarker sound", 5, hitmarker, SETTINGS::settings.hitmarker_val);
							TGUI_Checkbox("Lby indicator", SETTINGS::settings.lbyenable); TGUI_ToolTip("If your breaking lby or not, it'll tell you.");
						}
						TGUI_EndGroupbox();
					}
				} break;
					/* Miscellaneous */
				case 2: {
					TGUI_SetColumn(1);

					TGUI_Checkbox("Enable misc", SETTINGS::settings.misc_bool);
					if (SETTINGS::settings.misc_bool)
					{
						TGUI_BeginGroupbox("Movement", AutoCalc(6));
						{
							TGUI_Checkbox("Auto bunnyhop", SETTINGS::settings.bhop_bool); TGUI_ToolTip("Automatically jump for you.");
							TGUI_Checkbox("Auto strafer", SETTINGS::settings.strafe_bool); TGUI_ToolTip("Helps acheive higher speed while jumping.");
							TGUI_Checkbox("Duck in air", SETTINGS::settings.duck_bool); TGUI_ToolTip("Automatically duck while in air.");
							TGUI_Keybind("Circle strafe", SETTINGS::settings.circlestrafekey);
							TGUI_Checkbox("Fake-walk", SETTINGS::settings.fakewalk); TGUI_AttachToPreviousWidget(); TGUI_Keybind("bind##fake-walk", SETTINGS::settings.fakewalk_key);
							TGUI_Slider("Fakewalk speed", "", 3, 8, SETTINGS::settings.fakewalkspeed, 1);
						}
						TGUI_EndGroupbox();

						TGUI_BeginGroupbox("Clantag changer", AutoCalc(3));
						{
							TGUI_Checkbox("Enable", SETTINGS::settings.misc_enable_clantag); TGUI_ToolTip("Enables animated clan-tag.");

							TGUI_TextInput("Tag text", SETTINGS::settings.misc_clantag_text);
							if (TGUI_Combobox("Style", 3, clan_tag_types, SETTINGS::settings.misc_clantag_style) != 0)
								TGUI_Slider("Speed", "", SETTINGS::settings.misc_clantag_style == 2 ? 0.f : -1.f, 1.f, SETTINGS::settings.misc_clantag_speed, 2);
						}
						TGUI_EndGroupbox();

						TGUI_BeginGroupbox("Buy bot", AutoCalc(6));
						{
							TGUI_Checkbox("Enable", SETTINGS::settings.autobuy_bool);
							TGUI_Combobox("Primary", 5, Primayguns, SETTINGS::settings.Primaryweapons);
							TGUI_Checkbox("Zeus", SETTINGS::settings.zeus_bool);
							TGUI_Checkbox("Armour + Defuse", SETTINGS::settings.armour_bool);
							TGUI_Combobox("Primary", 6, secondaryguns, SETTINGS::settings.secondaryweapons);
							TGUI_MultiselectCombobox("Nades", 5, nadesbuyy, SETTINGS::settings.nadesbuy);
						}
						TGUI_EndGroupbox();

						TGUI_SetColumn(2);

						TGUI_BeginGroupbox("Fakelag", AutoCalc(5));
						{
							TGUI_Checkbox("Enable", SETTINGS::settings.lag_bool);
							TGUI_Combobox("Fakelag type", 2, fakelag_mode, SETTINGS::settings.lag_type);

							TGUI_Slider("Standing lag", "", 1, 14, SETTINGS::settings.stand_lag);
							TGUI_Slider("Moving lag", "", 1, 14, SETTINGS::settings.move_lag);
							TGUI_Slider("Jumping lag", "", 1, 14, SETTINGS::settings.jump_lag);
						}
						TGUI_EndGroupbox();
					}
				} break;
					/* Settings */
				case 3: {
					TGUI_SetColumn(1);
					TGUI_BeginGroupbox("Esp colors", AutoCalc(6));
					{
						TGUI_Combobox("Esp colour selection", 2, team_select, SETTINGS::settings.espteamcolourselection);
						if (SETTINGS::settings.espteamcolourselection == 0)
						{

							TGUI_ColorPicker("Enemy box color", SETTINGS::settings.boxenemy_col.RGBA[0], SETTINGS::settings.boxenemy_col.RGBA[1], SETTINGS::settings.boxenemy_col.RGBA[2], SETTINGS::settings.boxenemy_col.RGBA[3], false);
							TGUI_ColorPicker("Enemy name color", SETTINGS::settings.nameenemy_col.RGBA[0], SETTINGS::settings.nameenemy_col.RGBA[1], SETTINGS::settings.nameenemy_col.RGBA[2], SETTINGS::settings.nameenemy_col.RGBA[3], false);
							TGUI_ColorPicker("Enemy weapon color", SETTINGS::settings.weaponenemy_col.RGBA[0], SETTINGS::settings.weaponenemy_col.RGBA[1], SETTINGS::settings.weaponenemy_col.RGBA[2], SETTINGS::settings.weaponenemy_col.RGBA[3], false);
							TGUI_ColorPicker("Enemy fov arrows color", SETTINGS::settings.fov_col.RGBA[0], SETTINGS::settings.fov_col.RGBA[1], SETTINGS::settings.fov_col.RGBA[2], SETTINGS::settings.fov_col.RGBA[3], false);
							TGUI_ColorPicker("Damage Indicator Color", SETTINGS::settings.dmg_color.RGBA[0], SETTINGS::settings.dmg_color.RGBA[1], SETTINGS::settings.dmg_color.RGBA[2], SETTINGS::settings.dmg_color.RGBA[3], false);
						
						}
						else if (SETTINGS::settings.espteamcolourselection == 1)
						{

							TGUI_ColorPicker("Team box color", SETTINGS::settings.boxteam_col.RGBA[0], SETTINGS::settings.boxteam_col.RGBA[1], SETTINGS::settings.boxteam_col.RGBA[2], SETTINGS::settings.boxteam_col.RGBA[3], false);
							TGUI_ColorPicker("Team name color", SETTINGS::settings.nameteam_col.RGBA[0], SETTINGS::settings.nameteam_col.RGBA[1], SETTINGS::settings.nameteam_col.RGBA[2], SETTINGS::settings.nameteam_col.RGBA[3], false);
							TGUI_ColorPicker("Team weapon color", SETTINGS::settings.weaponteam_col.RGBA[0], SETTINGS::settings.weaponteam_col.RGBA[1], SETTINGS::settings.weaponteam_col.RGBA[2], SETTINGS::settings.weaponteam_col.RGBA[3], false);
							TGUI_ColorPicker("Team fov arrows color", SETTINGS::settings.arrowteam_col.RGBA[0], SETTINGS::settings.arrowteam_col.RGBA[1], SETTINGS::settings.arrowteam_col.RGBA[2], SETTINGS::settings.arrowteam_col.RGBA[3], false);
						
						}
					}
					TGUI_EndGroupbox();

					TGUI_BeginGroupbox("Chams colors", AutoCalc(5));
					{

						TGUI_ColorPicker("Enemy vis. Color", SETTINGS::settings.vmodel_col.RGBA[0], SETTINGS::settings.vmodel_col.RGBA[1], SETTINGS::settings.vmodel_col.RGBA[2], SETTINGS::settings.vmodel_col.RGBA[3], false);
						TGUI_ColorPicker("Enemy invis. Color", SETTINGS::settings.imodel_col.RGBA[0], SETTINGS::settings.imodel_col.RGBA[1], SETTINGS::settings.imodel_col.RGBA[2], SETTINGS::settings.imodel_col.RGBA[3], false);
						TGUI_ColorPicker("Team vis. Color", SETTINGS::settings.teamvis_color.RGBA[0], SETTINGS::settings.teamvis_color.RGBA[1], SETTINGS::settings.teamvis_color.RGBA[2], SETTINGS::settings.teamvis_color.RGBA[3], false);
						TGUI_ColorPicker("Team invis. Color", SETTINGS::settings.teaminvis_color.RGBA[0], SETTINGS::settings.teaminvis_color.RGBA[1], SETTINGS::settings.teaminvis_color.RGBA[2], SETTINGS::settings.teaminvis_color.RGBA[3], false);
						TGUI_ColorPicker("Local Color", SETTINGS::settings.localchams_col.RGBA[0], SETTINGS::settings.localchams_col.RGBA[1], SETTINGS::settings.localchams_col.RGBA[2], SETTINGS::settings.localchams_col.RGBA[3], false);

					}
					TGUI_EndGroupbox();

					TGUI_BeginGroupbox("Glow colors", AutoCalc(3));
					{

						TGUI_ColorPicker("Glow enemy Color", SETTINGS::settings.glow_col.RGBA[0], SETTINGS::settings.glow_col.RGBA[1], SETTINGS::settings.glow_col.RGBA[2], SETTINGS::settings.glow_col.RGBA[3], false);
						TGUI_ColorPicker("Glow team Color", SETTINGS::settings.teamglow_color.RGBA[0], SETTINGS::settings.teamglow_color.RGBA[1], SETTINGS::settings.teamglow_color.RGBA[2], SETTINGS::settings.teamglow_color.RGBA[3], false);
						TGUI_ColorPicker("Glow local Color", SETTINGS::settings.glowlocal_col.RGBA[0], SETTINGS::settings.glowlocal_col.RGBA[1], SETTINGS::settings.glowlocal_col.RGBA[2], SETTINGS::settings.glowlocal_col.RGBA[3], false);
					
					}
					TGUI_EndGroupbox();

					TGUI_SetColumn(2);
					TGUI_BeginGroupbox("Bullet tracer colors", AutoCalc(3));
					{

						TGUI_ColorPicker("Local player", SETTINGS::settings.bulletlocal_col.RGBA[0], SETTINGS::settings.bulletlocal_col.RGBA[1], SETTINGS::settings.bulletlocal_col.RGBA[2], SETTINGS::settings.bulletlocal_col.RGBA[3], false);
						TGUI_ColorPicker("Enemy player", SETTINGS::settings.bulletenemy_col.RGBA[0], SETTINGS::settings.bulletenemy_col.RGBA[1], SETTINGS::settings.bulletenemy_col.RGBA[2], SETTINGS::settings.bulletenemy_col.RGBA[3], false);
						TGUI_ColorPicker("Team player", SETTINGS::settings.bulletteam_col.RGBA[0], SETTINGS::settings.bulletteam_col.RGBA[1], SETTINGS::settings.bulletteam_col.RGBA[2], SETTINGS::settings.bulletteam_col.RGBA[3], false);

					}
					TGUI_EndGroupbox();

					TGUI_BeginGroupbox("World", AutoCalc(3));
					{

						TGUI_ColorPicker("Player Hitmarkers Color", SETTINGS::settings.awcolor.RGBA[0], SETTINGS::settings.awcolor.RGBA[1], SETTINGS::settings.awcolor.RGBA[2], SETTINGS::settings.awcolor.RGBA[3], false);
						TGUI_ColorPicker("Sky Color", SETTINGS::settings.skycolor.RGBA[0], SETTINGS::settings.skycolor.RGBA[1], SETTINGS::settings.skycolor.RGBA[2], SETTINGS::settings.skycolor.RGBA[3], false);
						TGUI_ColorPicker("World Color", SETTINGS::settings.night_col.RGBA[0], SETTINGS::settings.night_col.RGBA[1], SETTINGS::settings.night_col.RGBA[2], SETTINGS::settings.night_col.RGBA[3], false);
					
					}
					TGUI_EndGroupbox();

					TGUI_BeginGroupbox("Misc", AutoCalc(3));
					{

						TGUI_ColorPicker("Spread Color", SETTINGS::settings.spread_Col.RGBA[0], SETTINGS::settings.spread_Col.RGBA[1], SETTINGS::settings.spread_Col.RGBA[2], SETTINGS::settings.spread_Col.RGBA[3], false);
						TGUI_ColorPicker("AA color picker real", SETTINGS::settings.aacolorreal.RGBA[0], SETTINGS::settings.aacolorreal.RGBA[1], SETTINGS::settings.aacolorreal.RGBA[2], SETTINGS::settings.aacolorreal.RGBA[3], false);
						TGUI_ColorPicker("AA color picker fake", SETTINGS::settings.aacolorfake.RGBA[0], SETTINGS::settings.aacolorfake.RGBA[1], SETTINGS::settings.aacolorfake.RGBA[2], SETTINGS::settings.aacolorfake.RGBA[3], false);
					}
					TGUI_EndGroupbox();

					TGUI_BeginGroupbox("Colors settings", AutoCalc(1));
					{
						if (TGUI_Button("Reset colors"))
						{
							SETTINGS::settings.aacolorreal = CColor(0, 0, 255);
							SETTINGS::settings.aacolorfake = CColor(255, 0, 0);
							//
							//
							SETTINGS::settings.night_col = CColor(0, 255, 255);
							SETTINGS::settings.awcolor = CColor(255, 20, 147);
							SETTINGS::settings.skycolor = CColor(0, 0, 0);
							//
							//
							SETTINGS::settings.boxenemy_col = CColor(205, 0, 0);
							SETTINGS::settings.nameenemy_col = CColor(255, 255, 255);
							SETTINGS::settings.weaponenemy_col = CColor(255, 255, 255);
							SETTINGS::settings.fov_col = CColor(205, 0, 0);
							SETTINGS::settings.dmg_color = CColor(0, 0, 255);

							SETTINGS::settings.boxteam_col = CColor(0, 0, 205);
							SETTINGS::settings.nameteam_col = CColor(255, 255, 255);
							SETTINGS::settings.weaponteam_col = CColor(255, 255, 255);
							SETTINGS::settings.arrowteam_col = CColor(0, 0, 205);
							//
							//
							SETTINGS::settings.vmodel_col = CColor(205, 55, 0);
							SETTINGS::settings.imodel_col = CColor(205, 0, 0);

							SETTINGS::settings.teamvis_color = CColor(0, 55, 205);
							SETTINGS::settings.teaminvis_color = CColor(0, 0, 205);

							SETTINGS::settings.localchams_col = CColor(0, 205, 0);
							//
							//
							SETTINGS::settings.glow_col = CColor(205, 0, 0);
							SETTINGS::settings.teamglow_color = CColor(0, 0, 205);
							SETTINGS::settings.glowlocal_col = CColor(0, 205, 0);
							//
							//
							SETTINGS::settings.bulletlocal_col = CColor(0, 205, 0);
							SETTINGS::settings.bulletenemy_col = CColor(205, 0, 0);
							SETTINGS::settings.bulletteam_col = CColor(0, 0, 205);
							//
							//
							SETTINGS::settings.spread_Col = CColor(255, 0, 255);
						}
						TGUI_SameLine();
						if (TGUI_Button("Random colors"))
						{
							SETTINGS::settings.aacolorreal = RANDOMIZECOL;
							SETTINGS::settings.aacolorfake = RANDOMIZECOL;
							//
							//
							SETTINGS::settings.spread_Col = RANDOMIZECOL;
							SETTINGS::settings.night_col = RANDOMIZECOL;
							SETTINGS::settings.awcolor = RANDOMIZECOL;
							SETTINGS::settings.skycolor = RANDOMIZECOL;
							//
							//
							SETTINGS::settings.boxenemy_col = RANDOMIZECOL;
							SETTINGS::settings.nameenemy_col = RANDOMIZECOL;
							SETTINGS::settings.weaponenemy_col = RANDOMIZECOL;
							SETTINGS::settings.fov_col = RANDOMIZECOL;
							SETTINGS::settings.dmg_color = RANDOMIZECOL;

							SETTINGS::settings.boxteam_col = RANDOMIZECOL;
							SETTINGS::settings.nameteam_col = RANDOMIZECOL;
							SETTINGS::settings.weaponteam_col = RANDOMIZECOL;
							SETTINGS::settings.arrowteam_col = RANDOMIZECOL;
							//
							//
							SETTINGS::settings.vmodel_col = RANDOMIZECOL;
							SETTINGS::settings.imodel_col = RANDOMIZECOL;

							SETTINGS::settings.teamvis_color = RANDOMIZECOL;
							SETTINGS::settings.teaminvis_color = RANDOMIZECOL;

							SETTINGS::settings.localchams_col = RANDOMIZECOL;
							//
							//
							SETTINGS::settings.glow_col = RANDOMIZECOL;
							SETTINGS::settings.teamglow_color = RANDOMIZECOL;
							SETTINGS::settings.glowlocal_col = RANDOMIZECOL;
							//
							//
							SETTINGS::settings.bulletlocal_col = RANDOMIZECOL;
							SETTINGS::settings.bulletenemy_col = RANDOMIZECOL;
							SETTINGS::settings.bulletteam_col = RANDOMIZECOL;
						}
					}
					TGUI_EndGroupbox();

					TGUI_BeginGroupbox("Configuration", 90);
					{
						switch (TGUI_Combobox("Config", 7, configs, SETTINGS::settings.config_sel))
						{
						case 0: config = "Default"; break;
						case 1: config = "Legit"; break;
						case 2: config = "Auto_hvh"; break;
						case 3: config = "Scout_hvh"; break;
						case 4: config = "Pistol_hvh"; break;
						case 5: config = "AWP_hvh"; break;
						case 6: config = "Nospread_hvh"; break;
						}

						if (TGUI_Button("Load Config"))
						{
							InGameLogger::Log log;

							log.color_line.PushBack("[nnware] ", LIGHTBLUE);
							log.color_line.PushBack("Configuration loaded.", CColor(200, 200, 200));

							in_game_logger.AddLog(log);

							//INTERFACES::cvar->ConsoleColorPrintf(CColor(175, 200, 5), "[nnware] ");
							//GLOBAL::Msg("Configuration loaded.     \n");

							SETTINGS::settings.Load(config);
						}
						TGUI_SameLine();
						if (TGUI_Button("Save Config"))
						{
							InGameLogger::Log log;

							log.color_line.PushBack("[nnware] ", PURPLE);
							log.color_line.PushBack("Configuration saved.", CColor(200, 200, 200));

							in_game_logger.AddLog(log);

							//INTERFACES::cvar->ConsoleColorPrintf(CColor(175, 200, 5), "[nnware] ");
							//GLOBAL::Msg("Configuration saved.     \n");

							SETTINGS::settings.Save(config);
						}
					}
					TGUI_EndGroupbox();
				} break;
					case 4:
					{
						TGUI_SetColumn(1);
						TGUI_Checkbox("Skin Changer", SETTINGS::settings.Skins);
						if (SETTINGS::settings.Skins)
						{
							if (TGUI_Button(("Force update")))
								KnifeApplyCallbk();
							static int selected_main_tab2 = 0;
							switch (TGUI_Tab(3, skinstabs, selected_main_tab2))
							{
							case 0:
							{
								TGUI_SetColumn(1);
								TGUI_Checkbox("Knife Changer", SETTINGS::settings.knifes);
								TGUI_BeginGroupbox("Knife", AutoCalc(2));
								TGUI_Combobox(("Knife Model"), 13 , KnifeModel, SETTINGS::settings.Knife);
								TGUI_Combobox(("Knife Skin"),37, knifeskins, SETTINGS::settings.KnifeSkin);
								TGUI_EndGroupbox();
								break;
							}
							case 1:
							{
								TGUI_SetColumn(1);
								TGUI_Checkbox("Skin Changer", SETTINGS::settings.skinenabled);
								TGUI_BeginGroupbox("Rifles", AutoCalc(7));
								TGUI_Combobox(("AK-47"),17, ak47, SETTINGS::settings.AK47Skin);
								TGUI_Combobox(("M4A4"),16, M4A4, SETTINGS::settings.M4A4Skin);
								TGUI_Combobox(("M4A1-s"),17, M4A1, SETTINGS::settings.M4A1SSkin);
								TGUI_Combobox(("Aug"),5, Aug, SETTINGS::settings.AUGSkin);
								TGUI_Combobox(("Famas"),5, Famas, SETTINGS::settings.FAMASSkin);
								TGUI_Combobox(("Galil"),6, Galil, SETTINGS::settings.GalilSkin);
								TGUI_Combobox(("Sg553"),5, Sg553, SETTINGS::settings.Sg553Skin);
								TGUI_EndGroupbox();

								TGUI_BeginGroupbox("Pistols", AutoCalc(10));
								TGUI_Combobox(("Duals"),6, Duals, SETTINGS::settings.DualSkin);
								TGUI_Combobox(("Usp-s"),10, Usp, SETTINGS::settings.USPSkin);
								TGUI_Combobox(("Glock"),15, Glock, SETTINGS::settings.GlockSkin);
								TGUI_Combobox(("Deagle"),7, Deagle, SETTINGS::settings.DeagleSkin);
								TGUI_Combobox(("Five7"),8, Five7, SETTINGS::settings.FiveSkin);
								TGUI_Combobox(("Cz75"),5, Cz75, SETTINGS::settings.Cz75Skin);
								TGUI_Combobox(("Tec9"),10, tec9, SETTINGS::settings.tec9Skin);
								TGUI_Combobox(("P2000"),11, P2000, SETTINGS::settings.P2000Skin);
								TGUI_Combobox(("P250"),7, P250, SETTINGS::settings.P250Skin);
								TGUI_Combobox(("Revolver"),4, Revolver, SETTINGS::settings.RevolverSkin);
								TGUI_EndGroupbox();

								TGUI_SetColumn(2);

								TGUI_BeginGroupbox("Snipers", AutoCalc(4));
								TGUI_Combobox(("Awp"),22, Awp, SETTINGS::settings.AWPSkin);
								TGUI_Combobox(("G3sg1"),4, G3sg1Skin, SETTINGS::settings.G3sg1Skin);
								TGUI_Combobox(("SCAR20"),13, SCAR20, SETTINGS::settings.SCAR20Skin);
								TGUI_Combobox(("SSG08"),14, SSG08, SETTINGS::settings.SSG08Skin);
								TGUI_EndGroupbox();

								TGUI_BeginGroupbox("LMGs", AutoCalc(2));
								TGUI_Combobox(("M249"),4, M249, SETTINGS::settings.M249Skin);
								TGUI_Combobox(("Negev"),4, negev, SETTINGS::settings.NegevSkin);
								TGUI_EndGroupbox();

								TGUI_BeginGroupbox("Sub Machineguns", AutoCalc(5));
								TGUI_Combobox(("Mac10"),6, Mac10, SETTINGS::settings.Mac10Skin);
								TGUI_Combobox(("Ump45"),6, Ump45, SETTINGS::settings.UMP45Skin);
								TGUI_Combobox(("PPBizon"),4, PPBizon, SETTINGS::settings.BizonSkin);
								TGUI_Combobox(("Mp7"),4, Mp7, SETTINGS::settings.Mp7Skin);
								TGUI_Combobox(("Mp9"),4, Mp9, SETTINGS::settings.Mp9Skin);
								TGUI_EndGroupbox();

								TGUI_BeginGroupbox("Shotguns", AutoCalc(4));
								TGUI_Combobox(("XM1014"),4, XM1014, SETTINGS::settings.XmSkin);
								TGUI_Combobox(("Mag-7"),4, Mag7, SETTINGS::settings.MagSkin);
								TGUI_Combobox(("Sawed Off"),4, Sawedoff, SETTINGS::settings.SawedSkin);
								TGUI_Combobox(("Nova"),4, Nova, SETTINGS::settings.NovaSkin);
								TGUI_EndGroupbox();
								break;
							}
							case 2:
							{
								TGUI_SetColumn(1);
								TGUI_Checkbox("Glove Changer", SETTINGS::settings.glovesenabled);
								TGUI_BeginGroupbox("Gloves", AutoCalc(2));
								TGUI_Combobox(("Glove Model"),6, GloveModel, SETTINGS::settings.gloves);
								TGUI_Combobox(("Glove Skin"),4, xdshit, SETTINGS::settings.skingloves);
								TGUI_EndGroupbox();
								break;
							}
							}
						}
					}
				}
			}
			TGUI_EndWindow();

			TGUI_EndFrame();
		}
	}
}