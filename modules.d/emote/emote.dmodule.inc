new EMOTE_rap[MAX_PLAYERS]=0;
new EMOTE_sit[MAX_PLAYERS]=0;
new EMOTE_phone[MAX_PLAYERS]=0;

Float:EMOTE_Distance(Float:x,Float:y,Float:z,Float:x2,Float:y2,Float:z2,bool:noz) {
	new Float:result;
	if (!noz) result=floatsqroot((x2-x)*(x2-x)+(y2-y)*(y2-y)+(z2-z)*(z2-z)); else
	result=floatsqroot((x2-x)*(x2-x)+(y2-y)*(y2-y));
	if (result<0) {result=(-1)*result;}
	return result;
}

dcmd_me(const playerid, const msg[]) {
	new tmp[MAX_STRING];
	format(tmp, MAX_STRING, "* %s %s", PlayerName(playerid), msg);
	for (new p = 0; p < MAX_PLAYERS; p++)
		if (Val(PlayerSpawned, p) && 20 > EMOTE_Distance(PlayerPosition[p][0],
				PlayerPosition[p][1], 0, PlayerPosition[playerid][0],
				PlayerPosition[playerid][1], 0, true)) {
			SendClientMessage(p, EMOTE_me_color, tmp);
		}
	return 1;
}

stock systemEmote(const msg[], Float:x, Float:y, Float:z) {
#pragma unused z
	for (new p=0;p<MAX_PLAYERS;p++) if (Val(PlayerSpawned,p) && 20>EMOTE_Distance(PlayerPosition[p][0],PlayerPosition[p][1],0,x,y,0,true)) {
		SendClientMessage(p,EMOTE_me_color,msg);
	}
	return 1;
}

dcmd_dance(const playerid, const params[]) {
#pragma unused params
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		new dance = random(3);
		switch (dance) {
		case 0:
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);
		case 1:
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2);
		case 2:
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3);
		}
		return 1;
	} else {
		BadSystemMsg(playerid,
				"(emote) You can dance only while spawned and outside a car.");
	}
	return 1;
}

dcmd_ey(const playerid, const params[]) {
#pragma unused params
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		ClearAnimations(playerid);
		ApplyAnimation(playerid, "RIOT", "RIOT_ANGRY", 4, 0, 1, 1, 0, -1);
		return 1;
	} else {
		BadSystemMsg(playerid,
				"(emote) You can use emote only while spawned and outside a car.");
	}
	return 1;
}

dcmd_rap(const playerid, const params[]) {
#pragma unused params
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		ClearAnimations(playerid);
		EMOTE_rap[playerid] = 1;
		return 1;
	} else {
		BadSystemMsg(playerid,
				"(emote) You can use emote only while spawned and outside a car.");
	}
	return 1;
}

dcmd_sit(const playerid, const params[]) {
#pragma unused params
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		ClearAnimations(playerid);
		EMOTE_sit[playerid] = 1;
		return 1;
	} else {
		BadSystemMsg(playerid,
				"(emote) You can sit only while spawned and outside a car.");
	}
	return 1;
}

dcmd_strip(const playerid, const params[]) {
#pragma unused params
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		ClearAnimations(playerid);
		new strip = random(7);
		switch (strip) {
		case 0:
			ApplyAnimation(playerid, "STRIP", "strip_A", 4, 0, 1, 1, 0, -1);
		case 1:
			ApplyAnimation(playerid, "STRIP", "strip_B", 4, 0, 1, 1, 0, -1);
		case 2:
			ApplyAnimation(playerid, "STRIP", "strip_C", 4, 0, 1, 1, 0, -1);
		case 3:
			ApplyAnimation(playerid, "STRIP", "strip_D", 4, 0, 1, 1, 0, -1);
		case 4:
			ApplyAnimation(playerid, "STRIP", "strip_E", 4, 0, 1, 1, 0, -1);
		case 5:
			ApplyAnimation(playerid, "STRIP", "strip_F", 4, 0, 1, 1, 0, -1);
		case 6:
			ApplyAnimation(playerid, "STRIP", "strip_G", 4, 0, 1, 1, 0, -1);
		}

		return 1;
	} else {
		BadSystemMsg(playerid,
				"(emote) You can use emote only while spawned and outside a car.");
	}
	return 1;
}

dcmd_drunk(const playerid, const params[]) {
#pragma unused params
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		ClearAnimations(playerid);
		ApplyAnimation(playerid, "ped", "WALK_drunk", 4, 0, 1, 1, 0, -1);

		return 1;
	} else {
		BadSystemMsg(playerid,
				"(emote) You can use emote only while spawned and outside a car.");
	}
	return 1;
}

dcmd_phone(const playerid, const params[]) {
#pragma unused params
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		ClearAnimations(playerid);
		EMOTE_phone[playerid] = 1;
		return 1;
	} else {
		BadSystemMsg(playerid,
				"(emote) You can sit only while spawned and outside a car.");
	}
	return 1;
}

