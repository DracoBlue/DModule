new Float:PlayerPosition[MAX_PLAYERS][3];

dcache_EverySecond() {
    if(DCORE_HighestConnectedID != -1){
        for(new i=0; i<=DCORE_HighestConnectedID; i++){
            if(IsPlayerConnected(i)) GetPlayerPos(i, PlayerPosition[i][0], PlayerPosition[i][1], PlayerPosition[i][2]);
        }
    }
}