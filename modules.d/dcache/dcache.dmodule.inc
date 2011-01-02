new Float:PlayerPosition[MAX_PLAYERS][3];

dcache_EverySecond() {
	for (new i=0; i<MAX_PLAYERS;i++) if (IsPlayerConnected(i)) {
		GetPlayerPos(i, PlayerPosition[i][0], PlayerPosition[i][1], PlayerPosition[i][2]);
	}
}

