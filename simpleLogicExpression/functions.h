#pragma once
#include <string>
#include <vector>
#include <set>
#include "objects.h"

/**
 * Читает содержимое файла в строку.
 * @param filePath Путь к входному файлу
 * @return Содержимое файла в виде строки
 * @throw Error типа inputFile если файл не может быть открыт
 */
std::string readFile(const std::string& filePath);

/**
 * Записывает строку в файл.
 * @param filePath Путь к выходному файлу
 * @param content Строка для записи
 * @throw Error типа outputFile если файл не может быть открыт
 */
void writeFile(const std::string& filePath, const std::string& content);

/**
 * Разбивает строку на токены.
 * @param expression Строка с логическим выражением в постфиксной записи
 * @param errorList Список найденных ошибок
 * @return Вектор токенов
 * @throw Error при обнаружении некорректных токенов (invalidVariableChar, invalidVariableName, unsupportedOperation)
 */
std::vector<Token> tokenize(const std::string& expression, std::set<Error>& errorList);

/**
 * Строит дерево выражения из вектора токенов.
 * @param tokens Вектор токенов
 * @param errorList Список найденных ошибок
 * @return Указатель на корень построенного дерева
 * @throw Error при недостатке операндов (insufficientOperands) или пропущенной операции (missingOperation)
 */
ExpressionNode* buildExpressionTree(const std::vector<Token>& tokens, std::set<Error>& errorList);

/**
 * Создает глубокую копию узла дерева.
 * @param node Узел для копирования
 * @return Указатель на новый узел
 */
ExpressionNode* copyNode(ExpressionNode* node);

/**
 * Преобразует операции импликации и эквивалентности.
 *
 * Импликация A > B преобразуется в !A | B.
 * Эквивалентность A ~ B преобразуется в (A & B) | (!A & !B).
 * @param node Указатель на корень дерева для преобразования
 */
void transformImplicationAndEquivalence(ExpressionNode* node);


/**
 * Применяет законы де Моргана.
 *
 * Преобразования:
 * - !(A & B) → !A | !B
 * - !(A | B) → !A & !B
 * @param node Указатель на корень дерева для упрощения
 * @return true если были сделаны изменения, иначе false
 */
bool simplifyExpression(ExpressionNode* node);

/**
 * Удаление двойных отрицаний
 *
 * !!A преобразуется в A
 * @param node Указатель на корень дерева для преобразования
 */
void removeDoubleNot(ExpressionNode* node);

/**
 * Преобразует дерево выражения в инфиксную строку.
 *
 * Добавляет скобки с учетом приоритетов операций.
 * @param node Указатель на корень дерева
 * @return Строковое представление выражения в инфиксной форме
 */
std::string expressionTreeToInfix(ExpressionNode* node);
