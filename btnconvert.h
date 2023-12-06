#ifndef BTNCONVERT_H
#define BTNCONVERT_H
#include <QPushButton>
#include <QString>
class BtnConvert : public QPushButton {
    Q_OBJECT

public:
    explicit BtnConvert(const QString &videoPath, QWidget *parent = nullptr);
    QString getVideoPath() const;
private:
    QString videoPath;

};


#endif // BTNCONVERT_H
