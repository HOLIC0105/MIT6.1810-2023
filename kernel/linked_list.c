#include "types.h"
#include "spinlock.h"
#include "defs.h"
#include "linked_list.h"

/* An arena is a very simple type of memory allocator.
 * Arenas allocate memory by advancing a pointer through
 * an already-allocated segment of memory, and freeing the
 * memory when they're done. Memory management with an arena
 * is very similar to stack memory management, except one can
 * choose when that memory is freed, whereas memory on the stack
 * is freed when a function returns. */
struct arena {
  char *base;
  char *p;
}

struct arena
arena_init() {
  char *base = kalloc();
  struct arena a;
  a.base = base;
  a.p = p;
  return a;
}

char *arena_alloc(struct arena *a, int n) {
  a->p += n;
  return a->p;
}

void arena_free(struct arena *a) { kfree(a->base); }

/* Now, we're going to define an array of linked lists that can be
 * shared between processes. Initializing an already-initialized list
 * gives back the value that's already there, so in this way, processes
 * can share the linked lists amongst one another. For this reason, we
 * must lock the array of linked lists. */

#define MAX_LISTS 16

// We maintain a global table of linked lists which we can share across
// processes.
struct link table[MAX_LISTS];
// We maintain reference counts so we can safely only destroy a linked list
// once.
int refcnt[MAX_LISTS];
// We want an arena per linked list so we can release them all separately.
struct arena arenas[MAX_LISTS];
// We need to have a lock protecting changes to the table.
struct spinlock lock;

// Initialize the linked list pool exactly once.
void ll_init() {
  for (int i = 0; i < MAX_LISTS; i++) {
    table[i] = make_dummy(i);
    arenas[i] = arena_init();
  }
  initlock(&lock);
}

static struct link make_dummy(int arena) {
  struct link dummy;
  dummy.arena = arena;
  dummy.value = -1;
  dummy.prev = 0;
  dummy.next = 0;
  return dummy;
}

// Create a new linked list, or return the head of the existing one if one
// already exists with this id.
struct link *ll_create(int id) {
  acquire(&lock);
  // When we destroy a node, we'll set it to the dummy node so this is safe.
  struct link *x = table + id;
  refcnt[id]++;
  release(&lock);
  return x;
}

// Add an element to the end of the linked list.
void ll_push(struct link *link, int value) {
  acquire(&lock);
  struct link *curr;
  // This isn't a circular linked list, so we have to walk it to find the end.
  for (curr = link; curr->next; curr = curr->next)
    ;
  struct link *next =
      (struct link *)arena_alloc(&arenas[link->arena], sizeof(struct link));
  curr->next = next;
  next->value = value;
  next->prev = link;
  next->next = 0;
  release(&lock);
}

// Remove the first element of the linked list.
int ll_pop(struct link *link, int *ret) {
  acquire(&lock);
  // The desired semantics are popping the earliest non-dummy node in the list.
  // We expect to be handed the head of the list here, which is a dummy.
  if (!link->next) {
    release(&lock);
    return -1;
  }

  int value = link->next->value;
  link->next = link->next->next;
  link->next->prev = link;
  *ret = value;

  release(&lock);
  return 0;
}

void ll_destroy(struct link *link) {
  acquire(&lock);
  refcnt[link->arena]--;
  if (refcnt[link->arena] == 0) {
    struct arena *arena = arenas + link->arena;
    table[link->arena] = make_dummy(link->arena);
    arena_destroy(arena);
  }
  release(&lock);
}
