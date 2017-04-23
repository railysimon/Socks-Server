#ifndef PASSWORD_H
#define PASSWORD_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

class Password : public QDialog
{
    Q_OBJECT

public:
        Password(QWidget *parent = 0);
        ~Password();

private:
        QLineEdit *user, *passwd;

private slots:
                void ButtonClick();

signals:
        void signal_userdata(QString, QString);
};

#endif // PASSWORD_H
