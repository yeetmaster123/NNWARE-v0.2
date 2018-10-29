
#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CInputSystem.h"
#include "../UTILS/render.h"
#include "../SDK/ConVar.h"
#include "Components.h"
#include "../xdxdxd.h"
#include "menu_framework.h"
int menutab;
/*
_______   _______ .______    __    __    _______
|       \ |   ____||   _  \  |  |  |  |  /  _____|
|  .--.  ||  |__   |  |_)  | |  |  |  | |  |  __
|  |  |  ||   __|  |   _  <  |  |  |  | |  | |_ |
|  '--'  ||  |____ |  |_)  | |  `--'  | |  |__| |
|_______/ |_______||______/   \______/   \______|
_______    ___       _______   _______   ______   .___________.
|   ____|  /   \     /  _____| /  _____| /  __  \  |           |
|  |__    /  ^  \   |  |  __  |  |  __  |  |  |  | `---|  |----`
|   __|  /  /_\  \  |  | |_ | |  | |_ | |  |  |  |     |  |
|  |    /  _____  \ |  |__| | |  |__| | |  `--'  |     |  |
|__|   /__/     \__\ \______|  \______|  \______/      |__|
*/
int AutoCalc(int va)
{
	if (va == 1)
		return va * 35;
	else if (va == 2)
		return va * 34;
	else
		return va * 25 + 7.5;
}

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(UTILS::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

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

float MenuAlpha = 0.05f;
float MenuAlpha3 = 0.05f;
float MenuAlpha5 = 0.05f;
float MenuAlpha7 = 0.05f;
float MenuAlpha9 = 0.05f;
float MenuAlpha11 = 0.05f;
float MenuAlpha13 = 0.05f;
float MenuAlpha15 = 0.05f;
float MenuAlpha17 = 0.05f;
float MenuAlpha19 = 0.05f;
float MenuAlpha21 = 0.05f;
float MenuAlpha23 = 0.05f;
float MenuAlpha24 = MenuAlpha23;
float MenuAlpha22 = MenuAlpha21;
float MenuAlpha20 = MenuAlpha19;
float MenuAlpha18 = MenuAlpha17;
float MenuAlpha16 = MenuAlpha15;
float MenuAlpha14 = MenuAlpha13;
float MenuAlpha12 = MenuAlpha11;
float MenuAlpha10 = MenuAlpha9;
float MenuAlpha8 = MenuAlpha7;
float MenuAlpha6 = MenuAlpha5;
float MenuAlpha2 = MenuAlpha;
float MenuAlpha4 = MenuAlpha3;
#define RANDOMIZECOL CColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1)
namespace MENU
{
	void InitColors()
	{
		static float rainbow;
		rainbow += SETTINGS::settings.menurainbowspeed = 0.001;
		if (rainbow > 1.f) rainbow = 0.f;

		using namespace PPGUI_PP_GUI;

		colors[WINDOW_BODY] = CColor(17, 17, 17, MenuAlpha);
		colors[WINDOW_TITLE_BAR] = CColor::FromHSB(rainbow, 1.f, 1.f);
		colors[WINDOW_TEXT] = CColor(255, 255, 255, MenuAlpha);

		colors[GROUPBOX_BODY] = CColor(22, 22, 22, MenuAlpha);
		colors[GROUPBOX_OUTLINE] = CColor(60, 60, 60, MenuAlpha);
		colors[GROUPBOX_TEXT] = WHITE;

		colors[SCROLLBAR_BODY] = CColor::FromHSB(rainbow, 1.f, 1.f);

		colors[SEPARATOR_TEXT] = WHITE;
		colors[SEPARATOR_LINE] = CColor(60, 60, 60, MenuAlpha);

		colors[CHECKBOX_CLICKED] = CColor::FromHSB(rainbow, 1.f, 1.f);
		colors[CHECKBOX_NOT_CLICKED] = CColor(60, 60, 60, MenuAlpha);
		colors[CHECKBOX_TEXT] = WHITE;

		colors[BUTTON_BODY] = CColor(40, 40, 40, MenuAlpha);
		colors[BUTTON_OUTLINE] = CColor(120, 120, 120, MenuAlpha);
		colors[BUTTON_TEXT] = WHITE;

		colors[COMBOBOX_TEXT] = LIGHTGREY;
		colors[COMBOBOX_SELECTED] = CColor(40, 40, 40, MenuAlpha);
		colors[COMBOBOX_SELECTED_TEXT] = WHITE;
		colors[COMBOBOX_ITEM] = CColor(40, 40, 40, MenuAlpha);
		colors[COMBOBOX_ITEM_TEXT] = LIGHTGREY;

		colors[SLIDER_BODY] = CColor(40, 40, 40, MenuAlpha);
		colors[SLIDER_VALUE] = CColor::FromHSB(rainbow, 1.f, 1.f);
		colors[SLIDER_TEXT] = LIGHTGREY;

		colors[TAB_BODY] = CColor(40, 40, 40, MenuAlpha), CColor(70, 70, 70, MenuAlpha);;
		colors[TAB_TEXT] = CColor(255, 255, 255, MenuAlpha);
		colors[TAB_BODY_SELECTED] = CColor(55, 55, 55, MenuAlpha);
		colors[TAB_TEXT_SELECTED] = CColor::FromHSB(rainbow, 1.f, 1.f);

		colors[VERTICAL_TAB_BODY] = CColor(22, 22, 22, MenuAlpha);
		colors[VERTICAL_TAB_TEXT] = CColor(255, 255, 255, MenuAlpha);
		colors[VERTICAL_TAB_OUTLINE] = CColor(22, 22, 22, MenuAlpha);
		colors[VERTICAL_TAB_BODY_SELECTED] = CColor(22, 22, 22, MenuAlpha);
		colors[VERTICAL_TAB_TEXT_SELECTED] = CColor::FromHSB(rainbow, 1.f, 1.f);

		colors[COLOR_PICKER_BODY] = CColor(40, 40, 40, MenuAlpha);
		colors[COLOR_PICKER_TEXT] = WHITE;
		colors[COLOR_PICKER_OUTLINE] = CColor(0, 0, 0, MenuAlpha);
	}
	void Do()
	{
		//if (SETTINGS::settings.menutheme == 0)
		//{
			if (UTILS::INPUT::input_handler.GetKeyState(VK_INSERT) & 1)
			{
				menu_open = !menu_open;
				INTERFACES::InputSystem->EnableInput(!menu_open);
			}
			auto menu_openn = menu_open;
			{
				if (menu_openn)
					MenuAlpha = min(MenuAlpha + 15, 255);
				else
					MenuAlpha = max(MenuAlpha - 15, 0);

				if (menu_openn)
					MenuAlpha5 = min(MenuAlpha5 + 15, 120);
				else
					MenuAlpha5 = max(MenuAlpha5 - 15, 0);

				if (menu_openn)
					MenuAlpha3 = min(MenuAlpha3 + 15, 15);
				else
					MenuAlpha3 = max(MenuAlpha3 - 15, 0);

				if (menu_openn)
					MenuAlpha7 = min(MenuAlpha7 + 15, 200);
				else
					MenuAlpha7 = max(MenuAlpha7 - 15, 0);

				if (menu_openn)
					MenuAlpha9 = min(MenuAlpha9 + 15, 245);
				else
					MenuAlpha9 = max(MenuAlpha9 - 15, 0);

				if (menu_openn)
					MenuAlpha11 = min(MenuAlpha11 + 15, 80);
				else
					MenuAlpha11 = max(MenuAlpha11 - 15, 0);

				if (menu_openn)
					MenuAlpha13 = min(MenuAlpha13 + 15, 140);
				else
					MenuAlpha13 = max(MenuAlpha13 - 15, 0);

				if (menu_openn)
					MenuAlpha15 = min(MenuAlpha15 + 15, 40);
				else
					MenuAlpha15 = max(MenuAlpha15 - 15, 0);

				if (menu_openn)
					MenuAlpha17 = min(MenuAlpha17 + 15, 50);
				else
					MenuAlpha17 = max(MenuAlpha17 - 15, 0);

				if (menu_openn)
					MenuAlpha21 = min(MenuAlpha21 + 15, 1);
				else
					MenuAlpha21 = max(MenuAlpha21 - 15, 0);

				if (menu_openn)
					MenuAlpha19 = min(MenuAlpha19 + 15, 100);
				else
					MenuAlpha19 = max(MenuAlpha19 - 15, 0);

				if (menu_openn)
					MenuAlpha23 = min(MenuAlpha23 + 15, 255);
				else
					MenuAlpha23 = max(MenuAlpha23 - 15, 0);
			}
			if (UTILS::INPUT::input_handler.GetKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.flip_int)) & 1)
				SETTINGS::settings.flip_bool = !SETTINGS::settings.flip_bool;

			if (UTILS::INPUT::input_handler.GetKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.quickstopkey)) & 1)
				SETTINGS::settings.stop_flip = !SETTINGS::settings.stop_flip;

			if (UTILS::INPUT::input_handler.GetKeyState(UTILS::INPUT::input_handler.keyBindings(SETTINGS::settings.overridekey)) & 1)
				SETTINGS::settings.overridething = !SETTINGS::settings.overridething;

			//if (SETTINGS::settings.menutheme == 0)
			//{
			InitColors();
			if (menu_hide)
			{
			}
			else
			{

				if (menu_open)
				{


					using namespace PPGUI_PP_GUI;
					if (!menu_open)	return;

					DrawMouse();

					SetFont(FONTS::menu_window_font2);
					WindowBegin("NNWARE alpha", Vector2D(200, 200), Vector2D(617, 606));
					SetFont(FONTS::menu_window_font);
					std::vector<std::string> tabs = { "A", "B", "C", "D", "F" ,"E" };
					std::vector<std::string> aim_mode = { "Rage", "Legit" };
					std::vector<std::string> acc_mode = { "Head", "Body Aim", "Hitscan" };
					std::vector<std::string> chams_mode = { "None", "Visible", "Invisible" };
					std::vector<std::string> aa_pitch = { "None", "Emotion", "Fake Down", "Fake Up", "Fake Zero" };
					std::vector<std::string> aa_mode = { "None", "backwards", "Sideways", "BackJitter", "Lowerbody", "Legit Troll", "Rotational", "Freestanding", "retard aa" };
					std::vector<std::string> aa_fake = { "None", "BackJitter", "Random", "Local View", "Opposite", "Rotational" };
					std::vector<std::string> configs = { "Default", "Legit", "Autos", "Scouts", "Pistols", "Awps", "Nospread" };
					std::vector<std::string> box_style = { "None", "Full", "Corners" };
					std::vector<std::string> media_style = { "Perfect", "Random" };
					std::vector<std::string> hitmarker = { "None", "Nnware" ,"Grim#3944", "Skeet", "Shrek" };
					std::vector<std::string> delay_shot = { "Off", "lag Compensation" };
					std::vector<std::string> fakelag_mode = { "Factor", "Adaptive" };
					std::vector<std::string> key_binds = { "None", "Mouse1", "Mouse2", "Mouse3", "Mouse4", "Mouse5", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12" };
					std::vector<std::string> antiaimmode = { "Standing", "Moving", "Jumping" };
					std::vector<std::string> glow_styles = { "Regular", "Pulsing", "Outline" };
					std::vector<std::string> local_chams = { "None","Sim Fakelag: Normal", "Non Sim Fakelag", "Sim Fakelag: Color", "Rainbow Regular" };
					std::vector<std::string> chams_type = { "Metallic", "Basic" };
					std::vector<std::string> team_select = { "Enemy", "Team" };
					std::vector<std::string> crosshair_select = { "None", "Static", "Recoil" };
					std::vector<std::string> autostop_method = { "Head", "Hitscan" };
					std::vector<std::string> override_method = { "Set", "Key Press" };
					std::vector<std::string> newbuybot_options = { "None", "Auto", "Scout", "Awp" };
					std::vector<std::string> weptype = { "Weapon", "Weapon + Text Ammo", "Weapon Icon" };
					std::vector<std::string> ResolverOptions = { "None", "LBY based", "Resolvy.us" };
					std::vector<std::string> menucolor = { "Red", "Blue", "Green", "Yellow", "Cyan", "Purple", "Pink", "Gold" };
					std::vector<std::string> menutheme = { "Regular", "Gamesens" };	
					std::vector<std::string> skinstabs = { "Knife", "Weapons", "Glove" };
					std::vector<std::string> wirefram = { "None", "Sleeves", "Hands" };


					std::string config;

					std::vector<std::string> KnifeModel = { "Bayonet",
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
					std::vector<std::string> M4A4 = { "none",
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
						"Radiation Harzard",
						"Whiteout" };
					std::vector<std::string> knifeskins = { "none",
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
						"Whiteout" };
					std::vector<std::string> ak47 = { "none",
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
						"Whiteout" };
					std::vector<std::string> GloveModel = { "none",
						"Bloodhound",
						"Sport",
						"Driver",
						"Wraps",
						"Moto",
						"Specialist" };
					std::vector<std::string> xdshit = { "kit1",
						"kit2",
						"kit3",
						"kit4" };
					std::vector<std::string> Duals = { "none",
						"Panther",
						"Dualing Dragons",
						"Cobra Strike",
						"Royal Consorts",
						"Duelist",
						"Whiteout" };
					std::vector<std::string> M4A1 = { "none",
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
					std::vector<std::string> Usp = { "none",
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
					std::vector<std::string> Glock = { "none",
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
					std::vector<std::string> Deagle = { "none",
						"Blaze",
						"Kumicho Dragon",
						"Oxide Blaze",
						"Golden Koi",
						"Cobalt Disruption",
						"Directive",
						"Whiteout" };
					std::vector<std::string> Five7 = { "none",
						"Monkey Business",
						"Hyper Beast",
						"Fowl Play",
						"Triumvirate",
						"Retrobution",
						"Capillary",
						"Violent Daimyo",
						"Whiteout" };
					std::vector<std::string> Aug = { "none",
						"Bengal Tiger",
						"Hot Rod",
						"Chameleon",
						"Akihabara Accept",
						"Whiteout" };
					std::vector<std::string> Famas = { "none",
						"Djinn",
						"Styx",
						"Neural Net",
						"Survivor Z",
						"Whiteout" };
					std::vector<std::string> G3sg1Skin = { "none",
						"Hunter",
						"The Executioner",
						"Terrace",
						"Whiteout" };
					std::vector<std::string> Galil = { "none",
						"Chatterbox",
						"Crimson Tsunami",
						"Sugar Rush",
						"Eco",
						"Cerberus",
						"Whiteout" };
					std::vector<std::string> M249 = { "none",
						"Nebula Crusader",
						"System Lock",
						"Magma",
						"Whiteout" };
					std::vector<std::string> Mac10 = { "none",
						"Neon Rider",
						"Last Dive",
						"Curse",
						"Rangeen",
						"Rangeen",
						"Whiteout" };
					std::vector<std::string> Ump45 = { "none",
						"Blaze",
						"Minotaur's Labyrinth",
						"Pandora's Box",
						"Primal Saber",
						"Exposure",
						"Whiteout" };
					std::vector<std::string> XM1014 = { "none",
						"Seasons",
						"Traveler",
						"Ziggy",
						"Whiteout" };
					std::vector<std::string> Cz75 = { "none",
						"Red Astor",
						"Pole Position",
						"Victoria",
						"Xiangliu",
						"Whiteout" };
					std::vector<std::string> PPBizon = { "none",
						"High Roller",
						"Judgement of Anubis",
						"Fuel Rod",
						"Whiteout" };
					std::vector<std::string> Mag7 = { "none",
						"Bulldozer",
						"Heat",
						"Petroglyph",
						"Whiteout" };
					std::vector<std::string> Awp = { "none",
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
					std::vector<std::string> negev = { "none",
						"Power Loader",
						"Loudmouth",
						"Man-o'-war",
						"Whiteout" };
					std::vector<std::string> Sawedoff = { "none",
						"Wasteland Princess",
						"The Kraken",
						"Yorick",
						"Whiteout" };
					std::vector<std::string> tec9 = { "none",
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
					std::vector<std::string> P2000 = { "none",
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
					std::vector<std::string> Mp7 = { "none",
						"Nemesis",
						"Impire",
						"Special Delivery",
						"Whiteout" };
					std::vector<std::string> Mp9 = { "none",
						"Rose Iron",
						"Ruby Poison Dart",
						"Airlock",
						"Whiteout"};
					std::vector<std::string> Nova = { "none",
						"Hyper Beast",
						"Koi",
						"Antique",
						"Whiteout" };
					std::vector<std::string> P250 = { "none",
						"Whiteout",
						"Crimson Kimono",
						"Mint Kimono",
						"Wingshot",
						"Asiimov",
						"See Ya Later",
						"Whiteout" };
					std::vector<std::string> SCAR20 = { "none",
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
					std::vector<std::string> Sg553 = { "none",
						"Tiger Moth",
						"Cyrex",
						"Pulse",
						"Fallout Warning",
						"Whiteout" };
					std::vector<std::string> SSG08 = { "none",
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
					std::vector<std::string> Revolver = { "none",
						"Llama Cannon",
						"Fade",
						"Crimson Web",
						"Whiteout" };

					//102 = white out
					//TABS_font
					SplitWindow(5);
					SetLine(1);				GroupboxBegin2("", 100, 556, 0);

					switch (VerticalTab("xd", tabs, OBJECT_FLAGS::FLAG_NONE))
					{
					case 0:
					{
						menutab = 0;
						break;
					}
					case 1:
					{
						menutab = 1;
						break;
					}
					case 2:
					{
						menutab = 2;
						break;
					}
					case 3:
					{
						menutab = 3;
						break;
					}
					case 4:
					{
						menutab = 4;
						break;
					}
					case 5:
					{
						menutab = 5;
						break;
					}
					}
					GroupboxEnd();

					SetLine(2);
					GroupboxBegin2("", 467, 556, 0);
					if (menutab == 0)// RAGE
					{
						SetFont(FONTS::menu_window_font);
						Checkbox("HvH", SETTINGS::settings.rageYEET);
						if(SETTINGS::settings.rageYEET)
						{
							std::vector<std::string> SetFont(FONTS::menu_window_font), aimtabs = { "Rage", "Anti Aim" };
							switch (Tab("xd", aimtabs, OBJECT_FLAGS::FLAG_NONE))
							{
							case 0:
							{
								Checkbox("Enable Rage", SETTINGS::settings.aim_bool);
								if (SETTINGS::settings.aim_bool)
									SETTINGS::settings.legit2_bool = false;
								Separator("Damage Control");
								Combobox("Aimbot Mode", acc_mode, SETTINGS::settings.acc_type);
								Slider("															                                                    Minimum Hit Chance", 0, 100, SETTINGS::settings.chance_val);
								Slider("															                                                    Minimum Damage", 1, 100, SETTINGS::settings.damage_val);
								Slider("															                                                    Minimum Auto Wall Damage", 1, 100, SETTINGS::settings.awall_damage_val);
								Slider("															                                                    BAIM after x shots", 0, 20, SETTINGS::settings.baimaftershot);
								Slider("															                                                    BAIM under HP", 0, 100, SETTINGS::settings.baimafterhp);
								Checkbox("BAIM when in air", SETTINGS::settings.baiminair);
								//Checkbox("BAIM if awp", SETTINGS::settings.baimifawp);

								Separator("Multipoint");
								Checkbox("Turn Multipoint On", SETTINGS::settings.multi_bool);
								Slider("															                                                    Head Scale", 0, 1, SETTINGS::settings.point_val);
								Slider("															                                                    Body Scale", 0, 1, SETTINGS::settings.body_val);

								Separator("Addons");
								Combobox("Resolver", ResolverOptions, SETTINGS::settings.ResolverEnable);
								Checkbox("NoSpread", SETTINGS::settings.nospread);
								Checkbox("Auto Revolver", SETTINGS::settings.auto_revolver);
								Checkbox("Prediction", SETTINGS::settings.prediction);
								Checkbox("Velocity-prediction", SETTINGS::settings.vecvelocityprediction);
								Combobox("Delay shot", delay_shot, SETTINGS::settings.delay_shot);
								Checkbox("Fakewalk-fix", SETTINGS::settings.fakefix_bool);
								Checkbox("Enable override", SETTINGS::settings.overrideenable);
								Combobox("Override Key", key_binds, SETTINGS::settings.overridekey);
								if (SETTINGS::settings.overridekey)
								{
									Combobox("Override Method", override_method, SETTINGS::settings.overridemethod);
								}
								break;
							case 1:
							{
								Checkbox("Enable Anti-Aim", SETTINGS::settings.aa_bool);
								if (SETTINGS::settings.aa_bool)
									SETTINGS::settings.legit2_bool = false;
								std::vector<std::string> aatab = { "Standing", "Moving", "Jumping", "Misc" };
								switch (Tab("aa", aatab, OBJECT_FLAGS::FLAG_NONE))
								{
								case 0:
								{

									Combobox("Antiaim Pitch - Standing", aa_pitch, SETTINGS::settings.aa_pitch_type);
									Combobox("Antiaim Real - Standing", aa_mode, SETTINGS::settings.aa_real_type);
									Combobox("Antiaim Fake - Standing", aa_fake, SETTINGS::settings.aa_fake_type);
									Slider("															                                                    real additive", -180, 180, SETTINGS::settings.aa_realadditive_val);
									Slider("															                                                    fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive_val);
									Slider("															                                                    lowerbody yaw delta", -180, 180, SETTINGS::settings.delta_val);
									break;
									{
								case 1:
								{
									Combobox("Antiaim Pitch - Moving", aa_pitch, SETTINGS::settings.aa_pitch1_type);
									Combobox("Antiaim Real - Moving", aa_mode, SETTINGS::settings.aa_real1_type);
									Combobox("Antiaim Fake - Moving", aa_fake, SETTINGS::settings.aa_fake1_type);
									Slider("															                                                    real additive ", -180, 180, SETTINGS::settings.aa_realadditive1_val);
									Slider("															                                                    fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive1_val);
									Slider("															                                                    lowerbody yaw delta", -180, 180, SETTINGS::settings.delta1_val);
									break;
								}
								case 2:
								{
									Combobox("Antiaim Pitch - Jumping", aa_pitch, SETTINGS::settings.aa_pitch2_type);
									Combobox("Antiaim Real - Jumping", aa_mode, SETTINGS::settings.aa_real2_type);
									Combobox("Antiaim Rake - Jumping", aa_fake, SETTINGS::settings.aa_fake2_type);
									Slider("															                                                    real additive ", -180, 180, SETTINGS::settings.aa_realadditive2_val);
									Slider("															                                                    fake additive", -180, 180, SETTINGS::settings.aa_fakeadditive2_val);
									Slider("															                                                    lowerbody yaw delta", -180, 180, SETTINGS::settings.delta2_val);
									break;
								}
								case 3:
								{
									Checkbox("Disable AA If Important", SETTINGS::settings.Disableaanade);
									Checkbox("AA indicator", SETTINGS::settings.rifk_arrow);
									Slider("Anti-Aim arrow size", 10, 200, SETTINGS::settings.aasize);
									Combobox("Flip Key", key_binds, SETTINGS::settings.flip_int);
									Checkbox("mEmE WaLk", SETTINGS::settings.astro);
									Checkbox("Fakewalk", SETTINGS::settings.fakewalk);
									Slider("Fakewalk speed", 3, 8, SETTINGS::settings.fakewalkspeed, 1);
									Checkbox("Enable fakelag", SETTINGS::settings.lag_bool);
									Combobox("Fakelag type", fakelag_mode, SETTINGS::settings.lag_type);
									Slider("															                                                     Standing lag", 1, 14, SETTINGS::settings.stand_lag);
									Slider("															                                                     Move lag", 1, 14, SETTINGS::settings.move_lag);
									Slider("															                                                     Jump lag", 1, 14, SETTINGS::settings.jump_lag);
									Combobox("Rotate mode", antiaimmode, SETTINGS::settings.aa_mode);
									Slider("															                                                    Rotate fake angle", 0, 180, SETTINGS::settings.spinanglefake);
									Slider("															                                                    Rotate fake speed", 0, 100, SETTINGS::settings.spinspeedfake);
									switch (SETTINGS::settings.aa_mode)
									{
									case 0:
									{
										Slider("															                                                    Rotate standing angle", 0, 180, SETTINGS::settings.spinangle);
										Slider("															                                                    Rotate standing speed", 0, 100, SETTINGS::settings.spinspeed);
										break;
									}
									case 1:
									{
										Slider("															                                                    Rotate moving angle", 0, 180, SETTINGS::settings.spinangle1);
										Slider("															                                                    Rotate moving speed", 0, 100, SETTINGS::settings.spinspeed1);
										break;
									}
									case 2:
									{
										Slider("															                                                    Rotate jumping angle", 0, 180, SETTINGS::settings.spinangle2);
										Slider("															                                                    Rotate jumping speed", 0, 100, SETTINGS::settings.spinspeed2);
										break;
									}
									}
								}

									}
								}
								}
								break;
							}
							}
							}
						}
					}
					else if (menutab == 2)//VISUALS
					{
						std::vector<std::string> SetFont(FONTS::menu_window_font), visualtabs = { "Players", "Chams", "Glow", "World", "Local Player", "Colors" };
						Checkbox("Enable visuals", SETTINGS::settings.esp_bool);
						if (SETTINGS::settings.esp_bool)
						{
							//Slider("Rainbow speed", 0.001, 0.01, SETTINGS::settings.gradientspeed, 3);
							switch (Tab("ahah", visualtabs, OBJECT_FLAGS::FLAG_NONE))
							{
							case 0:
							{
								Separator("Players");
								Combobox("Select Team", team_select, SETTINGS::settings.espteamselection);
								if (SETTINGS::settings.espteamselection == 0)
								{
									Combobox("Draw Enemy Box", box_style, SETTINGS::settings.box_bool);
									Checkbox("Draw Enemy Name", SETTINGS::settings.name_bool);
									Checkbox("Draw Enemy Weapon Icons", SETTINGS::settings.WeaponIconsOn);
									Checkbox("Draw Enemy Weapon", SETTINGS::settings.weap_bool);
									if (SETTINGS::settings.weap_bool)
									{
										Combobox("Type", weptype, SETTINGS::settings.draw_wep);
									}
									Checkbox("Draw Enemy Flags", SETTINGS::settings.info_bool);
									Checkbox("Draw Enemy Health", SETTINGS::settings.health_bool);
									Checkbox("Draw Enemy Money", SETTINGS::settings.money_bool);
									//Checkbox("Draw Ammo Bar - Crashing Sometimes", SETTINGS::settings.ammo_bool);
									Checkbox("Draw Enemy Fov Arrows", SETTINGS::settings.fov_bool);
								}
								else if (SETTINGS::settings.espteamselection == 1)
								{
									Combobox("Draw Team Box", box_style, SETTINGS::settings.boxteam);
									Checkbox("Draw Team Name", SETTINGS::settings.nameteam);
									Checkbox("Draw Team Weapon", SETTINGS::settings.weaponteam);
									Checkbox("Draw Team Flags", SETTINGS::settings.flagsteam);
									Checkbox("Draw Team Health", SETTINGS::settings.healthteam);
									Checkbox("Draw Team Money", SETTINGS::settings.moneyteam);
									//Checkbox("Draw Ammo Bar - crashing sometimes", SETTINGS::settings.ammoteam);
									Checkbox("Draw Team Fov Arrows", SETTINGS::settings.arrowteam);
								}
							}
							break;
							case 1:
							{
								Separator("Chams");
								Combobox("Model Team Selection", team_select, SETTINGS::settings.chamsteamselection);
								if (SETTINGS::settings.chamsteamselection == 0)
								{
									Combobox("Enemy Coloured Models", chams_mode, SETTINGS::settings.chams_type);
								}
								else if (SETTINGS::settings.chamsteamselection == 1)
								{
									Combobox("Team Coloured Models", chams_mode, SETTINGS::settings.chamsteam);
								}
								Checkbox("Backtrack Chams", SETTINGS::settings.BacktrackChams);
							}break;
							case 2:
							{
								Separator("Glow");
								Combobox("Glow Team Selection", team_select, SETTINGS::settings.glowteamselection);
								if (SETTINGS::settings.glowteamselection == 0)
								{
									Checkbox("Enemy Glow Enable", SETTINGS::settings.glowenable);
								}
								else if (SETTINGS::settings.glowteamselection == 1)
								{
									Checkbox("Team Glow Enable", SETTINGS::settings.glowteam);
								}
								Combobox("Glow Style", glow_styles, SETTINGS::settings.glowstyle);
							}break;
							case 3:
							{
								Separator("World");
								//Checkbox("LC Hitboxes", SETTINGS::settings.lagcomhit);
								//if (SETTINGS::settings.lagcomhit)
								//{
									//Slider("															                                                    Duration", 0, 10, SETTINGS::settings.lagcomptime);
								//}
								Checkbox("Gravitational Ragdoll", SETTINGS::settings.ragdoll_bool);
								Slider("															                                                    Gravitational Ragdoll Amount", -800, 800, SETTINGS::settings.radgollslider);
								Checkbox("Night mode", SETTINGS::settings.night_mode);
								Checkbox("Sky color changer", SETTINGS::settings.sky_enabled); //fixed by sleevy
								Checkbox("World Color Changer", SETTINGS::settings.wolrd_enabled);
								Checkbox("Asus Props", SETTINGS::settings.asus_bool);
								Checkbox("Bullet Tracers", SETTINGS::settings.beam_bool);
								if (SETTINGS::settings.beam_bool)
								{
									Checkbox("Rainbow bullet tracers ", SETTINGS::settings.gradientbullet);
									Slider("															                                                    Rainbow bullet tracers speed", 0.001, 0.01, SETTINGS::settings.gradienttracerspeed, 3);
								}
								Checkbox("Bullet Impacts", SETTINGS::settings.impacts);
								Checkbox("Damage Indicator", SETTINGS::settings.dmg_bool);
								Checkbox("Aimware Hitmarkers", SETTINGS::settings.hitmarker);
								Checkbox("No Smoke", SETTINGS::settings.smoke_bool);
								if (!SETTINGS::settings.matpostprocessenable)
									Checkbox("Postprocessing Disabled", SETTINGS::settings.matpostprocessenable);
								else
									Checkbox("Postprocessing Enabled", SETTINGS::settings.matpostprocessenable);
							}break;
							case 4:
							{
								Separator("Local Player");
								Combobox("Local Chams", local_chams, SETTINGS::settings.localchams);
								if (SETTINGS::settings.localchams == 4)
								{
									Slider("															                                                    Rainbow local chams speed", 0.001, 0.01, SETTINGS::settings.gradientchamsspeed, 3);
								}
								Checkbox("Local Glow", SETTINGS::settings.glowlocal);
								if (SETTINGS::settings.glowlocal)
								{
									Combobox("Local Glow Style", glow_styles, SETTINGS::settings.glowstylelocal);
									Checkbox("Rainbow local glow", SETTINGS::settings.gradientglow);
									if(SETTINGS::settings.gradientglow)
									Slider("															                                                    Rainbow local glow speed", 0.001, 0.01, SETTINGS::settings.gradientglowspeed, 3);
								}
								Slider("															                                                    Viewmodel FOV", 0, 179, SETTINGS::settings.viewfov_val, 68);
								Slider("															                                                    Render Fov", 90, 179, SETTINGS::settings.fov_val, 90);
								Combobox("Hitmarker Sound", hitmarker, SETTINGS::settings.hitmarker_val);
								Checkbox("LBY Indicator", SETTINGS::settings.lbyenable);
								Checkbox("Force crosshair", SETTINGS::settings.forcehair);
								Checkbox("Watermark", SETTINGS::settings.Watermark);
								//Checkbox("No Flash",SETTINGS::settings.noflash); //not fixed
								Checkbox("Render spread", SETTINGS::settings.spread_bool);
								Combobox("Wire frame goodies", wirefram, SETTINGS::settings.wiresleeve_bool);
								Checkbox("Remove Visual Recoil", SETTINGS::settings.norecoil);
								Checkbox("No Scope", SETTINGS::settings.scope_bool);
								Checkbox("Remove Zoom", SETTINGS::settings.removescoping);
								Checkbox("Fix Zoom Sensitivity", SETTINGS::settings.fixscopesens);
								//Checkbox("Freestanding indicator", SETTINGS::settings.freestandind);
								Checkbox("Draw zeus range (laggy af)", SETTINGS::settings.zeusrange);
								Checkbox("Thirdperson", SETTINGS::settings.tp_bool);
								if (SETTINGS::settings.tp_bool)
								{
									Combobox("Thirdperson Key", key_binds, SETTINGS::settings.thirdperson_int);
									Slider("															                                                    Scope trans", 0, 100, SETTINGS::settings.transparency_when_scoped, 50);
									Slider("															                                                    Thirdperson fov", 0, 179, SETTINGS::settings.THIRDPERSONFOV, 90);
								}
								Combobox("Crosshair", crosshair_select, SETTINGS::settings.xhair_type);
							}break;
							case 5:
							{
								Separator("ESP Colours");
								Combobox("ESP Colour Selection", team_select, SETTINGS::settings.espteamcolourselection);
								if (SETTINGS::settings.espteamcolourselection == 0)
								{
									ColorPicker("Enemy Box Colour", SETTINGS::settings.boxenemy_col);
									ColorPicker("Enemy Name Colour", SETTINGS::settings.nameenemy_col);
									ColorPicker("Enemy Weapon Colour", SETTINGS::settings.weaponenemy_col);
									ColorPicker("Enemy Fov Arrows Colour", SETTINGS::settings.fov_col);
									ColorPicker("Damage Indicator Color", SETTINGS::settings.dmg_color);
								}
								else if (SETTINGS::settings.espteamcolourselection == 1)
								{
									ColorPicker("Team Box Colour", SETTINGS::settings.boxteam_col);
									ColorPicker("Team Name Colour", SETTINGS::settings.nameteam_col);
									ColorPicker("Team Weapon Colour", SETTINGS::settings.weaponteam_col);
									ColorPicker("Team Fov Arrows Colour", SETTINGS::settings.arrowteam_col);
								}
								Separator("Misc");
								ColorPicker("Spread", SETTINGS::settings.spread_Col);
								//ColorPicker("Freestanding indicator color", SETTINGS::settings.FREESTANDINGcolor);
								ColorPicker("Zeus Range Color", SETTINGS::settings.zeusrangecolor);
								//ColorPicker("Grenade Prediction Colour", SETTINGS::settings.grenadepredline_col); //slows game
								ColorPicker("AA arrow real color", SETTINGS::settings.aacolorreal);
								ColorPicker("AA arrow fake color", SETTINGS::settings.aacolorfake);

								Separator("Chams Colours");
								ColorPicker("Enemy Visible Colour", SETTINGS::settings.vmodel_col);
								ColorPicker("Enemy Invisible Colour", SETTINGS::settings.imodel_col);

								ColorPicker("Team Visible Colour", SETTINGS::settings.teamvis_color);
								ColorPicker("Team Invisible Colour", SETTINGS::settings.teaminvis_color);

								ColorPicker("Local Colour", SETTINGS::settings.localchams_col);
								//ColorPicker("Local Fake", SETTINGS::settings.fake_darw_col);

								Separator("Glow Colours");
								ColorPicker("Glow Enemy Colour", SETTINGS::settings.glow_col);
								ColorPicker("Glow Team Colour", SETTINGS::settings.teamglow_color);
								ColorPicker("Glow Local Colour", SETTINGS::settings.glowlocal_col);

								Separator("World");
								ColorPicker("Sky color", SETTINGS::settings.skycolor);
								ColorPicker("World Color", SETTINGS::settings.night_col);
								ColorPicker("AW Hitmarkers color", SETTINGS::settings.awcolor);
								//ColorPicker("Backtrack chams color", SETTINGS::settings.chamsbackcolor);

								Separator("Bullet Tracer Colours");
								ColorPicker("Local Player", SETTINGS::settings.bulletlocal_col);
								ColorPicker("Enemy Player", SETTINGS::settings.bulletenemy_col);
								ColorPicker("Team Player", SETTINGS::settings.bulletteam_col);

								if (Button("Randomize colors"))
								{
									SETTINGS::settings.aacolorfake = RANDOMIZECOL;
									SETTINGS::settings.aacolorreal = RANDOMIZECOL;
									SETTINGS::settings.boxenemy_col = RANDOMIZECOL;
									SETTINGS::settings.nameenemy_col = RANDOMIZECOL;
									SETTINGS::settings.weaponenemy_col = RANDOMIZECOL;
									SETTINGS::settings.dmg_color = RANDOMIZECOL;
									SETTINGS::settings.spread_Col = RANDOMIZECOL;
									SETTINGS::settings.zeusrangecolor = RANDOMIZECOL;
									SETTINGS::settings.skycolor = RANDOMIZECOL;
									SETTINGS::settings.night_col = RANDOMIZECOL;
									SETTINGS::settings.awcolor = RANDOMIZECOL;

									SETTINGS::settings.box_col = RANDOMIZECOL;
									SETTINGS::settings.name_col = RANDOMIZECOL;
									SETTINGS::settings.weapon_col = RANDOMIZECOL;
									SETTINGS::settings.fov_col = RANDOMIZECOL;

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
							}break;
							}
						}
					}
					else if (menutab == 1)//Legit
					{
						Checkbox("Enable legit", SETTINGS::settings.legit2_bool);
						if (SETTINGS::settings.legit2_bool)
						{
							Separator("If I Get Another Coder Maybe");
							SETTINGS::settings.aa_bool = false;
							SETTINGS::settings.aim_bool = false;
							SETTINGS::settings.rageYEET = false;
						}
					}
					else if (menutab == 3)//MISC
					{
						Checkbox("Enable Misc", SETTINGS::settings.misc_bool);
						if (SETTINGS::settings.misc_bool)
						{
							Separator("Movement");
							Checkbox("Auto Bunnyhop", SETTINGS::settings.bhop_bool);
							Checkbox("Auto Strafer", SETTINGS::settings.strafe_bool);
							Checkbox("Auto Duck In Air", SETTINGS::settings.duck_bool);
							Combobox("Circle Strafe", key_binds, SETTINGS::settings.circlestrafekey);

							Separator("Clantag Changer");
							Checkbox("nnware.cf Clantag", SETTINGS::settings.misc_clantag);

							Separator("Buy Bot");
							Checkbox("Enable", SETTINGS::settings.autobuy_bool);
							Checkbox("Auto", SETTINGS::settings.auto_bool);
							if (SETTINGS::settings.auto_bool)
								SETTINGS::settings.scout_bool = false;
							Checkbox("Scout", SETTINGS::settings.scout_bool);
							if (SETTINGS::settings.scout_bool)
								SETTINGS::settings.auto_bool = false;
							Checkbox("Revolver / Deagle", SETTINGS::settings.revolver_bool);
							if (SETTINGS::settings.revolver_bool)
								SETTINGS::settings.elite_bool = false;
							Checkbox("Elites", SETTINGS::settings.elite_bool);
							if (SETTINGS::settings.elite_bool)
								SETTINGS::settings.revolver_bool = false;
							Checkbox("Zeus", SETTINGS::settings.zeus_bool);
							Checkbox("Armour + Defuse", SETTINGS::settings.armour_bool);
							Checkbox("HE Grenade", SETTINGS::settings.henade);
							Checkbox("Incgrenade", SETTINGS::settings.molly);
							Checkbox("Smoke", SETTINGS::settings.smoke);

							Separator("Etc..");
							if (Button("fake crash (rq)"))
							{
								Sleep(10000000000000000000);
							}
							Checkbox("Trashtalk If hit Head", SETTINGS::settings.trashtalk2);
							Checkbox("Damage > 100", SETTINGS::settings.trashtalk);
							Checkbox("If HS With Awp", SETTINGS::settings.trashtalk3);
						}
					}
					else if (menutab == 4) //SETTINGS
					{
						Separator("Configuration");
						switch (Combobox("Config", configs, SETTINGS::settings.config_sel))
						{
						case 0: config = "Default"; break;
						case 1: config = "Legit"; break;
						case 2: config = "Auto"; break;
						case 3: config = "Scout"; break;
						case 4: config = "Pistol"; break;
						case 5: config = "Awp"; break;
						case 6: config = "Nospread"; break;
						}

						if (Button("Load Config"))
						{
							INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 0, 100), "[NNWARAE Config] ");
							GLOBAL::Msg("Configuration Loaded.     \n");

							SETTINGS::settings.Load(config);
						}
						if (Button("Save Config"))
						{
							INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 0, 100), "[NNWARE Config] ");
							GLOBAL::Msg("Configuration Saved.     \n");

							SETTINGS::settings.Save(config);
						}
					}
					else if (menutab == 5)// Skins
					{
						Checkbox("Enable Skins", SETTINGS::settings.Skins);
						if (SETTINGS::settings.Skins)
						{
							if (Button(("Force update")))
								KnifeApplyCallbk();

							switch (Tab("Skins", skinstabs, OBJECT_FLAGS::FLAG_NONE))
							{
							case 0:
							{
								Checkbox("Knife Changer", SETTINGS::settings.knifes);
								Combobox(("Knife Model"), KnifeModel, SETTINGS::settings.Knife);
								Combobox(("Knife Skin"), knifeskins, SETTINGS::settings.KnifeSkin);
								break;
							}
							case 1:
							{
								Checkbox("Skin Changer", SETTINGS::settings.skinenabled);
								Separator("Rilfes");
								Combobox(("AK-47"), ak47, SETTINGS::settings.AK47Skin);
								Combobox(("M4A4"), M4A4, SETTINGS::settings.M4A4Skin);
								Combobox(("M4A1-s"), M4A1, SETTINGS::settings.M4A1SSkin);
								Combobox(("Aug"), Aug, SETTINGS::settings.AUGSkin);
								Combobox(("Famas"), Famas, SETTINGS::settings.FAMASSkin);
								Combobox(("Galil"), Galil, SETTINGS::settings.GalilSkin);
								Combobox(("Sg553"), Sg553, SETTINGS::settings.Sg553Skin);

								Separator("Pistols");
								Combobox(("Duals"), Duals, SETTINGS::settings.DualSkin);
								Combobox(("Usp-s"), Usp, SETTINGS::settings.USPSkin);
								Combobox(("Glock"), Glock, SETTINGS::settings.GlockSkin);
								Combobox(("Deagle"), Deagle, SETTINGS::settings.DeagleSkin);
								Combobox(("Five7"), Five7, SETTINGS::settings.FiveSkin);
								Combobox(("Cz75"), Cz75, SETTINGS::settings.Cz75Skin);
								Combobox(("Tec9"), tec9, SETTINGS::settings.tec9Skin);
								Combobox(("P2000"), P2000, SETTINGS::settings.P2000Skin);
								Combobox(("P250"), P250, SETTINGS::settings.P250Skin);
								Combobox(("Revolver"), Revolver, SETTINGS::settings.RevolverSkin);

								Separator("Snipers");
								Combobox(("Awp"), Awp, SETTINGS::settings.AWPSkin);
								Combobox(("G3sg1"), G3sg1Skin, SETTINGS::settings.G3sg1Skin);
								Combobox(("SCAR20"), SCAR20, SETTINGS::settings.SCAR20Skin);
								Combobox(("SSG08"), SSG08, SETTINGS::settings.SSG08Skin);

								Separator("Lmgs");
								Combobox(("M249"), M249, SETTINGS::settings.M249Skin);
								Combobox(("Negev"), negev, SETTINGS::settings.NegevSkin);

								Separator("Sub Machineguns");
								Combobox(("Mac10"), Mac10, SETTINGS::settings.Mac10Skin);
								Combobox(("Ump45"), Ump45, SETTINGS::settings.UMP45Skin);
								Combobox(("PPBizon"), PPBizon, SETTINGS::settings.BizonSkin);
								Combobox(("Mp7"), Mp7, SETTINGS::settings.Mp7Skin);
								Combobox(("Mp9"), Mp9, SETTINGS::settings.Mp9Skin);

								Separator("Shotguns");
								Combobox(("XM1014"), XM1014, SETTINGS::settings.XmSkin);
								Combobox(("Mag-7"), Mag7, SETTINGS::settings.MagSkin);
								Combobox(("Sawed Off"), Sawedoff, SETTINGS::settings.SawedSkin);
								Combobox(("Nova"), Nova, SETTINGS::settings.NovaSkin);
								break;
							}
							case 2:
							{
								Checkbox("Glove Changer", SETTINGS::settings.glovesenabled);
								Combobox(("Glove Model"), GloveModel, SETTINGS::settings.gloves);
								Combobox(("Glove Skin"), xdshit, SETTINGS::settings.skingloves);
								break;
							}
							}
						}
					}
					GroupboxEnd();

					WindowEnd();

					int w, h;
					static int x, y;

					INTERFACES::Engine->GetScreenSize(w, h);
					static bool init = false;
					if (init == false) {
						x = w / 2 - (400 / 2);
						y = h / 2 - (200 / 2);
						init = true;
					}
				}
			}
		}
}