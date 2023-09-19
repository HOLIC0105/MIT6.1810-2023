// We want to write some code that exposes a mutable linked list
// We need a way to:
// - add to the linked list
// - remove the head of the linked list
// - create a new linked list
// - deallocate a linked list
// We're going to set one arbitrary rule: we can never mutate
// the linked list in userspace. Maybe this is because we want other
// processes to be able to reference the same linked list easily?
#include "kernel/types.h"
#include "kernel/linked_list.h"
#include "user/user.h"

int extend_and_sum(struct link link, int *nums, int len_nums)
{
  for (int *p = nums; p < nums + len_nums; p++) {
    ll_push(link, *p);
  }

  int sum = 0;
  for (struct link *curr = &link; curr->next; curr = curr->next) {
    sum += curr->value;
  }

  return sum;
}

int main()
{
  struct link link = ll_create(1);
  ll_push(link, 1);
  int x;
  if (ll_pop(link, &x) < 0) {
    exit(1);
  }
  ll_destroy(link);
}
