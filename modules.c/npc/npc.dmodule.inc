#define PLAYER_RECORDING_TYPE_NONE		0
#define PLAYER_RECORDING_TYPE_DRIVER	1
#define PLAYER_RECORDING_TYPE_ONFOOT	2

native StartRecordingPlayback(playbacktype, recordname[]);
native StopRecordingPlayback();
native PauseRecordingPlayback();
native ResumeRecordingPlayback();

native StartRecordingPlayerData(playerid, recordtype, recordname[]);
native StopRecordingPlayerData(playerid);

native ConnectNPC(name[], script[]);
native IsPlayerNPC(playerid);

forward OnNPCModeInit();
forward OnNPCModeExit();
forward OnNPCConnect(myplayerid);
forward OnNPCDisconnect(reason[]);
forward OnNPCSpawn();
forward OnNPCEnterVehicle(vehicleid, seatid);
forward OnNPCExitVehicle();