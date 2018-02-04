/*******************************************************************************
 * Program: unittest1.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: Unit test for isGameOver() function in dominion.c
 * Note: Dominion rules state that the game is over only if 
 *			1) the supply of Province cards is empty 
 *							or 
 *			2) any three supply piles are empty.
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>


// Toggle verbose output 0 = off, else on
#define VERBOSE 0

#define SEED 23

int main() {
	struct gameState state,	// Current game state 
					 control;	// For detecting and reverting state changes

	int i, j,	// general counters
		errors = 0,	// Counter for test failures
		final = 0;	// Counter for cumulative errors

	int k[10] = {adventurer, council_room, feast, gardens, mine,	// Arbitrary cards
					remodel, smithy, village, baron, great_hall};

	int singleTest[5] = {curse, duchy, copper, adventurer, province}; // One of each card type + province

	int doubleTest[12][2] = {{curse, duchy}, {curse, silver}, {curse, feast},
							{estate, silver}, {estate, gardens}, {gold, remodel}, 
							{estate, duchy}, {silver, gold}, {village, baron},
							{province, duchy}, {province, gold}, {province, baron}};

	int tripleTest[24][3] = {{curse, estate, duchy}, {curse, silver, gold}, {curse, mine, great_hall}, 
							{curse, duchy, gold}, {curse, estate, feast}, {curse, duchy, adventurer}, 
							{estate, silver, baron}, {duchy, village, mine}, {estate, silver, copper}, 
							{copper, silver, gold}, {copper, silver, smithy}, {gold, remodel, smithy}, 
							{copper, silver, gold}, {copper, silver, smithy}, {gold, remodel, smithy}, 
							{province, silver, gold}, {copper, province, smithy}, {gold, remodel, province}, 
							{province, silver, gold}, {province, silver, smithy}, {gold, province, smithy}, 
							{feast, smithy, village}, {curse, province, gold}, {curse, mine, province}};

	int quadTest[8][4] = {{curse, estate, silver, village}, {copper, silver, gold, estate}, 
							{curse, province, silver, village}, {province, silver, gold, estate}, 
							{estate, duchy, gold, great_hall}, {feast, gardens, mine, smithy},
							{estate, duchy, gold, province}, {province, gardens, mine, smithy}};

	/* Set up game */
	memset(&state, 23, sizeof(struct gameState)); // Clear data
	if (initializeGame(MAX_PLAYERS, k, SEED, &state)) {	// Print error and exit if new game fails
			printf("isGameOver(): ABORT: initializeGame() exited with error.\n");
			return 1;
	}
	memcpy(&control, &state, sizeof(struct gameState));	// Save beginning game state	
	

	/* Test with all in-play piles full */
	if (isGameOver(&state)) {
		if (VERBOSE) {
			printf("isGameOver(): FAIL Game over with all supply piles full.\n");
		}
		errors++;
	}
	else if (VERBOSE) {
		printf("isGameOver(): PASS Game not over with all supply piles full.\n");
	}

	/* Check for out of bound changes to game state */
	if (memcmp(&control, &state, sizeof(struct gameState))) {
		if (VERBOSE) {
			printf("isGameOver(): FAIL Out of bound changes made to game state struct.\n");
		}
		errors++;
	}
	else if (VERBOSE) {
		printf("isGameOver(): PASS No out of bound changes made to game state struct.\n");
	}

	memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	


	/* Test with all in-play piles at 9, 5, and 1 */
	for (i = 9; i > 0; i -= 4) { 
		for (j = curse; j <= treasure_map; j++) {	// Step through all cards (enum-ed)
			if (state.supplyCount[j] > -1)
			{	// Unused cards were initialized to -1
				state.supplyCount[j] = i;	// Set piles in play
			}
		}
	
		if (isGameOver(&state)) {	// Check for game over
			if (VERBOSE) {
				printf("isGameOver(): FAIL Game over with all supply piles at %d cards.\n", i);
			}
			errors++;
		}
		else if (VERBOSE) {
			printf("isGameOver(): PASS Game not over with all supply piles at %d cards.\n", i);
		}
	
		for (j = curse; j <= treasure_map; j++) {	// Step through all cards (enum-ed)
			if (state.supplyCount[j] > -1)
			{	// Unused cards were set to -1
				state.supplyCount[j] = control.supplyCount[j];	// Revert used cards to 10
			}
		}
	
		/* Check for out of bound changes to game state */
		if (memcmp(&control, &state, sizeof(struct gameState))) {
			if (VERBOSE) {
				printf("isGameOver(): FAIL Out of bound changes made to game state struct.\n");
			}
			errors++;
		}
		else if (VERBOSE) {
			printf("isGameOver(): PASS No out of bound changes made to game state struct.\n");
		}
	
		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}

	/* Display results no empty supply */
	printf("isGameOver(): ");
	if (errors) {
		printf("FAIL when no supply piles empty.\n");
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when no supply piles empty.\n");
	}

	/* Test with one empty pile, one test per card type + province */
	for (i = 0; i < 5; i++) {	// Step through each card type
		state.supplyCount[singleTest[i]] = 0;	// Empty the supply of chosen type
		
		if (state.supplyCount[province]) {	// If province supply not empty
			if (isGameOver(&state)) {	// If game over
				if (VERBOSE) {
					printf("isGameOver(): FAIL Game over with one non-province supply pile empty.\n");
				}
				errors++;
			}
			else {
				if (VERBOSE) {
					printf("isGameOver(): PASS Game not over with one non-province supply pile empty.\n");
				}
			}
		}
		else {	// If province supply empty
			if (isGameOver(&state)) {	// If game over
				if (VERBOSE) {
					printf("isGameOver(): PASS Game over with province supply pile empty.\n");
				}
			}
			else {
				if (VERBOSE) {
					printf("isGameOver(): FAIL Game not over with province supply pile empty.\n");
				}
				errors++;
			}
		}
		

		state.supplyCount[singleTest[i]] = control.supplyCount[singleTest[i]];	// Revert the supply of chosen type
	
		/* Check for out of bound changes to game state */
		if (memcmp(&control, &state, sizeof(struct gameState))) {
			if (VERBOSE) {
				printf("isGameOver(): FAIL Out of bound changes made to game state struct.\n");
			}
			errors++;
		}
		else if (VERBOSE) {
			printf("isGameOver(): PASS No out of bound changes made to game state struct.\n");
		}
	
		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}

	/* Display results one empty supply */
	printf("isGameOver(): ");
	if (errors) {
		printf("FAIL when one supply pile empty.\n");
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when one supply pile empty.\n");
	}

	/* Test with two empty pile, one test per card type combination */
	for (i = 0; i < 12; i++) {	// Step through each card type combination
		state.supplyCount[doubleTest[i][0]] = 0;	// Empty the supply of first chosen type
		state.supplyCount[doubleTest[i][1]] = 0;	// Empty the supply of second chosen type

		if (state.supplyCount[province]) {	// If province supply not empty
			if (isGameOver(&state)) {	// If game over
				if (VERBOSE) {
					printf("isGameOver(): FAIL Game over with two non-province supply piles empty.\n");
				}
				errors++;
			}
			else {
				if (VERBOSE) {
					printf("isGameOver(): PASS Game not over with two non-province supply piles empty.\n");
				}
			}
		}
		else {	// If province supply empty
			if (isGameOver(&state)) {	// If game over
				if (VERBOSE) {
					printf("isGameOver(): PASS Game over with province supply pile empty.\n");
				}
			}
			else {
				if (VERBOSE) {
					printf("isGameOver(): FAIL Game not over with province supply pile empty.\n");
				}
				errors++;
			}
		}
	
		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}

	/* Display results two empty supply piles */
	printf("isGameOver(): ");
	if (errors) {
		printf("FAIL when two supply piles empty.\n");
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when two supply piles empty.\n");
	}


	/* Test with three empty piles */
	for (i = 0; i < 24; i++) {	// Step through each combination in array
		state.supplyCount[tripleTest[i][0]] = 0;	// Empty the supply of first chosen type
		state.supplyCount[tripleTest[i][1]] = 0;	// Empty the supply of second chosen type
		state.supplyCount[tripleTest[i][2]] = 0;	// Empty the supply of third chosen type


		if (state.supplyCount[province]) {	// If province supply not empty
			if (isGameOver(&state)) {	// If game over
				if (VERBOSE) {
					printf("isGameOver(): PASS Game over with three non-province supply piles empty.\n");
				}
			}
			else { 
				if (VERBOSE) {
					printf("isGameOver(): FAIL Game not over with three non-province supply piles empty.\n");
				}
				errors++;
			}
		}
		else {	// If province supply empty
			if (isGameOver(&state)) {	// If game over
				if (VERBOSE) {
					printf("isGameOver(): PASS Game over with province and two other supply piles empty.\n");
				}
			}
			else {
				if (VERBOSE) {
					printf("isGameOver(): FAIL Game not over with province and two other supply piles empty.\n");
				}
				errors++;
			}
		}

		state.supplyCount[tripleTest[i][0]] = control.supplyCount[tripleTest[i][0]]; 	// Revert the supply of first chosen type
		state.supplyCount[tripleTest[i][1]] = control.supplyCount[tripleTest[i][1]]; 	// Revert the supply of second chosen type
		state.supplyCount[tripleTest[i][2]] = control.supplyCount[tripleTest[i][2]]; 	// Revert the supply of third chosen type
	
		/* Check for out of bound changes to game state */
		if (memcmp(&control, &state, sizeof(struct gameState))) {
			if (VERBOSE) {
				printf("isGameOver(): FAIL Out of bound changes made to game state struct.\n");
			}
			errors++;
		}
		else if (VERBOSE) {
			printf("isGameOver(): PASS No out of bound changes made to game state struct.\n");
		}
	
		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}

	/* Display results three empty supply piles */
	printf("isGameOver(): ");
	if (errors) {
		printf("FAIL when three supply piles empty./n");
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when three supply piles empty.\n");
	}

	/* Test with four empty piles */
	for (i = 0; i < 8; i++) {	// Step through each combination in array
		state.supplyCount[quadTest[i][0]] = 0;	// Empty the supply of chosen types
		state.supplyCount[quadTest[i][1]] = 0;	
		state.supplyCount[quadTest[i][2]] = 0;	
		state.supplyCount[quadTest[i][3]] = 0;	

		if (state.supplyCount[province]) {	// If province supply not empty
			if (isGameOver(&state)) {	// If game over
				if (VERBOSE) {
					printf("isGameOver(): PASS Game over with four non-province supply piles empty.\n");
				}
			}
			else { 
				if (VERBOSE) {
					printf("isGameOver(): FAIL Game not over with four non-province supply piles empty.\n");
				}
				errors++;
			}
		}
		else {	// If province supply empty
			if (isGameOver(&state)) {	// If game over
				if (VERBOSE) {
					printf("isGameOver(): PASS Game over with province and three other supply piles empty.\n");
				}
			}
			else {
				if (VERBOSE) {
					printf("isGameOver(): FAIL Game not over with province and three other supply piles empty.\n");
				}
				errors++;
			}
		}

		state.supplyCount[quadTest[i][0]] = control.supplyCount[quadTest[i][0]]; 	// Revert the supply of chosen types
		state.supplyCount[quadTest[i][1]] = control.supplyCount[quadTest[i][1]]; 	
		state.supplyCount[quadTest[i][2]] = control.supplyCount[quadTest[i][2]];	
		state.supplyCount[quadTest[i][3]] = control.supplyCount[quadTest[i][3]]; 
	
		/* Check for out of bound changes to game state */
		if (memcmp(&control, &state, sizeof(struct gameState))) {
			if (VERBOSE) {
				printf("isGameOver(): FAIL Out of bound changes made to game state struct.\n");
			}
			errors++;
		}
		else if (VERBOSE) {
			printf("isGameOver(): PASS No out of bound changes made to game state struct.\n");
		}
	
		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}

	/* Display results four empty supply piles */
	printf("isGameOver(): ");
	if (errors) {
		printf("FAIL when four supply piles empty.\n");
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when four supply piles empty.\n");
	}

	/* Test with all supply piles empty */
	for (j = curse; j <= treasure_map; j++) {	// Step through all cards (enum-ed)
		if (state.supplyCount[j] > -1)
		{	// Unused cards were initialized to -1
			state.supplyCount[j] = 0;	// Set piles in play
		}
	}
	
	if (isGameOver(&state)) {	// Check for game over
		if (VERBOSE) {
			printf("isGameOver(): PASS Game over with all supply piles empty.\n");
		}
	}
	else { 
		if (VERBOSE) {
			printf("isGameOver(): FAIL Game not over with all supply piles empty.\n");
		}
		errors++;
	}
	
	for (j = curse; j <= treasure_map; j++) {	// Step through all cards (enum-ed)
		if (state.supplyCount[j] > -1)
		{	// Unused cards were set to -1
			state.supplyCount[j] = control.supplyCount[j];	// Revert used cards
		}
	}
	
	/* Check for out of bound changes to game state */
	if (memcmp(&control, &state, sizeof(struct gameState))) {
		if (VERBOSE) {
			printf("isGameOver(): FAIL Out of bound changes made to game state struct.\n");
		}
		errors++;
	}
	else if (VERBOSE) {
		printf("isGameOver(): PASS No out of bound changes made to game state struct.\n");
	}

	memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	

	/* Display results no empty supply */
	printf("isGameOver(): ");
	if (errors) {
		printf("FAIL when all supply piles empty.\n");
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when all supply piles empty.\n");
	}

	/* Display final results */
	if (!final) {
		printf("isGameOver(): ALL TESTS PASSED.\n");
	}

	return 0;
}

