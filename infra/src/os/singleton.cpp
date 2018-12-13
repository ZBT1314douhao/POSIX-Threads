

#include "singleton.h"

namespace Infra {

CSimpleSingleton::CSimpleSingleton()
{

}

CSimpleSingleton::~CSimpleSingleton()
{

}

// Meyers Singleton Pattern 在C++11下时线程安全的
CSimpleSingleton& CSimpleSingleton::instance()
{
    static CSimpleSingleton instance;
    return instance;
}






}




