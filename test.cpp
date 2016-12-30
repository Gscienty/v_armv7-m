#include <iostream>
#include <typeinfo>
#include "devices\thumb.h"

int main() {
    machine_t machine;
    machine.arm.r0 = 3;
    std::cout << (int)add_with_carry<ubit8_t, 8>(126, 1, 0).item3;
    return 0;
}