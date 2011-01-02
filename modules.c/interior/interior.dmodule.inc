new INTERIOR_PlayerGetsWeaponsBack[MAX_PLAYERS];
new INTERIOR_PlayerWeapons[MAX_PLAYERS][13];
new INTERIOR_PlayerAmmo[MAX_PLAYERS][13];
new INTERIOR_WeaponInHand[MAX_PLAYERS];

new INTERIOR_WeaponsForbidden[50];

interior_InteriorChange(playerid, newinteriorid, oldinteriorid){
	if(INTERIOR_WeaponsForbidden[newinteriorid] == 1 && INTERIOR_WeaponsForbidden[oldinteriorid] == 0){
		for(new i=0; i<12; i++){
			GetPlayerWeaponData(playerid,i,INTERIOR_PlayerWeapons[playerid][i],INTERIOR_PlayerAmmo[playerid][i]);
			if(INTERIOR_PlayerAmmo[playerid][i] > 50000) INTERIOR_PlayerAmmo[playerid][i] = 0;
		}
		INTERIOR_WeaponInHand[playerid] = GetPlayerWeapon(playerid);
		INTERIOR_PlayerGetsWeaponsBack[playerid] = 1;
		ResetPlayerWeapons(playerid);
	}
	else if(INTERIOR_WeaponsForbidden[oldinteriorid] == 1 && INTERIOR_WeaponsForbidden[newinteriorid] == 0){
		if(INTERIOR_PlayerGetsWeaponsBack[playerid] == 1){
			for(new i=0; i<12; i++){
				GivePlayerWeapon(playerid,INTERIOR_PlayerWeapons[playerid][i],INTERIOR_PlayerAmmo[playerid][i]);
			}
			SetPlayerArmedWeapon(playerid,INTERIOR_WeaponInHand[playerid]);
			INTERIOR_PlayerGetsWeaponsBack[playerid] = 0;
		}
	}
	return 0;
}

ForbidWeaponsInInterior(interiorid){
	INTERIOR_WeaponsForbidden[interiorid] = 1;
}