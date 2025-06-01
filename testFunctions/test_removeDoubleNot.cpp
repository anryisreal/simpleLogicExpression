#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"
#include "testFunctions.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testRemoveDoubleNot
{
    TEST_CLASS(testRemoveDoubleNot)
    {
    public:
        /**
         * @brief Тест 1: Нет отрицаний
         */
        TEST_METHOD(Test2_NoNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Variable, "a");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 2: Одинарное отрицание
         * @details Проверяет, что функция не изменяет дерево с одним отрицанием
         */
        TEST_METHOD(Test2_SingleNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 3: Двойное отрицание
         * @details Проверяет удаление двойного отрицания
         */
        TEST_METHOD(Test3_DoubleNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 4: Тройное отрицание
         * @details Проверяет преобразование тройного отрицания в одинарное
         */
        TEST_METHOD(Test4_TripleNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 5: Одинарное отрицание перед оператором And
         * @details Проверяет, что одинарное отрицание перед оператором не изменяется
         */
        TEST_METHOD(Test5_SingleNegationBeforeAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::And);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::And);
            expected->left->left = new ExpressionNode(TokenType::Variable, "a");
            expected->left->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 6: Двойное отрицание перед оператором And
         * @details Проверяет удаление двойного отрицания перед оператором
         */
        TEST_METHOD(Test6_DoubleNegationBeforeAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::And);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 7: Вложенные двойные отрицания
         * @details Проверяет обработку вложенных двойных отрицаний
         */
        TEST_METHOD(Test7_NestedDoubleNegations)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::And);
            input->left->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 8: Двойное отрицание внутри оператора Or
         * @details Проверяет обработку двойного отрицания внутри дизъюнкции
         */
        TEST_METHOD(Test8_DoubleNegationInsideOr)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Or);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 9: Двойное отрицание внутри оператора And
         * @details Проверяет обработку двойного отрицания внутри конъюнкции
         */
        TEST_METHOD(Test9_DoubleNegationInsideAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 10: Двойное отрицание внутри оператора Implication
         * @details Проверяет обработку двойного отрицания внутри импликации
         */
        TEST_METHOD(Test10_DoubleNegationInsideImplication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Implication);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Implication);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 11: Двойное отрицание в операторе Equivalence
         * @details Проверяет обработку двойного отрицания внутри эквивалентности
         */
        TEST_METHOD(Test11_DoubleNegationInEquivalence)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Equivalence);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Equivalence);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 12: Двойное отрицание в обоих операндах Or
         * @details Проверяет обработку двойных отрицаний в обоих операндах дизъюнкции
         */
        TEST_METHOD(Test12_DoubleNegationInBothOperandsOr)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Not);
            input->right->left = new ExpressionNode(TokenType::Not);
            input->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Or);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 13: Двойное отрицание в обоих операндах And
         * @details Проверяет обработку двойных отрицаний в обоих операндах конъюнкции
         */
        TEST_METHOD(Test13_DoubleNegationInBothOperandsAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Not);
            input->right->left = new ExpressionNode(TokenType::Not);
            input->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 14: Двойное отрицание в обоих операндах Implication
         * @details Проверяет обработку двойных отрицаний в обоих операндах импликации
         */
        TEST_METHOD(Test14_DoubleNegationInBothOperandsImplication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Implication);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Not);
            input->right->left = new ExpressionNode(TokenType::Not);
            input->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Implication);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 15: Двойное отрицание в обоих операндах Equivalence
         * @details Проверяет обработку двойных отрицаний в обоих операндах эквивалентности
         */
        TEST_METHOD(Test15_DoubleNegationInBothOperandsEquivalence)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Equivalence);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Not);
            input->right->left = new ExpressionNode(TokenType::Not);
            input->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Equivalence);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 16: Отрицание внутри другого отрицания
         * @details Проверяет обработку отрицания внутри другого отрицания без двойного отрицания
         */
        TEST_METHOD(Test16_NegationInsideNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Or);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Or);
            expected->left->left = new ExpressionNode(TokenType::Not);
            expected->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            expected->left->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 17: Комплексный случай с вложенными операторами
         * @details Проверяет обработку комплексного выражения с вложенными операторами
         */
        TEST_METHOD(Test17_ComplexCase)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Implication);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Not);
            input->left->left->right->left = new ExpressionNode(TokenType::Not);
            input->left->left->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Implication);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }
    };
}