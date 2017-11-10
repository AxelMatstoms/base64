#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define OCTETS_FROM_INTS(a, b, c) (a << 16) + (b << 8) + c
#define B64_SIZE(n) ((n / 3) + !!(n % 3)) * 4
#define B64_HEXTET(bits, index) (bits & (0x3F << (18 - index * 6))) >> (18 - index * 6)

char ito_b64chr(int n) {
  if (n <= 25) {
    return 'A' + n;
  } else if (n <= 51) {
    return 'a' + (n - 26);
  } else if (n <= 61) {
    return '0' + (n - 52);
  } else if (n == 62) {
    return '+';
  } else {
    return '/';
  }
}

int b64chr_toi(char b64) {
  if (b64 >= 'A' && b64 <= 'Z') {
    return b64 - 'A';
  } else if (b64 >= 'a' && b64 <= 'z') {
    return 26 + b64 - 'a';
  } else if (b64 >= '0' && b64 <= '9') {
    return 52 + b64 - '0';
  } else if (b64 == '+') {
    return 62;
  } else {
    return 63;
  }
}


/**
 * Function: to_b64
 * ----------------
 * buf: 
 *   a pointer to a char pointer
 * str:
 *   a null terminated char array of bytes to encode
 *
 * returns:
 *   length of base64 output
 *
 * ex:
 * char* ptr;
 * char* str = "password_to_hash";
 * int l;
 * l = to_b64(&ptr, str);
 */
int to_b64(char** buf, char* str) {
  int str_size = strlen(str);
  int b64_size = B64_SIZE(str_size);

  //printf("str_size: %d, b64_size: %d\n", str_size, b64_size);
  *buf = calloc(b64_size + 1, sizeof(char));
  
  int i, k, bits, b64_index;
  int even_size = str_size - (str_size % 3);
  //printf("even_size: %d\n", even_size);
  for (i = 0; i < even_size; i += 3) {
    bits = OCTETS_FROM_INTS(str[i], str[i+1], str[i+2]);
    b64_index = (i / 3) * 4;
    for (k = 0; k < 4; k++) {
      (*buf)[b64_index + k] = ito_b64chr(B64_HEXTET(bits, k));
      //printf("index: %d, k=%d\n", b64_index + k, k);
    }
  }
  int rest_size = str_size % 3;
  //printf("restsize: %d\n", rest_size);

  if (rest_size) {
    bits = OCTETS_FROM_INTS((even_size < str_size ? str[even_size] : 0),
			    (even_size + 1 < str_size ? str[even_size + 1] : 0),
			    (even_size + 2 < str_size ? str[even_size + 2] : 0));

    int hextet;
    b64_index = (even_size / 3) * 4;

    for (k = 0; k < 4; k++) {
      hextet = B64_HEXTET(bits, k);
      //printf("index2: %d\n", even_size + k);
      if (hextet) {
	(*buf)[b64_index + k] = ito_b64chr(hextet);
      } else {
	(*buf)[b64_index + k] = '=';
      }
    }
  }

  return b64_size;
}

int from_b64(char** buf, char* b64) {

}

int main(int argc, char** argv) {
  char str[64];
  fgets(str, 64, stdin);
  
  char* buf;
  to_b64(&buf, str);
  printf("%s\n", buf);
  free(buf);
  
  return 0;
}
