/* The thermostat of Heater, its contract in ACSL (Frama-C). */

/*@ requires 0 <= T <= 200;
    ensures T < 74 ==> \result == 3;
    ensures T >= 74 ==> \result == 0;
*/
int thermostat(int T)
{
    if (T < 74)
        return 3;
    return 0;
}

/*@ requires \valid(y);
    requires -100 <= x <= 100;
    assigns *y;
    ensures 0 <= *y <= 50;
    ensures (x >= 0 && x <= 50) <==> *y == x;
*/
void clamp(int x, int *y)
{
    *y = x < 0 ? 0 : (x > 50 ? 50 : x);
}

//@ ensures \result >= 0;
int square_plus(int a)
{
    return a * a + 1;
}
