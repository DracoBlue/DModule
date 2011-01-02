#define DIMENSION_FOREVERYPLAYER 155
#define DIMENSION_FORGANGHOUSES 355
#define DIMENSION_FORGANGWARS 555

#define DIMENSION_WORLD 0
#define DIMENSION_VFIGHT 1
#define DIMENSION_RACE 2
#define DIMENSION_FIGHT 4
#define DIMENSION_LOUNGE 5
#define DIMENSION_FREEZE 6
#define DIMENSION_BATTLE 11
#define DIMENSION_EVENT 12
#define DIMENSION_TOURNAMENT1 13
#define DIMENSION_TOURNAMENT2 14
#define DIMENSION_TOURNAMENT3 15
#define DIMENSION_TOURNAMENT4 16
#define DIMENSION_TOURNAMENT5 17
#define DIMENSION_TOURNAMENT6 18
#define DIMENSION_TOURNAMENT7 19

// Virtual Worlds
native SetVehicleVirtualWorld(vehicleid, worldid);
native GetVehicleVirtualWorld(vehicleid);

// Virtual Worlds
native SAMP_SetPlayerVirtualWorld(playerid, worldid) = SetPlayerVirtualWorld;
native GetPlayerVirtualWorld(playerid);

stock SetPlayerVirtualWorld(playerid,newworldid){
	new oldworldid = GetPlayerVirtualWorld(playerid);
	SAMP_SetPlayerVirtualWorld(playerid,newworldid);
	_VirtualWorldChange(playerid,newworldid,oldworldid);
}

stock PrivatePlayerWorld(playerid){
	return (DIMENSION_FOREVERYPLAYER+playerid);
}