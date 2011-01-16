#define MAX_PICKUPS                             (2048)

native AddStaticPickup(model,type,Float:X,Float:Y,Float:Z,virtualworld=0);
native CreatePickup(model, type, Float:X, Float:Y, Float:Z,virtualworld=0);
native DestroyPickup(pickup);
forward OnPlayerPickUpPickup(playerid, pickupid);
public OnPlayerPickUpPickup(playerid, pickupid) _PickUpPickup(playerid, pickupid);
