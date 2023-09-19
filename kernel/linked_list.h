struct link {
  int value;
  struct link *prev;
  struct link *next;
  int arena;
};

typedef int ll_handle;
#define DUMMY_VALUE -1
