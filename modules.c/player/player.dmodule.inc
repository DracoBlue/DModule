/*  SA:MP Player Functions
 *
 *  (c) Copyright 2005-2006, SA:MP Team
 *
 */

#pragma library players

new PLAYER_class_for_player[MAX_PLAYERS];
new PLAYER_current_team[MAX_PLAYERS];

#define CLICK_SOURCE_SCOREBOARD     0

forward OnPlayerClickPlayer(playerid, clickedplayerid, source);
public OnPlayerClickPlayer(playerid, clickedplayerid, source){
	if(_ClickPlayer(playerid, clickedplayerid, source)){
		return 0;
	}
	return 1;
}

forward OnPlayerConnect(playerid);
public OnPlayerConnect(playerid) {
	_Connect(playerid);
	_Connected(playerid);
}

forward OnPlayerDisconnect(playerid, reason);
public OnPlayerDisconnect(playerid, reason) {
	_Disconnect(playerid, reason);
	_Disconnected(playerid, reason);
}

forward OnPlayerRequestClass(playerid, classid);
public OnPlayerRequestClass(playerid, classid) {
	PLAYER_class_for_player[playerid]=classid;
	SetPlayerTeam(playerid,SAMPCORE_team_for_class[PLAYER_class_for_player[playerid]]);
	if (_RequestClass(playerid, classid)) {
		return 0;
	}
	return 1;
}
forward OnPlayerRequestSpawn(playerid);
public OnPlayerRequestSpawn(playerid) {
	if (_RequestSpawn(playerid)) {
		return 0;
	}
	return 1;
}

forward OnPlayerSpawn(playerid);
public OnPlayerSpawn(playerid) {
	SetPlayerTeam(playerid,PLAYER_current_team[playerid]);
	if (_Spawn(playerid)) {
		return 0;
	}
	return 1;
}

forward OnPlayerDeath(playerid, killerid, reason);
public OnPlayerDeath(playerid, killerid, reason) _Death(playerid, killerid, reason);

// FIXME: still working?
// forward OnPlayerPrivmsg(playerid, recieverid, text[]);
// public OnPlayerPrivmsg(playerid, recieverid, text[]) {
//	if (_Privmsg(playerid, recieverid, text)) {
//		return 0;
//	}
//	return 1;
//}

forward OnPlayerText(playerid, text[]);
public OnPlayerText(playerid, text[]) {
	if (_Text(playerid, text)) {
		return 0;
	}
	return 1;
}
forward OnPlayerCommandText(playerid, cmdtext[]);
public OnPlayerCommandText(playerid, cmdtext[]) {
	if (_CommandText(playerid, cmdtext)) {
		return 1;
	}
	return 0;
}


forward OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid) _InteriorChange(playerid, newinteriorid, oldinteriorid);
// FIXME: still working?
// forward OnPlayerInfoChange(playerid);
// public OnPlayerInfoChange(playerid) _InfoChange(playerid);
forward OnPlayerStateChange(playerid, newstate, oldstate);
public OnPlayerStateChange(playerid, newstate, oldstate) _StateChange(playerid, newstate, oldstate);

// Checkpoints
forward OnPlayerEnterCheckpoint(playerid);
public OnPlayerEnterCheckpoint(playerid) _EnterCP(playerid);
forward OnPlayerLeaveCheckpoint(playerid);
public OnPlayerLeaveCheckpoint(playerid) _LeaveCP(playerid);
forward OnPlayerEnterRaceCheckpoint(playerid);
public OnPlayerEnterRaceCheckpoint(playerid) _EnterRaceCP(playerid);
forward OnPlayerLeaveRaceCheckpoint(playerid);
public OnPlayerLeaveRaceCheckpoint(playerid) _LeaveRaceCP(playerid);

forward OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	_KeysChange(playerid, newkeys, oldkeys);
	if(newkeys & KEY_ANALOG_RIGHT) _Hotkey1Pressed(playerid);
	if(newkeys & KEY_ANALOG_LEFT) _Hotkey2Pressed(playerid);
}
// FIXME: still working?
// forward OnPlayerTeamPrivmsg(playerid, teamid, text[]);
// public OnPlayerTeamPrivmsg(playerid, teamid, text[]) _TeamPrivmsg(playerid, teamid, text);

forward OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
forward OnPlayerUpdate(playerid);
forward OnPlayerStreamIn(playerid, forplayerid);
forward OnPlayerStreamOut(playerid, forplayerid);

#pragma library players

#define SPECIAL_ACTION_NONE             0
#define SPECIAL_ACTION_DUCK             1
#define SPECIAL_ACTION_USEJETPACK       2
#define SPECIAL_ACTION_ENTER_VEHICLE    3
#define SPECIAL_ACTION_EXIT_VEHICLE     4
#define SPECIAL_ACTION_DANCE1           5
#define SPECIAL_ACTION_DANCE2           6
#define SPECIAL_ACTION_DANCE3           7
#define SPECIAL_ACTION_DANCE4           8
#define SPECIAL_ACTION_HANDSUP          10
#define SPECIAL_ACTION_USECELLPHONE     11
#define SPECIAL_ACTION_SITTING          12
#define SPECIAL_ACTION_STOPUSECELLPHONE 13
#define SPECIAL_ACTION_DRINK_BEER       20
#define SPECIAL_ACTION_SMOKE_CIGGY      21
#define SPECIAL_ACTION_DRINK_WINE       22
#define SPECIAL_ACTION_DRINK_SPRUNK     23

