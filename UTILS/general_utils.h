#pragma once

#include "logging.h"

class Vector2D;
namespace SDK
{
	class CUserCmd;
}
namespace UTILS
{
	template <class T>
	constexpr const T& Min(const T& x, const T& y)
	{
		return (x > y) ? y : x;
	}
	template <class T>
	constexpr const T& Clamp(const T& v, const T& lo, const T& hi)
	{
		return (v >= lo && v <= hi) ? v : (v < lo ? lo : hi);
	}

	template<class T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		return (v >= lo && v <= hi) ? v : (v < lo ? lo : hi);
	}
	template<unsigned int IIdx, typename TRet, typename ... TArgs>
	static auto CallVFunc(void* thisptr, TArgs ... argList) -> TRet
	{
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}
	float GetLBYRotatedYaw(float lby, float yaw);

	bool is_point_in_range(Vector2D, Vector2D, Vector2D);
	float GetFraction(float, float, float);
	float GetValueFromFraction(float, float, float);
	unsigned int GetNumberOfDigits(int);
	float RoundToDecimalPlaces(float, int);
	std::string FloatToString(float, int);
	Vector CalcAngle(Vector src, Vector dst);
	bool IsOnScreen(Vector origin, Vector& screen);
	float GetCurtime();
	float Lerp(float fraction, float min, float max);
	DWORD FindSignaturenew(const char* szModuleName, const char* PatternName, char* szPattern);
	uint64_t FindSignature(const char* szModule, const char* szSignature);
	DWORD WaitOnModuleHandle(std::string moduleName);
	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask);
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);
	bool ClampLemon(Vector& angles);
	void SetClanTag(const char* tag, const char* name);
	short SafeWeaponID();
}
