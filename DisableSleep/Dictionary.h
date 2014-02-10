//
//  Dictionary.h
//  DisableSleep
//

#ifndef __DisableSleep__Dictionary__
#define __DisableSleep__Dictionary__

#include <libkern/c++/OSDictionary.h>

#define Dictionary github_sheeparegreat_Dictionary

class Dictionary : public OSDictionary
{
    OSDeclareDefaultStructors(Dictionary);

public:
     /*!
      * @typedef Notification
      *
      * @discussion
      * Type and arguments of callback function that is called when changes of
      * interest are detected.
      *
      * @param target      Traget of the function
      * @param aDictionary The Dictionary object that was updated
      * @param aKey        The Key that was updated
      * @param aValue      The new Value, or NULL if the Key was deleted.
     */
     typedef void (*Notification)(OSObject              *target,
                                  const Dictionary      *aDictionary,
                                  const OSSymbol        *aKey,
                                  const OSMetaClassBase *anObject);

    /*!
     * @function withDictionary
     *
     * @abstract
     * Create a Dictionary based on the one provided
     *
     * @param dict   The reference dictionary (must not be NULL)
     * @param cb     The function to be called when SleepDisabled is updated
     * @param target The target of the callback.  This is retained if non-null
     *
     * @result A new Dictionary with a retain count of 1 or NULL on failure
     */
    static Dictionary *withDictionary(const OSDictionary *dict,
                                      Notification       cb,
                                      OSObject           *target);

    /*!
     * @function free
     *
     * @abstract
     * Deallocates or releases any resources used by the instance
     *
     * @discussion
     * This function should not be called directly, use release instead
     */
    virtual void free();

    /*!
     * @function setObject
     *
     * @abstract
     * Stores an object in the dictionary under a key.
     *
     * @param aKey      An OSSymbol identifying the object placed within the
     *                  dictionary.  It is automatically retained.
     * @param anObject  The object to be stored in the dictionary.  It is
     *                  automatically retained.
     *
     * @result
     * true or false depending on whether the addition was successful or not
     */
    virtual bool setObject(const OSSymbol        *aKey,
                           const OSMetaClassBase *anObject);


    /*!
     * @function removeObject
     *
     * @abstract
     * Removes a key/object pair from the dictionary.
     *
     * @param aKey  An OSSymbol identifying the object to be removed from the
     *              dictionary.
     */
    virtual void removeObject(const OSSymbol *aKey);

    /*!
     * @function getBaseDictionary
     *
     * @abstract
     * Get the OSDictionary used during the creation of this dictionary
     *
     * @discussion
     * The returned dictionary should have identical contents to this instance.
     */
    virtual OSDictionary* getBaseDictionary();

private:
    Notification cb;
    OSObject *target;
    OSDictionary *original;

};

#endif /* defined(__DisableSleep__Dictionary__) */