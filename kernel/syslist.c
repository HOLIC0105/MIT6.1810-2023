#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"

uint64 sys_ll_create(void)
{
  ll_create();
  return 0;
}

uint64 sys_ll_push(void)
{
  int value;
  argint(0, &value);
  return ll_push(value);
}

uint64 sys_ll_pop(void)
{
  return ll_pop();
}

uint64 sys_ll_destroy(void)
{
  ll_destroy();
  return 0;
}
