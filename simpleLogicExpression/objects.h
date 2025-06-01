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

    /**
    * ����������� ������ ExpressionNode
    * @param t ��� ����
    * @param v �������� ����������
    * @param l ����� �������
    * @param r ������� �������
    */
    ExpressionNode(TokenType t, const std::string& v, ExpressionNode* l, ExpressionNode* r) : type(t), value(v), left(l), right(r) {}

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

    static std::string getErrorTypeString(ErrorType type) {
        switch (type) {
        case noError: return "noError";
        case inputFile: return "inputFile";
        case outputFile: return "outputFile";
        case invalidSeparator: return "invalidSeparator";
        case missingOperation: return "missingOperation";
        case insufficientOperands: return "insufficientOperands";
        case invalidVariableName: return "invalidVariableName";
        case invalidVariableChar: return "insufficientOperands";
        case unsupportedOperation: return "invalidVariableName";
        default: return "����������� ������";
        }
    }

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

    /**
    * // �������� ��������� ��� ������������� � std::set
    */
    bool operator<(const Error& other) const {
        if (type != other.type)
            return type < other.type;
        return position < other.position;
    }
};