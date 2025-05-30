#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"
#include "testFunctions.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testExpressionTreeToInfix
{
    TEST_CLASS(testExpressionTreeToInfix)
    {
    public:
        /**
         * @brief Тест 1: Простая переменная
         * @details Проверяет преобразование дерева с одной переменной
         */
        TEST_METHOD(Test1_SimpleVariable)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Variable, "a");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 2: Отрицание
         * @details Проверяет преобразование дерева с отрицанием
         */
        TEST_METHOD(Test2_Negation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!a";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 3: Конъюнкция
         * @details Проверяет преобразование дерева с конъюнкцией
         */
        TEST_METHOD(Test3_Conjunction)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a & b";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 4: Дизъюнкция
         * @details Проверяет преобразование дерева с дизъюнкцией
         */
        TEST_METHOD(Test4_Disjunction)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a || b";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 5: Импликация
         * @details Проверяет преобразование дерева с импликацией
         */
        TEST_METHOD(Test5_Implication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Implication);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a -> b";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 6: Эквивалентность
         * @details Проверяет преобразование дерева с эквивалентностью
         */
        TEST_METHOD(Test6_Equivalence)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Equivalence);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a ~ b";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 7: Приоритет операций
         * @details Проверяет правильность расстановки скобок согласно приоритету операций
         */
        TEST_METHOD(Test7_OperationPriority)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Or);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::Or);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "(a || b) & (c || d)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 8: Вложенные скобки
         * @details Проверяет правильность обработки вложенных скобок
         */
        TEST_METHOD(Test8_NestedParentheses)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Or);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::And);
            input->left->right->left = new ExpressionNode(TokenType::Variable, "b");
            input->left->right->right = new ExpressionNode(TokenType::Variable, "c");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!(a || (b & c))";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 9: Приоритет эквиваленции и импликации
         * @details Проверяет правильность расстановки скобок для операций с разным приоритетом
         */
        TEST_METHOD(Test9_PriorityOfEquivalenceAndImplication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Implication);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::Equivalence);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "(a -> b) & (c ~ d)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 10: Больший приоритет у левого операнда
         * @details Проверяет случай, когда левый операнд имеет больший приоритет
         */
        TEST_METHOD(Test10_HigherPriorityInLeftOperand)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::And);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::Equivalence);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a & b || (c ~ d)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 11: Больший приоритет у правого операнда
         * @details Проверяет случай, когда правый операнд имеет больший приоритет
         */
        TEST_METHOD(Test11_HigherPriorityInRightOperand)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::Equivalence);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::And);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "(a ~ b) || c & d";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 12: Одинаковый приоритет
         * @details Проверяет случай, когда оба операнда имеют одинаковый приоритет
         */
        TEST_METHOD(Test12_SamePriority)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::And);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::And);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a & b & c & d";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 13: Вложенное отрицание
         * @details Проверяет обработку вложенных отрицаний в сложном выражении
         */
        TEST_METHOD(Test13_NestedNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Or);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::And);
            input->left->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->left->right = new ExpressionNode(TokenType::And);
            input->left->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->left->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!(!(a & b) || c & d)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 14: Несколько отрицаний перед переменной
         * @details Проверяет обработку нескольких отрицаний перед переменной
         */
        TEST_METHOD(Test14_MultipleNegationsBeforeVariable)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!a";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 15: Несколько отрицаний перед оператором Or
         * @details Проверяет обработку нескольких отрицаний перед оператором дизъюнкции
         */
        TEST_METHOD(Test15_MultipleNegationsBeforeOr)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Or);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!(a || b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 16: Несколько отрицаний перед оператором And
         * @details Проверяет обработку нескольких отрицаний перед оператором конъюнкции
         */
        TEST_METHOD(Test16_MultipleNegationsBeforeAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::And);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!(a & b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 17: Несколько отрицаний перед оператором Implication
         * @details Проверяет обработку нескольких отрицаний перед оператором импликации
         */
        TEST_METHOD(Test17_MultipleNegationsBeforeImplication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Implication);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!(a -> b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 18: Несколько отрицаний перед оператором Equivalence
         * @details Проверяет обработку нескольких отрицаний перед оператором эквивалентности
         */
        TEST_METHOD(Test18_MultipleNegationsBeforeEquivalence)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Equivalence);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!(a ~ b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief Тест 19: Отрицание внутри и снаружи оператора
         * @details Проверяет обработку отрицания как внутри, так и снаружи оператора
         */
        TEST_METHOD(Test19_NegationInsideAndOutsideOperator)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::And);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Not);
            input->left->right->left = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!(a & !b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }
    };
}