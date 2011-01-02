native AddStaticPickup(model,type,Float:X,Float:Y,Float:Z,virtualworld=-1);
native CreatePickup(model, type, Float:X, Float:Y, Float:Z,virtualworld=-1);
native DestroyPickup(pickup);
forward OnPlayerPickUpPickup(playerid, pickupid);
public OnPlayerPickUpPickup(playerid, pickupid) _PickUpPickup(playerid, pickupid);

new PICKUP_Type[2048];
#define PICKUP_TypeIsPacket 1
#define PICKUP_TypeIsShop 2
#define PICKUP_TypeIsForFunRace 3
#define PICKUP_TypeIsForHouse 4
#define PICKUP_TypeIsForGanghouse 5
#define PICKUP_TypeToBuyGanghouse 6
#define PICKUP_TypeIsSpike 7
#define PICKUP_TypeIsForGodcheck 8
#define PICKUP_TypeIsForBusiness 9