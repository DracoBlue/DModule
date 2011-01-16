#define FIGHT_STYLE_NORMAL              4
#define FIGHT_STYLE_BOXING              5
#define FIGHT_STYLE_KUNGFU              6
#define FIGHT_STYLE_KNEEHEAD            7
#define FIGHT_STYLE_GRABKICK            15
#define FIGHT_STYLE_ELBOW               16

#define WEAPONSTATE_UNKNOWN             -1
#define WEAPONSTATE_NO_BULLETS          0
#define WEAPONSTATE_LAST_BULLET         1
#define WEAPONSTATE_MORE_BULLETS        2
#define WEAPONSTATE_RELOADING           3

native SAMP_GivePlayerWeapon(playerid, weaponid, ammo) = GivePlayerWeapon;
native SAMP_ResetPlayerWeapons(playerid) = ResetPlayerWeapons;
native GetPlayerWeapon(playerid);
// FIXME: Still working?
// native SetPlayerDisabledWeapons(playerid, ...);
native SAMP_SetPlayerAmmo(playerid, weaponslot, ammo) = SetPlayerAmmo;
native GetPlayerAmmo(playerid);
native SAMP_GetPlayerWeaponData(playerid, slot, &weapons, &ammo) = GetPlayerWeaponData;
native SetPlayerArmedWeapon(playerid,weaponid);

new WEAPON_Weapon[13];

stock GetPlayerWeaponData(playerid, slot, &weapons, &ammo){
	weapons = 0;
	ammo = 0;
	new samp_weapons; new samp_ammo;
	SAMP_GetPlayerWeaponData(playerid,slot,samp_weapons,samp_ammo);
	if(WEAPON_Weapon[slot] == samp_weapons){
		weapons = samp_weapons;
		ammo = samp_ammo;
	}
}

stock SetPlayerAmmo(playerid,weaponslot,ammo){
	new weaponid;
	new old_ammo;
	GetPlayerWeaponData(playerid,weaponslot,weaponid,old_ammo);
	ResetPlayerWeaponSlot(playerid,weaponslot);
	GivePlayerWeapon(playerid,weaponid,ammo);
}

stock GivePlayerAmmo(playerid,weaponslot,ammo){
	new weaponid;
	new old_ammo;
	GetPlayerWeaponData(playerid,weaponslot,weaponid,old_ammo);
	GivePlayerWeapon(playerid,weaponid,(old_ammo+ammo));
}

stock ResetPlayerWeaponSlot(playerid,weaponslot){
	new weapon[i]; new ammo[i];
	for(new i=0; i<13; i++){
		GetPlayerWeaponData(playerid,i,weapon[i],ammo[i]);
	}
	new armed = GetPlayerWeapon(playerid);
	ResetPlayerWeapons(playerid);
	for(new i=0; i<13; i++){
		if(i != weaponslot){
			GivePlayerWeapon(playerid,weapon[i],ammo[i]);
		}
	}
	SetPlayerArmedWeapon(playerid,armed);
}

stock GivePlayerWeapon(playerid,weaponid,ammo){
	new slot = WeaponSlot(weaponid);
	WEAPON_Weapon[slot] = weaponid;
	SAMP_GivePlayerWeapon(playerid,weaponid,ammo);
}

stock ResetPlayerWeapons(playerid){
	SAMP_ResetPlayerWeapons(playerid);
	for(new i=0; i<13; i++){
		WEAPON_Weapon[i] = 0;
	}
}

stock WeaponSlot(weapon){
	if(weapon == 0 || weapon == 1) return 0;
	else if(weapon >= 2 && weapon <= 9) return 1;
	else if(weapon >= 22 && weapon <= 24) return 2;
	else if(weapon >= 25 && weapon <= 27) return 3;
	else if(weapon == 28 || weapon == 29 || weapon == 32) return 4;
	else if(weapon == 30 || weapon == 31) return 5;
	else if(weapon == 33 || weapon == 34) return 6;
	else if(weapon >= 35 && weapon <= 38) return 7;
	else if((weapon >= 16 && weapon <= 18) || weapon == 39) return 8;
	else if(weapon >= 41 && weapon <= 43) return 9;
	else if(weapon >= 10 && weapon <= 15) return 10;
	else if(weapon >= 45 && weapon <= 46) return 11;
	else if(weapon == 40) return 12;
	else return -1;
}