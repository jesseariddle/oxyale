#include <stdio.h>
#include <stdlib.h>
#include "../../src/regcode.h"

int main(void)
{
    opalRegcode_t *regcode = OPAL_MakeRegcode();
    opalRegcode_t *anotherRegcode = OPAL_MakeRegcodeFromString("9YAT-C8MM-GJVZL");
    OPAL_PrintRegcode(regcode);
    OPAL_PrintRegcode(anotherRegcode);
    OPAL_FreeRegcode(regcode);
    OPAL_FreeRegcode(anotherRegcode);
    return 0;
}