native SendClientMessage(playerid, color, const message[]);
native SendPlayerMessageToPlayer(playerid, senderid, const message[]);
native SendPlayerMessageToAll(senderid, const message[]);
native SendDeathMessage(killer,killee,weapon);
native GameTextForPlayer(playerid,const string[],time,style);

#define MAX_CHATBUBBLE_LENGTH 144
native SetPlayerChatBubble(playerid, text[], color, Float:drawdistance, expiretime);

// Admin
native IsPlayerAdmin(playerid);
native Kick(playerid);
native Ban(playerid);
native BanEx(playerid, const reason[]);

// Player conditionals
native IsPlayerConnected(playerid);
native IsPlayerInCheckpoint(playerid);
native IsPlayerInRaceCheckpoint(playerid);
native IsPlayerStreamedIn(playerid, forplayerid);

native SetPlayerFightingStyle(playerid, style);
native GetPlayerFightingStyle(playerid);
native PlayCrimeReportForPlayer(playerid, suspectid, crime);
native SetPlayerShopName(playerid, shopname[]);


new SPAWNINFO_Info[MAX_PLAYERS][8]; // 0 = team, 1 = skin, 2 = w1, 3 = w1_ammo, 4 = w2, 5 = w2_ammo, 6 = w3, 7 = w3_ammo
new Float:SPAWNINFO_Pos[MAX_PLAYERS][4];
// Player
native SAMP_SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)=SetSpawnInfo;
stock SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo) {
	PLAYER_current_team[playerid] = team;
	SPAWNINFO_Info[playerid][0] = team;
	SPAWNINFO_Info[playerid][1] = skin;
	SPAWNINFO_Pos[playerid][0] = x;
	SPAWNINFO_Pos[playerid][1] = y;
	SPAWNINFO_Pos[playerid][2] = z;
	SPAWNINFO_Pos[playerid][3] = rotation;
	SPAWNINFO_Info[playerid][2] = weapon1;
	SPAWNINFO_Info[playerid][3] = weapon1_ammo;
	SPAWNINFO_Info[playerid][4] = weapon2;
	SPAWNINFO_Info[playerid][5] = weapon2_ammo;
	SPAWNINFO_Info[playerid][6] = weapon3;
	SPAWNINFO_Info[playerid][7] = weapon3_ammo;
	debug("SetSpawnInfo %d: %f,%f,%f",playerid,x,y,z);
	SAMP_SetSpawnInfo(playerid, team, skin, x, y, z, rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
}
stock SetSpawnPos(playerid,Float:x,Float:y,Float:z,Float:rotation){
	SetSpawnInfo(playerid,SPAWNINFO_Info[playerid][0],SPAWNINFO_Info[playerid][1],x,y,z,rotation,SPAWNINFO_Info[playerid][2],SPAWNINFO_Info[playerid][3],SPAWNINFO_Info[playerid][4],SPAWNINFO_Info[playerid][5],SPAWNINFO_Info[playerid][6],SPAWNINFO_Info[playerid][7]);
}

native SAMP_SpawnPlayer(playerid)=SpawnPlayer;
stock SpawnPlayer(playerid) {
	debug("SpawnPlayer %d",playerid);
	SAMP_SpawnPlayer(playerid);
}

// Player info
native SetPlayerPos(playerid, Float:x, Float:y, Float:z);
native SetPlayerPosFindZ(playerid, Float:x, Float:y, Float:z);
native GetPlayerPos(playerid, &Float:x, &Float:y, &Float:z);
native SetPlayerFacingAngle(playerid,Float:ang);
native GetPlayerFacingAngle(playerid,&Float:ang);
native SetPlayerInterior(playerid,interiorid);
native GetPlayerInterior(playerid);
native IsPlayerInRangeOfPoint(playerid, Float:range, Float:x, Float:y, Float:z);

native SAMP_SetPlayerHealth(playerid, Float:health)=SetPlayerHealth;
stock SetPlayerHealth(playerid,Float:health) {
	SetVal(PlayerHealth,playerid,floatround(health));
}
native SAMP_GetPlayerHealth(playerid, &Float:health)=GetPlayerHealth;
stock GetPlayerHealth(playerid, &Float:health) {
	SAMP_GetPlayerHealth(playerid,health);
}

player_ReadPlayerHealth(playerid) {
	new Float:health;
	SAMP_GetPlayerHealth(playerid,health);
	A_PlayerHealth[playerid]=floatround(health);
	return 1;
}

player_WritePlayerHealth(playerid,newval) {
	SAMP_SetPlayerHealth(playerid,float(newval));
	return 0;
}

native SetPlayerArmour(playerid, Float:armour);
native GetPlayerArmour(playerid, &Float:armour);

stock PlayerArmour(playerid){
	new Float:armor;
	GetPlayerArmour(playerid,armor);
	return floatround(armor);
}

native SAMP_SetPlayerTeam(playerid, teamid)=SetPlayerTeam;
stock SetPlayerTeam(playerid, teamid) {
	PLAYER_current_team[playerid]=teamid;
	for (new p;p<MAX_PLAYERS;p++) if (IsPlayerConnected(p)) {
		SAMP_SetPlayerTeam(p,PLAYER_current_team[p]);
	}
}

native GetPlayerTeam(playerid);

native SetPlayerScore(playerid,score);
native GetPlayerScore(playerid);
native SetPlayerColor(playerid,color);
native GetPlayerColor(playerid);
native SetPlayerSkin(playerid, skinid);
native GetPlayerSkin(playerid);
native SetPlayerName(playerid, const name[]);
native GetPlayerState(playerid);
native GetPlayerIp(playerid, name[], len);
native GetPlayerPing(playerid);
native GetPlayerKeys(playerid, &keys, &updown, &leftright);
native GetPlayerName(playerid, const name[], len);
native SetPlayerTime(playerid, hour, minute);
native GetPlayerTime(playerid, &hour, &minute);
native TogglePlayerClock(playerid, toggle);
native SetPlayerWeather(playerid, weather);
native ForceClassSelection(playerid);
native SetPlayerWantedLevel(playerid, level);
native GetPlayerWantedLevel(playerid);

// Player controls
native TogglePlayerControllable(playerid, toggle);
native PlayerPlaySound(playerid, soundid, Float:x, Float:y, Float:z);
native ApplyAnimation(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync = 0);
native ClearAnimations(playerid, forcesync = 0);
native GetPlayerSpecialAction(playerid);
native SetPlayerSpecialAction(playerid,actionid);
native GetPlayerAnimationIndex(playerid); // return the index of any running applied animations (0 if none are running)
native GetAnimationName(index, animlib[], len1, animname[], len2); // get the animation lib/name for the index

#define PLAYER_MARKERS_MODE_OFF         (0)
#define PLAYER_MARKERS_MODE_GLOBAL      (1)
#define PLAYER_MARKERS_MODE_STREAMED    (2)

// Player map commands
native LimitPlayerMarkerRadius(Float:marker_radius);
native SetPlayerCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size);
native DisablePlayerCheckpoint(playerid);
native SetPlayerRaceCheckpoint(playerid, type, Float:x, Float:y, Float:z, Float:nextx, Float:nexty, Float:nextz, Float:size);
native DisablePlayerRaceCheckpoint(playerid);
native SetPlayerWorldBounds(playerid,Float:x_max,Float:x_min,Float:y_max,Float:y_min);
native SetPlayerMarkerForPlayer(playerid, showplayerid, color);
native ShowPlayerNameTagForPlayer(playerid, showplayerid, show);

