#include <stdio.h>
#include <stdlib.h>
#include "../../src/regcode.h"

int main()
{
    OPAL_Regcode *Regcode = OPAL_MakeRegcode();
    OPAL_PrintRegcode(Regcode);
    OPAL_FreeRegcode(Regcode);
    return 0;
}
