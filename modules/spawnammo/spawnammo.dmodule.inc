// Version 1.0
spawnammo_CommandText(playerid, cmdtext[]) {
	dcmd(w,1,cmdtext);
	return 0;
}

stock dcmd_w(playerid,params[]) {
	new index=0;
	new weaponid_str[MAX_STRING];

	weaponid_str=strtok(params,index);
	new weaponid = StrToInt(weaponid_str);
	if (weaponid == 0) weaponid = WeaponID(weaponid_str);

	new name[MAX_STRING];
	GetWeaponName(weaponid, name, MAX_STRING);
	if (strlen(name)==0) return SystemMsg(playerid,"Can not find that weapon!");

	new wammo=StrToInt(strtok(params,index));
	GivePlayerWeapon(playerid,weaponid,wammo);
	format(name,sizeof(name),"Gave you a %s with %d bullets.",name,wammo);
	return SystemMsg(playerid,name);
}

