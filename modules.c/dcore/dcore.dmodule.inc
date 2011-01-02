// See Copyright Notice in core\LICENSE.txt
new DCORE_HighestConnectedID;

forward DCORE_EverySecondTimer();
public DCORE_EverySecondTimer() {
	_EverySecond();
	if(DCORE_HighestConnectedID != -1){
		for(new i=0; i<=DCORE_HighestConnectedID; i++){
			if(IsPlayerConnected(i)) _ESPlayer(i);
		}
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
		for(new i=0; i<DCORE_HighestConnectedID; i++){
			if(IsPlayerConnected(i)) help_var = i;
		}
		DCORE_HighestConnectedID = help_var;
	}
	return 0;
}

dcore_GameModeInit() {
	DCORE_HighestConnectedID = -1;
	SetTimer("DCORE_EverySecondTimer",1000,1);
}