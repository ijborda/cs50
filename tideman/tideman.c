#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool cycle_check(int src, int dest, int start);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j)
            {
                preferences[ranks[i]][ranks[j]] = 0;
            }
            else if (i < j)
            {
                preferences[ranks[i]][ranks[j]] = preferences[ranks[i]][ranks[j]] + 1;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] == 0)
            {
                pair_count = pair_count + 0;
            }
            else if (preferences[i][j] == preferences[j][i])
            {
                pair_count = pair_count + 0;
            }
            else if (preferences[i][j] != preferences[j][i] && i < j)
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                }
                else
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                }
                pair_count = pair_count + 1;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO

    int current_mag = 0;
    int highest_pair = 0;
    int highest_winner = 0;
    int highest_loser = 0;
    for (int i = 0; i < pair_count; i++)
    {
        int highest_mag = 0;
        // Identify the current highest
        for (int j = i; j < pair_count; j++)
        {
            current_mag = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            if (current_mag >= highest_mag)
            {
                highest_mag = current_mag;
                highest_pair = j;
                highest_winner = pairs[j].winner;
                highest_loser = pairs[j].loser;
            }
        }
        // Switch the current highest to the first unarranged index
        pairs[highest_pair].winner = pairs[i].winner;
        pairs[highest_pair].loser = pairs[i].loser;
        pairs[i].winner = highest_winner;
        pairs[i].loser = highest_loser;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        // If next pair would create cycle, skip. Otherwise, continue locking until there are still unassigned pairs.
        if (!(cycle_check(pairs[i].winner, pairs[i].loser, pairs[i].winner)))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO

    for (int i = 0; i < candidate_count; i++)
    {
        int final_col_false = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            // Count false in each column
            if (locked[j][i] == false)
            {
                final_col_false = final_col_false + 1;
            }

            // If all entries in a column is false, print that column as the winner
            if (final_col_false == candidate_count)
            {
                printf("%s\n", candidates[i]);
            }
        }
    }
    return;
}

// Checks for cycle
// Return true if next pair would create cycle. Otherwise, return false.
bool cycle_check(int src, int dest, int start)
{
    // Recursive (base case)
    if (dest == start)
    {
        return true;
    }

    // Recursion
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[dest][i])
        {
            if (cycle_check(dest, i, start))
            {
                return true;
            }
        }
    }
    return false;
}