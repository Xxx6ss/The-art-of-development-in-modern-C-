//
//  main.cpp
//  Factory
//
//  Created by Andrew Kireev on 02.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
  string from;
  string to;
  string body;
    friend std::istream& operator>> (std::istream& in, Email& email) {
        getline(in, email.from);
        getline(in, email.to);
        getline(in, email.body);
        return in;
    }
};


class Worker {
public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if (next_obj_) {
            next_obj_->Process(std::move(email));
        }
    }

public:
    void SetNext(unique_ptr<Worker> next) {
        next_obj_ = std::move(next);
    }
protected:
    unique_ptr<Worker> next_obj_;
};


class Reader : public Worker {
public:
    Reader(istream& input) : input_(input) {}
    
    void Run() override {
        for (;;) {
            Email email;
            input_ >> email;
            if (input_) {
                Process(make_unique<Email>(email));
            } else
                break;
        }
    }

    
    void Process(unique_ptr<Email> email) override{
           PassOn(std::move(email));
    }
    
private:
    istream& input_;
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;
    
    Filter(Function func) : func_(func) {}
    
    void Process(unique_ptr<Email> email) override {
        if (func_(*email.get()))
            PassOn(std::move(email));
    }

private:
    Function func_;
};


class Copier : public Worker {
public:
    Copier(string address) : address_(address) { }
    
    void Process(unique_ptr<Email> email) override {
        if (address_ == email->to)
            PassOn(std::move(email));
        else {
            auto copy = make_unique<Email>(*email);
            copy->to = address_;
            PassOn(std::move(email));
            PassOn(std::move(copy));
        }
    }
    
private:
    string address_;
};


class Sender : public Worker {
public:
    Sender(ostream& out) : out_(out) {}
    
    void Process(unique_ptr<Email> email) override {
        out_ << email->from << endl <<
        email->to << endl << email->body << endl;
        PassOn(move(email));
    }
    
private:
    ostream& out_;
};



// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) {
        ptr_.push_back(make_unique<Reader>(in));
    }

  // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        ptr_.push_back(make_unique<Filter>(filter));
        return *this;
    }

  // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        ptr_.push_back(make_unique<Copier>(recipient));
        return *this;
    }


  // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        ptr_.push_back(make_unique<Sender>(out));
        return *this;
    }

 // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        for (size_t i = ptr_.size() - 1; i > 0; --i) {
            ptr_[i - 1]->SetNext(move(ptr_[i]));
        }
        return move(ptr_[0]);
    }
private:
    vector<unique_ptr<Worker>> ptr_;
};


void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
