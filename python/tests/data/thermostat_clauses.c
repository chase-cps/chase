/* The thermostat of Heater, one call: its contract in the clauses of
   ESBMC. Open the file with chase-cps view (or in VS Code): each function
   is a box, checked against its clauses. */

int thermostat(int T)
{
    __ESBMC_requires(T >= 0 && T <= 200);
    __ESBMC_ensures(T >= 74 || __ESBMC_return_value == 3);
    __ESBMC_ensures(T < 74 || __ESBMC_return_value == 0);
    if (T < 74)
        return 3;
    return 0;
}

/* The heat per square meter of a room. No clause: the code alone is
   checked (no overflow, division by zero, access out of bounds, or failed
   assert). */
int heat_per_area(int power, int area)
{
    return power / area;
}
