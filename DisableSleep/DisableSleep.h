#include <IOKit/IOService.h>
class github_sheeparegreat_DisableSleep : public IOService
{
    OSDeclareDefaultStructors(github_sheeparegreat_DisableSleep)
public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
};