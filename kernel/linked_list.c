#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "linked_list.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

struct link {
  int value;
  struct link *next;
};

// For simplicity, we only allocate one page.
char *base;
char *p;

struct link *l;
int live_nodes;

struct link *ll_create() {
  // In this case, we'll say that we have to allocate a page.
  if (base == 0) {
    if ((base = kalloc()) == 0) {
      // Allocation failed.
      return 0;
    }

    p = base;
  }

  if (p + sizeof(struct link) > base + PGSIZE) {
    return 0;
  }

  struct link *ret = (struct link *)p;
  p += sizeof(struct link);
  live_nodes++;
  return ret;
}

int ll_push(int value) {
  struct link *new = ll_create();
  if (new == 0) {
    return -1;
  }

  struct link *end;
  for (end = l; end->next != 0; end++)
    ;

  end->next = new;
  new->next = 0;
  new->value = value;
  return 0;
}

void ll_destroy(struct link *l) {
  live_nodes--;
  if (live_nodes == 0) {
    kfree(base);
    base = 0;
  }
}

int ll_pop(int *ret) {
  if (l == 0) {
    return DUMMY_VALUE;
  }

  struct link *old = l;
  old->next = 0;
  l = l->next;
  // TODO: the bug is here! we set the next pointer of the *current*
  // l to 0 after changing it.
  l->next = 0;
  ll_destroy(l);
  return old->value;
}

