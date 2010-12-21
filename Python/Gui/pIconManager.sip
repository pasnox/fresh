/*!
	\file pIconManager.h
	\brief A cache class for icons and pixmaps
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
*/

/*!
	\ingroup Gui
	\namespace pIconManager
	\brief A cache class for icons and pixmaps.
	
	This class allow to cache and share QPixmap & QIcon.
	Icons and pixmaps can easily be loaded on demand by using coresponding members.
*/
namespace pIconManager
{
%TypeHeaderCode
#include <gui/pIconManager.h>
%End

	/*!
		\brief Return the filepath of the file \a fileName in \a prefix folder (check is done recursively).
		\param fileName The file name of the file to find (ie: toto.txt).
		\param prefix The start path to look from, the scan is done recursively until a match or nothing found.
		\return A QString containing the absolute file path of the file, the fileName/prefix pair is cached for fast lookup the next call.
	*/
	QString filePath( const QString& fileName, const QString& prefix = ":/" );
	/*!
		\brief Return the filepath of the pixmap \a fileName in \a prefix folder (check is done recursively).
		\param fileName The file name of the pixmap to find (ie: toto.png).
		\param prefix The start path to look from, the scan is done recursively until a match or nothing found.
		\return A QString containing the absolute file path of the pixmap, the fileName/prefix pair is cached for fast lookup the next call.
	*/
	QPixmap pixmap( const QString& fileName, const QString& prefix = ":/" );
	/*!
		\brief Return the filepath of the icon \a fileName in \a prefix folder (check is done recursively).
		\param fileName The file name of the icon to find (ie: toto.png).
		\param prefix The start path to look from, the scan is done recursively until a match or nothing found.
		\return A QString containing the absolute file path of the icon, the fileName/prefix pair is cached for fast lookup the next call.
	*/
	QIcon icon( const QString& fileName, const QString& prefix = ":/" );
};
