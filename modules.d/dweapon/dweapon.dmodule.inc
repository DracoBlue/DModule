#define DWEAPON_MAX_DYNAMIC_WEAPONNAMES 60
#define DWEAPON_MAX_WEAPONS 50

new WEAPONID_WeaponCount;

// Contains a weapon name for a weaponid, can be multiple
new WEAPONID_WeaponName[DWEAPON_MAX_DYNAMIC_WEAPONNAMES][MAX_STRING];
// weaponid array for the WEAPONID_WeaponName list
new WEAPONID_WeaponID[DWEAPON_MAX_DYNAMIC_WEAPONNAMES];

// Contains an object directly for each weapon
new DWEAPON_WeaponObjectID[DWEAPON_MAX_WEAPONS];

stock WeaponName(weapon_id) {
	new tmp[MAX_STRING];
	if (weapon_id<0 && weapon_id>DWEAPON_MAX_WEAPONS-1) return tmp;
	GetWeaponName(weapon_id,tmp,MAX_STRING);
	if(weapon_id == 17) format(tmp,MAX_STRING,"Moltov Cocktail");
	return tmp;
}

stock WeaponID(partofweaponname[]){
	if(strlen(partofweaponname) == 0) return -1;
	new i;
	for(i=0; i<WEAPONID_WeaponCount; i++){
	    if(WEAPONID_WeaponName[i][0]!=0 && strcmp(WEAPONID_WeaponName[i],partofweaponname,true)==0){
	        return WEAPONID_WeaponID[i];
	    }
	}
	new correctsigns_weaponid=-1;
	new hasmutiple=-1;

	for(i=0; i<WEAPONID_WeaponCount; i++){
	    if(WEAPONID_WeaponName[i][0]!=0 && strfind(WEAPONID_WeaponName[i],partofweaponname,true)==0){
	        hasmutiple++;
	        correctsigns_weaponid=i;
	    }
	}

	if(hasmutiple > 0){
	    return -2;
	}
	return WEAPONID_WeaponID[correctsigns_weaponid];
}

stock WeaponIDForModel(model_id) {
	for (new i;i<DWEAPON_MAX_WEAPONS;i++) {
		if (DWEAPON_WeaponObjectID[i]==model_id) return i;
	}
	return -1;
}

stock ModelForWeaponID(weapon_id) {
	if (weapon_id<0 || weapon_id>DWEAPON_MAX_WEAPONS-1) return -1;
	return (DWEAPON_WeaponObjectID[weapon_id] == 0 ? -1 : DWEAPON_WeaponObjectID[weapon_id]);
}

stock AddWeaponIDWeaponName(theWeaponName[],theWeaponID){
	WEAPONID_WeaponID[WEAPONID_WeaponCount] = theWeaponID;
	format(WEAPONID_WeaponName[WEAPONID_WeaponCount],MAX_STRING,"%s",theWeaponName);
	WEAPONID_WeaponCount++;
}

stock AddWeaponIDModelID(weapon_id,model_id) {
	if (weapon_id<0 || weapon_id>DWEAPON_MAX_WEAPONS-1) return ;
	DWEAPON_WeaponObjectID[weapon_id]=model_id;
}

