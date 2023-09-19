#include "kernel/linked_list.h"
#include "kernel/types.h"
#include "user/user.h"

void push(int n, char const *msg_prefix) {
  if (ll_push(n)) {
    fprintf(2, "%s: push failed\n", msg_prefix);
    exit(1);
  }
}

void pop(int expected, char const *msg_prefix) {
  int x = ll_pop();
  if (x != expected) {
    fprintf(2, "%s: unexpected value from pop: expected %d, got %d\n",
            msg_prefix, expected, x);
    exit(1);
  }
}

void small_push() {
  for (int i = 0; i < 10; i++) {
    push(i, "small_push");
  }
}

void empty_pop() {
  for (int i = 0; i < 10; i++) {
    pop(DUMMY_VALUE, "empty_pop");
  }
}

void many_pushes() {
  for (int i = 0; i < 100; i++) {
    push(i, "many_pushes");
  }
}

void push_and_pop() {
  for (int i = 0; i < 100; i++) {
    push(i, "push_and_pop");
  }
  for (int i = 0; i < 100; i++) {
    pop(i, "push_and_pop");
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
