// Version 1.0
welcomemsg_Connect(playerid) {
	SystemMsgFormatStr(playerid,gettext(playerid,WELCOME_MESSAGE_ONE),PlayerName(playerid));
	SystemMsgFormatStr(playerid,gettext(playerid,WELCOME_MESSAGE_TWO),PlayerName(playerid));
	SystemMsgFormatStr(playerid,gettext(playerid,WELCOME_MESSAGE_THREE),PlayerName(playerid));
}
