#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <locale>
#include "objects.h"
#include "functions.h"

/**
* Соответствие между строкой и типом операции
*/
const std::map<std::string, TokenType> stringToTokenType = {
    {"!", TokenType::Not},
    {"&", TokenType::And},
    {"|", TokenType::Or},
    {">", TokenType::Implication},
    {"~", TokenType::Equivalence}
};

/**
 * Главная функция программы.
 *
 * Обрабатывает аргументы командной строки, выполняет чтение входного файла,
 * преобразование выражения и запись результата в выходной файл.
 * @param argc Количество аргументов
 * @param argv Массив аргументов (argv[1] - входной файл, argv[2] - выходной файл)
 * @return 0 при успешном завершении
 */
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    // Проверяем количество аргументов командной строки
    if (argc != 3) {
        std::cerr << L"Использование: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    std::set<Error> errorList;
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string content;

    // Чтение входного файла
    try {
        content = readFile(inputFile);
    }
    catch (const Error& e) {
        e.message();
        return 1;
    }

    // Токенизация входной строки
    std::vector<Token> tokens = tokenize(content, errorList);

    // Проверка на ошибки токенизации
    if (!errorList.empty()) {
        for (const auto& error : errorList) {
            error.message();
        }
        return 1;
    }

    // Построение дерева выражения
    ExpressionNode* exprTree = buildExpressionTree(tokens, errorList);

    // Проверка на ошибки построения дерева
    if (!errorList.empty()) {
        for (const auto& error : errorList) {
            error.message();
        }
        delete exprTree; // Освобождаем память
        return 1;
    }

    // Преобразование импликации и эквивалентности
    transformImplicationAndEquivalence(exprTree);

    // Применение законов де Моргана до тех пор, пока есть изменения
    bool changed;
    do {
        changed = simplifyExpression(exprTree);
    } while (changed);

    // Удаление двойных отрицаний
    removeDoubleNot(exprTree);

    // Формирование выходной строки
    std::string result = expressionTreeToInfix(exprTree);

    // Запись результата в выходной файл
    try {
        writeFile(outputFile, result);
    }
    catch (const Error& e) {
        e.message();
        delete exprTree; // Освобождаем память
        return 1;
    }

    // Освобождение памяти
    delete exprTree;

    return 0;
}

/**
* Функци для чтения из файла
*/
std::string readFile(const std::string& filePath) {

    std::ifstream file(filePath);	// Создаем поток для чтения из файла

	// Выброс исключения, если файл не открылся
    if (!file.is_open()) {
        throw Error(Error::inputFile);
    }
    
    std::string content;			// Считываемая строка из файла
    std::getline(file, content);	// Считывание строки
    file.close();					// Закрытие файла

	return content;
}

/**
* Функция для записи в файл
*/
void writeFile(const std::string& filePath, const std::string& content) {

    std::ofstream file(filePath);  // Создаем поток для записи в файл

    // Выброс исключения, если файл не открылся
    if (!file.is_open()) {
        throw Error(Error::outputFile);
    }

    file << content;               // Запись в файл
    file.close();                  // Закрытие файла
}

std::vector<Token> tokenize(const std::string& expression, std::set<Error>& errorList) {
    std::vector<Token> tokens;
    std::istringstream iss(expression);
    std::string tokenStr;
    int position = 0;

    while (iss >> tokenStr) {
        position++;

        // Проверка на операцию
        auto it = stringToTokenType.find(tokenStr);
        if (it != stringToTokenType.end()) {
            tokens.emplace_back(it->second, tokenStr, position);
            continue;
        }

        // Проверка на переменную
        if (isalpha(tokenStr[0])) {
            bool valid = std::all_of(tokenStr.begin(), tokenStr.end(), [](char c) {
                return isalnum(c);
                });

            if (!valid) {
                errorList.insert(Error(Error::ErrorType::invalidVariableChar, position));
                continue;
            }

            tokens.emplace_back(TokenType::Variable, tokenStr, position);
            continue;
        }

        // Если токен начинается с цифры или недопустимого символа
        if (isdigit(tokenStr[0]) || !isalpha(tokenStr[0])) {
            errorList.insert(Error(Error::ErrorType::invalidVariableName, position));
            continue;
        }

        // Все остальные случаи - неподдерживаемая операция
        errorList.insert(Error(Error::ErrorType::unsupportedOperation, position));
    }

    return tokens;
}

