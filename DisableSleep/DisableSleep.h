#include <IOKit/IOService.h>

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
};
