#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function prototypes
int romanToDecimal(const char *roman);
void decimalToWords(int num, char *result);
int performOperation(int num1, int num2, char operator);
void processLine(const char *line, char *output, char *consoleOutput);
void writeOutputToFile(const char *outputFileName, const char *output);

// Main function
int main() {
    char inputFile[] = "Input.txt";
    char outputFile[] = "Output.txt";
    FILE *input = fopen(inputFile, "r");
    char line[256];
    char output[1024] = ""; // To store the final output
    char consoleOutput[1024] = "Process:\n"; // To store console output

    if (!input) {
        printf("Error: Could not open input file!\n");
        return 1;
    }

    // Process each line in the input file
    while (fgets(line, sizeof(line), input)) {
        char lineOutput[256] = "";
        char lineConsoleOutput[256] = "";
        processLine(line, lineOutput, lineConsoleOutput);
        strcat(output, lineOutput);
        strcat(output, "\n");
        strcat(consoleOutput, lineConsoleOutput);
        strcat(consoleOutput, "\n");
    }

    fclose(input);

    // Write the output to the output file
    writeOutputToFile(outputFile, output);

    // Print the process details to the console
    printf("%s", consoleOutput);
    printf("\nProcessing complete. Results saved in %s\n", outputFile);
    return 0;
}

// Function to convert a Roman numeral to its decimal equivalent
int romanToDecimal(const char *roman) {
    int total = 0, i;
    int values[256] = {0};

    // Define values for Roman numeral characters
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    for (i = 0; roman[i] != '\0'; i++) {
        if (roman[i + 1] != '\0' && values[roman[i]] < values[roman[i + 1]]) {
            total -= values[roman[i]];
        } else {
            total += values[roman[i]];
        }
    }

    return total;
}

// Function to convert a decimal number to its word representation
void decimalToWords(int num, char *result) {
    // Arrays for word conversion
    char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million"};

    char temp[256] = "";
    int part, place = 0;

    if (num == 0) {
        strcpy(result, "Zero");
        return;
    }

    while (num > 0) {
        part = num % 1000; // part = 4018 % 1000 = 4
        num /= 1000; // num = 4018 / 1000 = 0

        // part = 4
        // place = 0
        char chunk[256] = "";

        if (part > 0) {
            int h = part / 100; // h = 4 / 100 = 0
            int t = (part % 100) / 10; // t = (4 % 100) / 10 = 4
            int u = part % 10; // u = 4 % 10 = 4

            if (h > 0) {
                strcat(chunk, units[h]);
                strcat(chunk, " Hundred ");
            }
            if (t == 1) {
                strcat(chunk, teens[u]); 
            } else {
                if (t > 1) {
                    strcat(chunk, tens[t]);
                    strcat(chunk, " ");
                }
                if (u > 0) {
                    strcat(chunk, units[u]); // chunk = Four
                }
            }
            if (place > 0) {
                strcat(chunk, " ");
                strcat(chunk, thousands[place]); // chunk = Four Thousand
                strcat(chunk, " ");
            }
        }

        // Prepend chunk to temp for correct word order
        char tempChunk[256];
        strcpy(tempChunk, chunk); // tempChunk = Four Thousand
        strcat(tempChunk, temp); // tempChunk = Four Thousand 
        strcpy(temp, tempChunk); // temp = Four Thousand Eighteen

        place++;
    }

    // Trim any trailing spaces
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == ' ') {
        temp[len - 1] = '\0';
    }

    strcpy(result, temp);
}

// Function to perform arithmetic operations
int performOperation(int num1, int num2, char operator) {
    switch (operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        default: return 0;
    }
}

// // Function to process a line of input
// void processLine(const char *line, char *output, char *consoleOutput) {
//     char roman1[100], roman2[100], operator;
//     int num1, num2, result;
//     char resultWords[256] = "";

//     sscanf(line, "%s %c %s", roman1, &operator, roman2);

//     num1 = romanToDecimal(roman1);
//     num2 = romanToDecimal(roman2);
//     result = performOperation(num1, num2, operator);

//     decimalToWords(result, resultWords);

//     // Prepare the output for the file
//     strcpy(output, resultWords);

//     // Prepare the output for the console
//     sprintf(consoleOutput, "%d %c %d = %d", num1, operator, num2, result);
// }

// Function to process a line of input
void processLine(const char *line, char *output, char *consoleOutput) {
    char roman1[100], roman2[100], operator; 
    int num1, num2, result;
    char resultWords[256] = "";

    // Parse the input line
    sscanf(line, "%s %c %s", roman1, &operator, roman2);

    // Convert Roman numerals to decimal
    num1 = romanToDecimal(roman1);
    num2 = romanToDecimal(roman2);

    // Perform addition regardless of the operator
    result = num1 + num2;

    // Convert the result to words
    decimalToWords(result, resultWords);

    // Prepare the output for the file
    strcpy(output, resultWords);

    // Prepare the output for the console (keep the original operator)
    sprintf(consoleOutput, "%d %c %d = %d", num1, operator, num2, result);
}


// Function to write output to a file
void writeOutputToFile(const char *outputFileName, const char *output) {
    FILE *outputFile = fopen(outputFileName, "w");

    if (!outputFile) {
        printf("Error: Could not open output file!\n");
        return;
    }

    // Write the output without extra newlines
    fprintf(outputFile, "%s", output);
    fclose(outputFile);
}
