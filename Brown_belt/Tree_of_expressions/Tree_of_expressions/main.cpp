//
//  main.cpp
//  Tree_of_expressions
//
//  Created by Andrew Kireev on 03.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "Common.h"
#include "test_runner.h"

#include <sstream>

using namespace std;


class ValueExression : public Expression {
public:
    
    ValueExression(int value) : value_(std::move(value)) {}
    int Evaluate() const {
        return value_;
    }
    
    std::string ToString() const {
        return std::to_string(value_);
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
    
    std::string ToString() const final {
      return '(' + left_son_->ToString() + ')' + '-'
             + '(' + right_son_->ToString() + ')';
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
    
    std::string ToString() const final {
      return '(' + left_son_->ToString() + ')' + '+'
             + '(' + right_son_->ToString() + ')';
    }

};

class DivExression : public OperationExression {
public:
    int Evaluate() const {
        return left_son_->Evaluate() /
        right_son_->Evaluate();
    }
    
    std::string ToString() const final {
      return '(' + left_son_->ToString() + ')' + '/'
             + '(' + right_son_->ToString() + ')';
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
    
    std::string ToString() const final {
      return '(' + left_son_->ToString() + ')'
             + '*'
             + '(' + right_son_->ToString() + ')';
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

string Print(const Expression* e) {
  if (!e) {
    return "Null expression provided";
  }
  stringstream output;
  output << e->ToString() << " = " << e->Evaluate();
  return output.str();
}

void Test() {
  ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
  ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

  ExpressionPtr e2 = Sum(move(e1), Value(5));
  ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

  ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}
