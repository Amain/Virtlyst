#ifndef __MY_GENERIC_H
#define __MY_GENERIC_H

#include <QObject>
#include <QMap>
#include <QVariantList>
#include <QMetaEnum>

#include <Cutelyst/ParamsMultiMap>

using namespace Cutelyst;

template<typename QEnum>
const char* QtEnumToString(const int value)
{
    return QMetaEnum::fromType<QEnum>().valueToKey(value);
}

template<typename QEnum>
int QtStringToEnum (const char* key)
{
    return QMetaEnum::fromType<QEnum>().keyToValue(key);
}

template<typename QEnum>
QMap<int, const char*> QtEnumToQMap ()
{
    QMap<int, const char*> map;
    QMetaEnum mEnum = QMetaEnum::fromType<QEnum>();
    for(int i = 0; i < mEnum.keyCount(); i++) {
        int val = mEnum.value(i);
        map.insert(val, mEnum.valueToKey(val));
    }

    return map;
}

template<typename QEnum>
ParamsMultiMap QtEnumToParamsMultiMap ()
{
    ParamsMultiMap map;
    QMetaEnum mEnum = QMetaEnum::fromType<QEnum>();
    for(int i = 0; i < mEnum.keyCount(); i++) {
        int val = mEnum.value(i);
        if(! map.contains(QString(val))) {
            /* Enums may have multiple keys pointing to the same value. Only
               adding first */
               const char * key = mEnum.valueToKey(val);
            map.insert(QString::number(val), QString(key));
        }
    }

    return map;
}

#endif // __MY_GENERIC_H
