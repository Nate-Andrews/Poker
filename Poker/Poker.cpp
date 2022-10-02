#include <iostream>
#include <cstdlib>
#include <stdlib.h> 
#include <string>
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <vector>
using namespace std;

int game(int[], int, int, int);
int get_chip(int);
int get_bet(int);
int get_diff(int);
void gotoxy(int, int);
void chip_area(int[], int [], int, int);
int shuffle_deck(int[]);
string show_cards(int);
int card_value(int);
string card_suit(int);
int opponent_AI(int, int);
int hand_check(int, int, int, int, int);
void winnings(int[], int[], bool[], int);
bool royal_flush(int[]);
bool straight_flush(int[]);
bool four_kind(int[]);
bool full_house(int[]);
bool flush(int[]);
bool straight(int[]);
bool three_kind(int[]);
bool two_pair(int[]);
bool apair(int[]);
void bubble_sort(int[]);

int main()
{
	int chips = 2000, bet = 20, difficulty = 2, pot = 0;
	
	cout << "Poker" << endl;
	cout << "By: Nathan Andrews" << endl;
	
	cout << "\nHand Priority" << endl;
	cout << "(9) Royal Flush:     This hand includes a 10, Jack, Queen, King, and Ace of the same suit." << endl;
	cout << "(8) Straight Flush:  This hand is made up of 5 consecutive cards of the same suit." << endl;
	cout << "(7) Four of a Kind:  This means you have 4 cards of the same rank but different suits and a fifth card of any rank." << endl;
	cout << "(6) Full House:      This hand contains 3 matching cards of 1 rank and 2 matching cards of another rank." << endl;
	cout << "(5) Flush:           This hand contains any 5 cards of the same suit. They can skip around in rank or sequence." << endl;
	cout << "(4) Straight:        This hand contains 5 cards of consecutive rank but from more than one suit." << endl;
	cout << "(3) Three of a Kind: This means you have 3 cards of the same rank, plus two unmatched cards." << endl;
	cout << "(2) Two Pair:        This is made up of two cards of one rank, plus two cards of another rank." << endl;
	cout << "(1) One Pair:        This means you have 2 cards of the same rank, plus 3 other unmatched cards." << endl;
	cout << "(0) No Hand:         This means you have five cards that are not consecutive, and they are not all from the same suit." << endl;

	cout << "\nIf more than one player has the same winning hand all the players with the hand win. The pot will be divided evenly between all winners. This is true if all players have a Royal Flush or all have No Hand." << endl;

	cout << "\nIf a player folds and they have the winning hand their winnings will go to the pot. It is the same for multiple winners as well. The pot will be divided evenly but the folded players portion will remain in the pot." << endl;
	cout << "\nSuits: H = Hearts, D = Diamond, S = Spade and C = Clubs" << endl;
	cout << "Faces: A = Ace, T = 10, J = Jack, Q = Queen and K = King" << endl;

	chips = get_chip(chips);
	bet = get_bet(bet);
	difficulty = get_diff(difficulty);

	int playerChips[5] = { chips, chips, chips, chips, chips };

	system("cls");

	game(playerChips, bet, pot, difficulty);

	return 0;
}
int get_chip(int chips)
{
	cout << "\nStarting number of chip (default is 2000): ";
	cin >> chips;

	if (chips < 0)
	{
		return 2000;
	}
	else
		return chips;
}
int get_bet(int bet)
{
	cout << "Bet Limit (default is 10): ";
	cin >> bet;

	if (bet <= 0)
	{
		return 10;
	}
	else
		return bet;
}
int get_diff(int diff)
{
	cout << "Opponent Dificulty Easy(1), Medium(2), Hard(3): ";
	cin >> diff;

	if (diff != 1 && diff != 2 && diff != 3)
	{
		get_diff(diff);
		return 1;
	}
	else
		return diff;
}
int game(int playerChips[], int betLimit, int pot, int diff)
{
	system("COLOR 27");

	int deck[52], i, j = 1, count, card, line, ans, setBet = betLimit;

	char replay;

	int hands[5][5] = {
							{},
							{},
							{},
							{},
							{} };


	string fakeHand[] = { "[??]", "[??]", "[??]", "[??]", "[??]" };

	string winningHands[10] = { "No Hand", "One Pair", "Two Pair", "Three of a Kind", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush", "Royal Flush" };

	string erase = "                                                                                                           ";

	int handRanks[5];

	int potOwe[5] = { 0, 0, 0, 0, 0 };

	bool fold[] = { false, false, false, false, false };

	shuffle_deck(deck);

	// deal cards to players
	j = 0;

	for (i = 0; i < 5; i++)
	{
		hands[0][i] = deck[j];
		j++;

		hands[1][i] = deck[j];
		j++;

		hands[2][i] = deck[j];
		j++;

		hands[3][i] = deck[j];
		j++;

		hands[4][i] = deck[j];
		j++;
	}

	// set up the initial board
	line = 0;

	gotoxy(0, line);

	cout << setw(15) << "PLAYER 1" << setw(15) << "PLAYER 2" << setw(15) << "PLAYER 3" << setw(15)
		<< "PLAYER 4" << setw(18) << "PLAYER 5(you)" << endl;

	for (i = 0; i < 5; i++)
	{
		gotoxy(0, i + 1);

		cout << setw(15) << fakeHand[i] << setw(15) << fakeHand[i] << setw(15) << fakeHand[i] << setw(15)
			<< fakeHand[i] << setw(18) << show_cards(hands[4][i]) << endl;
	}

	// Display the chip area
	chip_area(playerChips, potOwe, pot, betLimit);
	
	for (i = 0; i < 5; i++)
	{
		handRanks[i] = hand_check(hands[i][0], hands[i][1], hands[i][2], hands[i][3], hands[i][4]);
	}

	for (count = 1; count <= 4; count++)
	{
		gotoxy(0, 13);
		cout << "Turn " << count << "/4" << endl;
		line = 14;
		for (i = 0; i < 5; i++)
		{
			if (fold[i] == true)
			{
				gotoxy(0, line);
				ans = 0;
			}
			else if (i >= 0 && i <= 3)
			{
				gotoxy(0, line);
				cout << "Player " << i + 1 << " do you want to Check/Call(1), Raise(2) or Fold(3)";
				Sleep(2000);
				ans = opponent_AI(diff, handRanks[i]);
				cout << ans;
			}
			else
			{
				gotoxy(0, line);
				cout << "Player 5 do you want to Check/Call(1), Raise(2) or Fold(3)";
				cin >> ans;
			}

			if (ans == 1)
			{
				if (potOwe[i] == 0)
				{
					potOwe[i] = betLimit;
				}
				playerChips[i] -= betLimit;
				pot += betLimit;
			}
			else if (ans == 2)
			{
				betLimit += setBet;
				playerChips[i] -= betLimit;
				potOwe[i] = betLimit;
			}
			else if (ans == 3)
			{
				fold[i] = true;
			}
			else
				cout << "Player " << i + 1 << " Folded";
			chip_area(playerChips, potOwe, pot, betLimit);
			line++;
			Sleep(3000);
		}
		gotoxy(0, 14);
		cout << erase << endl;
		cout << erase << endl;
		cout << erase << endl;
		cout << erase << endl;
		cout << erase << endl;
	}

	line = 0;
	// show opponet hands
	for (i = 0; i < 5; i++)
	{
		gotoxy(0, i + 1);

		cout << setw(15) << show_cards(hands[0][i])
			<< setw(15) << show_cards(hands[1][i])
			<< setw(15) << show_cards(hands[2][i])
			<< setw(15) << show_cards(hands[3][i])
			<< setw(18) << show_cards(hands[4][i]) << endl;
	}

	chip_area(playerChips, potOwe, pot, betLimit);

	gotoxy(0, 14);
	cout << erase << endl;
	cout << erase << endl;
	cout << erase << endl;
	cout << erase << endl;
	cout << erase << endl;

	gotoxy(0, 14);
	cout << "Player 1 " << winningHands[handRanks[0]] << endl;
	cout << "Player 2 " << winningHands[handRanks[1]] << endl;
	cout << "Player 3 " << winningHands[handRanks[2]] << endl;
	cout << "Player 4 " << winningHands[handRanks[3]] << endl;
	cout << "Player 5 " << winningHands[handRanks[4]] << endl;

	gotoxy(0, 20);
	winnings(handRanks, playerChips, fold, pot);
	chip_area(playerChips, potOwe, pot, betLimit);

	gotoxy(0, 22);
	cout << "Want to play again? (y/n)";
	cin >> replay;
	
	if (replay == 'y')
	{
		system("cls");
		game(playerChips, setBet, pot, diff);
	}
	else
	{
		exit(0);
	}

	return 0;
}
void gotoxy(int h, int w)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (INVALID_HANDLE_VALUE != hConsole)
	{
		COORD pos = { h,w };
		SetConsoleCursorPosition(hConsole, pos);
	}
	return;
}
void chip_area(int playerChips[], int potOwe[], int pot, int betLimit)
{
	gotoxy(7, 7);
	cout << "C: " << playerChips[0];
	gotoxy(23, 7);
	cout << "C: " << playerChips[1];
	gotoxy(38, 7);
	cout << "C: " << playerChips[2];
	gotoxy(53, 7);
	cout << "C: " << playerChips[3];
	gotoxy(65, 7);
	cout << "C: " << playerChips[4];

	gotoxy(7, 8);
	cout << "O: " << potOwe[0];
	gotoxy(23, 8);
	cout << "O: " << potOwe[1];
	gotoxy(38, 8);
	cout << "O: " << potOwe[2];
	gotoxy(53, 8);
	cout << "O: " << potOwe[3];
	gotoxy(65, 8);
	cout << "O: " << potOwe[4];

	gotoxy(0, 10);
	cout << "Pot: " << pot << endl;

	gotoxy(0, 11);
	cout << "Bet: " << betLimit << endl;
}
int shuffle_deck(int deck[])
{
	int picked[52] = { 0 }, i, card;

	// create random unigue numbers
	srand(time(NULL));

	for (i = 0; i < +52; i++)
	{
		do
		{
			card = rand() % 52;
		} while (picked[card]);

		deck[i] = card;
		picked[card] = 1;
	}

	return deck[i];
}
string show_cards(int deck)
{
	int value;
	string face = " ", suit, card;

	value = card_value(deck);

	suit = card_suit(deck);

	switch (value)
	{
		case 0: face = 'A'; value = 14;  break;
		case 1: value = 2; break;
		case 2: value = 3; break;
		case 3: value = 4; break;
		case 4: value = 5; break;
		case 5: value = 6; break;
		case 6: value = 7; break;
		case 7: value = 8; break;
		case 8: value = 9; break;
		case 9: face = "T";  value = 10; break;
		case 10: face = "J"; value = 11;  break;
		case 11: face = "Q"; value = 12;  break;
		case 12: face = "K"; value = 13;  break;
	}
	
	if (face == " ")
	{
		card = "[" + to_string(value) + suit + "]";
		return card;
	}
	else
	{
		card = "[" + face + suit + "]";
		return card;
	}
}
int card_value(int deck)
{
	int val;

	val = deck % 13;
	
	return val;
}
string card_suit(int deck)
{
	string suit;

	switch (deck / 13 + 3)
	{
	case 3:
		suit = 'H';
		break;
	case 4:
		suit = 'D';
		break;
	case 5:
		suit = 'S';
		break;
	case 6:
		suit = 'C';
		break;
	}

	return suit;
}
int opponent_AI(int diff, int rank) // Controls what the Opponents do during their turns
{
	srand(time(NULL));

	int random = rand() % 100;

	if (diff == 1) //  lowest difficulty and is completly random
	{
		if (random <= 40 && random >= 0)
		{
			return 1;
		}
		else if (random <= 70 && random >= 41)
		{
			return 2;
		}
		else if (random <= 100 && random >= 71)
		{
			return 3;
		}
	}
	if (diff == 2) // medium difficulty where the AI has some difficulty. Based on the rank of the hand the opponent has
	{
		if (rank == 0)
		{
			if (random <= 40 && random >= 0)
			{
				return 1;
			}
			else if (random <= 100 && random >= 41)
			{
				return 3;
			}
		}
		if (rank >= 1 && rank <= 6)
		{
			if (random <= 40 && random >= 0)
			{
				return 1;
			}
			else if (random <= 70 && random >= 41)
			{
				return 2;
			}
			else if (random <= 100 && random >= 71)
			{
				return 3;
			}
		}
		if (rank >= 7 && rank <= 9)
		{
			if (random <= 30 && random >= 0)
			{
				return 1;
			}
			else if (random <= 70 && random >= 31)
			{
				return 2;
			}
			else if (random <= 100 && random >= 71)
			{
				return 3;
			}
		}
	}
	if (diff == 3) //  highest difficulty which is heavily based on rank. More likely to pass or fold on lower ranks than higher ones
	{
		if (rank >= 0 && rank <= 3)
		{
			if (random <= 40 && random >= 0)
			{
				return 1;
			}
			else if (random <= 100 && random >= 41)
			{
				return 3;
			}
		}
		if (rank >= 4 && rank <= 6)
		{
			if (random <= 40 && random >= 0)
			{
				return 1;
			}
			else if (random <= 70 && random >= 41)
			{
				return 2;
			}
			else if (random <= 100 && random >= 71)
			{
				return 3;
			}
		}
		if (rank >= 7 && rank <= 8)
		{
			if (random <= 30 && random >= 0)
			{
				return 1;
			}
			else if (random <= 80 && random >= 31)
			{
				return 2;
			}
			else if (random <= 100 && random >= 81)
			{
				return 3;
			}
		}
		if (rank == 9)
		{
			if (random <= 40 && random >= 0)
			{
				return 1;
			}
			else if (random <= 100 && random >= 41)
			{
				return 2;
			}
		}
		return 1;
	}
}
int hand_check(int v1, int v2, int v3, int v4, int v5)
{
	int hand[5] = { v1, v2, v3, v4, v5 };

	if (royal_flush(hand) == true)
	{
		return 9;
	}
	else if (straight_flush(hand) == true)
	{
		return 8;
	}
	else if (four_kind(hand) == true)
	{
		return 7;
	}
	else if (full_house(hand) == true)
	{
		return 6;
	}
	else if (flush(hand) == true)
	{
		return 5;
	}
	else if (straight(hand) == true)
	{
		return 4;
	}
	else if (three_kind(hand) == true)
	{
		return 3;
	}
	else if (two_pair(hand) == true)
	{
		return 2;
	}
	else if (apair(hand) == true)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}
