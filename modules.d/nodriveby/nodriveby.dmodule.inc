nodriveby_StateChange(playerid, newstate, oldstate) {
    #pragma unused oldstate
    if(newstate == PLAYER_STATE_PASSENGER || newstate == PLAYER_STATE_DRIVER) {
        new weapon_id = GetPlayerWeapon(playerid);
        if (weapon_id != 0 && weapon_id != 29 && weapon_id != 28 && weapon_id != 32) { 
            SetPlayerArmedWeapon(playerid, 0);
        }
    }
}
