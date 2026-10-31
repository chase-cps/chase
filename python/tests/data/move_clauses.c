/* The move of a drone of DroneGrid (GridDroneSim, MDFS), on a 10 x 10
   grid: its contract in the clauses of CBMC. A move is 0 stay, 1 right,
   2 left, 3 down, 4 up. */

void move(int x, int y, int g, int *nx, int *ny)
    __CPROVER_requires(x >= 0 && x <= 9 && y >= 0 && y <= 9 && g >= 0 && g <= 4)
    __CPROVER_requires(__CPROVER_is_fresh(nx, sizeof(int)) && __CPROVER_is_fresh(ny, sizeof(int)))
    __CPROVER_assigns(*nx, *ny)
    __CPROVER_ensures(*nx >= 0 && *nx <= 9 && *ny >= 0 && *ny <= 9)
    __CPROVER_ensures(g != 1 || x == 9 || *nx == x + 1)
{
    *nx = x;
    *ny = y;
    if (g == 1 && x < 9) *nx = x + 1;
    else if (g == 2 && x > 0) *nx = x - 1;
    else if (g == 3 && y < 9) *ny = y + 1;
    else if (g == 4 && y > 0) *ny = y - 1;
}

/* The same move without the test at the edge of the grid: the contract
   says the drone stays on the grid, and it does not. */
void move_fast(int x, int y, int g, int *nx, int *ny)
    __CPROVER_requires(x >= 0 && x <= 9 && y >= 0 && y <= 9 && g >= 0 && g <= 4)
    __CPROVER_requires(__CPROVER_is_fresh(nx, sizeof(int)) && __CPROVER_is_fresh(ny, sizeof(int)))
    __CPROVER_assigns(*nx, *ny)
    __CPROVER_ensures(*nx >= 0 && *nx <= 9 && *ny >= 0 && *ny <= 9)
{
    *nx = x;
    *ny = y;
    if (g == 1) *nx = x + 1;
    else if (g == 2 && x > 0) *nx = x - 1;
    else if (g == 3 && y < 9) *ny = y + 1;
    else if (g == 4 && y > 0) *ny = y - 1;
}