ExpressionNode* buildExpressionTree(const std::vector<Token>& tokens, std::set<Error>& errorList) {
    std::vector<ExpressionNode*> stack;

    for (const auto& token : tokens) {
        if (token.type == TokenType::Variable) {
            stack.push_back(new ExpressionNode(token.type, token.value));
            continue;
        }

        if (token.type == TokenType::Not) {
            if (stack.empty()) {
                errorList.insert(Error(Error::ErrorType::insufficientOperands, token.position));
                continue;
            }

            ExpressionNode* operand = stack.back();
            stack.pop_back();
            stack.push_back(new ExpressionNode(token.type, nullptr, operand));
            continue;
        }

        // Бинарные операции
        if (stack.size() < 2) {
            errorList.insert(Error(Error::ErrorType::insufficientOperands, token.position));
            continue;
        }

        ExpressionNode* right = stack.back();
        stack.pop_back();
        ExpressionNode* left = stack.back();
        stack.pop_back();
        stack.push_back(new ExpressionNode(token.type, left, right));
    }

    if (stack.size() != 1) {
        errorList.insert(Error(Error::ErrorType::missingOperation, 0));
        return nullptr;
    }

    return stack.back();
}

ExpressionNode* copyNode(ExpressionNode* node) {
    if (!node) return nullptr;

    return new ExpressionNode(
        node->type,
        node->value,
        copyNode(node->left),
        copyNode(node->right)
    );
}

void transformImplicationAndEquivalence(ExpressionNode* node) {
    if (!node) return;

    // Рекурсивно преобразуем поддеревья
    transformImplicationAndEquivalence(node->left);
    transformImplicationAndEquivalence(node->right);

    // Преобразование импликации A > B в !A | B
    if (node->type == TokenType::Implication) {
        ExpressionNode* newNot = new ExpressionNode(TokenType::Not, nullptr, copyNode(node->left));
        node->type = TokenType::Or;
        node->left = newNot; // node->right остается без изменений
        return;
    }

    // Преобразование эквивалентности A ~ B в (A & B) | (!A & !B)
    if (node->type == TokenType::Equivalence) {
        // Создаем левый узел конъюнкции (A & B)
        ExpressionNode* newLeft = new ExpressionNode(TokenType::And, copyNode(node->left), copyNode(node->right));

        // Создаем правый узел конъюнкции (!A & !B)
        ExpressionNode* newRightL = new ExpressionNode(TokenType::Not, nullptr, copyNode(node->left));
        ExpressionNode* newRightR = new ExpressionNode(TokenType::Not, nullptr, copyNode(node->right));
        ExpressionNode* newRight = new ExpressionNode(TokenType::And, newRightL, newRightR);

        // Обновляем текущий узел
        node->type = TokenType::Or;
        node->left = newLeft;
        node->right = newRight;
    }
}
bool simplifyExpression(ExpressionNode* node) {
    if (!node) return false;

    bool changed = false;

    // Сначала рекурсивно упрощаем поддеревья
    changed |= simplifyExpression(node->left);
    changed |= simplifyExpression(node->right);

    // Применяем первый закон де Моргана: !(A & B) ? !A | !B
    if (node->type == TokenType::Not && node->right && node->right->type == TokenType::And) {
        // Сохраняем оригинальные узлы
        ExpressionNode* originalAnd = node->right;
        ExpressionNode* leftOperand = originalAnd->left;
        ExpressionNode* rightOperand = originalAnd->right;

        // Создаем новые узлы отрицания
        ExpressionNode* newNotLeft = new ExpressionNode(TokenType::Not, nullptr, leftOperand);
        ExpressionNode* newNotRight = new ExpressionNode(TokenType::Not, nullptr, rightOperand);

        // Преобразуем текущий узел
        node->type = TokenType::Or;
        node->left = newNotLeft;
        node->right = newNotRight;

        // Удаляем старый узел And (но не его потомков - они теперь используются в новых узлах)
        originalAnd->left = nullptr;
        originalAnd->right = nullptr;
        delete originalAnd;

        changed = true;
    }
    // Применяем второй закон де Моргана: !(A | B) ? !A & !B
    else if (node->type == TokenType::Not && node->right && node->right->type == TokenType::Or) {
        ExpressionNode* originalOr = node->right;
        ExpressionNode* leftOperand = originalOr->left;
        ExpressionNode* rightOperand = originalOr->right;

        ExpressionNode* newNotLeft = new ExpressionNode(TokenType::Not, nullptr, leftOperand);
        ExpressionNode* newNotRight = new ExpressionNode(TokenType::Not, nullptr, rightOperand);

        node->type = TokenType::And;
        node->left = newNotLeft;
        node->right = newNotRight;

        originalOr->left = nullptr;
        originalOr->right = nullptr;
        delete originalOr;

        changed = true;
    }

    return changed;
}

