/*  SA:MP Object Functions
 *
 *  (c) Copyright 2005-2010, SA-MP Team
 *
 */

#pragma library objects

#define MAX_OBJECTS								(400)
#define INVALID_OBJECT_ID						(0xFFFF)

forward OnPlayerObjectMoved(playerid, objectid);
public OnPlayerObjectMoved(playerid, objectid) _PlayerObjectMoved(playerid, objectid);
forward OnObjectMoved(objectid);
public OnObjectMoved(objectid) _ObjectMoved(objectid);

native CreateObject(modelid, Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ, Float:DrawDistance = 0.0);
native AttachObjectToVehicle(objectid, vehicleid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:RotZ);
native SetObjectPos(objectid, Float:X, Float:Y, Float:Z);
native GetObjectPos(objectid, &Float:X, &Float:Y, &Float:Z);
native SetObjectRot(objectid, Float:RotX, Float:RotY, Float:RotZ);
native GetObjectRot(objectid, &Float:RotX, &Float:RotY, &Float:RotZ);
native IsValidObject(objectid);
native DestroyObject(objectid);
native MoveObject(objectid, Float:X, Float:Y, Float:Z, Float:Speed);
native StopObject(objectid);
native CreatePlayerObject(playerid, modelid, Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ, Float:DrawDistance = 0.0);
native SetPlayerObjectPos(playerid, objectid, Float:X, Float:Y, Float:Z);
native GetPlayerObjectPos(playerid, objectid, &Float:X, &Float:Y, &Float:Z);
native SetPlayerObjectRot(playerid, objectid, Float:RotX, Float:RotY, Float:RotZ);
native GetPlayerObjectRot(playerid, objectid, &Float:RotX, &Float:RotY, &Float:RotZ);
native IsValidPlayerObject(playerid, objectid);
native DestroyPlayerObject(playerid, objectid);
native MovePlayerObject(playerid, objectid, Float:X, Float:Y, Float:Z, Float:Speed);
native StopPlayerObject(playerid, objectid);
native AttachObjectToPlayer(objectid, playerid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ);
native AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ);

#define MAX_PLAYER_ATTACHED_OBJECTS 5 // This is the number of attached indexes available ie 5 = 0-4

native SetPlayerAttachedObject(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0);
native RemovePlayerAttachedObject(playerid, index);
native IsPlayerAttachedObjectSlotUsed(playerid, index);
