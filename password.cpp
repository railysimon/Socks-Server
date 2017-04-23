#include "password.h"

Password::Password(QWidget *parent) : QDialog(parent)
{
    this->setFixedSize(300, 200);
    this->setWindowTitle("Log in");

    user = new QLineEdit("User");
    passwd = new QLineEdit("password");
    passwd->setEchoMode(QLineEdit::Password);

    QPushButton *btn = new QPushButton("Log in");
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(ButtonClick()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(user);
    layout->addWidget(passwd);
    layout->addWidget(btn);
    this->setLayout(layout);
}

Password::~Password()
{

}

void Password::ButtonClick()
{
       emit signal_userdata(user->text(), passwd->text());
       this->close();
}
