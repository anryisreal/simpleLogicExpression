/**
 * @file main.cpp
 * @brief Реализация всех функций программы. Реализация главной функции main.
 */

/**
 * \mainpage Документация для программы "simpleLogicExpression"
 *
 * Программа предназначена для раскрытия скобок в логическом выражении по законам де Моргана и удаления двойного отрицания
 * Программа разработана на языке C++ с использованием стандартных библиотек C++
 * Программа должна получать два аргумента командной строки: имя входного файла и имя выходного файла в формате ".txt".
 *
 * Пример команды запуска программы:
 * \code
 * ./simpleLogicExpression.exe ./input.txt ./output.txt
 * \endcode
 *
 * \author Pavel Andreyaschenko
 * \date June 2025
 * \version 1.0
 */

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
 * @brief Соответствие между строкой и типом операции.
 *
 * Карта, связывающая строковые представления логических операций с их типами в перечислении TokenType.
 */
const std::map<std::string, TokenType> stringToTokenType = {
    {"!", TokenType::Not},
    {"&", TokenType::And},
    {"|", TokenType::Or},
    {">", TokenType::Implication},
    {"~", TokenType::Equivalence}
};

/**
 * @brief Главная функция программы.
 *
 * Обрабатывает аргументы командной строки, выполняет чтение входного файла,
 * преобразование логического выражения и запись результата в выходной файл.
 * @param argc Количество аргументов командной строки.
 * @param argv Массив строк аргументов (argv[1] - путь к входному файлу, argv[2] - путь к выходному файлу).
 * @return 0 при успешном выполнении, 1 при возникновении ошибок.
 */
int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale(""));
    setlocale(LC_ALL, ".UTF8");

    // Проверяем количество аргументов командной строки
    if (argc != 3) {
        std::wcerr << L"Ошибка входных параметров, использование: " << argv[0] << " <input file> <output file>" << std::endl;
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
 * @brief Читает содержимое файла в строку.
 *
 * Открывает файл по указанному пути и считывает его содержимое в строку.
 * @param [in] filePath Путь к входному файлу.
 * @return Содержимое файла в виде строки.
 * @throw Error с типом inputFile, если файл не удалось открыть.
 */
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath); // Создаем поток для чтения из файла

    // Выброс исключения, если файл не открылся
    if (!file.is_open()) {
        throw Error(Error::inputFile);
    }

    std::string content;           // Считываемая строка из файла
    std::getline(file, content);   // Считывание строки
    file.close();                  // Закрытие файла

    return content;
}

/**
 * @brief Записывает строку в файл.
 *
 * Открывает файл по указанному пути и записывает в него переданную строку.
 * @param [in] filePath Путь к выходному файлу.
 * @param [in] content Строка для записи в файл.
 * @throw Error с типом outputFile, если файл не удалось открыть.
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

/**
 * @brief Разбивает строку на токены.
 *
 * Преобразует входную строку с логическим выражением в постфиксной записи в вектор токенов.
 * Проверяет корректность токенов и добавляет ошибки в errorList при их обнаружении.
 * @param [in] expression Строка с логическим выражением в постфиксной записи.
 * @param [in,out] errorList Множество для хранения обнаруженных ошибок.
 * @return Вектор токенов, представляющих входное выражение.
 */
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

        // Проверка на начало с цифры
        if (isdigit(tokenStr[0])) {
            errorList.insert(Error(Error::ErrorType::invalidVariableName, position));
            continue;
        }

        // Все остальные случаи — неподдерживаемая операция
        errorList.insert(Error(Error::ErrorType::unsupportedOperation, position));
    }

    return tokens;
}

/**
 * @brief Строит дерево выражения из вектора токенов.
 *
 * Создает бинарное дерево логического выражения на основе токенов в постфиксной записи.
 * При обнаружении ошибок (например, недостаток операндов) добавляет их в errorList.
 * @param [in] tokens Вектор токенов в постфиксной записи.
 * @param [in,out] errorList Множество для хранения обнаруженных ошибок.
 * @return Указатель на корень построенного дерева или nullptr при ошибке.
 */
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
        // Если стек содержит больше одного элемента, указываем позицию последнего токена
        int errorPosition = tokens.empty() ? 0 : tokens.back().position;
        errorList.insert(Error(Error::ErrorType::missingOperation, errorPosition));
        return nullptr;
    }

    return stack.back();
}

/**
 * @brief Создает глубокую копию узла дерева.
 *
 * Рекурсивно копирует узел и все его поддеревья, создавая независимую копию.
 * @param [in] node Указатель на узел для копирования.
 * @return Указатель на новый узел или nullptr, если входной узел равен nullptr.
 */
ExpressionNode* copyNode(ExpressionNode* node) {
    if (!node) return nullptr;

    return new ExpressionNode(
        node->type,
        node->value,
        copyNode(node->left),
        copyNode(node->right)
    );
}

/**
 * @brief Преобразует операции импликации и эквивалентности.
 *
 * Преобразует импликацию (A > B) в (!A | B) и эквивалентность (A ~ B) в ((A & B) | (!A & !B)).
 * Рекурсивно обрабатывает все узлы дерева.
 * @param [in,out] node Указатель на корень дерева для преобразования.
 */
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

/**
 * @brief Применяет законы де Моргана.
 *
 * Преобразует выражения вида !(A & B) в !A | !B и !(A | B) в !A & !B.
 * Рекурсивно обрабатывает поддеревья.
 * @param [in,out] node Указатель на корень дерева для упрощения.
 * @return true, если были внесены изменения, иначе false.
 */
bool simplifyExpression(ExpressionNode* node) {
    if (!node) return false;

    bool changed = false;

    // Сначала рекурсивно упрощаем поддеревья
    changed |= simplifyExpression(node->left);
    changed |= simplifyExpression(node->right);

    // Применяем первый закон де Моргана: !(A & B) → !A | !B
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
    // Применяем второй закон де Моргана: !(A | B) → !A & !B
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

/**
 * @brief Удаляет двойные отрицания.
 *
 * Преобразует выражения вида !!A в A, рекурсивно обрабатывая поддеревья.
 * @param [in,out] node Указатель на корень дерева для преобразования.
 */
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

/**
 * @brief Преобразует дерево выражения в инфиксную строку.
 *
 * Формирует строковое представление логического выражения в инфиксной нотации,
 * добавляя скобки с учетом приоритетов операций.
 * @param [in] node Указатель на корень дерева.
 * @return Строковое представление выражения в инфиксной форме.
 */
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