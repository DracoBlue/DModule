native ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]);

#define DIALOG_ID_RULES 0
#define DIALOG_ID_LOGIN 1
#define DIALOG_ID_WEAPONSLOTS 2
#define DIALOG_ID_WEAPONONE 3
#define DIALOG_ID_WEAPONTWO 4
#define DIALOG_ID_WEAPONTHREE 5
#define DIALOG_ID_WEAPONFOUR 6
#define DIALOG_ID_SPAWNSELECTION 7
#define DIALOG_ID_GANGWAR_MODE 8
#define DIALOG_ID_GANGWAR_AIM 9
#define DIALOG_ID_GANGWAR_WEAPONS 10
#define DIALOG_ID_GANGWAR_AMMO 11
#define DIALOG_ID_GANGWAR_ICONS 12
#define DIALOG_ID_GANGWAR_PLAYERAMMOUNT 13
#define DIALOG_ID_GANGWAR_GANG 14
#define DIALOG_ID_GANGWAR_AREA 15
#define DIALOG_ID_MODCHECK 16
#define DIALOG_ID_BUSINESS 17
#define DIALOG_ID_BUSINESS2 18
#define DIALOG_ID_BUSINESS3 19
#define DIALOG_ID_BUSINESS4 20

forward OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
	_DialogResponse(playerid, dialogid, response, listitem, inputtext);
	return 0;
}

#define DIALOG_STYLE_MSGBOX 0
#define DIALOG_STYLE_INPUT 1
#define DIALOG_STYLE_LIST 2

/*
	DIALOG_STYLE_MSGBOX:
		Response 1 beim ersten Button
		Response 0 beim zweiten Button
		
	DIALOG_STYLE_INPUT:
		listitem immer -1
		inputtext immer das eingegebene
		Response 1 beim ersten Button
		Response 0 beim zweiten Button
		
	DIALOG_STYLE_LIST:
		Response 1 beim ersten Button
			mit listitem = 0 für die erste Linie
			mit listitem = 1 für die zweite Linie
			usw
		Response 0 beim zweiten Button
		
	
*/