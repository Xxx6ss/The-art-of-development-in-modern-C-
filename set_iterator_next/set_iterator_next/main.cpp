//
//  main.cpp
//  set_iterator_next
//
//  Created by Andrew Kireev on 06.07.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"
#include <cassert>
#include <deque>
#include <iostream>

using namespace std;


struct Node {
  Node(int v, Node* p)
    : value(v)
    , parent(p)
  {}

  int value;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent;
};


class NodeBuilder {
public:
  Node* CreateRoot(int value) {
    nodes.emplace_back(value, nullptr);
    return &nodes.back();
  }

  Node* CreateLeftSon(Node* me, int value) {
    assert( me->left == nullptr );
    nodes.emplace_back(value, me);
    me->left = &nodes.back();
    return me->left;
  }

  Node* CreateRightSon(Node* me, int value) {
    assert( me->right == nullptr );
    nodes.emplace_back(value, me);
    me->right = &nodes.back();
    return me->right;
  }

private:
  deque<Node> nodes;
};


Node* Next(Node* me) {
    Node* cur_node;
    if (me->left == nullptr && me->right == nullptr && me->parent == nullptr)
        return nullptr;
    if (me->parent == nullptr) {
        me = me->right;
        while(me->left != nullptr)
            me = me->left;
        return me;
    } else if (me->parent->left == me) {
        if (me->right == nullptr)
            return me->parent;
        else {
            int value = me->value;
            me = me->right;
            while (me->left != nullptr) {
                me = me->left;
            }
            return me;
        }
    } else if (me->parent->right == me && me->right == nullptr) {
        int value = me->value;
        while (true) {
            if (me->parent == nullptr)
                return nullptr;
            me = me->parent;
            if (me->value < value)
                continue;
            else
                return me;
        }
    } else if (me->right != nullptr)
        return me->right;
    return me;
}


void Test1() {
  NodeBuilder nb;

  Node* root = nb.CreateRoot(50);
  ASSERT_EQUAL( root->value, 50 );

  Node* l = nb.CreateLeftSon(root, 2);
  Node* min = nb.CreateLeftSon(l, 1);
  Node* r = nb.CreateRightSon(l, 4);
  ASSERT_EQUAL( min->value, 1);
  ASSERT_EQUAL( r->parent->value, 2 );

  nb.CreateLeftSon(r, 3);
  nb.CreateRightSon(r, 5);

  r = nb.CreateRightSon(root, 100);
  l = nb.CreateLeftSon(r, 90);
  nb.CreateRightSon(r, 101);

  nb.CreateLeftSon(l, 89);
  r = nb.CreateRightSon(l, 91);

//  ASSERT_EQUAL( Next(l)->value, 91);
//  ASSERT_EQUAL( Next(root)->value, 89 );
//  ASSERT_EQUAL( Next(min)->value, 2 );
//  ASSERT_EQUAL( Next(r)->value, 100);

  while (min) {
    cout << min->value << '\n';
    min = Next(min);
  }
}

void TestRootOnly() {
  NodeBuilder nb;
  Node* root = nb.CreateRoot(42);
  ASSERT( Next(root) == nullptr);
};


int main() {
  TestRunner tr;
  RUN_TEST(tr, Test1);
  RUN_TEST(tr, TestRootOnly);
  return 0;
}