// Player camera
native SetPlayerCameraPos(playerid,Float:x, Float:y, Float:z);
native SetPlayerCameraLookAt(playerid, Float:x, Float:y, Float:z);
native SetCameraBehindPlayer(playerid);
native GetPlayerCameraPos(playerid, &Float:x, &Float:y, &Float:z);
native GetPlayerCameraFrontVector(playerid, &Float:x, &Float:y, &Float:z);
native AllowPlayerTeleport(playerid, allow);

// Insane Stunts
native EnableStuntBonusForPlayer(playerid, enable);

native TogglePlayerSpectating(playerid, toggle);
native PlayerSpectatePlayer(playerid, targetplayerid, mode = SPECTATE_MODE_NORMAL);
native PlayerSpectateVehicle(playerid, targetvehicleid, mode = SPECTATE_MODE_NORMAL);

// Alcohol
native SetPlayerDrunkLevel(playerid,drunkenlevel);
native GetPlayerDrunkLevel(playerid);

#define WEAPONSKILL_PISTOL 				0
#define WEAPONSKILL_PISTOL_SILENCED		1
#define WEAPONSKILL_DESERT_EAGLE		2
#define WEAPONSKILL_SHOTGUN				3
#define WEAPONSKILL_SAWNOFF_SHOTGUN		4
#define WEAPONSKILL_SPAS12_SHOTGUN		5
#define WEAPONSKILL_MICRO_UZI			6
#define WEAPONSKILL_MP5					7
#define WEAPONSKILL_AK47				8
#define WEAPONSKILL_M4					9
#define WEAPONSKILL_SNIPERRIFLE			10
native SetPlayerSkillLevel(playerid, skill, level);

native GetPlayerVelocity(playerid, &Float:x, &Float:y, &Float:z);
native SetPlayerVelocity(playerid, Float:x, Float:y, Float:z);

// Recording for NPC playback
#define PLAYER_RECORDING_TYPE_NONE      0
#define PLAYER_RECORDING_TYPE_DRIVER    1
#define PLAYER_RECORDING_TYPE_ONFOOT    2

native StartRecordingPlayerData(playerid, recordtype, recordname[]);
native StopRecordingPlayerData(playerid);

// NPC
native ConnectNPC(name[], script[]);
native IsPlayerNPC(playerid);