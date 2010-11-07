#include "pSingleton.h"

template <class T>
QWeakPointer<T> pSingletonExpose<T>::mInstance;
template <class T>
QMutex pSingletonExpose<T>::mMutex;
