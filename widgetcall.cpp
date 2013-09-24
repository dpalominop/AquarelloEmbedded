#include "widgetcall.h"

WidgetCall::WidgetCall(QWidget *parent, QString icon_background) :
    QWidget(parent)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("Form"));
    this->setEnabled(true);

    //this->setStyleSheet(QStringLiteral("background-color: rgb(100, 0, 255);"));

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    gridLayout_4 = new QGridLayout();
    gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
    dest_01 = new MyButton(this);
    dest_01->setObjectName(QStringLiteral("dest_01"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    //sizePolicy.setHeightForWidth(dest_01->sizePolicy().hasHeightForWidth());
    sizePolicy.setHeightForWidth(false);
    dest_01->setSizePolicy(sizePolicy);
    gridLayout_4->addWidget(dest_01, 2, 0, 1, 1);

    message = new QLabel(this);
    message->setObjectName(QStringLiteral("message"));
    message->setText("VOTRE APPEL CONCERNE:");
    message->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);font: 75 30pt FreeSerif;"));

    //this->setStyleSheet(QString("QPushButton {background-image:url(%1);background-color:#FFA600; border-style: outset;border-width: 2px;border-radius: 30px;border-color: beige;color:black; font: 75 italic 35pt Ubuntu Mono; text-align:left;}").arg(QCoreApplication::applicationDirPath()+icon_background));
    this->setStyleSheet(QString("QPushButton {background-image:url(%1);border-style: outset;border-width: 0px;border-radius: 30px;border-color: beige;color:black; font: 75 italic 35pt Ubuntu Mono; text-align:left;}").arg(QCoreApplication::applicationDirPath()+icon_background));


    gridLayout_4->addWidget(message, 0, 0, 2, 2);

    dest_06 = new MyButton(this);
    dest_06->setObjectName(QStringLiteral("dest_06"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    //sizePolicy1.setHeightForWidth(dest_06->sizePolicy().hasHeightForWidth());
    sizePolicy1.setHeightForWidth(false);
    dest_06->setSizePolicy(sizePolicy1);

    gridLayout_4->addWidget(dest_06, 4, 1, 1, 1);

    dest_07 = new MyButton(this);
    dest_07->setObjectName(QStringLiteral("dest_07"));
    //sizePolicy1.setHeightForWidth(dest_07->sizePolicy().hasHeightForWidth());
    dest_07->setSizePolicy(sizePolicy1);

    gridLayout_4->addWidget(dest_07, 5, 0, 1, 1);

    dest_03 = new MyButton(this);
    dest_03->setObjectName(QStringLiteral("dest_03"));
    //sizePolicy1.setHeightForWidth(dest_03->sizePolicy().hasHeightForWidth());
    dest_03->setSizePolicy(sizePolicy1);

    gridLayout_4->addWidget(dest_03, 3, 0, 1, 1);

    dest_05 = new MyButton(this);
    dest_05->setObjectName(QStringLiteral("dest_05"));
    //sizePolicy1.setHeightForWidth(dest_05->sizePolicy().hasHeightForWidth());
    dest_05->setSizePolicy(sizePolicy1);

    gridLayout_4->addWidget(dest_05, 4, 0, 1, 1);

    dest_02 = new MyButton(this);
    dest_02->setObjectName(QStringLiteral("dest_02"));
    //sizePolicy1.setHeightForWidth(dest_02->sizePolicy().hasHeightForWidth());
    dest_02->setSizePolicy(sizePolicy1);

    gridLayout_4->addWidget(dest_02, 2, 1, 1, 1);

    dest_04 = new MyButton(this);
    dest_04->setObjectName(QStringLiteral("dest_04"));
    //sizePolicy1.setHeightForWidth(dest_04->sizePolicy().hasHeightForWidth());
    dest_04->setSizePolicy(sizePolicy1);

    gridLayout_4->addWidget(dest_04, 3, 1, 1, 1);

    dest_08 = new MyButton(this);
    dest_08->setObjectName(QStringLiteral("dest_08"));
    //sizePolicy1.setHeightForWidth(dest_08->sizePolicy().hasHeightForWidth());
    dest_08->setSizePolicy(sizePolicy1);

    gridLayout_4->addWidget(dest_08, 5, 1, 1, 1);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout_4->addItem(verticalSpacer, 1, 0, 1, 1);


    verticalLayout->addLayout(gridLayout_4);

    gridLayout_2 = new QGridLayout();
    gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_2, 2, 5, 1, 1);


    verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout_2->addItem(verticalSpacer_5, 3, 4, 1, 1);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer, 2, 0, 1, 1);

    verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout_2->addItem(verticalSpacer_4, 0, 4, 1, 1);

    horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_6, 2, 3, 1, 1);

    horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_5, 2, 2, 1, 1);

    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_4, 2, 1, 1, 1);

    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout_2->addItem(verticalSpacer_2, 1, 4, 1, 1);


    verticalLayout->addLayout(gridLayout_2);


}
