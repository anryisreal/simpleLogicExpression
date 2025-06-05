/**
 * @file test_buildExpressionTree.cpp
 * @brief Юнит-тесты для построения дерева выражений из токенов.
 */

#include "pch.h"
#include "CppUnitTest.h"
#include <algorithm>
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"
#include "testFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testbuildExpressionTree
{
    TEST_CLASS(testbuildExpressionTree)
    {
    public:
        /**
         * @brief Тест 1: Проверка обработки пустого списка токенов.
         * @details Проверяет, что пустой список токенов приводит к нулевому дереву выражений и ожидаемой ошибке.
         */
        TEST_METHOD(Test1_EmptyTokenList)
        {
            std::vector<Token> tokens;
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::missingOperation, 0}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }

        /**
         * @brief Тест 2: Проверка обработки одного токена (переменная).
         * @details Проверяет, что один токен переменной создает корректное дерево выражений без ошибок.
         */
        TEST_METHOD(Test2_SingleVariableToken)
        {
            std::vector<Token> tokens = { Token(TokenType::Variable, "a", 0) };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);
            ExpressionNode* expected = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 3: Проверка обработки одного токена (оператор).
         * @details Проверяет, что один токен оператора приводит к нулевому дереву и ожидаемым ошибкам.
         */
        TEST_METHOD(Test3_SingleOperatorToken)
        {
            std::vector<Token> tokens = { Token(TokenType::Not, "!", 0) };
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::insufficientOperands, 0},
                {Error::missingOperation, 0}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }

        /**
         * @brief Тест 4: Проверка обработки простого отрицания.
         * @details Проверяет построение дерева для выражения с простым отрицанием.
         */
        TEST_METHOD(Test4_SimpleNegation)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Not, "!", 1)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->right = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 5: Проверка обработки простой конъюнкции.
         * @details Проверяет построение дерева для выражения с простой конъюнкцией.
         */
        TEST_METHOD(Test5_SimpleConjunction)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 6: Проверка обработки отрицания перед скобкой.
         * @details Проверяет построение дерева для выражения с отрицанием перед скобочной группой.
         */
        TEST_METHOD(Test6_NegationBeforeBracket)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2),
                Token(TokenType::Not, "!", 3)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->right = new ExpressionNode(TokenType::And);
            expected->right->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 7: Проверка обработки комплексного выражения.
         * @details Проверяет построение дерева для сложного выражения с несколькими операциями.
         */
        TEST_METHOD(Test7_ComplexExpression)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2),
                Token(TokenType::Variable, "c", 3),
                Token(TokenType::Variable, "d", 4),
                Token(TokenType::And, "&", 5),
                Token(TokenType::Or, "|", 6)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Or);
            expected->left = new ExpressionNode(TokenType::And);
            expected->left->left = new ExpressionNode(TokenType::Variable, "a");
            expected->left->right = new ExpressionNode(TokenType::Variable, "b");
            expected->right = new ExpressionNode(TokenType::And);
            expected->right->left = new ExpressionNode(TokenType::Variable, "c");
            expected->right->right = new ExpressionNode(TokenType::Variable, "d");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 9: Проверка обработки импликации.
         * @details Проверяет построение дерева для выражения с импликацией.
         */
        TEST_METHOD(Test9_Implication)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Implication, ">", 2)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Implication);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 10: Проверка обработки эквивалентности.
         * @details Проверяет построение дерева для выражения с эквивалентностью.
         */
        TEST_METHOD(Test10_Equivalence)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Equivalence, "~", 2)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Equivalence);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 11: Проверка обработки множественных операций.
         * @details Проверяет построение дерева для выражения с несколькими операциями.
         */
        TEST_METHOD(Test11_MultipleOperations)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "b", 0),
                Token(TokenType::Not, "!", 1),
                Token(TokenType::Variable, "c", 2),
                Token(TokenType::And, "&", 3),
                Token(TokenType::Variable, "d", 4),
                Token(TokenType::Or, "|", 5)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Or);
            expected->left = new ExpressionNode(TokenType::And);
            expected->left->left = new ExpressionNode(TokenType::Not);
            expected->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            expected->left->right = new ExpressionNode(TokenType::Variable, "c");
            expected->right = new ExpressionNode(TokenType::Variable, "d");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 13: Проверка обработки отрицания над сложным выражением.
         * @details Проверяет построение дерева для выражения с отрицанием над сложной структурой.
         */
        TEST_METHOD(Test13_NegationOverComplexExpression)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "b", 0),
                Token(TokenType::Not, "!", 1),
                Token(TokenType::Variable, "c", 2),
                Token(TokenType::And, "&", 3),
                Token(TokenType::Variable, "d", 4),
                Token(TokenType::Or, "|", 5),
                Token(TokenType::Not, "!", 6)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->right = new ExpressionNode(TokenType::Or);
            expected->right->left = new ExpressionNode(TokenType::And);
            expected->right->left->left = new ExpressionNode(TokenType::Not);
            expected->right->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            expected->right->left->right = new ExpressionNode(TokenType::Variable, "c");
            expected->right->right = new ExpressionNode(TokenType::Variable, "d");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 14: Проверка обработки двойного отрицания.
         * @details Проверяет построение дерева для выражения с двойным отрицанием.
         */
        TEST_METHOD(Test14_DoubleNegation)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Not, "!", 1),
                Token(TokenType::Not, "!", 2)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->right = new ExpressionNode(TokenType::Not);
            expected->right->right = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief Тест 15: Проверка обработки вложенных отрицаний.
         * @details Проверяет построение дерева для выражения с вложенными отрицаниями.
         */
        TEST_METHOD(Test15_NestedNegations)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Implication, ">", 2),
                Token(TokenType::Not, "!", 3),
                Token(TokenType::Variable, "c", 4),
                Token(TokenType::Variable, "d", 5),
                Token(TokenType::And, "&", 6),
                Token(TokenType::Not, "!", 7),
                Token(TokenType::Or, "|", 8),
                Token(TokenType::Not, "!", 9)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->right = new ExpressionNode(TokenType::Or);
            expected->right->left = new ExpressionNode(TokenType::Not);
            expected->right->left->right = new ExpressionNode(TokenType::Implication);
            expected->right->left->right->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right->left->right->right = new ExpressionNode(TokenType::Variable, "b");
            expected->right->right = new ExpressionNode(TokenType::Not);
            expected->right->right->right = new ExpressionNode(TokenType::And);
            expected->right->right->right->left = new ExpressionNode(TokenType::Variable, "c");
            expected->right->right->right->right = new ExpressionNode(TokenType::Variable, "d");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }
    };
}