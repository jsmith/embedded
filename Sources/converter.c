#include <stdio.h>
#include <math.h>

/**
 * Reverses a string 'str' of length 'len'
 */
void reverse(char *str, int len) {
    for (int i = 0; i < len/2; i++) {
        int j = len - 1 - i;
        int temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

/**
 * Converts a given integer x to string str[].  d is the number
 * of digits required in output. If d is more than the number
 * of digits in x, then 0s are added at the beginning. Returns
 * the length of the array.
 */
int int_to_str(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i] = (x % 10) + '0'; // Get last digit and convert to a char
        x = x / 10;
        i++;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    for (; i < d; i++) {
        str[i] = '0';
    }

    reverse(str, i);
    str[i] = '\0';
    return i;
}

/**
 * Converts a floating point number to string.
 */ 
void dtoa(double n, char *res, int afterpoint) {
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    double fpart = n - (float)ipart;

    // convert integer part to string
    int i = int_to_str(ipart, res, 1);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        int_to_str((int)fpart, res + i + 1, afterpoint);
    }
}
