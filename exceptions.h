#ifndef _EXP_DEPS_
#define _EXP_DEPS_

#include <setjmp.h>

const int RES_OK          = 0x00;

const int RES_GENERAL_ERR = 0x01;
const int RES_NULL_ERR    = 0x02;
const int RES_IO_ERR      = 0x03;
const int RES_OTHER_ERR   = 0x04;

const int RES_FINAL       = 0x05;

void throw(jmp_buf buf, int err) {
  longjmp(buf, err);
}

void try(jmp_buf buf, int(*fun)()) {
  int result = fun();
  if(result == RES_OK) return;
  throw(buf, result);
}

void catch(jmp_buf buf, int err, void(*ct)(int)) {
  ct(err);
  throw(buf, RES_FINAL);
}

void finally(jmp_buf buf, void(*fin)()) {
  fin();
}

/* Handler requires a routine to be run */
void handler(int(*fun)(), void(*ct)(), void(*fin)()) {
  jmp_buf buf;
  int result = setjmp(buf);

  if(result == RES_OK) {
    try(buf, fun);
  }

  if(result == RES_GENERAL_ERR ||
     result == RES_IO_ERR      ||
     result == RES_NULL_ERR    ||
     result == RES_OTHER_ERR)
  {
    catch(buf, result,  ct);
  }

  if(result == RES_FINAL) {
    finally(buf, fin);
  }
}

#endif
