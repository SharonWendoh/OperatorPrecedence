#include <stdio.h>
#include <stdlib.h>

// Define operator precedence levels
int get_precedence(char operator) {
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0; // For unknown operators
    }
}

// Function to perform binary operations
double apply_operator(char operator, double left, double right) {
    switch (operator) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            return left / right;
        default:
            return 0; // Handle unknown operators gracefully
    }
}

// Operator Precedence Parsing
double parse_expression(const char *expression) {
    int length = strlen(expression);
    char operators[length];
    double values[length];
    int operator_top = -1;
    int value_top = -1;
    int i = 0;

    while (i < length) {
        char token = expression[i];

        if (isdigit(token)) {
            int j = i;
            while (j < length && isdigit(expression[j])) {
                j++;
            }
            char num[32];
            strncpy(num, expression + i, j - i);
            num[j - i] = '\0';
            values[++value_top] = atof(num);
            i = j;
        } else if (strchr("+-*/", token) != NULL) {
            while (operator_top >= 0 && get_precedence(token) <= get_precedence(operators[operator_top])) {
                double right = values[value_top--];
                double left = values[value_top--];
                char op = operators[operator_top--];
                values[++value_top] = apply_operator(op, left, right);
            }
            operators[++operator_top] = token;
            i++;
        } else if (token == '(') {
            operators[++operator_top] = token;
            i++;
        } else if (token == ')') {
            while (operator_top >= 0 && operators[operator_top] != '(') {
                double right = values[value_top--];
                double left = values[value_top--];
                char op = operators[operator_top--];
                values[++value_top] = apply_operator(op, left, right);
            }
            if (operator_top >= 0 && operators[operator_top] == '(') {
                operator_top--; // Pop the open parenthesis
            }
            i++;
        } else {
            // Ignore whitespace and unknown characters
            i++;
        }
    }

    while (operator_top >= 0) {
        double right = values[value_top--];
        double left = values[value_top--];
        char op = operators[operator_top--];
        values[++value_top] = apply_operator(op, left, right);
    }

    return values[0];
}

int main() {
    char expression[100];
    while (1) {
        printf("Enter an arithmetic expression or type 'exit' to quit: ");
        fgets(expression, sizeof(expression), stdin);
        expression[strcspn(expression, "\n")] = '\0'; // Remove the newline character

        if (strcmp(expression, "exit") == 0) {
            break;
        }

        double result = parse_expression(expression);
        printf("Result: %f\n", result);
    }

    return 0;
}