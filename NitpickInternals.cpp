#pragma warning(disable : 4005)
#include "NitpickInternals.h"

IDebugLog						gLog("Nitpick.log");
PluginHandle					g_pluginHandle = kPluginHandle_Invalid;

_DefineHookHdlr(INICollectionLoadSetting, 0x00AFEE53);
_DefineHookHdlr(TESDataHandlerPopulatePluginList, 0x0043E457);




void __stdcall Inv3DTestHook(void* obj, UInt32 staticObj, UInt32 nodeSmrtPtr)
{
	UInt32 inv3DMgr = *((UInt32*)(0x01B2E99C));

	UInt32 pcNode = thisVirtualCall<UInt32>(0x1C0, *((UInt32*)0x01B2E8E4));
	//NiNode* clone = thisCall<NiNode*>(0x00AAFF30, pcNode);
	UInt32 clone = pcNode;

	thisCall<void>(0x004B7A20, nodeSmrtPtr, clone);
	thisCall<void>(0x008672E0, inv3DMgr, obj, staticObj, nodeSmrtPtr);
}

void __stdcall doinvtest2(void* nodeSmrtPtr)
{
	UInt32 pcNode = thisVirtualCall<UInt32>(0x1C0, *((UInt32*)0x01B2E8E4));
	NiNode* clone = thisCall<NiNode*>(0x00AAFF30, pcNode);

	thisCall<void>(0x004B7A20, nodeSmrtPtr, clone);
}

#define _hhName	invtest2
_hhBegin()
{
	_hhSetVar(Retn, 0x008676E3);
	__asm
	{
		lea		eax, [esp + 0x14]
		pushad
		push	eax
		call	doinvtest2
		popad
		call	edx

		jmp		[_hhGetVar(Retn)]
	}
}
_DefineHookHdlr(invtest2, 0x008676C7);

void StartPickingNit()
{
	_MemHdlr(INICollectionLoadSetting).WriteJump();
	_MemHdlr(TESDataHandlerPopulatePluginList).WriteJump();

	WriteRelCall(0x008675EC, (UInt32)Inv3DTestHook);
//	_MemHdlr(invtest2).WriteJump();
}

static char			s_GetPrivateProfileStringAuxBuffer[0x8000] = {0};		// large enough, I should think

#define _hhName	INICollectionLoadSetting
_hhBegin()
{
	_hhSetVar(Retn, 0x00AFEE74);
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

bool __stdcall FixPluginListPopulation(WIN32_FIND_DATA* FileData)
{
	static std::list<std::string> kActivePluginList;
	if (kActivePluginList.size() == 0)
	{
		const char* kAppDataPath = (const char*)0x01B9C2E8;
		const char* kPluginListName = ".\\Plugins.txt";

		char Buffer[0x104] = {0};
		strcpy_s(Buffer, sizeof(Buffer), kAppDataPath);
		strcat_s(Buffer, sizeof(Buffer), kPluginListName);

		std::fstream PluginListStream(Buffer, std::ios::in);
		if (PluginListStream.fail() == false)
		{
			char Entry[0x200] = {0};
			while (PluginListStream.eof() == false)
			{
				ZeroMemory(Entry, sizeof(Entry));
				PluginListStream.getline(Entry, sizeof(Entry));

				if (strlen(Entry) > 2 && Entry[0] != '#')
				{
					kActivePluginList.push_back(Entry);
				}
			}
		}
	}

	if (!_stricmp(FileData->cFileName, "skyrim.esm") ||
		!_stricmp(FileData->cFileName, "update.esm"))
	{
		return true;
	}

	for (std::list<std::string>::const_iterator Itr = kActivePluginList.begin(); Itr != kActivePluginList.end(); Itr++)
	{
		if (!_stricmp(Itr->c_str(), FileData->cFileName))
			return true;
	}

	return false;
}

#define _hhName	TESDataHandlerPopulatePluginList
_hhBegin()
{
	_hhSetVar(Retn, 0x0043E45E);
	_hhSetVar(Jump, 0x0043E5C3);
	__asm
	{
		lea     eax, [esp + 0x128]

		push	ecx
		push	esi
		push	eax
		call	FixPluginListPopulation
		pop		esi
		pop		ecx

		test	al, al
		jz		SKIPPLUGIN

		cmp     [esp + 0x144], ebp
		jmp		[_hhGetVar(Retn)]
	SKIPPLUGIN:
		jmp		[_hhGetVar(Jump)]
	}
}