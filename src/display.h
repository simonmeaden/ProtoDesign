#ifndef DISPLAY_H
#define DISPLAY_H

#include <QLabel>


class Display : public QLabel {
    Q_OBJECT
public:
    Display(QWidget *parent);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void paintEvent(QPaintEvent * /* event */);

};

#endif // DISPLAY_H
