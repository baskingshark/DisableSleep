//
//  Dictionary.cpp
//  DisableSleep
//

#include <libkern/c++/OSSymbol.h>
#include "Dictionary.h"

#define super OSDictionary

OSDefineMetaClassAndStructors(Dictionary, OSDictionary)

static
bool
isSleepDisabled(const OSSymbol *aKey)
{
    return aKey->isEqualTo("SleepDisabled");
}

void Dictionary::free()
{
    OSSafeRelease(target);
    OSSafeRelease(original);
}

bool Dictionary::setObject(const OSSymbol *aKey,
                           const OSMetaClassBase *anObject)
{
    bool result = super::setObject(aKey, anObject);
    if(result)
        original->setObject(aKey, anObject);

    if(cb && isSleepDisabled(aKey))
        cb(target, this, aKey, anObject);
    return result;
}

void Dictionary::removeObject(const OSSymbol *aKey)
{
    super::removeObject(aKey);
    original->removeObject(aKey);

    if(cb && isSleepDisabled(aKey))
        cb(target, this, aKey, nullptr);
}

OSDictionary* Dictionary::getBaseDictionary()
{
    return original;
}

Dictionary *Dictionary::withDictionary(const OSDictionary *dict,
                                       Notification        cb,
                                       OSObject           *target)
{
    if(!dict)
        return nullptr;

    Dictionary *me = new Dictionary;
    if(me) {
        if(!me->initWithDictionary(dict, dict->getCapacity())) {
            me->release();
            return nullptr;
        }
        else {
            me->cb = cb;
            me->target = target;
            me->original = const_cast<OSDictionary*>(dict);
            dict->retain();
            if(target)
                target->retain();
        }
    }

    return me;
}