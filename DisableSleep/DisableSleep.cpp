#include <IOKit/IOLib.h>
#include "DisableSleep.h"

#include <IOKit/pwr_mgt/IOPM.h>
#include <IOKit/pwr_mgt/RootDomain.h>

#ifdef DEBUG
#define DLOG(fmt, ...) IOLog(fmt, ## __VA_ARGS__)
#else
#define DLOG(fmt, ...)
#endif

// This required macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires.
OSDefineMetaClassAndStructors(github_sheeparegreat_DisableSleep, IOService)

// Define the driver's superclass.
#define super IOService

// Define Power States for the driver
static IOPMPowerState PowerStates[] = {
    {
        /* version */
        kIOPMPowerStateVersion1,
        /* capabilityFlags */
        kIOPMPowerOn | kIOPMPreventIdleSleep | kIOPMPreventSystemSleep,
        /* outputPowerCharacter */
        kIOPMPowerOn,
        /* inputPowerRequirement */
        kIOPMPowerOn,
        /* staticPower */
        0,
        /* unbudgetedPower */
        0,
        /* powerToAttain */
        0,
        /* timeToAttain */
        0,
        /* settleUpTime */
        0,
        /* timeToLower */
        0,
        /* settleDownTime */
        0,
        /* powerDomainBudget */
        0
    }
};

bool github_sheeparegreat_DisableSleep::init(OSDictionary *dict)
{
    // Calling getName() in this fuction causes Kernel Panic
    bool result = super::init(dict);
    return result;
}

void github_sheeparegreat_DisableSleep::free(void)
{
    DLOG("%s[%p]::%s\n", getName(), this, __FUNCTION__);

    super::free();
}

IOService *github_sheeparegreat_DisableSleep::probe(IOService *provider,
                                    SInt32 *score)
{
    DLOG("%s[%p]::%s\n", getName(), this, __FUNCTION__);

    IOService *result = super::probe(provider, score);
    return result;
}

bool github_sheeparegreat_DisableSleep::clamshellSleep(bool enable)
{
    DLOG("%s[%p]::%s(%d)\n", getName(), this, __FUNCTION__, enable ? 1 : 0);

    pRootDomain->receivePowerNotification( enable ? kIOPMEnableClamshell : kIOPMDisableClamshell );
    
    return true;
}

void github_sheeparegreat_DisableSleep::sleepDisabledDictionarySetting(bool enable)
{
    DLOG("%s[%p]::%s(%d)\n", getName(), this, __FUNCTION__, enable ? 1 : 0);

    const OSSymbol *sleepdisabled_string = OSSymbol::withCString("SleepDisabled");

    if(sleepdisabled_string) {
        const OSObject *objects[] = { OSBoolean::withBoolean(enable) };
        const OSSymbol *keys[] = { sleepdisabled_string };
        OSDictionary *dict = OSDictionary::withObjects(objects, keys, 1);
        if(dict) {
            pRootDomain->setProperties(dict);
            dict->release();
        }
        else
            IOLog("%s[%p]::%s error creating OSDictionary\n",
                  getName(), this, __FUNCTION__);
        sleepdisabled_string->release();
    }
    else
        IOLog("%s[%p]::%s error creating OSSymbol",
              getName(), this, __FUNCTION__);
}

bool github_sheeparegreat_DisableSleep::start(IOService *provider)
{
    DLOG("%s[%p]::%s\n", getName(), this, __FUNCTION__);

    bool result = super::start(provider);
    if(!result)
        return false;
    
    pRootDomain = getPMRootDomain();
    
    if(!pRootDomain){
        DLOG("%s[%p]::%s error calling getPMRootDomain()\n", getName(), this, __FUNCTION__);
        return false;
    }

    // Start and configure power management
    PMinit();
    provider->joinPMtree(this);
    registerPowerDriver(this, PowerStates,
                        sizeof(PowerStates)/sizeof(IOPMPowerState));

    sleepDisabledDictionarySetting(true);
    clamshellSleep(false);

    // Start the registration process ...
    // This makes the driver appear as registerd
    registerService();

    DLOG("%s[%p]::%s DisableSleep started\n", getName(), this, __FUNCTION__);

    return result;
}

void github_sheeparegreat_DisableSleep::stop(IOService *provider)
{
    DLOG("%s[%p]::%s\n", getName(), this, __FUNCTION__);

    sleepDisabledDictionarySetting(false);
    clamshellSleep(true);

    // Stop power management
    PMstop();

    super::stop(provider);
}