void winnings(int handRanks[], int playerChips[], bool fold[], int pot)
{
	int winner = 0, count = 0;

	//finds the highest hand rank
	for (int i = 0; i < 5; i++)
	{
		if (handRanks[i] < handRanks[i + 1])
		{
			winner = handRanks[i + 1];
		}
	}

	//checks if everyone won
	if (handRanks[0] == handRanks[1] && handRanks[1] == handRanks[2] && handRanks[2] == handRanks[3] && handRanks[3] == handRanks[4])
	{
		winner = handRanks[0];
	}

	// checks if there are multiple winners
	for (int i = 0; i < 5; i++)
	{
		if (handRanks[i] == winner)
		{
			count++;
		}
	}

	if (count == 1)
	{
		vector<int>winners;
		for (int i = 0; i < 5; i++)
		{
			if (handRanks[i] == winner)
			{
				winners.push_back(i);
			}
		}
		cout << "The winner is player " << winners[0] + 1 << endl;
		int dis = pot;
		pot = 0;
		if (fold[winners[0]] == true)
		{
			pot += dis;
		}
		else {
			playerChips[winners[0]] += dis;
		}
	}
	else if (count == 2)
	{
		vector<int>winners;
		for (int i = 0; i < 5; i++)
		{
			if (handRanks[i] == winner)
			{
				winners.push_back(i);
			}
		}
		cout << "The winners are Player " << winners[0] + 1 << " and Player " << winners[1] + 1 << endl;
		int dis = pot / 2;
		pot = 0;
		for (int i = 0; i < 2; i++)
		{
			if (fold[winners[i]] == true)
			{
				pot += dis;
			}
			else {
				playerChips[winners[i]] += dis;
			}
		}
	}
	else if (count == 3)
	{
		vector<int>winners;
		for (int i = 0; i < 5; i++)
		{
			if (handRanks[i] == winner)
			{
				winners.push_back(i);
			}
		}
		cout << "The winners are Player " << winners[0] + 1 << ", Player " << winners[1] + 1 << " and Player " << winners[2] + 1 << endl;
		int dis = pot / 3;
		pot = 0;
		for (int i = 0; i < 3; i++)
		{
			if (fold[winners[i]] == true)
			{
				pot += dis;
			}
			else {
				playerChips[winners[i]] += dis;
			}
		}
	}
	else if (count == 4)
	{
		vector<int>winners;
		for (int i = 0; i < 5; i++)
		{
			if (handRanks[i] == winner)
			{
				winners.push_back(i);
			}
		}
		cout << "The winners are Player " << winners[0] + 1 << ", Player " << winners[1] + 1 << ", Player " << winners[2] + 1 << " and Player " << winners[3] + 1 << endl;
		int dis = pot / 4;
		pot = 0;
		for (int i = 0; i < 3; i++)
		{
			if (fold[winners[i]] == true)
			{
				pot += dis;
			}
			else {
				playerChips[winners[i]] += dis;
			}
		}
	}
	else if (count == 5)
	{
		cout << "Everyone won!" << endl;
		int dis = pot / 5;
		pot = 0;
		for (int i = 0; i < 4; i++)
		{
			if (fold[i] == true)
			{
				pot += dis;
			}
			else {
				playerChips[i] += dis;
			}
		}
	}
}
bool royal_flush(int hand[])
{
	bubble_sort(hand);

	if (flush(hand) == true && card_value(hand[0]) == 0 && card_value(hand[1]) == 9 && card_value(hand[2]) == 10 && card_value(hand[3]) == 11 && card_value(hand[4]) == 12)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool straight_flush(int hand[])
{
	if (flush(hand) == true && straight(hand) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool four_kind(int hand[])
{
	bubble_sort(hand);

	if (card_value(hand[0]) == card_value(hand[1]) && card_value(hand[1]) == card_value(hand[2]) && card_value(hand[2]) == card_value(hand[3]) ||
		card_value(hand[1]) == card_value(hand[2]) && card_value(hand[2]) == card_value(hand[3]) && card_value(hand[3]) == card_value(hand[4]))
	{
		return true;
	}
	else
		return false;
}
bool full_house(int hand[])
{
	bubble_sort(hand);

	if (card_value(hand[0]) == card_value(hand[1]) && card_value(hand[2]) == card_value(hand[3]) && card_value(hand[3]) == card_value(hand[4]) ||
		card_value(hand[3]) == card_value(hand[4]) && card_value(hand[0]) == card_value(hand[1]) && card_value(hand[1]) == card_value(hand[2]))
	{
		return true;
	}
	else
		return false;
}
bool flush(int hand[])
{
	if (card_suit(hand[0]) == card_suit(hand[1]) && card_suit(hand[1]) == card_suit(hand[2]) && card_suit(hand[2]) == card_suit(hand[3]) && card_suit(hand[3]) == card_suit(hand[4]))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool straight(int hand[])
{
	bubble_sort(hand);

	if (card_value(hand[0]) + 1 == card_value(hand[1]) && card_value(hand[1]) + 1 == card_value(hand[2]) && card_value(hand[2]) + 1 == card_value(hand[3]) && card_value(hand[3]) + 1 == card_value(hand[4]))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool three_kind(int hand[]) // function to check if there are three similar cards 3
{
	bubble_sort(hand);

	if (card_value(hand[0]) == card_value(hand[1]) && card_value(hand[1]) == card_value(hand[2]) ||
		card_value(hand[1]) == card_value(hand[2]) && card_value(hand[2]) == card_value(hand[3]) ||
		card_value(hand[2]) == card_value(hand[3]) && card_value(hand[3]) == card_value(hand[4]))
	{
		return true;
	}
	else
		return false;
}
bool two_pair(int hand[]) // function to check if there are two pairs 2
{
	int i, j, matchs = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = i + 1; j < 5; j++)
		{
			if (card_value(hand[i]) == card_value(hand[j]))
			{
				matchs++;
			}
		}
	}
	if (matchs == 2)
		return true;
	else
		return false;
}
bool apair(int hand[]) // function to check if there is one pair 1
{
	int i, j, matches = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = i + 1; j < 5; j++)
		{
			if (card_value(hand[i]) == card_value(hand[j]))
			{
				matches++;
				//return true;
			}
		}
	}
	if (matches == 1)
		return true;
	else
		return false;
}
void bubble_sort(int arr[]) // 
{
	int temp;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j + 1 < 5 - i; j++)
		{
			// Swaping the elements if first one
			// is greater than second one.
			if (card_value(arr[j]) > card_value(arr[j + 1]))
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}