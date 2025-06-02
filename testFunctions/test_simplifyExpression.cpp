#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"
#include "testFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testSimplifyExpression
{
    TEST_CLASS(testSimplifyExpression)
    {
    public:
        /**
         * @brief Тест 1: Отрицание конъюнкции (первый закон де Моргана)
         */
        TEST_METHOD(Test1_NegationOfConjunction)
        {
            // Входные данные: !(a & b)
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(And);
            input->right->left = new ExpressionNode(Variable, "a");
            input->right->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат: !a | !b
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное применение первого закона де Моргана");

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 2: Отрицание дизъюнкции (второй закон де Моргана)
         */
        TEST_METHOD(Test2_NegationOfDisjunction)
        {
            // Входные данные: !(a | b)
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(Or);
            input->right->left = new ExpressionNode(Variable, "a");
            input->right->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат: !a & !b
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное применение второго закона де Моргана");

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 3: Нет преобразований (конъюнкция)
         */
        TEST_METHOD(Test3_NoTransformAnd)
        {
            // Входные данные: a & b
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат: a & b (без изменений)
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"Не ожидалось изменений выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Конъюнкция не должна была измениться");

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 4: Нет преобразований (дизъюнкция)
         */
        TEST_METHOD(Test4_NoTransformOr)
        {
            // Входные данные: a | b
            ExpressionNode* input = new ExpressionNode(Or);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат: a | b (без изменений)
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"Не ожидалось изменений выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Дизъюнкция не должна была измениться");

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 5: Нет преобразований (импликация)
         */
        TEST_METHOD(Test5_NoTransformImplication)
        {
            // Входные данные: a > b
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат: a > b (без изменений)
            ExpressionNode* expected = new ExpressionNode(Implication);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"Не ожидалось изменений выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Импликация не должна была измениться");

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 6: Нет преобразований (эквиваленция)
         */
        TEST_METHOD(Test6_NoTransformEquivalence)
        {
            // Входные данные: a ~ b
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат: a ~ b (без изменений)
            ExpressionNode* expected = new ExpressionNode(Equivalence);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"Не ожидалось изменений выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Эквиваленция не должна была измениться");

            delete input;
            delete expected;
        }

        /**
        * @brief Тест 7: Первый закон внутри отрицания
        * !(!(a ∧ b)) → !!a ∧ !!b
        */
        TEST_METHOD(Test7_FirstLawInsideNegation)
        {
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(Not);
            input->right->right = new ExpressionNode(And);
            input->right->right->left = new ExpressionNode(Variable, "a");
            input->right->right->right = new ExpressionNode(Variable, "b");

            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(Not);
            expected->left->right->right = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное преобразование");

            delete input;
            delete expected;
        }

        /**
        * @brief Тест 8: Второй закон внутри отрицания
        * !(!(a ∨ b)) → !!a ∨ !!b
        */
        TEST_METHOD(Test8_SecondLawInsideNegation)
        {
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(Not);
            input->right->right = new ExpressionNode(Or);
            input->right->right->left = new ExpressionNode(Variable, "a");
            input->right->right->right = new ExpressionNode(Variable, "b");

            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(Not);
            expected->left->right->right = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное преобразование");

            delete input;
            delete expected;
        }

        /**
        * @brief Тест 9: Первый закон для сложного выражения
        * !((a ∧ b) ∨ c) → !(a ∧ b) ∧ !c
        */
        TEST_METHOD(Test9_FirstLawForComplexExpression)
        {
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(Or);
            input->right->left = new ExpressionNode(And);
            input->right->left->left = new ExpressionNode(Variable, "a");
            input->right->left->right = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(And);
            expected->left->right->left = new ExpressionNode(Variable, "a");
            expected->left->right->right = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное преобразование");

            delete input;
            delete expected;
        }

        /**
        * @brief Тест 10: Второй закон для сложного выражения
        * !((a ∨ b) ∧ c) → !(a ∨ b) ∨ !c
        */
        TEST_METHOD(Test10_SecondLawForComplexExpression)
        {
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(And);
            input->right->left = new ExpressionNode(Or);
            input->right->left->left = new ExpressionNode(Variable, "a");
            input->right->left->right = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(Or);
            expected->left->right->left = new ExpressionNode(Variable, "a");
            expected->left->right->right = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное преобразование");

            delete input;
            delete expected;
        }

        /**
        * @brief Тест 11: Вложенные первые законы
        * !(!(a ∧ b) ∧ c) → !(!a ∨ !b) ∨ !c
        */
        TEST_METHOD(Test11_NestedFirstLaws)
        {
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(And);
            input->right->left = new ExpressionNode(Not);
            input->right->left->right = new ExpressionNode(And);
            input->right->left->right->left = new ExpressionNode(Variable, "a");
            input->right->left->right->right = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(Or);
            expected->left->right->left = new ExpressionNode(Not);
            expected->left->right->left->right = new ExpressionNode(Variable, "a");
            expected->left->right->right = new ExpressionNode(Not);
            expected->left->right->right->right = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное преобразование");

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 12: Вложенные вторые законы
         * !(!(a ∨ b) ∨ c) → !(!a ∧ !b) ∧ !c
         */
        TEST_METHOD(Test12_NestedSecondLaws)
        {
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(Or);
            input->right->left = new ExpressionNode(Not);
            input->right->left->right = new ExpressionNode(Or);
            input->right->left->right->left = new ExpressionNode(Variable, "a");
            input->right->left->right->right = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(And);
            expected->left->right->left = new ExpressionNode(Not);
            expected->left->right->left->right = new ExpressionNode(Variable, "a");
            expected->left->right->right = new ExpressionNode(Not);
            expected->left->right->right->right = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное преобразование");

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 13: Последовательные первые и вторые законы
         * !(!(a ∨ b) ∧ !(c ∧ d)) → !(!a ∧ !b) ∨ !(!c ∨ !d)
         */
        TEST_METHOD(Test13_SequentialFirstAndSecondLaws)
        {
            ExpressionNode* input = new ExpressionNode(Not);
            input->right = new ExpressionNode(And);
            input->right->left = new ExpressionNode(Not);
            input->right->left->right = new ExpressionNode(Or);
            input->right->left->right->left = new ExpressionNode(Variable, "a");
            input->right->left->right->right = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Not);
            input->right->right->right = new ExpressionNode(And);
            input->right->right->right->left = new ExpressionNode(Variable, "c");
            input->right->right->right->right = new ExpressionNode(Variable, "d");

            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->right = new ExpressionNode(And);
            expected->left->right->left = new ExpressionNode(Not);
            expected->left->right->left->right = new ExpressionNode(Variable, "a");
            expected->left->right->right = new ExpressionNode(Not);
            expected->left->right->right->right = new ExpressionNode(Variable, "b");

            expected->right = new ExpressionNode(Not);
            expected->right->right = new ExpressionNode(Or);
            expected->right->right->left = new ExpressionNode(Not);
            expected->right->right->left->right = new ExpressionNode(Variable, "c");
            expected->right->right->right = new ExpressionNode(Not);
            expected->right->right->right->right = new ExpressionNode(Variable, "d");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"Ожидалось изменение выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Некорректное преобразование");

            delete input;
            delete expected;
        }

        /**
         * @brief Тест 14: Сохранение структуры сложного выражения
         * a ∧ (b ∨ c) → a ∧ (b ∨ c)
         */
        TEST_METHOD(Test14_PreserveComplexStructure)
        {
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Or);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"Не ожидалось изменений выражения");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"Структура не должна была измениться");

            delete input;
            delete expected;
        }
    };
}