#include <string>
#include <iostream>
#pragma once


/**
 * ������������ ����� ������� ����������� ���������.
 *
 * �������� ��������� ���� �������: ����������, �������� � ����������� ��������.
 */
enum TokenType {
    Variable,    /// ����������
    Not,         /// ��������� (!)
    And,         /// ���������� (&)
    Or,          /// ���������� (|)
    Implication, /// ���������� (>)
    Equivalence, /// ��������������� (~)
    Any          /// ����������� ��� ��� ������ �� ������������ �������
};

/**
 * ����� ��� ������������� ������ ����������� ���������.
 *
 * �������� ��� ������, ��� ��������� �������� � ������� � �������� ������.
 */
class Token {
public:
    TokenType type;     ///< ��� ������
    std::string value;  ///< ��������� �������� ������
    int position;       ///< ������� ������ � �������� ������

    /**
     * ����������� ������ Token.
     * @param t ��� ������
     * @param v ��������� ��������
     * @param pos ������� � ������
     */
    Token(TokenType t, const std::string& v, int pos) : type(t), value(v), position(pos) {}
};

/**
 * ����� ��� ���� ������ ���������.
 *
 * ������������ ���� � �������� ������ ����������� ���������.
 */
class ExpressionNode {
public:
    TokenType type;             /// ��� ��������/����������
    std::string value;          /// �������� ����������
    ExpressionNode* left;       /// ��������� �� ����� ���������
    ExpressionNode* right;      /// ��������� �� ������ ���������

    /**
     * ����������� ������ ExpressionNode.
     * @param t ��� ����
     * @param v �������� ���������� (�� ��������� ������ ������)
     */
    ExpressionNode(TokenType t, const std::string& v = "") : type(t), value(v), left(nullptr), right(nullptr) {}

    /// ���������� (���������� ������� ����������)
    ~ExpressionNode() {
        delete left;
        delete right;
    }
};

/**
 * ����� ��� ��������� ������ ���������.
 *
 * �������� ��� ������, ������� � ��������.
 */
class Error {
public:
    /**
     * ������������ ����� ������.
     */
    enum ErrorType {
        noError,              /// ��� ������
        inputFile,            /// ������ �������� �����
        outputFile,           /// ������ ��������� �����
        invalidSeparator,     /// ������������ �����������
        missingOperation,     /// ��������� ��������
        insufficientOperands, /// ������������ ���������
        invalidVariableName,  /// ������������ ��� ����������
        invalidVariableChar,  /// ������������ ������ � ����� ����������
        unsupportedOperation /// ���������������� ��������
    };

    ErrorType type;          /// ��� ������
    int position;            /// ������� ������
    std::string description; /// �������� ������

    /**
     * ����������� ������ Error.
     * @param t ��� ������
     * @param pos ������� ������ (�� ��������� -1)
     */
    Error(ErrorType t, int pos = -1) : type(t), position(pos) {
        /* ������������� description � ����������� �� ���� ������ */
    }

    /**
     * ������� ��������� �� ������ � ����������� �����.
     */
    void message() const {
        std::cout << description.c_str() << std::endl;
    }
};