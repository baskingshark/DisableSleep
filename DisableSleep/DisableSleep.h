#include <IOKit/IOService.h>
#include "Dictionary.h"

#define DisableSleep github_sheeparegreat_DisableSleep

class DisableSleep : public IOService
{
    OSDeclareDefaultStructors(github_sheeparegreat_DisableSleep)
public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
private:
    IOPMrootDomain *pRootDomain;
    IONotifier *pGeneralInterestNotifier;

    bool clamshellSleep(bool enable);
    void sleepDisabledDictionarySetting(bool enable);

    static IOReturn interestHandler(void *target,
                                    void *refCon,
                                    UInt32 messageType,
                                    IOService *provider,
                                    void *messageArgument,
                                    vm_size_t argSize);

    /*!
     * @function hookRootDomainProperties
     *
     * @abstract
     * Replace the property table on the IOPMrootDomain
     *
     * @discussion
     * This function replaces the property table on the IOPMrootDomain object
     * with a custom Dictionary that looks for chnages to the SleepDisabled
     * property.  Any changes are signalled by a call to the
     * sleepDisabledChanged method.
     *
     * @param target  A pointer to self
     * @param arg0    Unused
     * @param arg1    Unused
     * @param arg2    Unused
     * @param arg3    Unused
     */
    static IOReturn hookRootDomainProperties(OSObject *target,
                                             void     *arg0,
                                             void     *arg1,
                                             void     *arg2,
                                             void     *arg3);
    /*!
     * @function unhookRootDomainProperties
     *
     * @abstract
     * Reset the property table on the IOPMrootDomain
     *
     * @discussion
     * This function restores the property table on the IOPMrootDomain object
     * back to the original version.
     *
     * @param target  A pointer to self
     * @param arg0    Unused
     * @param arg1    Unused
     * @param arg2    Unused
     * @param arg3    Unused
     */
    static IOReturn unhookRootDomainProperties(OSObject *target,
                                               void     *arg0,
                                               void     *arg1,
                                               void     *arg2,
                                               void     *arg3);

    /*!
     * @function sleepDisabledChanged
     *
     * @abstract
     * Function called when the SleepDisabled property is changed
     *
     * @discussion
     * When the SleepDisabled property is changed, we check whether it was
     * changed to True.  If it was, we do nothing becuase this is what we
     * want.  If it was set to False, or deleted, we reset it to Ture.
     *
     * @param target       A pointer to self
     * @param aDictionary  The Dictionary object that was updated
     * @param aKey         The Key that was updated
     * @param aValue       The new Value, or NULL if the Key was deleted.
     */
    static void sleepDisabledChanged(OSObject              *target,
                                     const Dictionary      *aDictionary,
                                     const OSSymbol        *aKey,
                                     const OSMetaClassBase *aValue);
};
