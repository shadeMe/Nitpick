#pragma once

#include "skse/PluginAPI.h"
#include "skse/skse_version.h"
#include "skse/GameTypes.h"
#include "skse/GameForms.h"

#include "[Libraries]\SME Sundries\MemoryHandler.h"

#include <fstream>
#include <string>
#include <list>

using namespace SME;
using namespace SME::MemoryHandler;

extern IDebugLog		gLog;
extern PluginHandle		g_pluginHandle;

// 04
class hkBaseObject
{
public:
	// members
	///*00*/ void**					vtbl;

	// methods
	virtual void*					Dtor(bool FreeMemory = true) = 0;
};
STATIC_ASSERT(sizeof(hkBaseObject) == 0x04);

// 10 - always 16-byte aligned
struct hkVector4
{
	float	x;
	float	y;
	float	z;
	float	w;
};

class bhkWorld;
class bhkWorldM;
class hkpCollisionFilter;
class hkpConvexListFilter;
class hkWorldMemoryAvailableWatchDog;

// EC
class hkpWorldCinfo : public hkBaseObject
{
public:
	// members									default values (0 if not mentioned)
	///*00*/ hkBaseObject
	/*04*/ UInt16					pad04;
	/*06*/ UInt8					unk06;		// 1
	/*07*/ UInt8					pad07;
	/*08*/ UInt32					pad08;
	/*0C*/ hkVector4				gravity;	// z = -9.8
	/*1C*/ UInt32					pad1C;
	/*20*/ UInt32					broadPhaseQuerySize;		// 1024
	/*24*/ float					contactRestingVelocity;		// 1.0		(OB = 0.2, bhkWorldCinfo::ctor = 3.40282e38)
	/*28*/ UInt8					broadPhaseBorderBehaviour;		// 0		(bhkWorldCinfo::ctor = 3)
	/*29*/ UInt8					mtPostponeAndSortBroadPhaseBorderCallbacks;
	/*2A*/ UInt16					pad29;
	/*2C*/ UInt32					pad2C;
	/*30*/ hkVector4				broadPhaseWorldAabb_Min;		// -500.0, -500.0, -500.0
	/*40*/ hkVector4				broadPhaseWorldAabb_Max;		// 500.0, 500.0, 500.0
	/*50*/ UInt8					useKdTree;
	/*51*/ UInt8					useMultipleTree;
	/*52*/ UInt8					treeUpdateType;
	/*53*/ UInt8					autoUpdateKdTree;		// 1
	/*54*/ float					collisionTolerance;		// 0.1
	/*58*/ hkpCollisionFilter*		collisionFilter;
	/*5C*/ hkpConvexListFilter*		convexListFilter;
	/*60*/ float					expectedMaxLinearVelocity;		// 200.0
	/*64*/ UInt32					sizeOfToiEventQueue;		// 250
	/*68*/ float					expectedMinPsiDeltaTime;	// 0.033	(bhkWorldCinfo::ctor = fMaxTime:HAVOK)
	/*6C*/ hkWorldMemoryAvailableWatchDog*		memoryWatchDog;
	/*70*/ float					unk70;
	/*74*/ UInt8					contactPointGeneration;		// 2
	/*75*/ UInt8					allowToSkipConfirmedCallbacks;
	/*76*/ UInt8					useHybridBroadphase;
	/*77*/ UInt8					pad77;
	/*78*/ float					solverTau;		// 0.6
	/*7C*/ float					solverDamp;		// 1.0
	/*80*/ UInt32					solverIterations;		// 4
	/*84*/ UInt32					solverMicrosteps;		// 1
	/*88*/ float					maxConstraintViolation;		// 3.40282e38	(bhkWorldCinfo::ctor = 0.5)
	/*8C*/ UInt8					forceCoherentConstraintOrderingInSolver;
	/*8D*/ UInt8					pad8D[3];
	/*90*/ float					snapCollisionToConvexEdgeThreshold;		// 0.524
	/*94*/ float					snapCollisionToConcaveEdgeThreshold;		// 0.698
	/*98*/ UInt8					enableToiWeldRejection;
	/*99*/ UInt8					enableDeprecatedWelding;
	/*9A*/ UInt16					pad9A;
	/*9C*/ float					iterativeLinearCastEarlyOutDistance;		// 0.0099
	/*A0*/ UInt32					iterativeLinearCastMaxIterations;		// 20
	/*A4*/ UInt8					deactivationNumInactiveFramesSelectFlag0;
	/*A5*/ UInt8					deactivationNumInactiveFramesSelectFlag1;
	/*A6*/ UInt8					deactivationIntegrateCounter;
	/*A7*/ UInt8					shouldActivateOnRigidBodyTransformChange;		// 1		(bhkWorldCinfo::ctor = 0)
	/*A8*/ float					deactivationReferenceDistance;		// 0.02		(OB = 0.1?)
	/*AC*/ float					toiCollisionResponseRotateNormal;		// 0.2
	/*B0*/ UInt32					maxSectorsPerMidphaseCollideTask;		// 2		(OB = 1?)
	/*B4*/ UInt32					maxSectorsPerNarrowphaseCollideTask;		// 4
	/*B8*/ UInt8					processToisMultithreaded;		// 1
	/*B9*/ UInt8					padB9[3];
	/*BC*/ UInt32					maxEntriesPerToiMidphaseCollideTask;		// 4
	/*C0*/ UInt32					maxEntriesPerToiNarrowphaseCollideTask;		// 12
	/*C4*/ UInt32					maxNumToiCollisionPairsSinglethreaded;
	/*C8*/ float					numToisTillAllowedPenetrationSimplifiedToi;		// 3.0
	/*CC*/ float					numToisTillAllowedPenetrationToi;		// 3.0
	/*D0*/ float					numToisTillAllowedPenetrationToiHigher;		// 4.0
	/*D4*/ float					numToisTillAllowedPenetrationToiForced;		// 20.0
	/*D8*/ UInt8					enableDeactivation;		// 1
	/*D9*/ UInt8					simulationType;		// 2		(bhkWorldCinfo::ctor = iSimType:HAVOK)
	/*DA*/ UInt16					padDA;
	/*DC*/ UInt32					minDesiredIslandSize;		// 64
	/*E0*/ UInt8					processActionsInSingleThread;		// 1
	/*E1*/ UInt8					allowIntegrationOfIslandsWithoutConstraintsInASeparateJob;
	/*E2*/ UInt16					padE2;
	/*E4*/ float					frameMarkerPsiSnap;		// 0.0000999
	/*E8*/ UInt8					fireCollisionCallbacks;
	/*E9*/ UInt8					padE9[3];
	//*EC*/ UInt32					unkEC[(0xFC - 0xEC) >> 2];

	// methods
	virtual void*					Dtor(bool FreeMemory = true) = 0;
};
STATIC_ASSERT(sizeof(hkpWorldCinfo) == 0xEC);

struct LODSettings
{
	UInt8 valid;
	UInt8 pad01;
	signed __int16 left;
	signed __int16 bottom;
	signed __int16 pad06;
	int stride_pow2;
	int lowestsize_lod;
	int highestsize_lod;
};

void StartPickingNit(void);

_DeclareMemHdlr(INICollectionLoadSetting, "removes the 0x100-char limit placed on string settings");
_DeclareMemHdlr(TESDataHandlerPopulatePluginList, "fixes the plugin list init code to skip inactive files");