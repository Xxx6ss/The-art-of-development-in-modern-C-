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
        in >> email.from;
        in >> email.to;
        in >> email.body;
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
        next_obj_->Process(std::move(email));
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
    
    
    void Run() {
        Email email;
        while (input_ >> email) {
            Process(make_unique<Email>(email));
        }
    }
    void Process(unique_ptr<Email> email) {
           PassOn(std::move(email));
    }
private:
    istream& input_;
    vector<Email> emails_;
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;
    
    Filter(Function func) : func_(func) {}
    
    void Process(unique_ptr<Email> email) {
        if (func_(*email.get()))
            PassOn(std::move(email));
    }

private:
    Function func_;
};


class Copier : public Worker {
public:
    Copier(string address) : address_(address) { }
    
    void Process(unique_ptr<Email> email) {
        if (address_ == email->to)
            PassOn(std::move(email));
        else {
            PassOn(std::move(email));
            email->to = address_;
            PassOn(std::move(email));
        }
    }
    
private:
    string address_;
};


class Sender : public Worker {
public:
    Sender(ostringstream& out) : out_(move(out)) {}
    
    void Process(unique_ptr<Email> email) {
        out_ << email->from;
        out_ << email->to;
        out_ << email->body;
    }
    
private:
    ostringstream out_;
};



// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) {
        ptr_ = make_unique<Worker>();
        ptr_->input_ = move(in);
    }

  // добавляет новый обработчик Filter
  PipelineBuilder& FilterBy(Filter::Function filter);

  // добавляет новый обработчик Copier
  PipelineBuilder& CopyTo(string recipient);


  // добавляет новый обработчик Sender
  PipelineBuilder& Send(ostream& out);

 // возвращает готовую цепочку обработчиков
  unique_ptr<Worker> Build();
private:
    unique_ptr<Worker> ptr_;
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
