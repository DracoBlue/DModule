/**
 * Timestamp always containing current timestamp
 */
new timestamp;
#pragma unused timestamp
#define _Itimestamp_EverySecond timestamp=Now();

timestamp_GameModeInit() {
	timestamp=Now();
}
