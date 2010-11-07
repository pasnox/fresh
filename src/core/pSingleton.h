#ifndef PSINGLETON_H
#define PSINGLETON_H

/*!
	\file pSingleton.h
	\date 2008-01-14T00:27:37
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief Singletonize your QObject class
	\details thread safe
*/

#include "FreshExport.h"

#include <QWeakPointer>
#include <QMutex>
#include <QMutexLocker>
#include <QCoreApplication>

/*!
	\brief Internal hash of instances
	\details This class contains unique instance pointer of singletonized classes
*/
template <class T>
class FRESH_EXPORT pSingletonExpose
{
protected:
	static QWeakPointer<T> mInstance;
	static QMutex mMutex;
};

/*!
	\brief Singletonize your QObject class in a thread safe way.
	\details When heriting this template, you self made your class singletonizable ( unique instance ) ( ie: call like youclass::instance() )
*/
template <class T>
class FRESH_EXPORT pSingleton : public pSingletonExpose<T>
{
protected:
	pSingleton() {};
	virtual ~pSingleton()
	{
		QMutexLocker locker( &pSingletonExpose<T>::mMutex );
		pSingletonExpose<T>::mInstance.clear();
	}

public:
	template <typename P>
	static T* instance( P* pointer );
	static T* instance();
	static bool instanceAvailable();
	static void deleteInstance();
};

/*!
	\details Return an unique instance of the class
	\param pointer A pointer that is given to constructor of the class
*/
template <class T>
template <typename P>
T* pSingleton<T>::instance( P* pointer )
{
	QMutexLocker locker( &pSingletonExpose<T>::mMutex );
	T* instance = pSingletonExpose<T>::mInstance.data();
	
	if ( !instance ) {
		pSingletonExpose<T>::mInstance = ( instance = new T( pointer ) );
	}
	
	return instance;
}

/*!
	\details Return an unique instance of the class
*/
template <class T>
T* pSingleton<T>::instance()
{
	QMutexLocker locker( &pSingletonExpose<T>::mMutex );
	T* instance = pSingletonExpose<T>::mInstance.data();
	
	if ( !instance ) {
#ifdef Q_CC_GNU
		pSingletonExpose<T>::mInstance = ( instance = new T );
#else
		pSingletonExpose<T>::mInstance = ( instance = new T( QCoreApplication::instance() ) );
#endif
	}
	
	return instance;
}

/*!
	\details Return true if an instance of the class already exists, else return false
*/
template <class T>
bool pSingleton<T>::instanceAvailable()
{
	QMutexLocker locker( &pSingletonExpose<T>::mMutex );
	return !pSingletonExpose<T>::mInstance.isNull();
}

/*!
	\details Clear the instance if instance is available
*/
template <class T>
void pSingleton<T>::deleteInstance()
{
	if ( instanceAvailable() ) {
		QMutexLocker locker( &pSingletonExpose<T>::mMutex );
		delete pSingletonExpose<T>::mInstance.data();
	}
}

#endif // PSINGLETON_H
