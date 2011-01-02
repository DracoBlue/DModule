#define MAPICONS_GTATMAXIMUM 1000
#define MAPICONS_MAX_SAMPICONS 32
new MAPICONS_SampIDInUse[MAX_PLAYERS][MAPICONS_MAX_SAMPICONS];
new MAPICONS_IconIDInUse[MAPICONS_GTATMAXIMUM];
new MAPICONS_MapIconModel[MAPICONS_GTATMAXIMUM];
new Float:MAPICONS_MapIconPos[MAPICONS_GTATMAXIMUM][3];
new MAPICONS_Quader[MAPICONS_GTATMAXIMUM];
new MAPICONS_SampID[MAX_PLAYERS][MAPICONS_GTATMAXIMUM];

new MAPICONS_PlayerQuader[MAX_PLAYERS];
#define MAPICONS_MAXQUADERS 400
new MAPICONS_QuadersArroundPlayer[MAX_PLAYERS][9];
new MAPICONS_IsLoading[MAX_PLAYERS];

new MAPICONS_Player[MAPICONS_GTATMAXIMUM];

mapicons_Disconnect(playerid,reason){
#pragma unused reason
	for(new i=0; i<MAPICONS_GTATMAXIMUM; i++){
	    if(MAPICONS_IconIDInUse[i] == 1){
	        if(MAPICONS_Player[i] == playerid){
	            MI_DeletePlayerMapIcon(playerid,i);
	        }
	    }
	}
	return 0;
}

mapicons_Connect(playerid){
    MAPICONS_PlayerQuader[playerid] = -1;
    for(new i=0; i<9; i++){
        MAPICONS_QuadersArroundPlayer[playerid][i] = -1;
    }
    for(new i=0; i<MAPICONS_MAX_SAMPICONS; i++){
        MAPICONS_SampIDInUse[playerid][i] = 0;
    }
    return 0;
}

#define _CD_mapicons_ESPlayer (((MAPICONS_IsLoading[playerid] == 0) && (Val(PlayerSpawned,playerid) == 1)) && ((MAPICONS_PlayerQuader[playerid] == -1) || (MAPICONS_PlayerQuader[playerid] != MAPICONS_GetQuaderArroundPos(PlayerPosition[playerid][0],PlayerPosition[playerid][1]))))
mapicons_ESPlayer(playerid){
    MAPICONS_IsLoading[playerid] = 1;
    MI_UpdateMapIconsArroundPlayer(playerid);
    MAPICONS_PlayerQuader[playerid] = MAPICONS_GetQuaderArroundPos(PlayerPosition[playerid][0],PlayerPosition[playerid][1]);
    MAPICONS_IsLoading[playerid] = 0;
}

MI_UpdateMapIconsArroundPlayer(playerid){
	new quaders[9];
	for(new i=0; i<9; i++) quaders[i] = -1;
	quaders[0] = MAPICONS_GetQuaderArroundPos(PlayerPosition[playerid][0],PlayerPosition[playerid][1]);
	if(((quaders[0]+1)%20) != 0) quaders[1] = quaders[0]+1; // nicht am rechten rand
	if(((quaders[0]+20)%20) != 0) quaders[2] = quaders[0]-1; // nicht am linken rand
	if((quaders[0]+20) < MAPICONS_MAXQUADERS){ // nicht am oberen rand
	    quaders[3] = quaders[0]+20;
	    if(quaders[1] != -1) quaders[4] = quaders[3]+1;
	    if(quaders[2] != -1) quaders[5] = quaders[3]-1;
	}
	if((quaders[0]-20) >= 0) { // nicht am unteren rand
	    quaders[6] = quaders[0]-20;
	    if(quaders[1] != -1) quaders[7] = quaders[6]+1;
	    if(quaders[2] != -1) quaders[8] = quaders[6]-1;
	}
	new dontreset[9];
	new dontload[9];
	for(new i=0; i<9; i++){
	    if(MAPICONS_QuadersArroundPlayer[playerid][i] != -1){
			for(new j=0; j<9; j++){
				if(MAPICONS_QuadersArroundPlayer[playerid][i] == quaders[j]){
				    dontreset[i] = 1;
				}
			}
			if(dontreset[i] == 0){
				MI_HideMIsForPlayerInQuader(playerid,MAPICONS_QuadersArroundPlayer[playerid][i]);
			}
		}
	} // alle ungebrauchten resetet, nun die neu laden, die noch gebraucht werden
	for(new i=0; i<9; i++){
	    if(quaders[i] != -1){
	        for(new j=0; j<9; j++){
	            if(quaders[i] == MAPICONS_QuadersArroundPlayer[playerid][j]){
					dontload[i] = 1;
	            }
	        }
		    if(dontload[i] == 0){
		        MI_ShowMIsForPlayerInQuader(playerid,quaders[i]);
		    }
	    }
	} // alle neuen geladen
	for(new i=0; i<9; i++) MAPICONS_QuadersArroundPlayer[playerid][i] = quaders[i];
}

