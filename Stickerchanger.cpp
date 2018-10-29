#include "Stickerchanger.h"
#include "Skinchanger.h"


enum class EStickerAttributeType
{
	Index,
	Wear,
	Scale,
	Rotation
};

static uint16_t s_iwoff = 0;

static void* o_uint_fn;

static unsigned int __fastcall hooked_uint_fn(void* thisptr, void*, int slot, EStickerAttributeType attribute, unsigned fl)
{
	auto item = reinterpret_cast<SDK::CBaseWeapon*>(uintptr_t(thisptr) - s_iwoff);

	if (attribute == EStickerAttributeType::Index)
	{
		switch (slot)
		{
		case 0:
			return (int)SETTINGS::settings.weapons[item->GetItemDefinitionIndexbam()].Stikers1;
			break;
		case 1:
			return (int)SETTINGS::settings.weapons[item->GetItemDefinitionIndexbam()].Stikers2;
			break;
		case 2:
			return (int)SETTINGS::settings.weapons[item->GetItemDefinitionIndexbam()].Stikers3;
			break;
		case 3:
			return (int)SETTINGS::settings.weapons[item->GetItemDefinitionIndexbam()].Stikers4;
			break;
		default:
			break;
		}
	}

	return reinterpret_cast<decltype(hooked_uint_fn)*>(o_uint_fn)(thisptr, nullptr, slot, attribute, fl);
}

void ApplyStickerHooks(SDK::CBaseWeapon* item)
{
	if (!s_iwoff)
		s_iwoff = OFFSETS::m_Item + 0xC;

	void**& iw_vt = *reinterpret_cast<void***>(uintptr_t(item) + s_iwoff);

	static void** iw_hook_vt = nullptr;

	if (!iw_hook_vt)
	{
		size_t len = 0;
		for (; iw_vt[len]; ++len);
		iw_hook_vt = new void*[len];

		memcpy(iw_hook_vt, iw_vt, len * sizeof(void*));

		o_uint_fn = iw_hook_vt[5];
		iw_hook_vt[5] = &hooked_uint_fn;
	}

	iw_vt = iw_hook_vt;
}