void removeDoubleNot(ExpressionNode* node) {
    if (!node) return;

    // Рекурсивно обрабатываем поддеревья
    removeDoubleNot(node->left);
    removeDoubleNot(node->right);

    // Удаляем двойное отрицание
    if (node->type == TokenType::Not && node->right && node->right->type == TokenType::Not) {
        ExpressionNode* temp = node->right;
        node->type = temp->right->type;
        node->value = temp->right->value;
        node->left = temp->right->left;
        node->right = temp->right->right;

        // Обнуляем указатели, чтобы избежать двойного удаления
        temp->right->left = nullptr;
        temp->right->right = nullptr;
        delete temp;
    }
}

std::string expressionTreeToInfix(ExpressionNode* node) {
    if (!node) return "";

    // Карта приоритетов операций
    static const std::map<TokenType, int> priority = {
        {TokenType::Equivalence, 1},
        {TokenType::Implication, 2},
        {TokenType::Or, 3},
        {TokenType::And, 4},
        {TokenType::Not, 5}
    };

    // Для переменных
    if (node->type == TokenType::Variable) {
        return node->value;
    }

    // Для унарных операций (отрицание)
    if (node->type == TokenType::Not) {
        std::string rightExpr = expressionTreeToInfix(node->right);
        // Добавляем скобки, если подвыражение сложное
        if (node->right && node->right->type != TokenType::Variable &&
            node->right->type != TokenType::Not) {
            return "!(" + rightExpr + ")";
        }
        return "!" + rightExpr;
    }

    // Для бинарных операций
    std::string leftExpr = expressionTreeToInfix(node->left);
    std::string rightExpr = expressionTreeToInfix(node->right);

    // Определяем оператор
    std::string op;
    switch (node->type) {
    case TokenType::And: op = " & "; break;
    case TokenType::Or: op = " || "; break;
    case TokenType::Implication: op = " -> "; break;
    case TokenType::Equivalence: op = " ~ "; break;
    default: op = " ? "; break;
    }

    // Проверяем приоритеты для скобок (левый операнд)
    if (node->left && priority.count(node->left->type)) {
        if (priority.at(node->left->type) < priority.at(node->type)) {
            leftExpr = "(" + leftExpr + ")";
        }
    }

    // Проверяем приоритеты для скобок (правый операнд)
    if (node->right && priority.count(node->right->type)) {
        int rightPrio = priority.at(node->right->type);
        int currPrio = priority.at(node->type);
        if (rightPrio < currPrio ||
            (rightPrio == currPrio && node->type == TokenType::Implication)) {
            rightExpr = "(" + rightExpr + ")";
        }
    }

    return leftExpr + op + rightExpr;
}