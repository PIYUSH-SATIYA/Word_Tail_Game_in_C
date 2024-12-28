#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define WORD_MAX 50
#define DICT_SIZE 10
#define MAX_TURNS 50

void explain_game_rules();

int game(char *player_input, int max_len);

int main()
{
    char player_a[32], player_b[32];
    char word_collection[DICT_SIZE][WORD_MAX] = {
        "galaxy", "rhythm", "mystic", "flame",
        "echo", "quantum", "silver", "nebula",
        "whisper", "tornado"};
    char used_word_tracker[MAX_TURNS][WORD_MAX];
    int total_used_words = 0;

    printf("Let's set up our word warriors!\n");
    printf("Enter first player's name: ");
    fgets(player_a, sizeof(player_a), stdin);
    player_a[strcspn(player_a, "\n")] = '\0';

    printf("Enter second player's name: ");
    fgets(player_b, sizeof(player_b), stdin);
    player_b[strcspn(player_b, "\n")] = '\0';

    explain_game_rules();

    srand(time(NULL));
    char current_word[WORD_MAX];
    strcpy(current_word, word_collection[rand() % DICT_SIZE]);
    printf("Starting word chosen: %s\n\n", current_word);

    int current_turn = 0;
    while (1)
    {
        char *active_player = (current_turn % 2 == 0) ? player_a : player_b;
        printf("%s, your move!\n", active_player);

        char new_word[WORD_MAX];
        if (!game(new_word, sizeof(new_word)))
        {
            printf("%s couldn't keep up! Game over.\n", active_player);
            break;
        }

        if (new_word[0] != current_word[strlen(current_word) - 1])
        {
            printf("Word doesn't follow the rule! %s is out.\n", active_player);
            break;
        }

        int word_repeated = 0;
        for (int i = 0; i < total_used_words; i++)
        {
            if (strcmp(new_word, used_word_tracker[i]) == 0)
            {
                word_repeated = 1;
                break;
            }
        }

        if (word_repeated)
        {
            printf("Duplicate word! %s loses.\n", active_player);
            break;
        }

        strcpy(current_word, new_word);
        strcpy(used_word_tracker[total_used_words++], new_word);
        current_turn++;
    }

    FILE *game_log = fopen("score.txt", "a");
    if (!game_log)
    {
        printf("Couldn't save game results.\n");
        return 1;
    }

    fprintf(game_log, "Match Details:\n");
    fprintf(game_log, " * Player 1: %s\n", player_a);
    fprintf(game_log, " * Player 2: %s\n", player_b);
    fprintf(game_log, " * Winner: %s\n\n",
            (current_turn % 2 == 0) ? player_b : player_a);
    fclose(game_log);

    printf("Game completed. Results logged.\n");
    return 0;
}

void explain_game_rules()
{
    printf("Word Relay Game - How to Play:\n");
    printf("------------------------------\n");
    printf("Get ready for a brain-teasing challenge!\n\n");
    printf("Basic rules:\n");
    printf(" * We start with a random word\n");
    printf(" * Your word MUST begin with the last letter of previous word\n");
    printf(" * No repeating words allowed!\n");
    printf(" * You've got just 10 seconds per turn\n");
    printf(" * Mess up? You're out!\n\n");
    printf("Sharp minds win. Think fast!\n\n");
}

int game(char *player_input, int max_len)
{
    time_t start_moment = time(NULL);
    int input_length = 0;

    while (difftime(time(NULL), start_moment) < 60)
    {
        printf("\rTime remaining: %d: ", 60 - (int)difftime(time(NULL), start_moment));

        if (kbhit())
        {
            char single_char = getch();

            if (single_char == '\r' || single_char == '\n')
            {
                player_input[input_length] = '\0';
                printf("\n");
                return 1;
            }

            if (input_length < max_len - 1)
            {
                printf("%c", single_char);
                player_input[input_length++] = single_char;
            }
        }
    }

    printf("\nOops! Time's up!\n");
    return 0;
}