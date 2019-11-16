#define ISEQUAL(value, to_check) ((value == to_check) ? "\33[1;32mSuccess\33[0;0m" : "\33[1;31mFailure\33[0;0m")
#define ISNEQUAL(value, to_check) ((value != to_check) ? "\33[1;32mSuccess\33[0;0m" : "\33[1;31mFailure\33[0;0m")

#define ISTRUE(value, to_check) ((value == to_check) ? "\33[1;32mTrue\33[0;0m" : "\33[1;31mFalse\33[0;0m")
#define ISNTRUE(value, to_check) ((value != to_check) ? "\33[1;32mTrue\33[0;0m" : "\33[1;31mFalse\33[0;0m")


int fail_counter = 0;


#define ISEQUAL1(value, to_check) ((value == to_check) ?  printf(" \33[1;32mSUCCESS\33[0;0m\n") : printf(" \33[1;31mFAIL\33[0;0m\n"))
#define ISNEQUAL1(value, to_check) ((value != to_check) ?  ++fail_counter : printf(" \33[0;0mIn file: %s, line %d, \33[1;31mFAIL\33[0;0m\n", __FILE__, __LINE__)) 

#define ISTRUE1(value, to_check) ((value == to_check) ?  ++fail_counter : printf("\33[0;0mTest in file: %s, file: %s, in line: %d, \33[1;31mFAIL\33[0;0m\n", __FILE__, __LINE__)) 
#define ISNTRUE1(value, to_check) ((value != to_check) ?  ++fail_counter : printf("\33[0;0mTest in file: %s, line: %d, \33[1;31mFAIL\33[0;0m\n", __FILE__, __LINE__)) 