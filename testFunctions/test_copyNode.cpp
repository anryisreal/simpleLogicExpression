/**
 * @file test_copyNode.cpp
 * @brief Юнит-тесты для копирования узлов выражений.
 */

#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"
#include "testFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testcopyNode
{
    TEST_CLASS(testcopyNode)
    {
    private:

    public:
        /**
         * @brief Тест 1: Копия пустого узла.
         * @details Проверяет, что функция возвращает nullptr при копировании nullptr.
         */
        TEST_METHOD(Test1_CopyNullNode)
        {
            ExpressionNode* node = nullptr;
            ExpressionNode* copiedNode = copyNode(node);
            Assert::IsNull(copiedNode);
        }

        /**
         * @brief Тест 2: Копия переменной.
         * @details Проверяет создание копии узла типа Variable с значением "x".
         */
        TEST_METHOD(Test2_CopyVariableNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Variable, "x");
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 3: Копия отрицания.
         * @details Проверяет создание копии узла типа Not без операндов.
         */
        TEST_METHOD(Test3_CopyNotNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Not);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 4: Копия конъюнкции.
         * @details Проверяет создание копии узла типа And без операндов.
         */
        TEST_METHOD(Test4_CopyAndNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 5: Копия дизъюнкции.
         * @details Проверяет создание копии узла типа Or без операндов.
         */
        TEST_METHOD(Test5_CopyOrNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Or);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 6: Копия импликации.
         * @details Проверяет создание копии узла типа Implication без операндов.
         */
        TEST_METHOD(Test6_CopyImplicationNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Implication);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 7: Копия эквивалентности.
         * @details Проверяет создание копии узла типа Equivalence без операндов.
         */
        TEST_METHOD(Test7_CopyEquivalenceNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Equivalence);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 8: Копия отрицания с операндом.
         * @details Проверяет создание копии узла Not с одним операндом "a".
         */
        TEST_METHOD(Test8_CopyNotWithOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Not);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 9: Копия конъюнкции с левым операндом.
         * @details Проверяет создание копии узла And с левым операндом "a".
         */
        TEST_METHOD(Test9_CopyAndWithLeftOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 10: Копия конъюнкции с правым операндом.
         * @details Проверяет создание копии узла And с правым операндом "a".
         */
        TEST_METHOD(Test10_CopyAndWithRightOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 11: Копия конъюнкции с двумя операндами.
         * @details Проверяет создание копии узла And с операндами "a" и "b".
         */
        TEST_METHOD(Test11_CopyAndWithTwoOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 12: Копия дизъюнкции с левым операндом.
         * @details Проверяет создание копии узла Or с левым операндом "a".
         */
        TEST_METHOD(Test12_CopyOrWithLeftOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Or);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 13: Копия дизъюнкции с правым операндом.
         * @details Проверяет создание копии узла Or с правым операндом "a".
         */
        TEST_METHOD(Test13_CopyOrWithRightOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Or);
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 14: Копия дизъюнкции с двумя операндами.
         * @details Проверяет создание копии узла Or с операндами "a" и "b".
         */
        TEST_METHOD(Test14_CopyOrWithTwoOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Or);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 15: Копия импликации с левым операндом.
         * @details Проверяет создание копии узла Implication с левым операндом "a".
         */
        TEST_METHOD(Test15_CopyImplicationWithLeftOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Implication);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 16: Копия импликации с правым операндом.
         * @details Проверяет создание копии узла Implication с правым операндом "a".
         */
        TEST_METHOD(Test16_CopyImplicationWithRightOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Implication);
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 17: Копия импликации с двумя операндами.
         * @details Проверяет создание копии узла Implication с операндами "a" и "b".
         */
        TEST_METHOD(Test17_CopyImplicationWithTwoOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Implication);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 18: Копия эквивалентности с левым операндом.
         * @details Проверяет создание копии узла Equivalence с левым операндом "a".
         */
        TEST_METHOD(Test18_CopyEquivalenceWithLeftOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Equivalence);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 19: Копия эквивалентности с правым операндом.
         * @details Проверяет создание копии узла Equivalence с правым операндом "a".
         */
        TEST_METHOD(Test19_CopyEquivalenceWithRightOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Equivalence);
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 20: Копия эквивалентности с двумя операндами.
         * @details Проверяет создание копии узла Equivalence с операндами "a" и "b".
         */
        TEST_METHOD(Test20_CopyEquivalenceWithTwoOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Equivalence);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 21: Копия вложенной операции слева.
         * @details Проверяет создание копии узла с вложенной операцией And слева.
         */
        TEST_METHOD(Test21_CopyNestedOperationLeft)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::And);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 22: Копия вложенной операции справа.
         * @details Проверяет создание копии узла с вложенной операцией And справа.
         */
        TEST_METHOD(Test22_CopyNestedOperationRight)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::And);

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 23: Копия вложенной операции слева и справа.
         * @details Проверяет создание копии узла с вложенными операциями And с обеих сторон.
         */
        TEST_METHOD(Test23_CopyNestedOperationBothSides)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::And);
            node->right = new ExpressionNode(TokenType::And);

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 24: Копия переменной с пустым значением.
         * @details Проверяет создание копии узла Variable с пустой строкой в качестве значения.
         */
        TEST_METHOD(Test24_CopyVariableWithEmptyValue)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Variable, "");
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 25: Копия вложенной операции слева (с операндами).
         * @details Проверяет создание копии узла с вложенной операцией And слева, содержащей операнды.
         */
        TEST_METHOD(Test25_CopyNestedLeftOperationWithOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::And);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->right = new ExpressionNode(TokenType::Variable, "b");
            node->right = new ExpressionNode(TokenType::Variable, "");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 26: Копия вложенной операции справа (с операндами).
         * @details Проверяет создание копии узла с вложенной операцией And справа, содержащей операнды.
         */
        TEST_METHOD(Test26_CopyNestedRightOperationWithOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Variable, "");
            node->right = new ExpressionNode(TokenType::And);
            node->right->left = new ExpressionNode(TokenType::Variable, "a");
            node->right->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 27: Копия вложенной операции слева и справа (с операндами).
         * @details Проверяет создание копии узла с вложенными операциями And с обеих сторон, содержащими операнды.
         */
        TEST_METHOD(Test27_CopyNestedOperationsBothSidesWithOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::And);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->right = new ExpressionNode(TokenType::Variable, "b");
            node->right = new ExpressionNode(TokenType::And);
            node->right->left = new ExpressionNode(TokenType::Variable, "c");
            node->right->right = new ExpressionNode(TokenType::Variable, "d");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 28: Копия разнородной вложенной операции.
         * @details Проверяет создание копии узла с разнородными вложенными операциями (Or слева и Variable справа).
         */
        TEST_METHOD(Test28_CopyHeterogeneousNestedOperation)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Or);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->right = new ExpressionNode(TokenType::Variable, "b");
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 29: Копия с комбинацией всех типов операций.
         * @details Проверяет создание копии сложного дерева с комбинацией всех типов операций.
         */
        TEST_METHOD(Test29_CopyWithAllOperationTypesCombination)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);

            node->left = new ExpressionNode(TokenType::Or);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->right = new ExpressionNode(TokenType::Not);
            node->left->right->left = new ExpressionNode(TokenType::Variable, "b");

            node->right = new ExpressionNode(TokenType::Implication);
            node->right->left = new ExpressionNode(TokenType::Equivalence);
            node->right->left->left = new ExpressionNode(TokenType::Variable, "c");
            node->right->left->right = new ExpressionNode(TokenType::Variable, "d");
            node->right->right = new ExpressionNode(TokenType::Variable, "e");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief Тест 30: Глубокое копирование дерева.
         * @details Проверяет создание копии глубокого дерева выражений.
         */
        TEST_METHOD(Test30_DeepCopyTree)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);

            node->left = new ExpressionNode(TokenType::Or);
            node->left->left = new ExpressionNode(TokenType::And);
            node->left->left->left = new ExpressionNode(TokenType::Or);
            node->left->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->left->left->right = new ExpressionNode(TokenType::Variable, "b");

            node->left->left->right = new ExpressionNode(TokenType::Variable, "a");

            node->left->right = new ExpressionNode(TokenType::Or);
            node->left->right->left = new ExpressionNode(TokenType::Variable, "c");
            node->left->right->right = new ExpressionNode(TokenType::Variable, "d");

            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }
    };
}