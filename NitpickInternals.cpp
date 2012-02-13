#pragma warning(disable : 4005)
#include "NitpickInternals.h"

IDebugLog		gLog("Nitpick.log");
PluginHandle	g_pluginHandle = kPluginHandle_Invalid;


_DefineHookHdlr(INICollectionLoadSetting, 0x00AE37F3);


void StartPickingNit()
{
	_MemHdlr(INICollectionLoadSetting).WriteJump();
}

static char			s_GetPrivateProfileStringAuxBuffer[0x8000] = {0};		// large enough, I should think

#define _hhName	INICollectionLoadSetting
_hhBegin()
{
	_hhSetVar(Retn, 0x00AE3814);
	__asm
	{
		push    0x8000
		lea		eax, s_GetPrivateProfileStringAuxBuffer
		push	eax
		push	ecx
		lea     edx, [esp + 0x74]
		push    edx
		lea     eax, [esp + 0x38]
		push    eax
		call    ebp			// GetPrivateProfileStringA
		lea		ecx, s_GetPrivateProfileStringAuxBuffer
		jmp		[_hhGetVar(Retn)]
	}
}

