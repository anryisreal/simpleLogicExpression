#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"
#include "testFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testTransformImplicationAndEquivalence
{
    TEST_CLASS(testTransformImplicationAndEquivalence)
    {
    public:

        /**
         * @brief Тест 1: Простое преобразование импликации
         *
         * Проверяет, что импликация A > B корректно преобразуется в !A | B
         */
        TEST_METHOD(Test1_SimpleImplicationTransform)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            // Проверка результата
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Дерево не соответствует ожидаемому после преобразования импликации");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 2: Простое преобразование эквивалентности
         *
         * Проверяет, что эквивалентность A ~ B корректно преобразуется в (A & B) | (!A & !B)
         */
        TEST_METHOD(Test2_SimpleEquivalenceTransform)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат
            ExpressionNode* expected = new ExpressionNode(Or);

            // Левая часть: A & B
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Variable, "b");

            // Правая часть: !A & !B
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "a");
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Variable, "b");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            // Проверка результата
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Дерево не соответствует ожидаемому после преобразования эквивалентности");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 3: Отсутствие преобразования для конъюнкции
         *
         * Проверяет, что конъюнкция A & B остается без изменений
         */
        TEST_METHOD(Test3_NoTransformForAnd)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат (должен остаться таким же)
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            // Проверка результата
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Конъюнкция не должна была измениться, но изменилась");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 4: Отсутствие преобразования для дизъюнкции
         *
         * Проверяет, что дизъюнкция A | B остается без изменений
         */
        TEST_METHOD(Test4_NoTransformForOr)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Or);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат (должен остаться таким же)
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            // Проверка результата
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Дизъюнкция не должна была измениться, но изменилась");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 5: Импликация внутри отрицания
         *
         * Проверяет преобразование импликации внутри оператора отрицания: !(A > B)
         */
        TEST_METHOD(Test5_ImplicationInsideNot)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Implication);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат: !(!A | B)
            ExpressionNode* expected = new ExpressionNode(Not);
            expected->left = new ExpressionNode(Or);
            expected->left->left = new ExpressionNode(Not);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Variable, "b");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            // Проверка результата
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование импликации внутри отрицания");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 6: Эквивалентность внутри отрицания
         *
         * Проверяет преобразование эквивалентности внутри оператора отрицания: !(A ~ B)
         */
        TEST_METHOD(Test6_EquivalenceInsideNot)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Equivalence);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");

            // Ожидаемый результат: !((A & B) | (!A & !B))
            ExpressionNode* expected = new ExpressionNode(Not);
            expected->left = new ExpressionNode(Or);

            // Левая часть OR: A & B
            expected->left->left = new ExpressionNode(And);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right = new ExpressionNode(Variable, "b");

            // Правая часть OR: !A & !B
            expected->left->right = new ExpressionNode(And);
            expected->left->right->left = new ExpressionNode(Not);
            expected->left->right->left->left = new ExpressionNode(Variable, "a");
            expected->left->right->right = new ExpressionNode(Not);
            expected->left->right->right->left = new ExpressionNode(Variable, "b");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            // Проверка результата
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование эквивалентности внутри отрицания");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 7: Импликация внутри конъюнкции
         *
         * Проверяет преобразование импликации внутри конъюнкции: A & (B > C)
         */
        TEST_METHOD(Test7_ImplicationInsideAnd)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Implication);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат: A & (!B | C)
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            // Проверка результата
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование импликации внутри конъюнкции");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 8: Эквивалентность внутри конъюнкции
         *
         * Проверяет преобразование эквивалентности внутри конъюнкции: A & (B ~ C)
         */
        TEST_METHOD(Test8_EquivalenceInsideAnd)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Equivalence);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат: A & ((B & C) | (!B & !C))
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);

            // Левая часть OR: B & C
            expected->right->left = new ExpressionNode(And);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->left->right = new ExpressionNode(Variable, "c");

            // Правая часть OR: !B & !C
            expected->right->right = new ExpressionNode(And);
            expected->right->right->left = new ExpressionNode(Not);
            expected->right->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right->right = new ExpressionNode(Not);
            expected->right->right->right->left = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            // Проверка результата
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование эквивалентности внутри конъюнкции");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 9: Импликация внутри дизъюнкции
         *
         * Проверяет преобразование импликации внутри дизъюнкции: A | (B > C)
         */
        TEST_METHOD(Test9_ImplicationInsideOr)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Or);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Implication);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат: A | (!B | C)
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование импликации внутри дизъюнкции");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 10: Эквивалентность внутри дизъюнкции
         *
         * Проверяет преобразование эквивалентности внутри дизъюнкции: A | (B ~ C)
         */
        TEST_METHOD(Test10_EquivalenceInsideOr)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Or);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Equivalence);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат: A | ((B & C) | (!B & !C))
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);

            // Левая часть вложенного OR: B & C
            expected->right->left = new ExpressionNode(And);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->left->right = new ExpressionNode(Variable, "c");

            // Правая часть вложенного OR: !B & !C
            expected->right->right = new ExpressionNode(And);
            expected->right->right->left = new ExpressionNode(Not);
            expected->right->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right->right = new ExpressionNode(Not);
            expected->right->right->right->left = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование эквивалентности внутри дизъюнкции");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 11: Двойная импликация
         *
         * Проверяет преобразование двойной импликации: A > (B > C)
         */
        TEST_METHOD(Test11_DoubleImplication)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Implication);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат: !A | (!B | C)
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование двойной импликации");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 12: Двойная эквивалентность
         *
         * Проверяет преобразование двойной эквивалентности: A ~ (B ~ C)
         */
        TEST_METHOD(Test12_DoubleEquivalence)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Equivalence);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат: (A & ((B & C) | (!B & !C))) | (!A & !((B & C) | (!B & !C)))
            ExpressionNode* expected = new ExpressionNode(Or);

            // Левая часть OR: A & ((B & C) | (!B & !C))
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Or);

            // Левая часть вложенного OR: B & C
            expected->left->right->left = new ExpressionNode(And);
            expected->left->right->left->left = new ExpressionNode(Variable, "b");
            expected->left->right->left->right = new ExpressionNode(Variable, "c");

            // Правая часть вложенного OR: !B & !C
            expected->left->right->right = new ExpressionNode(And);
            expected->left->right->right->left = new ExpressionNode(Not);
            expected->left->right->right->left->left = new ExpressionNode(Variable, "b");
            expected->left->right->right->right = new ExpressionNode(Not);
            expected->left->right->right->right->left = new ExpressionNode(Variable, "c");

            // Правая часть основного OR: !A & !((B & C) | (!B & !C))
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "a");
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Or);

            // Копия левой части вложенного OR
            expected->right->right->left->left = new ExpressionNode(And);
            expected->right->right->left->left->left = new ExpressionNode(Variable, "b");
            expected->right->right->left->left->right = new ExpressionNode(Variable, "c");
            expected->right->right->left->right = new ExpressionNode(And);
            expected->right->right->left->right->left = new ExpressionNode(Not);
            expected->right->right->left->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right->left->right->right = new ExpressionNode(Not);
            expected->right->right->left->right->right->left = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование двойной эквивалентности");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 13: Эквивалентность внутри импликации
         *
         * Проверяет преобразование (A ~ B) > C
         */
        TEST_METHOD(Test13_EquivalenceInsideImplication)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Equivalence);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");
            input->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат: !((A & B) | (!A & !B)) | C
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);

            // Левая часть NOT: (A & B) | (!A & !B)
            expected->left->left = new ExpressionNode(Or);

            // Левая часть OR: A & B
            expected->left->left->left = new ExpressionNode(And);
            expected->left->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->left->right = new ExpressionNode(Variable, "b");

            // Правая часть OR: !A & !B
            expected->left->left->right = new ExpressionNode(And);
            expected->left->left->right->left = new ExpressionNode(Not);
            expected->left->left->right->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right->right = new ExpressionNode(Not);
            expected->left->left->right->right->left = new ExpressionNode(Variable, "b");

            // Правая часть основного OR: C
            expected->right = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование эквивалентности внутри импликации");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 14: Импликация внутри эквивалентности
         *
         * Проверяет преобразование (A > B) ~ C
         */
        TEST_METHOD(Test14_ImplicationInsideEquivalence)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Implication);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");
            input->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат: ((!A | B) & C) | (!(!A | B) & !C)
            ExpressionNode* expected = new ExpressionNode(Or);

            // Левая часть OR: (!A | B) & C
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Or);
            expected->left->left->left = new ExpressionNode(Not);
            expected->left->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right = new ExpressionNode(Variable, "b");
            expected->left->right = new ExpressionNode(Variable, "c");

            // Правая часть OR: !(!A | B) & !C
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Or);
            expected->right->left->left->left = new ExpressionNode(Not);
            expected->right->left->left->left->left = new ExpressionNode(Variable, "a");
            expected->right->left->left->right = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"Некорректное преобразование импликации внутри эквивалентности");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 15: Множественная эквиваленция
        *
        * Проверяет преобразование a ~ (c ~ (d ~ e))
        */
        TEST_METHOD(Test15_MultipleEquivalence)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Equivalence);
            input->right->left = new ExpressionNode(Variable, "c");
            input->right->right = new ExpressionNode(Equivalence);
            input->right->right->left = new ExpressionNode(Variable, "d");
            input->right->right->right = new ExpressionNode(Variable, "e");

            // Ожидаемый результат
            ExpressionNode* expected = new ExpressionNode(Or);

            // Левая часть OR: a ∧ ((c ∧ (d ∧ e)) ∨ (¬c ∧ ¬(d ∧ e)))
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Or);

            // Левая часть вложенного OR: c ∧ (d ∧ e)
            expected->left->right->left = new ExpressionNode(And);
            expected->left->right->left->left = new ExpressionNode(Variable, "c");
            expected->left->right->left->right = new ExpressionNode(And);
            expected->left->right->left->right->left = new ExpressionNode(Variable, "d");
            expected->left->right->left->right->right = new ExpressionNode(Variable, "e");

            // Правая часть вложенного OR: ¬c ∧ ¬(d ∧ e)
            expected->left->right->right = new ExpressionNode(And);
            expected->left->right->right->left = new ExpressionNode(Not);
            expected->left->right->right->left->left = new ExpressionNode(Variable, "c");
            expected->left->right->right->right = new ExpressionNode(Not);
            expected->left->right->right->right->left = new ExpressionNode(And);
            expected->left->right->right->right->left->left = new ExpressionNode(Variable, "d");
            expected->left->right->right->right->left->right = new ExpressionNode(Variable, "e");

            // Правая часть основного OR: ¬a ∧ ¬((c ∧ (d ∧ e)) ∨ (¬c ∧ ¬(d ∧ e)))
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "a");
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Or);

            // Копия левой части вложенного OR
            expected->right->right->left->left = new ExpressionNode(And);
            expected->right->right->left->left->left = new ExpressionNode(Variable, "c");
            expected->right->right->left->left->right = new ExpressionNode(And);
            expected->right->right->left->left->right->left = new ExpressionNode(Variable, "d");
            expected->right->right->left->left->right->right = new ExpressionNode(Variable, "e");

            expected->right->right->left->right = new ExpressionNode(And);
            expected->right->right->left->right->left = new ExpressionNode(Not);
            expected->right->right->left->right->left->left = new ExpressionNode(Variable, "c");
            expected->right->right->left->right->right = new ExpressionNode(Not);
            expected->right->right->left->right->right->left = new ExpressionNode(And);
            expected->right->right->left->right->right->left->left = new ExpressionNode(Variable, "d");
            expected->right->right->left->right->right->left->right = new ExpressionNode(Variable, "e");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input), 
                L"Некорректное преобразование множественной эквивалентности");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 16: Множественная импликация
         *
         * Проверяет преобразование a → (c → (d → e))
         */
        TEST_METHOD(Test16_MultipleImplication)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Implication);
            input->right->left = new ExpressionNode(Variable, "c");
            input->right->right = new ExpressionNode(Implication);
            input->right->right->left = new ExpressionNode(Variable, "d");
            input->right->right->right = new ExpressionNode(Variable, "e");

            // Ожидаемый результат
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "c");
            expected->right->right = new ExpressionNode(Or);
            expected->right->right->left = new ExpressionNode(Not);
            expected->right->right->left->left = new ExpressionNode(Variable, "d");
            expected->right->right->right = new ExpressionNode(Variable, "e");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input), 
                L"Некорректное преобразование множественной импликации");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 17: Множественная комбинация эквиваленции и импликации
         *
         * Проверяет преобразование (a → b) ↔ (c ↔ d)
         */
        TEST_METHOD(Test17_CombinationImplicationEquivalence)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Implication);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");
            input->right = new ExpressionNode(Equivalence);
            input->right->left = new ExpressionNode(Variable, "c");
            input->right->right = new ExpressionNode(Variable, "d");

            // Ожидаемый результат
            ExpressionNode* expected = new ExpressionNode(Or);

            // Левая часть OR: (¬a ∨ b) ∧ ((c ∧ d) ∨ (¬c ∧ ¬d))
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Or);
            expected->left->left->left = new ExpressionNode(Not);
            expected->left->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right = new ExpressionNode(Variable, "b");

            expected->left->right = new ExpressionNode(Or);
            expected->left->right->left = new ExpressionNode(And);
            expected->left->right->left->left = new ExpressionNode(Variable, "c");
            expected->left->right->left->right = new ExpressionNode(Variable, "d");
            expected->left->right->right = new ExpressionNode(And);
            expected->left->right->right->left = new ExpressionNode(Not);
            expected->left->right->right->left->left = new ExpressionNode(Variable, "c");
            expected->left->right->right->right = new ExpressionNode(Not);
            expected->left->right->right->right->left = new ExpressionNode(Variable, "d");

            // Правая часть OR: ¬(¬a ∨ b) ∧ ¬((c ∧ d) ∨ (¬c ∧ ¬d))
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Or);
            expected->right->left->left->left = new ExpressionNode(Not);
            expected->right->left->left->left->left = new ExpressionNode(Variable, "a");
            expected->right->left->left->right = new ExpressionNode(Variable, "b");

            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Or);
            expected->right->right->left->left = new ExpressionNode(And);
            expected->right->right->left->left->left = new ExpressionNode(Variable, "c");
            expected->right->right->left->left->right = new ExpressionNode(Variable, "d");
            expected->right->right->left->right = new ExpressionNode(And);
            expected->right->right->left->right->left = new ExpressionNode(Not);
            expected->right->right->left->right->left->left = new ExpressionNode(Variable, "c");
            expected->right->right->left->right->right = new ExpressionNode(Not);
            expected->right->right->left->right->right->left = new ExpressionNode(Variable, "d");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input), 
                L"Некорректное преобразование комбинации импликации и эквивалентности");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 18: Простая операция в составе импликации
         *
         * Проверяет преобразование (a ∨ b) → c
         */
        TEST_METHOD(Test18_SimpleOperationInImplication)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Or);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");
            input->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->left = new ExpressionNode(Or);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);

            Assert::IsTrue(compareExpressionTrees(expected, input), 
                L"Некорректное преобразование простой операции в составе импликации");

            // Очистка памяти
            delete input;
            delete expected;
        }

        /**
         * @brief Тест 19: Простая операция в составе эквиваленции
         *
         * Проверяет преобразование (a ∨ b) ↔ c
         */
        TEST_METHOD(Test19_SimpleOperationInEquivalence)
        {
            // Подготовка входных данных
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Or);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");
            input->right = new ExpressionNode(Variable, "c");

            // Ожидаемый результат
            ExpressionNode* expected = new ExpressionNode(Or);

            // Левая часть OR: (a ∨ b) ∧ c
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Or);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right = new ExpressionNode(Variable, "b");
            expected->left->right = new ExpressionNode(Variable, "c");

            // Правая часть OR: ¬(a ∨ b) ∧ ¬c
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Or);
            expected->right->left->left->left = new ExpressionNode(Variable, "a");
            expected->right->left->left->right = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Variable, "c");

            // Выполнение преобразования
            transformImplicationAndEquivalence(input);
            
            Assert::IsTrue(compareExpressionTrees(expected, input), 
                L"Некорректное преобразование простой операции в составе эквивалентности");

            // Очистка памяти
            delete input;
            delete expected;
        }
    };
}