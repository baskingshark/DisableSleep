#include <IOKit/IOLib.h>
#include "DisableSleep.h"

// This required macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires.
OSDefineMetaClassAndStructors(github_sheeparegreat_DisableSleep, IOService)

// Define the driver's superclass.
#define super IOService

bool github_sheeparegreat_DisableSleep::init(OSDictionary *dict)
{
    // Calling getName() in this fuction causes Kernel Panic
    bool result = super::init(dict);
    return result;
}

void github_sheeparegreat_DisableSleep::free(void)
{
#ifdef DEBUG
    IOLog("%s[%p]::%s\n", getName(), this, __FUNCTION__);
#endif
    
    super::free();
}

IOService *github_sheeparegreat_DisableSleep::probe(IOService *provider,
                                    SInt32 *score)
{
#ifdef DEBUG
    IOLog("%s[%p]::%s\n", getName(), this, __FUNCTION__);
#endif
    
    IOService *result = super::probe(provider, score);
    return result;
}

bool github_sheeparegreat_DisableSleep::start(IOService *provider)
{
#ifdef DEBUG
    IOLog("%s[%p]::%s\n", getName(), this, __FUNCTION__);
#endif
    
    bool result = super::start(provider);
    return result;
}

void github_sheeparegreat_DisableSleep::stop(IOService *provider)
{
#ifdef DEBUG
    IOLog("%s[%p]::%s\n", getName(), this, __FUNCTION__);
#endif
    
    super::stop(provider);
}