dcmd_hi(const playerid, const params[]) {
#pragma unused params
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
		ClearAnimations(playerid);
		ApplyAnimation(playerid, "GANGS", "hndshkaa", 4, 0, 0, 0, 0, -1);

		return 1;
	} else {
		BadSystemMsg(playerid,
				"(emote) You can use emote only while spawned and outside a car.");
	}
	return 1;
}

emote_CommandText(playerid, cmdtext[]) {
	dcmd(me, 2, cmdtext);
	dcmd(dance, 5, cmdtext);
	dcmd(rap, 3, cmdtext);
	dcmd(sit, 3, cmdtext);
	dcmd(ey, 2, cmdtext);
	dcmd(strip, 5, cmdtext);
	dcmd(drunk, 5, cmdtext);
	dcmd(hi, 2, cmdtext);
	dcmd(phone, 5, cmdtext);

	return 0;
}

#define _CD_emote_ESPlayer ((EMOTE_rap[playerid] > 0) || (EMOTE_sit[playerid] > 0) || (EMOTE_phone[playerid] > 0))
emote_ESPlayer(playerid) {
	if (EMOTE_rap[playerid] > 0) {
		new rap = random(2);
		if (EMOTE_rap[playerid] == 1) {
			ClearAnimations(playerid);
			switch (rap) {
			case 0:
				ApplyAnimation(playerid, "RAPPING", "RAP_A_IN", 4, 0, 1, 1,
						0, -1);
			case 1:
				ApplyAnimation(playerid, "RAPPING", "RAP_B_IN", 4, 0, 1, 1,
						0, -1);
			}
		}
		if (EMOTE_rap[playerid] == 2) {
			ClearAnimations(playerid);
			switch (rap) {
			case 0:
				ApplyAnimation(playerid, "RAPPING", "RAP_A_Loop", 4, 0, 1,
						1, 0, -1);
			case 1:
				ApplyAnimation(playerid, "RAPPING", "RAP_B_Loop", 4, 0, 1,
						1, 0, -1);
			}
		}

		if (EMOTE_rap[playerid] == 6) {
			ClearAnimations(playerid);
			switch (rap) {
			case 0:
				ApplyAnimation(playerid, "RAPPING", "RAP_A_OUT", 4, 0, 1, 1,
						0, -1);
			case 1:
				ApplyAnimation(playerid, "RAPPING", "RAP_B_OUT", 4, 0, 1, 1,
						0, -1);
			}
		}
		EMOTE_rap[playerid]++;
		if (EMOTE_rap[playerid] == 7) {
			ClearAnimations(playerid);
			EMOTE_rap[playerid] = 0;
		}
	}

	if (EMOTE_sit[playerid] > 0) {

		if (EMOTE_sit[playerid] == 1) {
			new sit = random(3);
			ClearAnimations(playerid);
			switch (sit) {
			case 0:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleA", 4,
						1, 1, 1, 0, -1);
			case 1:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleB", 4,
						1, 1, 1, 0, -1);
			case 2:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleC", 4,
						1, 1, 1, 0, -1);
			}
		}

		if (EMOTE_sit[playerid] == 4) {
			new sit = random(3);
			switch (sit) {
			case 0:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleA", 4,
						1, 1, 1, 0, -1);
			case 1:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleB", 4,
						1, 1, 1, 0, -1);
			case 2:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleC", 4,
						1, 1, 1, 0, -1);
			}
		}

		if (EMOTE_sit[playerid] == 7) {
			new sit = random(3);
			switch (sit) {
			case 0:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleA", 4,
						1, 1, 1, 0, -1);
			case 1:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleB", 4,
						1, 1, 1, 0, -1);
			case 2:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleC", 4,
						1, 1, 1, 0, -1);
			}
		}

		if (EMOTE_sit[playerid] == 10) {
			new sit = random(3);
			switch (sit) {
			case 0:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleA", 4,
						1, 0, 0, 0, -1);
			case 1:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleB", 4,
						1, 0, 0, 0, -1);
			case 2:
				ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_IdleC", 4,
						1, 0, 0, 0, -1);
			}
		}

		if (EMOTE_sit[playerid] == 13) {
			ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_out", 4, 0, 0,
					0, 0, -1);
		}
		EMOTE_sit[playerid]++;
		if (EMOTE_sit[playerid] == 16) {
			ClearAnimations(playerid);
			EMOTE_sit[playerid] = 0;
		}
	}

	if (EMOTE_phone[playerid] > 0) {
		if (EMOTE_phone[playerid] == 1) {
			ClearAnimations(playerid);
			ApplyAnimation(playerid, "ped", "phone_in", 4, 0, 1, 1, 0, -1);
		}
		if (EMOTE_phone[playerid] == 2) {
			ApplyAnimation(playerid, "ped", "phone_talk", 4, 1, 1, 1, 0, -1);
		}
		if (EMOTE_phone[playerid] == 5) {
			ApplyAnimation(playerid, "ped", "phone_out", 4, 0, 1, 1, 0, -1);
		}
		EMOTE_phone[playerid]++;
		if (EMOTE_phone[playerid] == 6) {
			ClearAnimations(playerid);
			EMOTE_phone[playerid] = 0;
		}
	}
}
