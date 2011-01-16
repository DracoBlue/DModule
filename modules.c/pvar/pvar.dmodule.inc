/*  SA-MP Player Functions
 *
 *  (c) Copyright 2005-2010, SA-MP Team
 *
 */

native SetPVarInt(playerid, varname[], int_value);
native GetPVarInt(playerid, varname[]);
native SetPVarString(playerid, varname[], string_value[]);
native GetPVarString(playerid, varname[], string_return[], len);
native SetPVarFloat(playerid, varname[], Float:float_value);
native Float:GetPVarFloat(playerid, varname[]);
native DeletePVar(playerid, varname[]);

// PVar enumeration
#define PLAYER_VARTYPE_NONE         0
#define PLAYER_VARTYPE_INT          1
#define PLAYER_VARTYPE_STRING       2
#define PLAYER_VARTYPE_FLOAT        3

native GetPVarsUpperIndex(playerid);
native GetPVarNameAtIndex(playerid, index, ret_varname[], ret_len);
native GetPVarType(playerid, varname[]);