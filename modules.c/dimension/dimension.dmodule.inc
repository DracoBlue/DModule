#define DIMENSION_FOREVERYPLAYER 155

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