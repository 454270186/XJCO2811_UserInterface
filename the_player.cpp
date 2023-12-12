//
//
//

#include "the_player.h"

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;

    // 停止定时器
    mTimer->stop();

    // 连接按钮的新信号
    for (auto button : *buttons) {
        connect(button, SIGNAL(buttonClicked(TheButtonInfo*)), this, SLOT(jumpTo(TheButtonInfo*)));
    }
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
    int newIndex = -1;
    do {
        newIndex = rand() % infos->size();
    } while (newIndex == lastButtonIndex);

    TheButtonInfo* i = &infos->at(newIndex);
    setMedia(*i->url);
    buttons->at(updateCount++ % buttons->size())->init(i);

    lastButtonIndex = newIndex;
}

void ThePlayer::playStateChanged(QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play();  // starting playing again...
            break;
        default:
            break;
    }
}

void ThePlayer::jumpTo(TheButtonInfo* button) {
    setMedia(*button->url);
    play();
}
