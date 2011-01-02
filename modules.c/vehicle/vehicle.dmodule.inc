/*  SA:MP Vehicle Functions
 *
 *  (c) Copyright 2005-2006, SA:MP Team
 *
 */

#pragma library vehicles

#define INVALID_VEHICLE_ID						(0xFFFF)
#define MAX_VEHICLES							(900)

new Float:VEHICLE_SpawnPos[MAX_VEHICLES][3];

native SAMP_AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2) = AddStaticVehicle;
native AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay);
native CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay);
native DestroyVehicle(vehicleid);
native GetVehiclePos(vehicleid, &Float:x, &Float:y, &Float:z);
native SetVehiclePos(vehicleid, Float:x, Float:y, Float:z);
native GetVehicleZAngle(vehicleid, &Float:z_angle);
native SetVehicleZAngle(vehicleid, Float:z_angle);
native SetVehicleParamsForPlayer(vehicleid,playerid,objective,doorslocked);
native SetVehicleToRespawn(vehicleid);
native LinkVehicleToInterior(vehicleid, interiorid);
native AddVehicleComponent(vehicleid, componentid);
native RemoveVehicleComponent(vehicleid, componentid);
native GetVehicleComponentType(componentid);
native GetVehicleComponentInSlot(vehicleid,slot);
native ChangeVehicleColor(vehicleid, color1, color2);
native ChangeVehiclePaintjob(vehicleid, paintjobid);
native SetVehicleHealth(vehicleid, Float:health);
native GetVehicleHealth(vehicleid, &Float:health);
stock  Float:VehicleHealth(vehicleid){
	new Float:health;
	GetVehicleHealth(vehicleid,health);
	return health;
}
native AttachTrailerToVehicle(trailerid, vehicleid);
native DetachTrailerFromVehicle(vehicleid);
native IsTrailerAttachedToVehicle(vehicleid);
native GetVehicleTrailer(vehicleid);
native SetVehicleNumberPlate(vehicleid, numberplate[]);
native GetVehicleModel(vehicleid);

native PutPlayerInVehicle(playerid, vehicleid, seatid);
native GetPlayerVehicleID(playerid);
native RemovePlayerFromVehicle(playerid);
native IsPlayerInVehicle(playerid, vehicleid);
native IsPlayerInAnyVehicle(playerid);

// Spectating
native PlayerSpectateVehicle(playerid, targetvehicleid, mode = SPECTATE_MODE_NORMAL);

forward OnVehicleSpawn(vehicleid);
public OnVehicleSpawn(vehicleid) _VehicleSpawn(vehicleid);
forward OnVehicleDeath(vehicleid, killerid);
public OnVehicleDeath(vehicleid, killerid) _VehicleDeath(vehicleid, killerid);
forward OnVehicleMod(vehicleid, componentid);
public OnVehicleMod(vehicleid, componentid) _VehicleMod(vehicleid, componentid);
forward OnVehiclePaintjob(vehicleid, paintjobid);
public OnVehiclePaintjob(vehicleid, paintjobid) _VehiclePaintjob(vehicleid, paintjobid);
forward OnVehicleRespray(vehicleid, color1, color2);
public OnVehicleRespray(vehicleid, color1, color2) _VehicleRespray(vehicleid, color1, color2);
forward OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) _EnterVehicle(playerid, vehicleid, ispassenger);
forward OnPlayerExitVehicle(playerid, vehicleid);
public OnPlayerExitVehicle(playerid, vehicleid) _ExitVehicle(playerid, vehicleid);
forward OnEnterExitModShop(playerid,enterexit,interiorid);
public OnEnterExitModShop(playerid,enterexit,interiorid) _EnterExitModShop(playerid,enterexit,interiorid);

AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2){
	new vehicleid = SAMP_AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, Float:z_angle, color1, color2);
	VEHICLE_SpawnPos[vehicleid][0] = spawn_x;
	VEHICLE_SpawnPos[vehicleid][1] = spawn_y;
	VEHICLE_SpawnPos[vehicleid][2] = spawn_z;
	return vehicleid;
}

native RepairVehicle(vehicleid);
native GetVehicleVelocity(vehicleid, &Float:X, &Float:Y, &Float:Z);
native SetVehicleVelocity(vehicleid, Float:X, Float:Y, Float:Z);

native GetVehicleDamageStatus(vehicleid, &panels, &doors, &lights, &tires);
native UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

stock encode_tires(tire1, tire2, tire3, tire4) {	// written by Kye
	return tire1 | (tire2 << 1) | (tire3 << 2) | (tire4 << 3);
}

stock SetVehicleTires(vehicleid,tire1,tire2,tire3,tire4){
	new panels, doors, lights, tires;
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
	tires = encode_tires(tire1,tire2,tire3,tire4);
	UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
}
