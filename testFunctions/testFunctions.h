#pragma once
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
* @brief Рекурсивно сравнивает два дерева выражений
* @param expected Ожидаемое дерево
* @param actual Фактическое дерево
* @param path Текущий путь в дереве (для диагностики)
* @return true если деревья идентичны, false в противном случае
*/
bool compareExpressionTrees(const ExpressionNode* expected, const ExpressionNode* actual, const std::string& path = "root");

/**
 * @brief Сравнивает два множества ошибок и выводит различия
 * @param expected Ожидаемое множество ошибок
 * @param actual Фактическое множество ошибок
 * @return true если множества идентичны, false в противном случае
 */
bool compareErrorSets(const std::set<Error>& expected, const std::set<Error>& actual);

/**
 * @brief Рекурсивно проверяет соответствие двух узлов и выводит путь при несоответствии
 * @param original Оригинальный узел
 * @param copied Скопированный узел
 * @param path Текущий путь в дереве (для сообщений об ошибках)
 * @return true если узлы идентичны, false в противном случае
 */
bool compareNodesRecursive(const ExpressionNode* original, const ExpressionNode* copied, const std::string& path = "node");

/**
 * @brief Проверяет, что два узла идентичны, включая все поддеревья
 * @param original Оригинальный узел
 * @param copied Скопированный узел
 */
void AssertNodesEqual(const ExpressionNode* original, const ExpressionNode* copied);