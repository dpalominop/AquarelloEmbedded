#include <QtGui>
#include <QObject>
#include <QtCore>
#include <QLineEdit>
#include <QLineEdit>
//#include <QDesktopWidget>
#include "base.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
/*
  const int width = QApplication::desktop()->width();
  const int height = QApplication::desktop()->height();
  const QRect rect = QApplication::desktop()->rect();

  const int left = rect.left();
  const int right = rect.right();
  const int bottom = rect.bottom();
  const int top = rect.top();

  printf("Width:%d\n",width);
  printf("Height:%d\n ",height);
*/
    app.setOverrideCursor(QCursor(Qt::BlankCursor));
    Base *window=new Base();
    window->show();
    return app.exec();
}


