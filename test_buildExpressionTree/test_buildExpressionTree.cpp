#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testbuildExpressionTree
{
    TEST_CLASS(testbuildExpressionTree)
    {
    public:

        /**
         * @brief Тест 1: Проверка обработки пустого списка токенов
         *
         * Функция должна добавить ошибку missingOperation при пустом списке токенов
         */
        TEST_METHOD(EmptyTokenList)
        {
            std::vector<Token> tokens;
            std::set<Error> errorList;

            // Вызываем функцию с пустым списком токенов
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула nullptr
            Assert::IsNull(result);

            // Проверяем, что в списке ошибок есть ошибка missingOperation
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::missingOperation) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"Ошибка missingOperation не добавлена в список ошибок");
        }

        /**
         * @brief Тест 2: Проверка обработки одного токена (переменная)
         *
         * Функция должна корректно создать дерево с одной переменной
         */
        TEST_METHOD(SingleVariableToken)
        {
            std::vector<Token> tokens = { Token(TokenType::Variable, "a", 0) };
            std::set<Error> errorList;

            // Вызываем функцию с одним токеном-переменной
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - переменная
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->type));

            // Проверяем, что значение переменной правильное
            Assert::AreEqual(std::string("a"), result->value);

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 3: Проверка обработки одного токена (оператор)
         *
         * Функция должна добавить ошибку insufficientOperands при одном токене-операторе
         */
        TEST_METHOD(SingleOperatorToken)
        {
            std::vector<Token> tokens = { Token(TokenType::Not, "!", 0) };
            std::set<Error> errorList;

            // Вызываем функцию с одним токеном-оператором
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула nullptr
            Assert::IsNull(result);

            // Проверяем, что в списке ошибок есть ошибка insufficientOperands
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::insufficientOperands) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"Ошибка insufficientOperands не добавлена в список ошибок");
        }

        /**
         * @brief Тест 4: Проверка обработки простого отрицания
         *
         * Функция должна корректно создать дерево с отрицанием переменной
         */
        TEST_METHOD(SimpleNegation)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Not, "!", 1)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами для простого отрицания
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - отрицание
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // Проверяем, что у отрицания есть операнд
            Assert::IsNotNull(result->left);

            // Проверяем, что операнд - переменная "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->type));
            Assert::AreEqual(std::string("a"), result->left->value);

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 5: Проверка обработки простой конъюнкции
         *
         * Функция должна корректно создать дерево с конъюнкцией двух переменных
         */
        TEST_METHOD(SimpleConjunction)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами для простой конъюнкции
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - конъюнкция
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->type));

            // Проверяем, что у конъюнкции есть левый и правый операнды
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // Проверяем, что левый операнд - переменная "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->type));
            Assert::AreEqual(std::string("a"), result->left->value);

            // Проверяем, что правый операнд - переменная "b"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->right->type));
            Assert::AreEqual(std::string("b"), result->right->value);

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 6: Проверка обработки отрицания перед скобкой
         *
         * Функция должна корректно создать дерево с отрицанием выражения в скобках
         */
        TEST_METHOD(NegationBeforeBracket)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2),
                Token(TokenType::Not, "!", 3)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами для отрицания перед скобкой
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - отрицание
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // Проверяем, что у отрицания есть операнд
            Assert::IsNotNull(result->left);

            // Проверяем, что операнд - конъюнкция
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->left->type));

            // Проверяем, что у конъюнкции есть левый и правый операнды
            Assert::IsNotNull(result->left->left);
            Assert::IsNotNull(result->left->right);

            // Проверяем, что левый операнд конъюнкции - переменная "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->left->type));
            Assert::AreEqual(std::string("a"), result->left->left->value);

            // Проверяем, что правый операнд конъюнкции - переменная "b"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->right->type));
            Assert::AreEqual(std::string("b"), result->left->right->value);

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 7: Проверка обработки комплексного выражения
         *
         * Функция должна корректно создать дерево для комплексного выражения
         */
        TEST_METHOD(ComplexExpression)
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

            // Вызываем функцию с токенами для комплексного выражения
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - дизъюнкция
            Assert::AreEqual(static_cast<int>(TokenType::Or), static_cast<int>(result->type));

            // Проверяем, что у дизъюнкции есть левый и правый операнды
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // Проверяем, что левый операнд - конъюнкция
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->left->type));

            // Проверяем, что правый операнд - конъюнкция
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->right->type));

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 8: Проверка обработки некорректного токена
         *
         * Функция должна добавить ошибку invalidVariableName при некорректном имени переменной
         */
        TEST_METHOD(InvalidToken)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "1a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами, содержащими некорректное имя переменной
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула nullptr
            Assert::IsNull(result);

            // Проверяем, что в списке ошибок есть ошибка invalidVariableName
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::invalidVariableName && error.position == 0) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"Ошибка invalidVariableName не добавлена в список ошибок");
        }

        /**
         * @brief Тест 9: Проверка обработки импликации
         *
         * Функция должна корректно создать дерево с импликацией двух переменных
         */
        TEST_METHOD(Implication)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Implication, ">", 2)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами для импликации
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - импликация
            Assert::AreEqual(static_cast<int>(TokenType::Implication), static_cast<int>(result->type));

            // Проверяем, что у импликации есть левый и правый операнды
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // Проверяем, что левый операнд - переменная "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->type));
            Assert::AreEqual(std::string("a"), result->left->value);

            // Проверяем, что правый операнд - переменная "b"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->right->type));
            Assert::AreEqual(std::string("b"), result->right->value);

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 10: Проверка обработки эквивалентности
         *
         * Функция должна корректно создать дерево с эквивалентностью двух переменных
         */
        TEST_METHOD(Equivalence)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Equivalence, "~", 2)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами для эквивалентности
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - эквивалентность
            Assert::AreEqual(static_cast<int>(TokenType::Equivalence), static_cast<int>(result->type));

            // Проверяем, что у эквивалентности есть левый и правый операнды
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // Проверяем, что левый операнд - переменная "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->type));
            Assert::AreEqual(std::string("a"), result->left->value);

            // Проверяем, что правый операнд - переменная "b"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->right->type));
            Assert::AreEqual(std::string("b"), result->right->value);

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 11: Проверка обработки множественных операций
         *
         * Функция должна корректно создать дерево для выражения с несколькими операциями
         */
        TEST_METHOD(MultipleOperations)
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

            // Вызываем функцию с токенами для множественных операций
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - дизъюнкция
            Assert::AreEqual(static_cast<int>(TokenType::Or), static_cast<int>(result->type));

            // Проверяем, что у дизъюнкции есть левый и правый операнды
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // Проверяем, что левый операнд - конъюнкция
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->left->type));

            // Проверяем, что правый операнд - переменная "d"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->right->type));
            Assert::AreEqual(std::string("d"), result->right->value);

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 12: Проверка обработки ошибки в середине выражения
         *
         * Функция должна добавить ошибку unsupportedOperation при неподдерживаемой операции
         */
        TEST_METHOD(ErrorInMiddleOfExpression)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Any, "+", 2),
                Token(TokenType::Variable, "c", 3),
                Token(TokenType::And, "&", 4)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами, содержащими неподдерживаемую операцию
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула nullptr
            Assert::IsNull(result);

            // Проверяем, что в списке ошибок есть ошибка unsupportedOperation
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::unsupportedOperation && error.position == 2) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"Ошибка unsupportedOperation не добавлена в список ошибок");
        }

        /**
         * @brief Тест 13: Проверка обработки отрицания над сложным выражением
         *
         * Функция должна корректно создать дерево с отрицанием сложного выражения
         */
        TEST_METHOD(NegationOverComplexExpression)
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

            // Вызываем функцию с токенами для отрицания над сложным выражением
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - отрицание
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // Проверяем, что у отрицания есть операнд
            Assert::IsNotNull(result->left);

            // Проверяем, что операнд - дизъюнкция
            Assert::AreEqual(static_cast<int>(TokenType::Or), static_cast<int>(result->left->type));

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 14: Проверка обработки двойного отрицания
         *
         * Функция должна корректно создать дерево с двойным отрицанием переменной
         */
        TEST_METHOD(DoubleNegation)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Not, "!", 1),
                Token(TokenType::Not, "!", 2)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами для двойного отрицания
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - отрицание
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // Проверяем, что у отрицания есть операнд
            Assert::IsNotNull(result->left);

            // Проверяем, что операнд - отрицание
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->left->type));

            // Проверяем, что у вложенного отрицания есть операнд
            Assert::IsNotNull(result->left->left);

            // Проверяем, что операнд вложенного отрицания - переменная "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->left->type));
            Assert::AreEqual(std::string("a"), result->left->left->value);

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 15: Проверка обработки вложенных отрицаний
         *
         * Функция должна корректно создать дерево с вложенными отрицаниями
         */
        TEST_METHOD(NestedNegations)
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

            // Вызываем функцию с токенами для вложенных отрицаний
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула не nullptr
            Assert::IsNotNull(result);

            // Проверяем, что тип узла - отрицание
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // Проверяем, что список ошибок пуст
            Assert::IsTrue(errorList.empty());

            // Освобождаем память
            delete result;
        }

        /**
         * @brief Тест 16: Проверка обработки переменной с запрещенными символами
         *
         * Функция должна добавить ошибку invalidVariableChar при переменной с запрещенными символами
         */
        TEST_METHOD(VariableWithForbiddenChars)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a_!", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами, содержащими переменную с запрещенными символами
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула nullptr
            Assert::IsNull(result);

            // Проверяем, что в списке ошибок есть ошибка invalidVariableChar
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::invalidVariableChar && error.position == 0) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"Ошибка invalidVariableChar не добавлена в список ошибок");
        }

        /**
         * @brief Тест 17: Проверка обработки только бинарных операций
         *
         * Функция должна добавить ошибку insufficientOperands для каждой бинарной операции
         */
        TEST_METHOD(OnlyBinaryOperations)
        {
            std::vector<Token> tokens = {
                Token(TokenType::And, "&", 0),
                Token(TokenType::Implication, ">", 1),
                Token(TokenType::Equivalence, "~", 2)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами, содержащими только бинарные операции
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула nullptr
            Assert::IsNull(result);

            // Проверяем, что в списке ошибок есть ошибки insufficientOperands для каждой операции
            int errorCount = 0;
            for (const auto& error : errorList) {
                if (error.type == Error::insufficientOperands) {
                    errorCount++;
                }
            }
            Assert::AreEqual(3, errorCount, L"Неверное количество ошибок insufficientOperands");
        }

        /**
         * @brief Тест 18: Проверка обработки нескольких типов ошибок
         *
         * Функция должна добавить ошибки разных типов при наличии нескольких проблем
         */
        TEST_METHOD(MultipleErrorTypes)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a_!", 0),
                Token(TokenType::And, "&", 1)
            };
            std::set<Error> errorList;

            // Вызываем функцию с токенами, содержащими несколько типов ошибок
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // Проверяем, что функция вернула nullptr
            Assert::IsNull(result);

            // Проверяем, что в списке ошибок есть ошибка invalidVariableChar
            bool hasInvalidCharError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::invalidVariableChar && error.position == 0) {
                    hasInvalidCharError = true;
                    break;
                }
            }
            Assert::IsTrue(hasInvalidCharError, L"Ошибка invalidVariableChar не добавлена в список ошибок");

            // Проверяем, что в списке ошибок есть ошибка insufficientOperands
            bool hasInsufficientOperandsError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::insufficientOperands && error.position == 1) {
                    hasInsufficientOperandsError = true;
                    break;
                }
            }
            Assert::IsTrue(hasInsufficientOperandsError, L"Ошибка insufficientOperands не добавлена в список ошибок");
        }
    };
}