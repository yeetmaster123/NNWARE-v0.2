#pragma once

namespace SDK
{
	class IMatRenderContext;
	class Ray_t;
	class ITraceFilter;

	struct CViewSetup;
	struct DrawModelState_t;
	struct ModelRenderInfo_t;
	struct trace_t;
}

namespace HOOKS
{
	typedef bool(__stdcall* CreateMoveFn)(float , SDK::CUserCmd*);
	typedef void(__thiscall* FrameStageNotifyFn)(void*, int);
	typedef void(__thiscall* PaintFn)(void*, void*, int);
	typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
	typedef void(__thiscall* DrawModelExecuteFn)(void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);
	typedef void(__thiscall* SceneEndFn)(void*);
	typedef void(__thiscall* TraceRayFn)(void *thisptr, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter* pTraceFilter, SDK::trace_t *pTrace);
	typedef void(__thiscall* OverrideViewFn)(void* ecx, SDK::CViewSetup* pSetup);
	typedef void(__thiscall* RenderViewFn)(void*, SDK::CViewSetup&, SDK::CViewSetup&, unsigned int nClearFlags, int whatToDraw);
	typedef int(__thiscall* SendDatagramFn)(void*, void*);
	typedef bool(__thiscall* SvCheatsGetBoolFn)(void*);
	typedef float(__thiscall* GetViewmodelFOVFn)();



	extern CreateMoveFn original_create_move;
	extern PaintFn original_paint;
	extern PaintTraverseFn original_paint_traverse;
	extern FrameStageNotifyFn original_frame_stage_notify;
	extern DrawModelExecuteFn original_draw_model_execute;
	extern SceneEndFn original_scene_end;
	extern TraceRayFn original_trace_ray;
	extern SendDatagramFn original_send_datagram;
	extern OverrideViewFn original_override_view;
	extern RenderViewFn original_render_view;
	extern SvCheatsGetBoolFn original_get_bool;
	extern GetViewmodelFOVFn original_viewmodel_fov;

	extern vfunc_hook client;
	extern vfunc_hook panel;
	extern vfunc_hook paint;
	extern vfunc_hook modelrender;
	extern vfunc_hook sceneend;
	extern vfunc_hook renderview;
	extern vfunc_hook trace;
	extern vfunc_hook netchannel;
	extern vfunc_hook overrideview;
	extern vfunc_hook input;
	extern vfunc_hook getbool;

	void InitHooks();

	// virtual function hooks
	bool __stdcall   HookedCreateMove(float sample_input_frametime, SDK::CUserCmd* cmd);
	void __fastcall  HookedFrameStageNotify(void*, void*, int);
	void __stdcall   HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce);
	void __fastcall  HookedDrawModelExecute(void*, void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);
	void __fastcall  HookedSceneEnd(void*, void*);
	void __fastcall  HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace);
	void __fastcall  HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup);
	void __fastcall  HookedRenderView(void* thisptr, void*, SDK::CViewSetup& setup, SDK::CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw);
	bool __fastcall  HookedGetBool(void* pConVar, void* edx);
	float __fastcall GetViewmodelFOV();
	void __fastcall  HookedRenderView(void* thisptr, void*, SDK::CViewSetup& setup, SDK::CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw);
	void Hook_LockCursor(void* xd);
	void hookKnife();
	// netvar hooks
	void InitNetvarHooks();
}
namespace RecVarShit
{
	void hookKnife();
}