#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <QWidget>

namespace Ui {
class directories;
}

class directories : public QWidget
{
    Q_OBJECT

public:
    explicit directories(QWidget *parent = 0);
    ~directories();

private:
    Ui::directories *ui;
};

#endif // DIRECTORIES_H
