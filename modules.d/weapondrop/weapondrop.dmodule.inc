new WEAPONDROP_pickup_time[MAX_ACTIVE_WEAPONDROPS];
new WEAPONDROP_pickup_id[MAX_ACTIVE_WEAPONDROPS];
new WEAPONDROP_weapon_id[MAX_ACTIVE_WEAPONDROPS];
new WEAPONDROP_weapon_ammo[MAX_ACTIVE_WEAPONDROPS];
new WEAPONDROP_highest_pickup_id=1;

#define WEAPONDROP_HEALTH_PICKUP -1
#define WEAPONDROP_HEALTH_PICKUP_MODEL_ID 1240
weapondrop_CommandText(playerid,cmdtext[]) {
	dcmd(dw,2,cmdtext);
	return 0;
}
dcmd_dw(playerid,params[]) {
#pragma unused params
	DropPlayersWeapons(playerid);
	ResetPlayerWeapons(playerid);
	SystemMsg(playerid,"You dropped all your weapons.");
	return 1;
}

weapondrop_Death(playerid,killerid,reason) {
#pragma unused reason
#pragma unused killerid
	DropPlayersWeapons(playerid,true);
	return 0;
}

stock WEAPONDROP_CreateNewDrop(weapon_id,weapon_ammo,Float:x,Float:y,Float:z) {
	new model_id;
	if (weapon_id==WEAPONDROP_HEALTH_PICKUP) {
		// if we do WEAPONDROP_HEALTH_PICKUP, its a health pickup
		model_id=WEAPONDROP_HEALTH_PICKUP_MODEL_ID;
	} else {
		model_id=ModelForWeaponID(weapon_id);
	}
	if (model_id==-1) return ;
	new pickupid=-1;
	for (new i;i<WEAPONDROP_highest_pickup_id;i++) if (WEAPONDROP_pickup_time[i]==0) {
		// we found a free one
		pickupid=i;
		WEAPONDROP_pickup_id[pickupid] = CreatePickup(model_id, 19, x, y, z);
		WEAPONDROP_weapon_id[pickupid] = weapon_id;
		WEAPONDROP_weapon_ammo[pickupid] = weapon_ammo;
		WEAPONDROP_pickup_time[pickupid] = timestamp + MAX_TIME_A_WEAPON_DROP_SHOULD_STAY;
		return ;
	}
	
	pickupid=WEAPONDROP_highest_pickup_id;
	WEAPONDROP_highest_pickup_id++;
	WEAPONDROP_pickup_id[pickupid] = CreatePickup(model_id, 19, x, y, z);
	WEAPONDROP_weapon_id[pickupid] = weapon_id;
	WEAPONDROP_weapon_ammo[pickupid] = weapon_ammo;
	WEAPONDROP_pickup_time[pickupid] = timestamp + MAX_TIME_A_WEAPON_DROP_SHOULD_STAY;
}

/**
 * Drops all weapons of a player (you should use ResetPlayerWeapons after
 * that function is called, if the player is not dead.
 * @param int playerid ID of the player
 * @param bool withhealth sets if player should also drop health package (default is false)
 */
stock DropPlayersWeapons(playerid,withhealth=false) {
	if (!IsPlayerConnected(playerid)) return ;
	new cur_weapon_id,cur_weapon_ammo,cur_weapon_model;
	
	new drop_weapons[13][3];
	new drop_count=0;
	
	for(new s=0;s<13;s++) {
		GetPlayerWeaponData(playerid,s,cur_weapon_id,cur_weapon_ammo);
		if (s==0 && cur_weapon_id==0) {
			// fists, we'll ignore it
		} else if (cur_weapon_ammo > 0 && cur_weapon_id>0 && cur_weapon_id<47) {
			// a valid weapon with ammo dropped
			cur_weapon_model=ModelForWeaponID(cur_weapon_id);
			if (cur_weapon_model!=-1) {
				// we can drop that one .. nice!
				drop_weapons[drop_count][0] = cur_weapon_model;
				drop_weapons[drop_count][1] = cur_weapon_ammo;
				drop_weapons[drop_count][2] = cur_weapon_id;
				drop_count++;
			}
		}
	}
	
	if (withhealth) {
		drop_weapons[drop_count][0] = WEAPONDROP_HEALTH_PICKUP_MODEL_ID;
		drop_weapons[drop_count][1] = 30;
		drop_weapons[drop_count][2] = WEAPONDROP_HEALTH_PICKUP;
		drop_count++;
	}
	
	// we won't continue if we don't drop anything
	if (drop_count==0) return ;
	
	// calculate the angle distance between the drops
	new Float:drop_angle;
	drop_angle = (360.0 / drop_count);
	
	new Float:pp[3];
	GetPlayerPos(playerid,pp[0],pp[1],pp[2]);

	new Float:cur_weapon_radius;
	new Float:cur_weapon_variation;
	
	for (new d;d<drop_count;d++) {
		// Calculate how far we throw that weapon
		cur_weapon_radius = (((MAX_WEAPON_DROP_RADIUS-MIN_WEAPON_DROP_RADIUS)*(100-random(100)))/100)*(MAX_WEAPON_DROP_RADIUS-MIN_WEAPON_DROP_RADIUS)+MIN_WEAPON_DROP_RADIUS;
		cur_weapon_variation = ((random(200)-100)/100)*WEAPON_DROP_VARIATION;
		
		WEAPONDROP_CreateNewDrop(
			drop_weapons[d][2],
			drop_weapons[d][1],
			pp[0] + floatsin(drop_angle*(d+d*cur_weapon_variation),degrees) * cur_weapon_radius,
			pp[1] + floatcos(drop_angle*(d+d*cur_weapon_variation),degrees) * cur_weapon_radius,
			pp[2]
		);
	}
}

weapondrop_GameModeInit() {
	for (new i;i<MAX_ACTIVE_WEAPONDROPS;i++) {
		WEAPONDROP_pickup_id[i]=-1;
		WEAPONDROP_pickup_time[i] = 0;
	}
}

weapondrop_EverySecond() {
	for (new i;i<WEAPONDROP_highest_pickup_id;i++) if (WEAPONDROP_pickup_time[i]!=0 && WEAPONDROP_pickup_time[i]<timestamp) {
		// its over now ...
		DestroyPickup(WEAPONDROP_pickup_id[i]);
		WEAPONDROP_pickup_id[i]=-1;
		WEAPONDROP_pickup_time[i] = 0;
	}
}

weapondrop_PickUpPickup(playerid,pickupid) {
	for (new i;i<WEAPONDROP_highest_pickup_id;i++) if (WEAPONDROP_pickup_time[i]!=0 && WEAPONDROP_pickup_id[i]==pickupid) {
		if (WEAPONDROP_HEALTH_PICKUP==WEAPONDROP_weapon_id[i]) {
			// Its a Health Pickup
			new health_amount;
			health_amount=20+random(10);
			new current_health;
			current_health=Val(PlayerHealth,playerid);
			if (current_health+health_amount>100) {
				SetPlayerHealth(playerid,100);
			} else {
				SetPlayerHealth(playerid,current_health+health_amount);
			}
		} else {
			GivePlayerWeapon(playerid,WEAPONDROP_weapon_id[i],WEAPONDROP_weapon_ammo[i]);
		}
		DestroyPickup(WEAPONDROP_pickup_id[i]);
		WEAPONDROP_pickup_id[i]=-1;
		WEAPONDROP_pickup_time[i] = 0;
	}
	return 1;
}