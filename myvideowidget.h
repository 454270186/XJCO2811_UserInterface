#ifndef MYVIDEOWIDGET_H
#define MYVIDEOWIDGET_H

#include <QVideoWidget>

class MyVideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit MyVideoWidget(QWidget *parent = nullptr);

protected:

};

#endif // MYVIDEOWIDGET_H
