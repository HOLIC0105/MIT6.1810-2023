#include "kernel/linked_list.h"
#include "kernel/types.h"
#include "user/user.h"

void small_push() {
  for (int i = 0; i < 10; i++) {
    ll_push(i);
  }
}

void empty_pop() {
  for (int i = 0; i < 10; i++) {
    if (ll_pop() != DUMMY_VALUE) {
      exit(1);
    }
  }
}

void many_pushes() {
  for (int i = 0; i < 100; i++) {
    ll_push(i);
  }
}

void push_and_pop() {
  for (int i = 0; i < 100; i++) {
    ll_push(i);
  }
  for (int i = 0; i < 100; i++) {
    int x = ll_pop();
    if (x != i) {
      exit(1);
    }
  }
}

void (*tests[])(void) = {
    small_push,
    empty_pop,
    many_pushes,
    push_and_pop,
};

int main() {
  for (int i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
    ll_create();
    tests[i]();
    ll_destroy();
  }

  printf("ALL TESTS PASSED\n");
  exit(0);
}
