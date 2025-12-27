#include "lib.h"

int main(){
    LOGIN();
    account** accs = LOAD();

    MENU(accs);

    return 0;
}
