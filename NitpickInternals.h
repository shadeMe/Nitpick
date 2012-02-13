#pragma once

#include "skse/PluginAPI.h"
#include "skse/skse_version.h"

#include "[Libraries]\MemoryHandler\MemoryHandler.h"
using namespace SME;
using namespace SME::MemoryHandler;

extern IDebugLog		gLog;
extern PluginHandle		g_pluginHandle;

_DeclareMemHdlr(INICollectionLoadSetting, "removes the 0x100-char limit placed on string settings");

void StartPickingNit(void);

