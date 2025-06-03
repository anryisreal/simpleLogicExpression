#include <string>
#include <iostream>
#pragma once


/**
 * Перечисление типов токенов логического выражения.
 *
 * Содержит возможные типы токенов: переменные, операции и специальные значения.
 */
enum TokenType {
    Variable,    /// Переменная
    Not,         /// Отрицание (!)
    And,         /// Конъюнкция (&)
    Or,          /// Дизъюнкция (|)
    Implication, /// Импликация (>)
    Equivalence, /// Эквивалентность (~)
    Any          /// Специальный тип для других не используемых токенов
};

/**
 * Класс для представления токена логического выражения.
 *
 * Содержит тип токена, его строковое значение и позицию в исходной строке.
 */
class Token {
public:
    TokenType type;     ///< Тип токена
    std::string value;  ///< Строковое значение токена
    int position;       ///< Позиция токена в исходной строке

    /**
     * Конструктор класса Token.
     * @param t Тип токена
     * @param v Строковое значение
     * @param pos Позиция в строке
     */
    Token(TokenType t, const std::string& v, int pos) : type(t), value(v), position(pos) {}
};

/**
 * Класс для узла дерева выражения.
 *
 * Представляет узел в бинарном дереве логического выражения.
 */
class ExpressionNode {
public:
    TokenType type;             /// Тип операции/переменной
    std::string value;          /// Значение переменной
    ExpressionNode* left;       /// Указатель на левое поддерево
    ExpressionNode* right;      /// Указатель на правое поддерево

    /**
     * Конструктор класса ExpressionNode.
     * @param t Тип узла
     * @param v Значение переменной (по умолчанию пустая строка)
     */
    ExpressionNode(TokenType t, const std::string& v = "") : type(t), value(v), left(nullptr), right(nullptr) {}

    /**
    * Конструктор класса ExpressionNode
    * @param t Тип узла
    * @param l Левый ребенок
    * @param r Правыйй ребенок
    */
    ExpressionNode(TokenType t, ExpressionNode* l, ExpressionNode* r) : type(t), value(""), left(l), right(r) {}

    /**
    * Конструктор класса ExpressionNode
    * @param t Тип узла
    * @param v Значение переменной
    * @param l Левый ребенок
    * @param r Правыйй ребенок
    */
    ExpressionNode(TokenType t, const std::string& v, ExpressionNode* l, ExpressionNode* r) : type(t), value(v), left(l), right(r) {}

    /// Деструктор (рекурсивно удаляет поддеревья)
    ~ExpressionNode() {
        delete left;
        delete right;
    }
};

/**
 * Класс для обработки ошибок программы.
 *
 * Содержит тип ошибки, позицию и описание.
 */
class Error {
public:
    /**
     * Перечисление типов ошибок.
     */
    enum ErrorType {
        inputFile,            /// Ошибка входного файла
        outputFile,           /// Ошибка выходного файла
        invalidSeparator,     /// Некорректные разделители
        missingOperation,     /// Пропущена операция
        insufficientOperands, /// Недостаточно операндов
        invalidVariableName,  /// Некорректное имя переменной
        invalidVariableChar,  /// Некорректный символ в имени переменной
        unsupportedOperation /// Неподдерживаемая операция
    };

    static std::string getErrorTypeString(ErrorType type) {
        switch (type) {
        case inputFile: return "inputFile";
        case outputFile: return "outputFile";
        case invalidSeparator: return "invalidSeparator";
        case missingOperation: return "missingOperation";
        case insufficientOperands: return "insufficientOperands";
        case invalidVariableName: return "invalidVariableName";
        case invalidVariableChar: return "insufficientOperands";
        case unsupportedOperation: return "invalidVariableName";
        default: return "Неизвестная ошибка";
        }
    }

    ErrorType type;          /// Тип ошибки
    int position;            /// Позиция ошибки
    std::string description; /// Описание ошибки

    /**
     * Конструктор класса Error.
     * @param t Тип ошибки
     * @param pos Позиция ошибки (по умолчанию -1)
     */
    Error(ErrorType t, int pos = -1) : type(t), position(pos) {
        switch (type) {
        case inputFile:
            description = "Неверно указан файл с входными данными. Возможно, файл не существует.";
            break;
        case outputFile:
            description = "Неверно указан файл для выходных данных. Возможно, указанного расположения не существует или нет прав на запись.";
            break;
        case invalidSeparator:
            description = "Некорректные разделители (позиция " + std::to_string(position) + ").";
            break;
        case missingOperation:
            description = "Во входной строке пропущен знак операции между операндами (позиция " + std::to_string(position) + ").";
            break;
        case insufficientOperands:
            description = "Во входной строке пропущен операнд для операции (позиция " + std::to_string(position) + ").";
            break;
        case invalidVariableName:
            description = "Во входной строке имя переменной (позиция " + std::to_string(position) + ") некорректно. Имя переменной должно начинаться только с символа латинского алфавита.";
            break;
        case invalidVariableChar:
            description = "Во входной строке имя переменной (позиция " + std::to_string(position) + ") некорректно. Имя переменно должно содержать только символы латинского алфавита и цифры.";
            break;
        case unsupportedOperation:
            description = "Во входной строке указана неподдерживаемая логическая операция (позиция " + std::to_string(position) + ").";
            break;
        default:
            description = "Неизвестная ошибка.";
        }
    }

    /**
     * Выводит сообщение об ошибке в стандартный вывод.
     */
    void message() const {
        std::cout << description.c_str() << std::endl;
    }

    /**
    * Оператор сравнения для использования в std::set
    */
    bool operator<(const Error& other) const {
        if (type != other.type)
            return type < other.type;
        return position < other.position;
    }

    /**
    * Оператор равенства для сравнения ошибок в std::set
    */
    bool operator==(const Error& other) const {
        return type == other.type && position == other.position;
    }
};