// See Copyright Notice in core\LICENSE.txt
new DCORE_HighestConnectedID;

#define forEachConnectedPlayer(%1) if(DCORE_HighestConnectedID != -1) for(new %1=0; %1<=DCORE_HighestConnectedID; %1++) if(IsPlayerConnected(%1))

forward DCORE_EverySecondTimer();
public DCORE_EverySecondTimer() {
	_EverySecond();
	forEachConnectedPlayer(i) {
		_ESPlayer(i);
	}
}

dcore_Connect(playerid){
	if(playerid > DCORE_HighestConnectedID){
		DCORE_HighestConnectedID = playerid;
	}
	return 0;
}

dcore_Disconnect(playerid,reason){
	#pragma unused reason
	if(DCORE_HighestConnectedID == playerid){
		new help_var = -1;
		forEachConnectedPlayer(i) {
			if (playerid != i) {
				help_var = i;
			}
		}
		DCORE_HighestConnectedID = help_var;
	}
	return 0;
}

dcore_GameModeInit() {
	DCORE_HighestConnectedID = -1;
	SetTimer("DCORE_EverySecondTimer",1000,1);
}

