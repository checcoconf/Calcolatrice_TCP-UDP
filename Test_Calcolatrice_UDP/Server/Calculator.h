#ifndef SERVER_CALCULATOR_H_
#define SERVER_CALCULATOR_H_

/**
 * @file Calculator.h
 * @brief Header file for a simple calculator.
 * @date 13 Nov 2023
 * @author Francesco Conforti
 */

/**
 * @brief Performs addition of two numbers.
 *
 * This function takes two numbers and returns their sum.
 *
 * @param num1 The first number.
 * @param num2 The second number.
 * @return The result of the addition operation.
 */
double add(double num1, double num2);

/**
 * @brief Performs multiplication of two numbers.
 *
 * This function takes two numbers and returns their product.
 *
 * @param num1 The first number.
 * @param num2 The second number.
 * @return The result of the multiplication operation.
 */
double mult(double num1, double num2);

/**
 * @brief Performs subtraction of two numbers.
 *
 * This function takes two numbers and returns the result of subtracting
 * the second number from the first.
 *
 * @param num1 The first number.
 * @param num2 The second number.
 * @return The result of the subtraction operation.
 */
double sub(double num1, double num2);

/**
 * @brief Performs division of two numbers.
 *
 * This function takes two numbers and returns the result of dividing
 * the first number by the second.
 *
 * @param num1 The numerator.
 * @param num2 The denominator.
 * @return The result of the division operation.
 * @note Division by zero is not handled in this implementation.
 */
double division(double num1, double num2);

#endif /* SERVER_CALCULATOR_H_ */
