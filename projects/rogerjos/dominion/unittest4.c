/*******************************************************************************
 * Program: unittest4.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: Unit test for disardCard() function in dominion.c
*******************************************************************************/
	
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

// Toggle verbose output 0 = off, else on
#define VERBOSE 1

int main() {
	struct gameState state,	// Current game state 
					 control;	// For detecting unexpected state changes

	int i,		// A general counter/variable
		player,		// Current player
		discarded,	// Last card discarded
		discardCount,	// Counter for number of cards discarded
		trashFlag,	// Tracks whether card is to be trashed
		testHandCount = 10,
		errors = 0;	// Counter for test failures
		final = 0;	// Counter for final test result

	int testHand[10] = {estate, duchy, province, copper, silver, gold, 
						adventurer, council_room, feast, gardens};

	memset(&control, 23, sizeof(struct gameState));	// Clear junk data

	for (player = 0; player < MAX_PLAYERS; player++) {	// Update all players
		memcpy(control.hand[player], testHand, sizeof(int) * testHandCount);	// Give player a test hand
		control.handCount[player] = testHandSize;	// Update player hand size
		control.discardCount = 0;	// Reset player discard count
	}
	
	for (trashFlag = 0; trashFlag < 2; trashFlag++) {	// Test both trashed and untrashed discards

		for (player = 0; player < MAX_PLAYERS; player++) {	// Test all players

			/* Reset game state */
			memcpy(&state, &control, sizeof(struct gameState));
			discardCount = 0;

			/* Test discarding last card in hand */
			for (i = 1; i <= testHandCount; i++) {	// While hand is not empty
				discarded = state.hand[player][testHandCount - i];
				discardCard(testHandCount - i, player, &state, trashFlag)	// Discard the last card in hand

				if (!trashFlag) {	// Check discard count if trashFlag is off
					discardCount++;
					if (state.discardCount[player] != discardCount) {
						if (VERBOSE) {
							printf("discardCard() FAIL: game state discardCount not incremented with trashFlag off\n");
						}
						state.discardCount[player] = discardCount;	// Correct game state variable
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: game state discardCount incremented with trashFlag off\n");
					}	
					
					if (state.discard[player][state.discardCount[player] - 1] != discarded) {	// Check discard pile if trashFlag is off
						if (VERBOSE) {
							printf("discardCard() FAIL: discarded card not copied to player discard array with trashFlag on\n");
						}
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: discarded card copied to player discard array with trashFlag on\n");
					}	
				}
				else {	// Check discard count if trashFlag is on					
					if (state.discardCount > discardCount) {
						if (VERBOSE) {
							printf("discardCard() FAIL: game state discardCount incremented with trashFlag on\n");
						}
						errors++;
						state.discardCount[player] = discardCount;	// Correct game state variable
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: game state discardCount not incremented with trashFlag on\n");
					}	
					
					if (state.discard[player][state.discardCount[player] - 1] == discarded) {	// Check discard pile if trashFlag on
						if (VERBOSE) {
							printf("discardCard() FAIL: discarded card copied to player discard array with trashFlag on\n");
						}
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: discarded card not copied to player discard array with trashFlag on\n");
					}	

				}	
			}
			
			/* All cards discarded. Check for out of bound changes to game state */
			memcpy(state.hand[player], testHand, sizeof(int) * testHandCount);	// Reset player hand to test hand
			state.handCount[player] = testHandSize;	// Reset player hand size
			memset(state.discard[player], 23, sizeof(int) * testHandCount);	// Clear player discard pile
			state.discardCount = 0;	// Reset player discard count
			if (memcmp(control, state, sizeof(struct gameState))) {
				if (VERBOSE) {
					printf("discardCard() FAIL: Out of bound changes made to game state struct.\n");
				}
				errors++;
			}
			else if (VERBOSE) {
				printf("discardCard() PASS: No out of bound changes made to game state struct.\n");
			}
		}
	}

	/* Display results discard last card */
	printf("discardCard(): ");
	if (errors) {
		printf("FAIL when discarding highest index card. (%d failures)\n", errors);
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when discarding highest index card.\n");
	}

	/* Test discarding first card in hand */
	for (trashFlag = 0; trashFlag < 2; trashFlag++) {	// Test both trashed and untrashed discards

		for (player = 0; player < MAX_PLAYERS; player++) {	// Test all players


			/* Reset game state  */
			memcpy(&state, &control, sizeof(struct gameState));
			discardCount = 0;

			/* While hand not empty */
			for (i = 0; i < testHandCount; i++) {
				discarded = state.hand[player][0];
				discardCard(0, player, &state, trashFlag)	// Discard the first card in hand

				if (!trashFlag) {	// Check discard count if trashFlag is off
					discardCount++;
					if (state.discardCount[player] != discardCount) {
						if (VERBOSE) {
							printf("discardCard() FAIL: game state discardCount not incremented with trashFlag off\n");
						}
						state.discardCount[player] = discardCount;	// Correct game state variable
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: game state discardCount incremented with trashFlag off\n");
					}	
					
					if (state.discard[player][state.discardCount[player] - 1] != discarded) {	// Check discard pile if trashFlag is off
						if (VERBOSE) {
							printf("discardCard() FAIL: discarded card not copied to player discard array with trashFlag on\n");
						}
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: discarded card copied to player discard array with trashFlag on\n");
					}	
				}
				else {	// Check discard count if trashFlag is on					
						if (state.discardCount > discardCount) {
						if (VERBOSE) {
							printf("discardCard() FAIL: game state discardCount incremented with trashFlag on\n");
						}
						errors++;
						state.discardCount[player] = discardCount;	// Correct game state variable
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: game state discardCount not incremented with trashFlag on\n");
					}	
					
					if (state.discard[player][state.discardCount[player] - 1] == discarded) {	// Check discard pile if trashFlag on
						if (VERBOSE) {
							printf("discardCard() FAIL: discarded card copied to player discard array with trashFlag on\n");
						}
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: discarded card not copied to player discard array with trashFlag on\n");
					}	
				}
			}
			
			/* All cards discarded. Check for out of bound changes to game state */
			memcpy(state.hand[player], testHand, sizeof(int) * testHandCount);	// Reset player hand to test hand
			state.handCount[player] = testHandSize;	// Reset player hand size
			memset(state.discard[player], 23, sizeof(int) * testHandCount);	// Clear player discard pile
			state.discardCount = 0;	// Reset player discard count
			if (memcmp(control, state, sizeof(struct gameState))) {
				if (VERBOSE) {
					printf("discardCard() FAIL: Out of bound changes made to game state struct.\n");
				}
				errors++;
			}
			else if (VERBOSE) {
				printf("discardCard() PASS: No out of bound changes made to game state struct.\n");
			}

		}
	}

	/* Display results discard last card */
	printf("discardCard(): ");
	if (errors) {
		printf("FAIL when discarding lowest index card. (%d failures)\n", errors);
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when discarding lowest index card.\n");
	}

	/* Test discarding mid index card in */
	for (trashFlag = 0; trashFlag < 2; trashFlag++) {	// Test both trashed and untrashed discards

		for (player = 0; player < MAX_PLAYERS; player++) {	// Test all players

			/* Reset game state */
			memcpy(&state, &control, sizeof(struct gameState));
			discardCount = 0;

			/* While hand not empty */
			for (i = 1; i <= testHandCount; i++) {	// While hand is not empty
				discarded = state.hand[player][(testHandCount - i)/2];
				discardCard((testHandCount - i)/2, player, &state, trashFlag)	// Discard a mid card in hand (int div)

				if (!trashFlag) {	// Check discard count if trashFlag is off
					discardCount++;
					if (state.discardCount[player] != discardCount) {
						if (VERBOSE) {
							printf("discardCard() FAIL: game state discardCount not incremented with trashFlag off\n");
						}
						state.discardCount[player] = discardCount;	// Correct game state variable
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: game state discardCount incremented with trashFlag off\n");
					}	
					
					if (state.discard[player][state.discardCount[player] - 1] != discarded) {	// Check discard pile if trashFlag is off
						if (VERBOSE) {
							printf("discardCard() FAIL: discarded card not copied to player discard array with trashFlag on\n");
						}
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: discarded card copied to player discard array with trashFlag on\n");
					}	
				}
				else {	// Check discard count if trashFlag is on					
						if (state.discardCount > discardCount) {
						if (VERBOSE) {
							printf("discardCard() FAIL: game state discardCount incremented with trashFlag on\n");
						}
						errors++;
						state.discardCount[player] = discardCount;	// Correct game state variable
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: game state discardCount not incremented with trashFlag on\n");
					}	
					
					if (state.discard[player][state.discardCount[player] - 1] == discarded) {	// Check discard pile if trashFlag on
						if (VERBOSE) {
							printf("discardCard() FAIL: discarded card copied to player discard array with trashFlag on\n");
						}
						errors++;
					}
					else if (VERBOSE) { 
						printf("discardCard() PASS: discarded card not copied to player discard array with trashFlag on\n");
					}	

				}	
			}
			
			/* All cards discarded. Check for out of bound changes to game state */
			memcpy(state.hand[player], testHand, sizeof(int) * testHandCount);	// Reset player hand to test hand
			state.handCount[player] = testHandSize;	// Reset player hand size
			memset(state.discard[player], 23, sizeof(int) * testHandCount);	// Clear player discard pile
			state.discardCount = 0;	// Reset player discard count
			if (memcmp(control, state, sizeof(struct gameState))) {
				if (VERBOSE) {
					printf("discardCard() FAIL: Out of bound changes made to game state struct.\n");
				}
				errors++;
			}
			else if (VERBOSE) {
				printf("discardCard() PASS: No out of bound changes made to game state struct.\n");
			}
		}
	}	

	/* Display results discard mid card */
	printf("discardCard(): ");
	if (errors) {
		printf("FAIL when discarding mid index card. (%d failures)\n", errors);
		final += errors;
	} 
	else {
		printf("PASS when discarding mid index card.\n");
	}

	/* Display final results */
	printf("discardCard(): ");
	if (final) {
		printf("%d TESTS FAILED.\n", final);
	} 
	else {
		printf("ALL TESTS PASSED.\n");
	}

	return 0;
}
