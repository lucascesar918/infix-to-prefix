#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define STACKSIZE 256

typedef struct stack {
  char arr[STACKSIZE];
  size_t size;
} stack;

int get_precedencia(char op);
int is_operador(char ch);
int is_vazio(stack *s);
void push(stack *s, char ch);
char pop(stack *s);
char peek(stack *s);
void inverter_string(char *str);
void infixa_to_prefixa(char *infixa, char *prefixa);

int main(int argc, char *argv[]) {
  char expressao_infixa[] = "K+L-M*N+(O^P)*W/U/V*T+Q";
  char expressao_prefixa[STACKSIZE];

  printf("Infixa: %s\n", expressao_infixa);
  infixa_to_prefixa(expressao_infixa, expressao_prefixa);
  printf("Prefixa: %s\n", expressao_prefixa);

  return 0;
}

int get_precedencia(char op) {
  switch (op) {
  case '^':
    return 3;
  case '*':
  case '/':
    return 2;
  case '+':
  case '-':
    return 1;
  default:
    return -1;
  }
}

int is_operador(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
         (ch >= '0' && ch <= '9');
}

int is_vazio(stack *s) { return s->size == 0; }

void push(stack *s, char ch) {
  if (s->size < STACKSIZE)
    s->arr[s->size++] = ch;
}

char pop(stack *s) {
  if (is_vazio(s))
    return '\0';

  return s->arr[--(s->size)];
}

char peek(stack *s) {
  if (is_vazio(s))
    return '\0';

  return s->arr[s->size - 1];
}

void inverter_string(char *str) {
  size_t len = strlen(str);
  for (size_t i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = temp;
  }
}

void infixa_to_prefixa(char *infixa, char *prefixa) {
  stack s;
  s.size = 0;

  size_t len = strlen(infixa);
  size_t j = 0;

  inverter_string(infixa);

  for (size_t i = 0; i < len; i++) {
    char c = infixa[i];

    if (c == '(')
      c = ')';
    else if (c == ')')
      c = '(';

    if (isalnum(c))
      prefixa[j++] = c;
    else if (c == '(')
      push(&s, c);
    else if (c == ')') {
      while (!is_vazio(&s) && peek(&s) != '(')
        prefixa[j++] = pop(&s);

      if (!is_vazio(&s))
        pop(&s);
    } else {
      while (!is_vazio(&s) && get_precedencia(peek(&s)) > get_precedencia(c))
        prefixa[j++] = pop(&s);

      while (!is_vazio(&s) && get_precedencia(peek(&s)) == get_precedencia(c) &&
             c == '^')
        prefixa[j++] = pop(&s);

      push(&s, c);
    }
  }

  while (!is_vazio(&s))
    prefixa[j++] = pop(&s);

  prefixa[j] = '\0';

  inverter_string(prefixa);
  inverter_string(infixa);
}
