#include "../../numerals.h"
#include "math.h"
#include "prog.h"
#include "val/arr.h"
#include "val/num.h"
#include "val/str.h"
#include "val/val.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Add individual opcodes or native functions here

OPCODE(swap) {
  val_t *v1 = POP;
  val_t *v2 = POP;
  PUSH(v1);
  PUSH(v2);
}

OPCODE(rot) {
  val_t *v1 = POP;
  val_t *v2 = POP;
  val_t *v3 = POP;

  PUSH(v1);
  PUSH(v3);
  PUSH(v2);
}

OPCODE(power) {
  val_t *v1 = POP;
  val_t *v2 = POP;
  val_t *ret;
  if (v1->type == T_NUM && v2->type == T_NUM) {
    ret = v_num_new_int(pow(v1->u.num, v2->u.num));
  } else {
    ret = &val_undef;
  }
  PUSH(ret);
}

OPCODE(romanint) {
  char buf[100];
  printf("%s", "< ");
  fflush(stdout);

  if (fgets(buf, sizeof(buf), stdin)) {
    buf[strcspn(buf, "\r\n")] = 0;

    int nr;
    if (strcmp(buf, "nil") == 0 || buf[0] == '\0') {
      nr = 0;
    } else {
      nr = roman_to_integer(buf);
    }

    val_t *v = v_num_new_int(nr);
    PUSH(v);
  } else {
    PUSH(v_num_new_int(0));
  }
}

OPCODE(printroman) {
  MINARGS(1);
  val_t *a = POP;
  val_t *s;
  if (a->type == T_NUM && a->u.num > 0) {
    char *repr = integer_to_roman_alloc(a->u.num);
    s = v_str_new_cstr(repr);
    free(repr);
  } else if (a->type == T_NUM && a->u.num == 0) {
    s = v_str_new_cstr("nil");
  } else {
    s = val_to_string(a);
  }
  printf("> %s\n", cstr(s));
  fflush(stdout);
}

NATIVE(substring) {
  val_t *string = ARG(0);
  val_t *start = ARG(1);
  val_t *end = ARG(2);

  if (string->type == T_STR && start->type == T_NUM && end->type == T_NUM) {
    int s = start->u.num;
    int e = end->u.num;

    int len = e - s;

    str_t *underlying = string->u.str;

    if (len > underlying->len) {
      fprintf(stderr,
              "Attempting to take substring with length (%d) longer than "
              "actual length(%d)\n",
              len, underlying->len);
      exit(1);
    }

    if (e > underlying->len) {
      fprintf(stderr, "End (%d) out of bounds for length(%d)\n", e,
              underlying->len);
      exit(1);
    }

    if (s < 0 || s > underlying->len) {

      fprintf(stderr, "Start (%d) out of bounds for length(%d)\n", e,
              underlying->len);
      exit(1);
    }

    return v_str_new_buf(&underlying->buf[s], len);
  }
  return &val_undef;
}

NATIVE(length) {
  val_t *val = ARG(0);
  if (val->type == T_STR) {
    str_t *string = val->u.str;
    return v_num_new_int(string->len);
  } else if (val->type == T_ARR) {
    arr_t *array = val->u.arr;
    return v_num_new_int(array->size);
  } else {
    return &val_undef;
  }
}

val_t *to_string(val_t *val) {
  if (val->type == T_STR) {
    return val;
  } else if (val->type == T_NUM && val->u.num > 0) {
    char *repr = integer_to_roman_alloc(val->u.num);
    val_t *str = v_str_new_cstr(repr);
    return str;
  } else if (val->type == T_NUM && val->u.num == 0) {
    return v_str_new_cstr("nil");
  } else {
    return &val_undef;
  }
}

OPCODE(concat) {
  MINARGS(2);

  str_t *first = to_string(POP)->u.str;
  str_t *second = to_string(POP)->u.str;

  size_t len = first->len + second->len;
  char *buf = calloc(len, sizeof(char));
  strcpy(buf, first->buf);
  strcat(buf, second->buf);

  val_t *conc = v_str_new_buf(buf, len);

  free(buf);

  PUSH(conc);
}
