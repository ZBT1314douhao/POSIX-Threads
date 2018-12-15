#include <iostream>
#include "gtest/gtest.h"
#include "../utils/colorfulprint.h"


using namespace std;

int main(int argc, char *argv[])
{
    Infra::Utils::setPrintLevel(Infra::Utils::PRINT_LEVEL_ALL);
    printf("[test_main.cpp] current print level = %d\n", Infra::Utils::getPrintLevel());
    fatalf("--------------------------  ");
    errorf("--------------------------\n");
    warnf("--------------------------  ");
    infof("--------------------------\n");
    debugf("--------------------------  ");
    tracef("--------------------------\n");
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

