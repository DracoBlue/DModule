// Global 3D Text Labels
native Text3D:Create3DTextLabel(text[], color, Float:X, Float:Y, Float:Z, Float:DrawDistance, virtualworld, testLOS=0);
native Delete3DTextLabel(Text3D:id);
native Attach3DTextLabelToPlayer(Text3D:id, playerid, Float:OffsetX, Float:OffsetY, Float:OffsetZ);
native Attach3DTextLabelToVehicle(Text3D:id, vehicleid, Float:OffsetX, Float:OffsetY, Float:OffsetZ);
native Update3DTextLabelText(Text3D:id, color, text[]);

// Per-player 3D Text Labels
native PlayerText3D:CreatePlayer3DTextLabel(playerid, text[], color, Float:X, Float:Y, Float:Z, Float:DrawDistance, attachedplayer=INVALID_PLAYER_ID, attachedvehicle=INVALID_VEHICLE_ID, testLOS=0);
native DeletePlayer3DTextLabel(playerid, PlayerText3D:id);
native UpdatePlayer3DTextLabelText(playerid, PlayerText3D:id, color, text[]);
