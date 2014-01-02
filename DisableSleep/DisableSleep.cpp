#include <IOKit/IOLib.h>
#include "DisableSleep.h"

#include <IOKit/pwr_mgt/IOPM.h>
#include <IOKit/pwr_mgt/RootDomain.h>

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

bool github_sheeparegreat_DisableSleep::clamshellSleep(bool enable)
{
#ifdef DEBUG
    IOLog("%s[%p]::%s(%d)\n", getName(), this, __FUNCTION__, enable ? 1 : 0);
#endif
    
    pRootDomain->receivePowerNotification( enable ? kIOPMEnableClamshell : kIOPMDisableClamshell );
    
    return true;
}

void github_sheeparegreat_DisableSleep::sleepDisabledDictionarySetting(bool enable)
{
#ifdef DEBUG
    IOLog("%s[%p]::%s(%d)\n", getName(), this, __FUNCTION__, enable ? 1 : 0);
#endif
    
    const OSSymbol *sleepdisabled_string = OSSymbol::withCString("SleepDisabled");
    
    const OSObject *objects[] = { OSBoolean::withBoolean(enable) };
    const OSSymbol *keys[] = { sleepdisabled_string };
    OSDictionary *dict = OSDictionary::withObjects(objects, keys, 1);
    pRootDomain->setProperties(dict);
    dict->release();
}

bool github_sheeparegreat_DisableSleep::start(IOService *provider)
{
#ifdef DEBUG
    IOLog("%s[%p]::%s\n", getName(), this, __FUNCTION__);
#endif

    if( !super::start( provider ))
        return( false );
    
    pRootDomain = getPMRootDomain();
    
    if(!pRootDomain){
#ifdef DEBUG
        IOLog("%s[%p]::%s error calling getPMRootDomain()\n", getName(), this, __FUNCTION__);
#endif
        return false;
    }
    
    sleepDisabledDictionarySetting(true);
    clamshellSleep(false);
    
    bool result = super::start(provider);

#ifdef DEBUG
    if(result == true)
        IOLog("%s[%p]::%s DisableSleep started sucessfully\n", getName(), this, __FUNCTION__);
#endif

    return result;
}

void github_sheeparegreat_DisableSleep::stop(IOService *provider)
{
#ifdef DEBUG
    IOLog("%s[%p]::%s\n", getName(), this, __FUNCTION__);
#endif

    sleepDisabledDictionarySetting(false);
    clamshellSleep(true);
    super::stop(provider);
}