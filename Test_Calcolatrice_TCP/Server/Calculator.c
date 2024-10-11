#include "Calculator.h"

/**
 * @file Calculator.c
 * @brief Implementation file for a simple calculator.
 * @date 13 Nov 2023
 * @author Francesco Conforti
 */

/**
 * @brief Performs addition operation.
 *
 * This function takes two operands and returns their sum.
 *
 * @param a The first operand.
 * @param b The second operand.
 * @return The result of the addition operation.
 */
double add(double a, double b) {
    return a + b;
}

/**
 * @brief Performs multiplication operation.
 *
 * This function takes two operands and returns their product.
 *
 * @param a The first operand.
 * @param b The second operand.
 * @return The result of the multiplication operation.
 */
double mult(double a, double b) {
    return a * b;
}

/**
 * @brief Performs subtraction operation.
 *
 * This function takes two operands and returns the result of subtracting
 * the second operand from the first.
 *
 * @param a The first operand.
 * @param b The second operand.
 * @return The result of the subtraction operation.
 */
double sub(double a, double b) {
    return a - b;
}

/**
 * @brief Performs division operation.
 *
 * This function takes a numerator and a denominator and returns the result
 * of dividing the numerator by the denominator. If the denominator is zero,
 * the function returns 0.0 as an error handling measure.
 *
 * @param a The numerator.
 * @param b The denominator.
 * @return The result of the division operation, or 0.0 if division by zero.
 */
double division(double a, double b) {
    if (b != 0) {
        return a / b;
    } else {
        return 0.0; // Error handling: Division by zero
    }
}
