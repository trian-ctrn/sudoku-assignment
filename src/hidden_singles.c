#include "hidden_singles.h"
#include <stddef.h>
#include <stdlib.h>
int findIndex(Cell **arr, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i]->value == value) {
            return i;
        }
    }
    return -1;
}

int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
{
    int num_hidden_singles = 0;

    for (int values = 1; values <= BOARD_SIZE; values++)
    {
        int count = 0;
        Cell *hidden_single_cell = NULL;

        for (int i = 0; i < BOARD_SIZE; i++)
        {

            if (p_cells[i]->num_candidates > 1 && (is_candidate(p_cells[i], values)))
            {
                count++;
                hidden_single_cell = p_cells[i];
            }
        }

        if (count == 1) {
            hidden_single_values[num_hidden_singles++] = values;
            hidden_single_cell->value = values;
        }
    }

    return num_hidden_singles;
}

void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_single, int *p_counter)
{
    int *hidden_single_values = malloc(BOARD_SIZE * sizeof(int));
    int num_hidden_singles = find_hidden_single_values(p_cells, hidden_single_values);

    for (int i = 0; i < num_hidden_singles; i++) {
        bool is_duplicate = false;

        for (int j = 0; j < *p_counter; j++)
        {
            if (p_hidden_single[j].p_cell == p_cells[findIndex(p_cells, BOARD_SIZE, hidden_single_values[i])])
            {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate)
        {

            p_hidden_single[*p_counter].value = hidden_single_values[i];
            p_hidden_single[*p_counter].p_cell =  p_cells[findIndex(p_cells, BOARD_SIZE, hidden_single_values[i])];
            (*p_counter)++;
        }
    }

    
    free(hidden_single_values);
}

int hidden_singles(SudokuBoard *p_board)
{
    HiddenSingle hidden_singles[BOARD_SIZE * 3];
    int counter = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &counter);
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_single(p_board->p_cols[i], hidden_singles, &counter);
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_single(p_board->p_boxes[i], hidden_singles, &counter);
    }

    for (int i = 0; i < counter; i++){
        Cell *p_cell = hidden_singles[i].p_cell;
        int* candidate_array = get_candidates(p_cell);
        int num_candidates = p_cell->num_candidates;
        for (int j = 0; j < num_candidates; j++){
            if (candidate_array[j] != hidden_singles[i].value)
            {
                unset_candidate(p_cell, candidate_array[j]);
            }
        }
        free(candidate_array);
    }
    return counter;
}
