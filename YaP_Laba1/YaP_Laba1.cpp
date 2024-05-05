#include <iostream>
#include <stack>
#include <algorithm>
#define pi 3.14159265358979323846
using namespace std;

int priority(char operation) {
    if (operation == 's' || operation == 'c')
        return 5;
    else if (operation == '~')
        return 4;
    else if (operation == '^')
        return 3;
    else if (operation == '*' || operation == '/')
        return 2;
    else if (operation == '+' || operation == '-')
        return 1;
    else
        return -1;
}

template <typename any>
bool contains(any arr[], int size, const any& target) {
    return find(arr, arr + size, target) != arr + size;
}

string toPolishNotation(const string commonExpression) {
    stack <char> operationStack;
    string PolishNotation;
    char operationsSymbols[] = { '+', '-', '*', '/', '^', 's', 'c' };
    bool operationStackNotEmpty = false;

    auto putTopFromStackToNotation = [&]() { // & - передача переменных по ссылке
        PolishNotation += operationStack.top();
        if (operationStack.top() == 's') { PolishNotation += "in"; }
        if (operationStack.top() == 'c') { PolishNotation += "os"; }
        PolishNotation += " ";
        operationStack.pop();
        };

    for (int i = 0; i < commonExpression.length(); i++) {
        char ch = commonExpression[i];
        if (isdigit(ch) || ch == '.') {  //Операнд (цифра)
            PolishNotation += ch;
            if (i != commonExpression.length() - 1 && !isdigit(commonExpression[i + 1]) && commonExpression[i + 1] != '.') { // Для пробела между числами
                PolishNotation += " ";
            }
        }
        else if (ch == '(') {
            operationStack.push(ch);
        }
        else if (ch == ')') {
            while (!operationStack.empty() && operationStack.top() != '(') { //Все операции до ( переносятся в конец польской записи
                putTopFromStackToNotation();
            }
            operationStack.pop();  //Удаление (
            if (i == commonExpression.length() - 1) { PolishNotation.pop_back(); }
        }
        else if (ch == '-' && (i == 0 || commonExpression[i - 1] == '(' || !isdigit(commonExpression[i - 1]))) {
            operationStack.push('~'); // Унарный минус
        }
        else if (contains(operationsSymbols, size(operationsSymbols), ch)) { //Операция
            if (ch == '^') { // ^ - правоассоциативная операция
                while (!operationStack.empty() && priority(ch) < priority(operationStack.top())) {
                    putTopFromStackToNotation();
                }
            }
            else {
                while (!operationStack.empty() && priority(ch) <= priority(operationStack.top())) { // Если приоритет операции ниже или равен приоритету операции в стеке, то операции из стека заносятся в запись
                    putTopFromStackToNotation();
                }
            }
            operationStack.push(ch);
            if (ch == 's' || ch == 'c') { i += 2; } // Пропуск 2 символов для sin, cos
        }
        else {
            cerr << "При переводе в обратную польскую нотацию встречен некорректный символ\n";
            return "";
        }
    }
    if (!operationStack.empty()) { PolishNotation += " "; }
    while (!operationStack.empty()) { //Перевод остатка стека в конец польской записи после завершения чтения commonExpression
        putTopFromStackToNotation();
        operationStackNotEmpty = true;
    }
    if (operationStackNotEmpty) { PolishNotation.pop_back(); } // Удаление лишнего пробела
    return PolishNotation;
}

float calculatePolishNotation(string PolishNotation) {
    stack <float> operandStack;
    char operations2[] = {'+', '-', '*', '/', '^'};
    float operand1 = 0;
    float operand2 = 0;
    string number = ""; // Строка, чтобы хранить в ней цифры, пока они собираются в число

    for (int i = 0; i < PolishNotation.length(); i++) {
        char ch = PolishNotation[i];
        if (ch == ' ') {
            if (number != "") {
                operandStack.push(strtof(number.c_str(), NULL));
                number = "";
            }
        }
        else if (isdigit(ch) || ch == '.') {  //Для чисел
            number += ch;
        }
        else {  //Для операций
            try {
                if (contains(operations2, size(operations2), ch)) { // Операция с 2 операндами

                    if (operandStack.empty()) {
                        throw "Стек операндов оказался пустым\n";
                    }

                    operand2 = operandStack.top();
                    operandStack.pop();

                    if (operandStack.empty()) {
                        throw "Стек операндов оказался пустым\n";
                    }

                    operand1 = operandStack.top();
                    operandStack.pop();
                }
                else { // Операция с 1 операндом
                    if (operandStack.empty()) {
                        throw "Стек операндов оказался пустым\n";
                    }

                    operand1 = operandStack.top();
                    operandStack.pop();
                }
            }
            catch(const char* errorMessage) {
                cerr << errorMessage;
                return NAN;
            }

            switch (ch) {
            case '+':
                operandStack.push(operand1 + operand2);
                break;
            case '-':
                operandStack.push(operand1 - operand2);
                break;
            case '*':
                operandStack.push(operand1 * operand2);
                break;
            case '/':
                if (fabs(operand2) < pow(10, -10)) {
                    cerr << "Ошибка деления на 0\n";
                    return NAN;
                }
                operandStack.push(operand1 / operand2);
                break;
            case '^':
                operandStack.push(powf(operand1, operand2));
                break;
            case '~':
                operandStack.push(-1 * operand1);
                break;
            case 's':
                operandStack.push(sinf(operand1 * pi / 180));
                i += 2; // Пропуск символов i, n
                break;
            case 'c':
                operandStack.push(cosf(operand1 * pi / 180));
                i += 2;
                break;
            default:
                cerr << "Такой операции нету: '" << ch << "'" << endl;
                return NAN;
            }
        }
    }

    if (!operandStack.empty()) {
        float result = operandStack.top();
        operandStack.pop();
        if (operandStack.empty()) { return result; }
        else {
            cerr << "Обратная польская запись некорректна" << endl;
            return NAN;
        }
    }
    else {
        cerr << "Стек оказался пустым" << endl;
        return 0;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    //string commonExpression = "5+(3*(-5))/(sin(5)^cos(5))" 2^2^3
    cout << "Введите запись в стандартной форме: ";
    string commonExpression;
    cin >> commonExpression;
    string PolishNotation = toPolishNotation(commonExpression);
    cout << "Ваша запись в польской нотации: " << "\"" << PolishNotation << "\"" << endl;
    float result = calculatePolishNotation(PolishNotation);
    cout << "Результат вычислений: " << result << endl;
    return 0;
}