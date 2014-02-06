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

bool DisableSleep::init(OSDictionary *dict)
{
    // Calling getName() in this fuction causes Kernel Panic
    bool result = super::init(dict);
    return result;
}

void DisableSleep::free(void)
{
    DLOG("%s[%p]::%s\n", getName(), this, __FUNCTION__);

    super::free();
}

IOService *DisableSleep::probe(IOService *provider, SInt32 *score)
{
    DLOG("%s[%p]::%s\n", getName(), this, __FUNCTION__);

    IOService *result = super::probe(provider, score);
    return result;
}

bool DisableSleep::clamshellSleep(bool enable)
{
    DLOG("%s[%p]::%s(%d)\n", getName(), this, __FUNCTION__, enable ? 1 : 0);

    pRootDomain->receivePowerNotification( enable ? kIOPMEnableClamshell : kIOPMDisableClamshell );
    
    return true;
}

void DisableSleep::sleepDisabledDictionarySetting(bool enable)
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

bool DisableSleep::start(IOService *provider)
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

    // Register for general interest notifications
    pGeneralInterestNotifier = pRootDomain->registerInterest(gIOGeneralInterest,
                                                             interestHandler,
                                                             this);

    sleepDisabledDictionarySetting(true);

    // Start the registration process ...
    // This makes the driver appear as registerd
    registerService();

    DLOG("%s[%p]::%s DisableSleep started\n", getName(), this, __FUNCTION__);

    return result;
}

void DisableSleep::stop(IOService *provider)
{
    DLOG("%s[%p]::%s\n", getName(), this, __FUNCTION__);

    // Unregister general interest notifier
    // Need to do this before re-enabling clamshell sleep
    if(pGeneralInterestNotifier)
        pGeneralInterestNotifier->remove();

    sleepDisabledDictionarySetting(false);
    // Set clamshell sleep state based on whether we have a clamshell
    if(pRootDomain->getProperty(kAppleClamshellCausesSleepKey) ||
       pRootDomain->getProperty(kAppleClamshellStateKey))
        clamshellSleep(true);

    // Stop power management
    PMstop();

    super::stop(provider);
}

IOReturn
DisableSleep::interestHandler(void *target,
                              void *refCon,
                              UInt32 messageType,
                              IOService *provider,
                              void *messageArgument,
                              vm_size_t argSize)
{
    if(kIOPMMessageClamshellStateChange == messageType) {
        /* Clamshell state change ...
         * Generated when either AppleClamshellState or
         * AppleClamshellCausesSleep changes.
         *
         * State of both variables is encoded in messageArgument - check
         * kClamshellStateBit & kClamshellSleepBit
         */
        DisableSleep *self = (DisableSleep*) target;
        bool sleep = (bool)((uintptr_t)messageArgument & kClamshellSleepBit);
        bool closed = (bool)((uintptr_t)messageArgument & kClamshellStateBit);

        // If AppleClamshellCausesSleep is set, then disable it
        if(sleep)
            self->clamshellSleep(false);

        DLOG("%s[%p]::%s closed = %d, sleep = %d\n",
             self->getName(), self, __FUNCTION__, closed, sleep);
    }
    return kIOReturnSuccess;
}
