#include <iostream>
#include "gtest/gtest.h"
#include "utils/colorfulprint.h"


using namespace std;

int main(int argc, char *argv[])
{
    Infra::Utils::setPrintLevel(Infra::Utils::PRINT_LEVEL_ALL);
    printf("[test_main.cpp] current print level = %d\n", Infra::Utils::getPrintLevel());
    fatalf("fatal\n");
    errorf("error\n");
    warnf("warn\n");
    infof("info\n");
    debugf("debug\n");
    tracef("trace\n");
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

