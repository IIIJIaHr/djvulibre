//C-  -*- C++ -*-
//C-
//C- DjVu� Unix Viewer (v. 3.5)
//C- 
//C- Copyright � 1999-2001 LizardTech, Inc. All Rights Reserved.
//C- The DjVu Reference Library is protected by U.S. Pat. No.
//C- 6,058,214 and patents pending.
//C- 
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, Version 2. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C- 
//C- The computer code originally released by LizardTech under this
//C- license and unmodified by other parties is deemed the "LizardTech
//C- Original Code."
//C- 
//C- With respect to the LizardTech Original Code ONLY, and subject
//C- to any third party intellectual property claims, LizardTech
//C- grants recipient a worldwide, royalty-free, non-exclusive license
//C- under patent claims now or hereafter owned or controlled by
//C- LizardTech that are infringed by making, using, or selling
//C- LizardTech Original Code, but solely to the extent that any such
//C- patent(s) is/are reasonably necessary to enable you to make, have
//C- made, practice, sell, or otherwise dispose of LizardTech Original
//C- Code (or portions thereof) and not to any greater extent that may
//C- be necessary to utilize further modifications or combinations.
//C- 
//C- The LizardTech Original Code is provided "AS IS" WITHOUT WARRANTY
//C- OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//C- TO ANY WARRANTY OF NON-INFRINGEMENT, OR ANY IMPLIED WARRANTY OF
//C- MERCHANTIBILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//C-
// 
// $Id$
// $Name$

#ifndef HDR_QLIB
#define HDR_QLIB
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef __GNUG__
#pragma interface
#endif


#include "GException.h"
#include "GRect.h"
#include "Arrays.h"
#include "GPixmap.h"
#include "GString.h"

#include <qapplication.h>
#include <qdialog.h>
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qrect.h>
#include <qmenudata.h>
#include <qstring.h>

#include "debug.h"

// ------------------------------------------------

inline GUTF8String
GStringFromQString(const QString & x)
{
  GUTF8String retval=(const char *)x.utf8();
  return retval;
}

inline QString
QStringFromGString(const GUTF8String & x)
{
  QString retval=QString::fromUtf8((const char *)x);
  return retval;
}

inline GRect 
Q2G(const QRect & r)
{
  return GRect(r.x(), r.y(), r.width(), r.height());
}

inline QRect
G2Q(const GRect & r)
{
  return QRect(r.xmin, r.ymin, r.width(), r.height());
}


// ------------------------------------------------


class QeApplication : public QApplication
{
  Q_OBJECT
public:
  QeApplication(int &argc, char **argv);
  ~QeApplication();

  // Function killWidget() schedules a 
  // widget for destruction.
public:
  void           killWidget(QWidget * widget);
private:
  QList<QWidget> widgetsToKill;
  QTimer         kill_timer;
private slots:
  void           killDestroy(void);
  void           killTimeout(void);
  
  // Signal gotX11Event tracks X11 events
#ifdef UNIX
signals:
   void         gotX11Event(XEvent *);
public:
   bool         x11EventResult;
   virtual bool x11EventFilter(XEvent *);
#endif

  // Contain starting gamma and geometry
public:
   float	gamma;
   QString	geometry;
   void		setWidgetGeometry(QWidget * w);
};

extern QeApplication * qeApp;


// ------------------------------------------------

class QeDialog : public QDialog
{
  Q_OBJECT
public:
  QeDialog(QWidget * parent=0, const char * name=0, bool modal=FALSE, WFlags f=0);
  QWidget *startWidget() { return this; }
  static void makeTransient(QWidget *w, QWidget *fw);
};

// ------------------------------------------------

class QeExcMessage : public QeDialog
{
   Q_OBJECT
private:
   QWidget	* details;
   QPushButton	* details_butt;
private slots:
   void		switchDetails(void);
public:
   QeExcMessage(const GException &exc, const char *title=0,
		QWidget *parent=0, const char *name=0, bool modal=TRUE);
};

void showError(QWidget * parent, const GException & exc);
void showError(QWidget * parent, const QString &title, const GException & exc);
void showError(QWidget * parent, const QString &title, const QString &message);
void showWarning(QWidget * parent, const QString &title, const QString &message);
void showInfo(QWidget * parent, const QString &title, const QString &message);
void showMessage(QWidget * parent, const QString &title, const QString &message,
		 bool draw_frame=false, bool use_fixed=false,
		 bool word_wrap=false);

// ------------------------------------------------

class QeFileDialog : public QFileDialog
{
   Q_OBJECT
private:
   bool	forWriting;
protected slots:
   virtual void done(int rc);
public:
  static QString lastSaveDir;
  static QString lastLoadDir;
  void setForWriting(bool fwr);
  QeFileDialog(const QString &dirName, const char *filter=0,
               QWidget *parent=0, const char * name=0, bool modal=FALSE);
  QeFileDialog(QWidget *parent=0, const char * name=0, bool modal=FALSE);
};


// ------------------------------------------------

QPixmap createIcon(const GPixmap & gpix);
void setItemsEnabled(QMenuData * menu, bool flag);
bool setComboBoxCurrentItem(QComboBox *combo, QString item);

#endif
