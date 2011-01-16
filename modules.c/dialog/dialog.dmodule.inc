native ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]);

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
		Response 1 for first button
		Response 0 for second button
		
	DIALOG_STYLE_INPUT:
		listitem is always -1
		inputtext is the input text 
		Response 1 for the first button
		Response 0 for the second button
		
	DIALOG_STYLE_LIST:
		Response 1 for the first button
			with listitem = 0 for the first line
			with listitem = 1 for the second line
			etc.
		Response 0 for the second button
	
*/