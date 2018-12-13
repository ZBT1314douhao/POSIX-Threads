#ifndef __INFRA_OS_SINGLETON_H__
#define __INFRA_OS_SINGLETON_H__

#include "singleton.h"

namespace Infra {

class CSimpleSingleton
{
public:
    static CSimpleSingleton& instance();
    
private:
    CSimpleSingleton();
    ~CSimpleSingleton(); 
    CSimpleSingleton(const CSimpleSingleton&);
    CSimpleSingleton operator=(const CSimpleSingleton&);  
};

}





#endif
