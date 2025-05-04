#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testTransformImplicationAndEquivalence
{
    TEST_CLASS(testTransformImplicationAndEquivalence)
    {
    public:
        /**
         * ���������� ���������� ��� ������ ���������
         * @param expected ��������� ������
         * @param actual ����������� ������
         * @param path ������� ���� � ������ (��� �����������)
         * @return true ���� ������� ���������, false � ��������� ������
         */
        bool compareExpressionTrees(const ExpressionNode* expected, const ExpressionNode* actual, const std::string& path = "root") const
        {
            // ��� ���� nullptr - ���������
            if (expected == nullptr && actual == nullptr) {
                return true;
            }

            // ������ ���� �� ����� nullptr - �� ���������
            if (expected == nullptr) {
                Logger::WriteMessage(("������ � ����: " + path + " - �������� nullptr").c_str());
                return false;
            }
            if (actual == nullptr) {
                Logger::WriteMessage(("������ � ����: " + path + " - ����������� nullptr").c_str());
                return false;
            }

            // �������� ���� ����
            if (expected->type != actual->type) {
                Logger::WriteMessage(("������ � ����: " + path + " - ��� ���� �� ���������. ��������: " + tokenTypeToString(expected->type) + ", �������: " + tokenTypeToString(actual->type)).c_str());
                return false;
            }

            // �������� �������� ���������� (���� ����)
            if (expected->value != actual->value) {
                Logger::WriteMessage(("������ � ����: " + path + " - �������� �� ���������. ���������: '" + expected->value + "', ��������: '" + actual->value + "'").c_str());
                return false;
            }

            // ����������� �������� ������ � ������� �����������
            bool leftMatch = compareExpressionTrees(expected->left, actual->left, path + "-left");
            bool rightMatch = compareExpressionTrees(expected->right, actual->right, path + "-right");

            return leftMatch && rightMatch;
        }

        /**
         * @brief ����������� TokenType � ������ ��� ��������� �� �������
         * @param type ��� ������
         * @return ��������� ������������� ����
         */
        std::string tokenTypeToString(TokenType type) const
        {
            switch (type) {
            case Variable:    return "Variable";
            case Not:         return "Not";
            case And:         return "And";
            case Or:          return "Or";
            case Implication: return "Implication";
            case Equivalence: return "Equivalence";
            case Any:         return "Any";
            default:          return "Unknown";
            }
        }

        /**
         * @brief ���� 1: ������� �������������� ����������
         *
         * ���������, ��� ���������� A > B ��������� ������������� � !A | B
         */
        TEST_METHOD(Test1_SimpleImplicationTransform)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // ��������� ���������
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������ �� ������������� ���������� ����� �������������� ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 2: ������� �������������� ���������������
         *
         * ���������, ��� ��������������� A ~ B ��������� ������������� � (A & B) | (!A & !B)
         */
        TEST_METHOD(Test2_SimpleEquivalenceTransform)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // ��������� ���������
            ExpressionNode* expected = new ExpressionNode(Or);

            // ����� �����: A & B
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Variable, "b");

            // ������ �����: !A & !B
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "a");
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Variable, "b");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������ �� ������������� ���������� ����� �������������� ���������������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 3: ���������� �������������� ��� ����������
         *
         * ���������, ��� ���������� A & B �������� ��� ���������
         */
        TEST_METHOD(Test3_NoTransformForAnd)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // ��������� ��������� (������ �������� ����� ��)
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"���������� �� ������ ���� ����������, �� ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 4: ���������� �������������� ��� ����������
         *
         * ���������, ��� ���������� A | B �������� ��� ���������
         */
        TEST_METHOD(Test4_NoTransformForOr)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Or);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // ��������� ��������� (������ �������� ����� ��)
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"���������� �� ������ ���� ����������, �� ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 5: ���������� ������ ���������
         *
         * ��������� �������������� ���������� ������ ��������� ���������: !(A > B)
         */
        TEST_METHOD(Test5_ImplicationInsideNot)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Implication);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: !(!A | B)
            ExpressionNode* expected = new ExpressionNode(Not);
            expected->left = new ExpressionNode(Or);
            expected->left->left = new ExpressionNode(Not);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Variable, "b");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ���������� ������ ���������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 6: ��������������� ������ ���������
         *
         * ��������� �������������� ��������������� ������ ��������� ���������: !(A ~ B)
         */
        TEST_METHOD(Test6_EquivalenceInsideNot)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Equivalence);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: !((A & B) | (!A & !B))
            ExpressionNode* expected = new ExpressionNode(Not);
            expected->left = new ExpressionNode(Or);

            // ����� ����� OR: A & B
            expected->left->left = new ExpressionNode(And);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right = new ExpressionNode(Variable, "b");

            // ������ ����� OR: !A & !B
            expected->left->right = new ExpressionNode(And);
            expected->left->right->left = new ExpressionNode(Not);
            expected->left->right->left->left = new ExpressionNode(Variable, "a");
            expected->left->right->right = new ExpressionNode(Not);
            expected->left->right->right->left = new ExpressionNode(Variable, "b");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ��������������� ������ ���������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 7: ���������� ������ ����������
         *
         * ��������� �������������� ���������� ������ ����������: A & (B > C)
         */
        TEST_METHOD(Test7_ImplicationInsideAnd)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Implication);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: A & (!B | C)
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Variable, "c");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ���������� ������ ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 8: ��������������� ������ ����������
         *
         * ��������� �������������� ��������������� ������ ����������: A & (B ~ C)
         */
        TEST_METHOD(Test8_EquivalenceInsideAnd)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Equivalence);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: A & ((B & C) | (!B & !C))
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);

            // ����� ����� OR: B & C
            expected->right->left = new ExpressionNode(And);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->left->right = new ExpressionNode(Variable, "c");

            // ������ ����� OR: !B & !C
            expected->right->right = new ExpressionNode(And);
            expected->right->right->left = new ExpressionNode(Not);
            expected->right->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right->right = new ExpressionNode(Not);
            expected->right->right->right->left = new ExpressionNode(Variable, "c");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ��������������� ������ ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 9: ���������� ������ ����������
         *
         * ��������� �������������� ���������� ������ ����������: A | (B > C)
         */
        TEST_METHOD(Test9_ImplicationInsideOr)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Or);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Implication);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: A | (!B | C)
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Variable, "c");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ���������� ������ ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 10: ��������������� ������ ����������
         *
         * ��������� �������������� ��������������� ������ ����������: A | (B ~ C)
         */
        TEST_METHOD(Test10_EquivalenceInsideOr)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Or);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Equivalence);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: A | ((B & C) | (!B & !C))
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);

            // ����� ����� ���������� OR: B & C
            expected->right->left = new ExpressionNode(And);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->left->right = new ExpressionNode(Variable, "c");

            // ������ ����� ���������� OR: !B & !C
            expected->right->right = new ExpressionNode(And);
            expected->right->right->left = new ExpressionNode(Not);
            expected->right->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right->right = new ExpressionNode(Not);
            expected->right->right->right->left = new ExpressionNode(Variable, "c");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ��������������� ������ ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 11: ������� ����������
         *
         * ��������� �������������� ������� ����������: A > (B > C)
         */
        TEST_METHOD(Test11_DoubleImplication)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Implication);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: !A | (!B | C)
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Variable, "c");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ������� ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 12: ������� ���������������
         *
         * ��������� �������������� ������� ���������������: A ~ (B ~ C)
         */
        TEST_METHOD(Test12_DoubleEquivalence)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Equivalence);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: (A & ((B & C) | (!B & !C))) | (!A & !((B & C) | (!B & !C)))
            ExpressionNode* expected = new ExpressionNode(Or);

            // ����� ����� OR: A & ((B & C) | (!B & !C))
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Or);

            // ����� ����� ���������� OR: B & C
            expected->left->right->left = new ExpressionNode(And);
            expected->left->right->left->left = new ExpressionNode(Variable, "b");
            expected->left->right->left->right = new ExpressionNode(Variable, "c");

            // ������ ����� ���������� OR: !B & !C
            expected->left->right->right = new ExpressionNode(And);
            expected->left->right->right->left = new ExpressionNode(Not);
            expected->left->right->right->left->left = new ExpressionNode(Variable, "b");
            expected->left->right->right->right = new ExpressionNode(Not);
            expected->left->right->right->right->left = new ExpressionNode(Variable, "c");

            // ������ ����� ��������� OR: !A & !((B & C) | (!B & !C))
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Variable, "a");
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Or);

            // ����� ����� ����� ���������� OR
            expected->right->right->left->left = new ExpressionNode(And);
            expected->right->right->left->left->left = new ExpressionNode(Variable, "b");
            expected->right->right->left->left->right = new ExpressionNode(Variable, "c");
            expected->right->right->left->right = new ExpressionNode(And);
            expected->right->right->left->right->left = new ExpressionNode(Not);
            expected->right->right->left->right->left->left = new ExpressionNode(Variable, "b");
            expected->right->right->left->right->right = new ExpressionNode(Not);
            expected->right->right->left->right->right->left = new ExpressionNode(Variable, "c");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ������� ���������������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 13: ��������������� ������ ����������
         *
         * ��������� �������������� (A ~ B) > C
         */
        TEST_METHOD(Test13_EquivalenceInsideImplication)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Equivalence);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");
            input->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: !((A & B) | (!A & !B)) | C
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);

            // ����� ����� NOT: (A & B) | (!A & !B)
            expected->left->left = new ExpressionNode(Or);

            // ����� ����� OR: A & B
            expected->left->left->left = new ExpressionNode(And);
            expected->left->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->left->right = new ExpressionNode(Variable, "b");

            // ������ ����� OR: !A & !B
            expected->left->left->right = new ExpressionNode(And);
            expected->left->left->right->left = new ExpressionNode(Not);
            expected->left->left->right->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right->right = new ExpressionNode(Not);
            expected->left->left->right->right->left = new ExpressionNode(Variable, "b");

            // ������ ����� ��������� OR: C
            expected->right = new ExpressionNode(Variable, "c");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ��������������� ������ ����������");

            // ������� ������
            delete input;
            delete expected;
        }

        /**
         * @brief ���� 14: ���������� ������ ���������������
         *
         * ��������� �������������� (A > B) ~ C
         */
        TEST_METHOD(Test14_ImplicationInsideEquivalence)
        {
            // ���������� ������� ������
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Implication);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");
            input->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: ((!A | B) & C) | (!(!A | B) & !C)
            ExpressionNode* expected = new ExpressionNode(Or);

            // ����� ����� OR: (!A | B) & C
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Or);
            expected->left->left->left = new ExpressionNode(Not);
            expected->left->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->left->right = new ExpressionNode(Variable, "b");
            expected->left->right = new ExpressionNode(Variable, "c");

            // ������ ����� OR: !(!A | B) & !C
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Not);
            expected->right->left->left = new ExpressionNode(Or);
            expected->right->left->left->left = new ExpressionNode(Not);
            expected->right->left->left->left->left = new ExpressionNode(Variable, "a");
            expected->right->left->left->right = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Not);
            expected->right->right->left = new ExpressionNode(Variable, "c");

            // ���������� ��������������
            transformImplicationAndEquivalence(input);

            // �������� ����������
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ �������������� ���������� ������ ���������������");

            // ������� ������
            delete input;
            delete expected;
        }
    };
}