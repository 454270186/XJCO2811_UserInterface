#ifndef MYVIDEOWIDGET_H
#define MYVIDEOWIDGET_H

#include <QVideoWidget>

class MyVideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    // Constructor for MyVideoWidget class
    explicit MyVideoWidget(QWidget *parent = nullptr);

protected:
           // Add any protected members or additional functionalities if needed
};

#endif // MYVIDEOWIDGET_H
