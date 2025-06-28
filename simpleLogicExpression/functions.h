/**
 * @file functions.h
 * @brief Заголовки для всех функций программы.
 */

#pragma once
#include <string>
#include <vector>
#include <set>
#include "objects.h"

/**
 * @brief Читает содержимое файла в строку.
 *
 * Открывает файл по указанному пути и считывает его содержимое в строку.
 * @param [in] filePath Путь к входному файлу.
 * @return Содержимое файла в виде строки.
 * @throw Error с типом inputFile, если файл не удалось открыть.
 */
std::string readFile(const std::string& filePath);

/**
 * @brief Записывает строку в файл.
 *
 * Открывает файл по указанному пути и записывает в него переданную строку.
 * @param [in] filePath Путь к выходному файлу.
 * @param [in] content Строка для записи в файл.
 * @throw Error с типом outputFile, если файл не удалось открыть.
 */
void writeFile(const std::string& filePath, const std::string& content);

/**
 * @brief Разбивает строку на токены.
 *
 * Преобразует входную строку с логическим выражением в постфиксной записи в вектор токенов.
 * Проверяет корректность токенов и добавляет ошибки в errorList при их обнаружении.
 * @param [in] expression Строка с логическим выражением в постфиксной записи.
 * @param [in,out] errorList Множество для хранения обнаруженных ошибок.
 * @return Вектор токенов, представляющих входное выражение.
 */
std::vector<Token> tokenize(const std::string& expression, std::set<Error>& errorList);

/**
 * @brief Строит дерево выражения из вектора токенов.
 *
 * Создает бинарное дерево логического выражения на основе токенов в постфиксной записи.
 * При обнаружении ошибок (например, недостаток операндов) добавляет их в errorList.
 * @param [in] tokens Вектор токенов в постфиксной записи.
 * @param [in,out] errorList Множество для хранения обнаруженных ошибок.
 * @return Указатель на корень построенного дерева или nullptr при ошибке.
 */
ExpressionNode* buildExpressionTree(const std::vector<Token>& tokens, std::set<Error>& errorList);

/**
 * @brief Создает глубокую копию узла дерева.
 *
 * Рекурсивно копирует узел и все его поддеревья, создавая независимую копию.
 * @param [in] node Указатель на узел для копирования.
 * @return Указатель на новый узел или nullptr, если входной узел равен nullptr.
 */
ExpressionNode* copyNode(ExpressionNode* node);

/**
 * @brief Преобразует операции импликации и эквивалентности.
 *
 * Преобразует импликацию (A > B) в (!A | B) и эквивалентность (A ~ B) в ((A & B) | (!A & !B)).
 * Рекурсивно обрабатывает все узлы дерева.
 * @param [in,out] node Указатель на корень дерева для преобразования.
 */
void transformImplicationAndEquivalence(ExpressionNode* node);

/**
 * @brief Применяет законы де Моргана.
 *
 * Преобразует выражения вида !(A & B) в !A | !B и !(A | B) в !A & !B.
 * Рекурсивно обрабатывает поддеревья.
 * @param [in,out] node Указатель на корень дерева для упрощения.
 * @return true, если были внесены изменения, иначе false.
 */
bool simplifyExpression(ExpressionNode* node, bool& changed);

/**
 * @brief Удаляет двойные отрицания.
 *
 * Преобразует выражения вида !!A в A, рекурсивно обрабатывая поддеревья.
 * @param [in,out] node Указатель на корень дерева для преобразования.
 */
void removeDoubleNot(ExpressionNode* node);

/**
 * @brief Преобразует дерево выражения в инфиксную строку.
 *
 * Формирует строковое представление логического выражения в инфиксной нотации,
 * добавляя скобки с учетом приоритетов операций.
 * @param [in] node Указатель на корень дерева.
 * @return Строковое представление выражения в инфиксной форме.
 */
std::string expressionTreeToInfix(ExpressionNode* node);