dweapon_GameModeInit(){
	AddWeaponIDModelID(WEAPON_BRASSKNUCKLE,331);
	AddWeaponIDModelID(WEAPON_GOLFCLUB,333);
	AddWeaponIDModelID(WEAPON_NITESTICK,334);
	AddWeaponIDModelID(WEAPON_KNIFE,335);
	AddWeaponIDModelID(WEAPON_BAT,336);
	AddWeaponIDModelID(WEAPON_SHOVEL,337);
	AddWeaponIDModelID(WEAPON_POOLSTICK,338);
	AddWeaponIDModelID(WEAPON_KATANA,339);
	AddWeaponIDModelID(WEAPON_CHAINSAW,341);
	AddWeaponIDModelID(WEAPON_DILDO,321);
	AddWeaponIDModelID(WEAPON_DILDO2,322);
	AddWeaponIDModelID(WEAPON_VIBRATOR,323);
	AddWeaponIDModelID(WEAPON_VIBRATOR2,324);
	AddWeaponIDModelID(WEAPON_FLOWER,325);
	AddWeaponIDModelID(WEAPON_CANE,326);
	AddWeaponIDModelID(WEAPON_GRENADE,342);
	AddWeaponIDModelID(WEAPON_TEARGAS,343);
	AddWeaponIDModelID(WEAPON_MOLTOV,344);
	AddWeaponIDModelID(WEAPON_COLT45,346);
	AddWeaponIDModelID(WEAPON_SILENCED,347);
	AddWeaponIDModelID(WEAPON_DEAGLE,348);
	AddWeaponIDModelID(WEAPON_SHOTGUN,349);
	AddWeaponIDModelID(WEAPON_SAWEDOFF,350);
	AddWeaponIDModelID(WEAPON_SHOTGSPA,351);
	AddWeaponIDModelID(WEAPON_UZI,352);
	AddWeaponIDModelID(WEAPON_MP5,353);
	AddWeaponIDModelID(WEAPON_AK47,355);
	AddWeaponIDModelID(WEAPON_M4,356);
	AddWeaponIDModelID(WEAPON_RIFLE,357);
	AddWeaponIDModelID(WEAPON_SNIPER,358);
	AddWeaponIDModelID(WEAPON_TEC9,372);
	AddWeaponIDModelID(WEAPON_ROCKETLAUNCHER,359);
	AddWeaponIDModelID(WEAPON_HEATSEEKER,360);
	AddWeaponIDModelID(WEAPON_FLAMETHROWER,361);
	AddWeaponIDModelID(WEAPON_MINIGUN,362);
	AddWeaponIDModelID(WEAPON_SATCHEL,363);
	AddWeaponIDModelID(WEAPON_SPRAYCAN,365);
	AddWeaponIDModelID(WEAPON_FIREEXTINGUISHER,366);
	AddWeaponIDModelID(WEAPON_CAMERA,367);
	AddWeaponIDModelID(WEAPON_PARACHUTE,371);

	AddWeaponIDWeaponName("Brassknuckle",1);
	AddWeaponIDWeaponName("Golfclub",2);
	AddWeaponIDWeaponName("Nitestick",3);
	AddWeaponIDWeaponName("Knife",4);
	AddWeaponIDWeaponName("Baseballbat",5);
	AddWeaponIDWeaponName("Bat",5);
	AddWeaponIDWeaponName("Shovel",6);
	AddWeaponIDWeaponName("Poolstick",7);
	AddWeaponIDWeaponName("Katana",8);
	AddWeaponIDWeaponName("Chainsaw",9);
	AddWeaponIDWeaponName("Dildo",10);
	AddWeaponIDWeaponName("Dildo2",11);
	AddWeaponIDWeaponName("Vibrator",12);
	AddWeaponIDWeaponName("Vibrator2",13);
	AddWeaponIDWeaponName("Flower",14);
	AddWeaponIDWeaponName("Cane",15);
	AddWeaponIDWeaponName("Grenade",16);
	AddWeaponIDWeaponName("Teargas",17);
	AddWeaponIDWeaponName("Molotov",18);
	AddWeaponIDWeaponName("Colt45",22);
	AddWeaponIDWeaponName("Pistole",22);
	AddWeaponIDWeaponName("Silenced",23);
	AddWeaponIDWeaponName("Deagle",24);
	AddWeaponIDWeaponName("DesertEagle",24);
	AddWeaponIDWeaponName("Shotgun",25);
	AddWeaponIDWeaponName("Sawedoff",26);
	AddWeaponIDWeaponName("Sawnoff",26);
	AddWeaponIDWeaponName("CombatShotgun",27);
	AddWeaponIDWeaponName("Uzi",28);
	AddWeaponIDWeaponName("Mac10",28);
	AddWeaponIDWeaponName("Mp5",29);
	AddWeaponIDWeaponName("SMG",29);
	AddWeaponIDWeaponName("AK47",30);
	AddWeaponIDWeaponName("M4A1",31);
	AddWeaponIDWeaponName("Tec9",32);
	AddWeaponIDWeaponName("Rifle",33);
	AddWeaponIDWeaponName("Sniperrifle",34);
	AddWeaponIDWeaponName("Rocketlauncher",35);
	AddWeaponIDWeaponName("Heatseeker",36);
	AddWeaponIDWeaponName("Flamethrower",37);
	AddWeaponIDWeaponName("Minigun",38);
	AddWeaponIDWeaponName("Satchel",38);
	AddWeaponIDWeaponName("Spraycan",41);
	AddWeaponIDWeaponName("FireExtinguisher",42);
	AddWeaponIDWeaponName("Camera",43);
	AddWeaponIDWeaponName("Parachute",46);
}