MAPICONS_GetHighestUsedGTATID(){
	for(new i=0; i<MAPICONS_GTATMAXIMUM; i++){
	//for(new i=MAPICONS_GTATMAXIMUM; i>=0; i--){
	    if(MAPICONS_IconIDInUse[i] == 0) {
			return (i-1);
	    }
	}
	return (MAPICONS_GTATMAXIMUM-1);
}

MAPICONS_HidePlayerMapIcon(playerid,gtatid){
	if(IsPlayerConnected(playerid)) RemovePlayerMapIcon(playerid,MAPICONS_SampID[playerid][gtatid]);
    MAPICONS_SampIDInUse[playerid][MAPICONS_SampID[playerid][gtatid]] = 0;
	MAPICONS_SampID[playerid][gtatid] = -1;
}

MAPICONS_ShowPlayerMapIcon(playerid,gtatid){
    MAPICONS_SampID[playerid][gtatid] = MAPICONS_GetLowestFreeSAMPID(playerid);
//    printf("%d",MAPICONS_SampID[playerid][gtatid]);
    MAPICONS_SampIDInUse[playerid][MAPICONS_SampID[playerid][gtatid]] = 1;
	if(IsPlayerConnected(playerid)) SetPlayerMapIcon(playerid,MAPICONS_SampID[playerid][gtatid],MAPICONS_MapIconPos[gtatid][0],MAPICONS_MapIconPos[gtatid][1],MAPICONS_MapIconPos[gtatid][2],MAPICONS_MapIconModel[gtatid],0);
}

MAPICONS_GetLowestFreeSAMPID(playerid){
	for(new i=0; i<MAPICONS_MAX_SAMPICONS; i++){
	    if(MAPICONS_SampIDInUse[playerid][i] == 0) return i;
	}
	return (MAPICONS_MAX_SAMPICONS-1);
}

MI_HideMIsForPlayerInQuader(playerid,quader){
	new MI_max = MAPICONS_GetHighestUsedGTATID();
	for(new i=0; i<MI_max; i++){
	    if((MAPICONS_Quader[i] == quader) && ((MAPICONS_Player[i] == playerid) || (MAPICONS_Player[i] == -1))){
	        MAPICONS_HidePlayerMapIcon(playerid,i);
	    }
	}
	//remove all mapicons in that quader for that player
}

MI_ShowMIsForPlayerInQuader(playerid,quader){
	new MI_max = MAPICONS_GetHighestUsedGTATID();
	for(new i=0; i<(MI_max+1); i++){
	    if((MAPICONS_Quader[i] == quader) && ((MAPICONS_Player[i] == playerid) || (MAPICONS_Player[i] == -1))){
	        MAPICONS_ShowPlayerMapIcon(playerid,i);
	    }
	}
	//load all mapicons in that quader for that player
}

stock MI_DeletePlayerMapIcon(playerid,gtatid){
	if(IsPlayerConnected(playerid)) RemovePlayerMapIcon(playerid,MAPICONS_SampID[playerid][gtatid]);
    MAPICONS_SampIDInUse[playerid][MAPICONS_SampID[playerid][gtatid]] = 0;
	MAPICONS_SampID[playerid][gtatid] = -1;
	MAPICONS_IconIDInUse[gtatid] = 0;
}

stock DeleteMapIcon(gtatid){
	if(MAPICONS_Player[gtatid] != -1) MI_DeletePlayerMapIcon(MAPICONS_Player[gtatid],gtatid);
	if(MAPICONS_Player[gtatid] == -1){
	    for(new i=0; i<MAX_PLAYERS; i++){
	        MI_DeletePlayerMapIcon(i,gtatid);
	    }
	}
}

