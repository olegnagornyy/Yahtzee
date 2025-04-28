/*
 * 04/28/2025
 * Oleg Nagornyy
 * Proposal Project
 * Yahtzee Game
 * Github repository : https://github.com/olegnagornyy/Yahtzee.git
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

// Constants
const int NONE = -1;
const int ONES = 0, TWOS = 1, THREES = 2, FOURS = 3, FIVES = 4, SIXES = 5,
          THREE_OF_A_KIND = 6, FOUR_OF_A_KIND = 7, FULL_HOUSE = 8,
          SMALL_STRAIGHT = 9, LARGE_STRAIGHT = 10, CHANCE = 11, YAHTZEE = 12,
          SUBTOTAL = 13, BONUS = 14, TOTAL = 15;

// Function Declarations
void ResetScorecard(vector<int>& scorecard, int& scorecardCount);
void UpdateScorecard(vector<int>& scorecard);
void DisplayScoreCards(const vector<int>& uScorecard, const vector<int>& cScorecard);
void Roll(int numberOfDice, vector<int>& dice);
void DisplayDice(const vector<int>& dice);
int GetComputerScorecardItem(const vector<int>& scorecard, const vector<int>& keeping);
void ComputerPlay(vector<int>& scorecard, int& scorecardCount);
void UserPlay(vector<int>& scorecard, int& scorecardCount);
string FormatCell(int cell);
void Count(const vector<int>& dice, vector<int>& counts);
bool GetCounts(const vector<int>& dice, vector<int>& counts);
int Score(int category, const vector<int>& dice);
int Sum(const vector<int>& dice);
bool HasStraight(const vector<int>& counts, int length);


int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<int> userScorecard(16);
    vector<int> computerScorecard(16);
    int userScorecardCount = 0;
    int computerScorecardCount = 0;
    ResetScorecard(userScorecard, userScorecardCount);
    ResetScorecard(computerScorecard, computerScorecardCount);
    while (userScorecardCount < 13 || computerScorecardCount < 13) {
        // User Turn
        if (userScorecardCount < 13) {
            DisplayScoreCards(userScorecard, computerScorecard);
            cout << "\nYour Turn!" << endl;
            UserPlay(userScorecard, userScorecardCount);
            UpdateScorecard(userScorecard);
        }
        // Computer Turn
        if (computerScorecardCount < 13) {
            DisplayScoreCards(userScorecard, computerScorecard);
            cout << "\nComputer's Turn!" << endl;
            ComputerPlay(computerScorecard, computerScorecardCount);
            UpdateScorecard(computerScorecard);
            cout << "Press Enter to continue..." << endl;
            cin.ignore();
            cin.get();
        }
    }
    // Final Scores
    DisplayScoreCards(userScorecard, computerScorecard);
    cout << endl;
    if (userScorecard[TOTAL] > computerScorecard[TOTAL])
        cout << "You win" << endl;
    else if (userScorecard[TOTAL] < computerScorecard[TOTAL])
        cout << "Computer wins" << endl;
    else
        cout << "Its a tie" << endl;
    cout << "Game Over" << endl;
    cin.ignore();
    cin.get();

    return 0;
}

// Functions
void ResetScorecard(vector<int>& scorecard, int& scorecardCount) {
    fill(scorecard.begin(), scorecard.end(), NONE);
    scorecardCount = 0;
}

void UpdateScorecard(vector<int>& scorecard) {
    scorecard[SUBTOTAL] = 0;
    scorecard[BONUS] = 0;
    for (int i = ONES; i <= SIXES; i++)
        if (scorecard[i] != NONE)
            scorecard[SUBTOTAL] += scorecard[i];

    if (scorecard[SUBTOTAL] >= 63)
        scorecard[BONUS] = 35;

    scorecard[TOTAL] = scorecard[SUBTOTAL] + scorecard[BONUS];
    for (int i = THREE_OF_A_KIND; i <= YAHTZEE; i++)
        if (scorecard[i] != NONE)
            scorecard[TOTAL] += scorecard[i];
}
// Displays both the user`s and computer`s scorecards
void DisplayScoreCards(const vector<int>& uScorecard, const vector<int>& cScorecard) {
    string labels[] = {"0-Ones", "1-Twos", "2-Threes", "3-Fours", "4-Fives", "5-Sixes",
                       "6-3 of a Kind", "7-4 of a Kind", "8-Full House", "9-Small Straight",
                       "10-Large Straight", "11-Chance", "12-Yahtzee", "Sub Total", "Bonus", "Total Score"};

    string border(39, '-');
    cout << border << endl;
    cout << "|    Category     |   You  | Computer|" << endl;
    cout << border << endl;
    for (int i = ONES; i <= SIXES; i++)
        cout << "| " << setw(16) << left << labels[i] << "|"
             << setw(7) << right << FormatCell(uScorecard[i]) << "|"
             << setw(9) << FormatCell(cScorecard[i]) << "|" << endl;

    cout << border << endl;
    cout << "| " << setw(16) << left << labels[SUBTOTAL] << "|"
         << setw(7) << right << FormatCell(uScorecard[SUBTOTAL]) << "|"
         << setw(9) << FormatCell(cScorecard[SUBTOTAL]) << "|" << endl;

    cout << border << endl;
    cout << "| " << setw(16) << left << labels[BONUS] << "|"
         << setw(7) << right << FormatCell(uScorecard[BONUS]) << "|"
         << setw(9) << FormatCell(cScorecard[BONUS]) << "|" << endl;

    cout << border << endl;
    for (int i = THREE_OF_A_KIND; i <= YAHTZEE; i++)
        cout << "| " << setw(16) << left << labels[i] << "|"
             << setw(7) << right << FormatCell(uScorecard[i]) << "|"
             << setw(9) << FormatCell(cScorecard[i]) << "|" << endl;

    cout << border << endl;
    cout << "| " << setw(16) << left << labels[TOTAL] << "|"
         << setw(7) << right << FormatCell(uScorecard[TOTAL]) << "|"
         << setw(9) << FormatCell(cScorecard[TOTAL]) << "|" << endl;
    cout << border << endl;
}

void Roll(int numberOfDice, vector<int>& dice) {
    dice.clear();
    for (int i = 0; i < numberOfDice; i++)
        dice.push_back(rand() % 6 + 1);
}

void DisplayDice(const vector<int>& dice) {
    string border = "*------*";
    string empty = "|      |";
    string format = "|   {}  |";

    for (int die : dice)
        cout << border;
    cout << endl;

    for (int die : dice)
        cout << empty;
    cout << endl;

    for (int die : dice)
        cout << "|   " << die << "  |";
    cout << endl;

    for (int die : dice)
        cout << empty;
    cout << endl;

    for (int die : dice)
        cout << border;
    cout << endl;
}

int GetComputerScorecardItem(const vector<int>& scorecard, const vector<int>& keeping) {
    int bestCategory = 0;
    int bestScore = -1;

    for (int i = ONES; i <= YAHTZEE; i++) {
        if (scorecard[i] == NONE) {
            int currentScore = Score(i, keeping);
            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestCategory = i;
            }
        }
    }
    return bestCategory;
}
// Computer`s turn
void ComputerPlay(vector<int>& scorecard, int& scorecardCount) {
    vector<int> dice;
    Roll(5, dice);
    vector<int> keeping = dice;

    int chosenCategory = GetComputerScorecardItem(scorecard, keeping);
    scorecard[chosenCategory] = Score(chosenCategory, keeping);
    scorecardCount++;
}
// User`s turn
void UserPlay(vector<int>& scorecard, int& scorecardCount) {
    vector<int> dice;
    vector<int> keeping;

    Roll(5, dice);
    keeping = dice;

    DisplayDice(dice);

    int chosenCategory;
    do {
        cout << "Enter category 0-12 to score: ";
        cin >> chosenCategory;
        if (cin.fail() || chosenCategory < 0 || chosenCategory > 12 || scorecard[chosenCategory] != NONE) {
            cin.clear();
            cin.ignore(10000, '\n');
            chosenCategory = -1;
        }
    } while (chosenCategory == -1);

    scorecard[chosenCategory] = Score(chosenCategory, keeping);
    scorecardCount++;
}

string FormatCell(int cell) {
    return (cell == NONE) ? "-" : to_string(cell);
}

void Count(const vector<int>& dice, vector<int>& counts) {
    fill(counts.begin(), counts.end(), 0);
    for (int die : dice)
        counts[die - 1]++;
}

bool GetCounts(const vector<int>& dice, vector<int>& counts) {
    counts.resize(6);
    Count(dice, counts);
    return true;
}
// Calculating score for given categories based on current dice values
int Score(int category, const vector<int>& dice) {
    vector<int> counts(6);
    GetCounts(dice, counts);

    switch (category) {
        case ONES: return counts[0] * 1;
        case TWOS: return counts[1] * 2;
        case THREES: return counts[2] * 3;
        case FOURS: return counts[3] * 4;
        case FIVES: return counts[4] * 5;
        case SIXES: return counts[5] * 6;
        case THREE_OF_A_KIND:
            for (int i = 0; i < 6; i++)
                if (counts[i] >= 3)
                    return Sum(dice);
            return 0;
        case FOUR_OF_A_KIND:
            for (int i = 0; i < 6; i++)
                if (counts[i] >= 4)
                    return Sum(dice);
            return 0;
        case FULL_HOUSE: {
            bool two = false, three = false;
            for (int i = 0; i < 6; i++) {
                if (counts[i] == 2) two = true;
                if (counts[i] == 3) three = true;
            }
            return (two && three) ? 25 : 0;
        }
        case SMALL_STRAIGHT: return HasStraight(counts, 4) ? 30 : 0;
        case LARGE_STRAIGHT: return HasStraight(counts, 5) ? 40 : 0;
        case CHANCE: return Sum(dice);
        case YAHTZEE:
            for (int i = 0; i < 6; i++)
                if (counts[i] == 5)
                    return 50;
            return 0;
        default: return 0;
    }
}
// Return sum of all dice values
int Sum(const vector<int>& dice) {
    int sum = 0;
    for (int die : dice)
        sum += die;
    return sum;
}

bool HasStraight(const vector<int>& counts, int length) {
    int inARow = 0;
    for (int i = 0; i < 6; i++) {
        if (counts[i] > 0) {
            inARow++;
            if (inARow >= length)
                return true;
        } else
            inARow = 0;
    }
    return false;
}
