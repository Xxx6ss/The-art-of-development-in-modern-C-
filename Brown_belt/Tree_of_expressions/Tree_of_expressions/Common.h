//
//  Common.h
//  Tree_of_expressions
//
//  Created by Andrew Kireev on 03.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef Common_h
#define Common_h

#include <memory>
#include <string>

// Базовый класс арифметического выражения
class Expression {
public:
  virtual ~Expression() = default;

    // Вычисляет значение выражения
  virtual int Evaluate() const = 0;

    // Форматирует выражение как строку
    // Каждый узел берётся в скобки, независимо от приоритета
  virtual std::string ToString() const = 0;
    int Get_value = 0;
};

using ExpressionPtr = std::unique_ptr<Expression>;


class ValueExression : public Expression {
public:
    
    ValueExression(int value) : value_(std::move(value)) {}
    int Evaluate() const {
        return value_;
    }
    
    std::string ToString() const {
        return "(" + std::to_string(value_) + ")";
    }
    
private:
    int value_;
};


class OperationExression : public Expression {
public:
    
protected:
    std::unique_ptr<Expression> left_son_;
    std::unique_ptr<Expression> right_son_;
};


class MinusExression : public OperationExression {
public:
    int Evaluate() const {
        return left_son_->Evaluate() -
        right_son_->Evaluate();
    }
    
    std::string ToString() const {
        return "(" + left_son_->ToString() + "-" +
        right_son_->ToString() + ")";
    }

};

class PlusExression : public OperationExression {
public:
    PlusExression(ExpressionPtr lhs, ExpressionPtr rhs) {
        left_son_ = std::move(lhs);
        right_son_ = std::move(rhs);
    }
    int Evaluate() const {
        return left_son_->Evaluate() +
        right_son_->Evaluate();
    }
    
    std::string ToString() const {
        return "(" + left_son_->ToString() + "+" +
        right_son_->ToString() + ")";
    }

};

class DivExression : public OperationExression {
public:
    int Evaluate() const {
        return left_son_->Evaluate() /
        right_son_->Evaluate();
    }
    
    std::string ToString() const {
        return "(" + left_son_->ToString() + "/" +
        right_son_->ToString() + ")";
    }

};

class MultExression : public OperationExression {
public:
    MultExression(ExpressionPtr lhs, ExpressionPtr rhs) {
        left_son_ = std::move(lhs);
        right_son_ = std::move(rhs);
    }
    
    int Evaluate() const {
        return left_son_->Evaluate() *
        right_son_->Evaluate();
    }
    
    std::string ToString() const {
        return "(" + left_son_->ToString() + "*" +
        right_son_->ToString() + ")";
    }

};

// Функции для формирования выражения
ExpressionPtr Value(int value) {
    return std::make_unique<ValueExression>(value);
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<PlusExression>(std::move(left), std::move(right));
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<MultExression>(std::move(left), std::move(right));
}

#endif /* Common_h */