stock AddPlayerMapIcon(playerid,mapiconmodel,Float:x,Float:y,Float:z){
	new id = MAPICONS_GetLowestFreeGTATID();
	MAPICONS_Player[id] = playerid;
	MAPICONS_MapIconModel[id] = mapiconmodel;
	MAPICONS_MapIconPos[id][0] = x;
	MAPICONS_MapIconPos[id][1] = y;
	MAPICONS_MapIconPos[id][2] = z;
	MAPICONS_Quader[id] = MAPICONS_GetQuaderArroundPos(x,y);
	MAPICONS_IconIDInUse[id] = 1;
	MAPICONS_SampID[playerid][id] = -1;
	return id;
}

stock AddMapIcon(mapiconmodel,Float:x,Float:y,Float:z){
	new id = MAPICONS_GetLowestFreeGTATID();
	MAPICONS_Player[id] = -1;
	MAPICONS_MapIconModel[id] = mapiconmodel;
	MAPICONS_MapIconPos[id][0] = x;
	MAPICONS_MapIconPos[id][1] = y;
	MAPICONS_MapIconPos[id][2] = z;
	MAPICONS_Quader[id] = MAPICONS_GetQuaderArroundPos(x,y);
	MAPICONS_IconIDInUse[id] = 1;
	for(new i=0; i<MAX_PLAYERS; i++) MAPICONS_SampID[i][id] = -1;
	return id;
}

stock MAPICONS_GetLowestFreeGTATID(){
	for(new i=0; i<MAPICONS_GTATMAXIMUM; i++){
	    if(MAPICONS_IconIDInUse[i] == 0) return i;
	}
	return -1;
}

stock MAPICONS_GetQuaderArroundPos(Float:x,Float:y){
	new quaderid;
	if(x >= -3000 && x < -2700) quaderid = 0;
	if(x >= -2700 && x < -2400) quaderid = 1;
	if(x >= -2400 && x < -2100) quaderid = 2;
	if(x >= -2100 && x < -1800) quaderid = 3;
	if(x >= -1800 && x < -1500) quaderid = 4;
	if(x >= -1500 && x < -1200) quaderid = 5;
	if(x >= -1200 && x < -900) quaderid = 6;
	if(x >= -900 && x < -600) quaderid = 7;
	if(x >= -600 && x < 300) quaderid = 8;
	if(x >= -300 && x < 0) quaderid = 9;
	if(x >= 0 && x < 300) quaderid = 10;
	if(x >= 300 && x < 600) quaderid = 11;
	if(x >= 600 && x < 900) quaderid = 12;
	if(x >= 900 && x < 1200) quaderid = 13;
	if(x >= 1200 && x < 1500) quaderid = 14;
	if(x >= 1500 && x < 1800) quaderid = 15;
	if(x >= 1800 && x < 2100) quaderid = 16;
	if(x >= 2100 && x < 2400) quaderid = 17;
	if(x >= 2400 && x < 2700) quaderid = 18;
	if(x >= 2700 && x < 3000) quaderid = 19;

	if(y >= -3000 && y < -2700) quaderid+=0;
	if(y >= -2700 && y < -2400) quaderid+=20;
	if(y >= -2400 && y < -2100) quaderid+=40;
	if(y >= -2100 && y < -1800) quaderid+=60;
	if(y >= -1800 && y < -1500) quaderid+=80;
	if(y >= -1500 && y < -1200) quaderid+=100;
	if(y >= -1200 && y < -900) quaderid+=120;
	if(y >= -900 && y < -600) quaderid+=140;
	if(y >= -600 && y < 300) quaderid+=160;
	if(y >= -300 && y < 0) quaderid+=180;
	if(y >= 0 && y < 300) quaderid+=200;
	if(y >= 300 && y < 600) quaderid+=220;
	if(y >= 600 && y < 900) quaderid+=240;
	if(y >= 900 && y < 1200) quaderid+=260;
	if(y >= 1200 && y < 1500) quaderid+=280;
	if(y >= 1500 && y < 1800) quaderid+=300;
	if(y >= 1800 && y < 2100) quaderid+=320;
	if(y >= 2100 && y < 2400) quaderid+=340;
	if(y >= 2400 && y < 2700) quaderid+=360;
	if(y >= 2700 && y < 3000) quaderid+=380;

	return quaderid;
}
