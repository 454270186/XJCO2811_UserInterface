//
//
//

#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H

#include <QPushButton>
#include <QUrl>

class TheButtonInfo {
public:
    QUrl* url;    // video file to play
    QIcon* icon;  // icon to display

    TheButtonInfo(QUrl* url, QIcon* icon) : url(url), icon(icon) {}
};

class TheButton : public QPushButton {
    Q_OBJECT

public:
    TheButtonInfo* info;

    TheButton(QWidget* parent);

    void init(TheButtonInfo* i);

private slots:
    void onClicked();

signals:
    void buttonClicked(TheButtonInfo*);
};
#endif  //CW2_THE_BUTTON_H
