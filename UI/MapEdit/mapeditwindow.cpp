﻿#include "mapeditwindow.h"
#include "global.h"

#define MAP_ITEM_DATA_ROLE_TYPE Qt::UserRole + 1

#define MAP_ITEM_DATA_ROLL_TYPE_POINT   1
#define MAP_ITEM_DATA_ROLL_TYPE_POINT   1
#define MAP_ITEM_DATA_ROLL_TYPE_POINT   1

MapEditWindow::MapEditWindow(OneMap _oneMap, QWidget *parent) : QMainWindow(parent),
    oneMap(_oneMap)
{
    createActions();
    createStatusBar();

    init();
}

void MapEditWindow::init()
{
    connect(&msgCenter,SIGNAL(sig_connection_connected()),this,SLOT(onServerConnect()));
    connect(&msgCenter,SIGNAL(sig_connection_disconnected()),this,SLOT(onServerDisconnect()));
    connect(&msgCenter,SIGNAL(sig_connection_conntectting()),this,SLOT(onServerConnectting()));

    connect(&msgCenter,SIGNAL(sendRequestFail()),this,SLOT(onSendFail()));
    connect(&msgCenter,SIGNAL(waitResponseTimeOut()),this,SLOT(onWaitResponseTimeOut()));
    connect(&msgCenter,SIGNAL(tip(QString)),this,SLOT(onTip(QString)));
    connect(&msgCenter,SIGNAL(err(int,QString)),this,SLOT(onErr(int,QString)));
    connect(&msgCenter,SIGNAL(sendNewRequest()),this,SLOT(onNewRequest()));
}

void MapEditWindow::onServerConnect()
{
    statusbar_info(QStringLiteral("已连接"));
}

void MapEditWindow::onServerDisconnect()
{
    statusbar_info(QStringLiteral("连接已断开"));
}

void MapEditWindow::onServerConnectting()
{
    statusbar_info(QStringLiteral("尝试连接服务器..."));
}

void MapEditWindow::onSendFail()
{
    statusbar_err(QStringLiteral("发送请求失败"));
}

void MapEditWindow::onWaitResponseTimeOut()
{
    statusbar_err(QStringLiteral("等待响应超时"));
}
void MapEditWindow::onTip(QString s)
{
    statusbar_info(s);
}
void MapEditWindow::onErr(int errcode,QString info)
{
    statusbar_err(getErrorString(errcode)+"  "+info);
}

void MapEditWindow::onNewRequest()
{
    statusbar_err("");
    statusbar_info("");
}

void MapEditWindow::createStatusBar()
{
    QStatusBar *statusbar = new QStatusBar(this);
    QLabel *usernamelabel = new QLabel(QStringLiteral("当前用户:")+QString::fromStdString(std::string(current_user_info.username)));
    QString role_name = "";
    switch (current_user_info.role) {
    case USER_ROLE_VISITOR:
        role_name =QStringLiteral( "未登录");
        break;
    case USER_ROLE_OPERATOR:
        role_name =QStringLiteral( "普通用户");
        break;
    case USER_ROLE_ADMIN:
        role_name =QStringLiteral( "管理员");
        break;
    case USER_ROLE_SUPER_ADMIN:
        role_name =QStringLiteral( "超级管理员");
        break;
    case USER_ROLE_DEVELOP:
        role_name =QStringLiteral( "开发人员");
        break;
    default:
        break;
    }
    QLabel *userrolelabel = new QLabel(QStringLiteral("当前角色:")+role_name);
    info_label = new QLabel(QStringLiteral("信息:"));
    error_label = new QLabel(QStringLiteral("错误:"));
    usernamelabel->setMinimumWidth(200);
    userrolelabel->setMinimumWidth(200);
    info_label->setMinimumWidth(200);

    statusbar->addWidget(usernamelabel);
    statusbar->addWidget(userrolelabel);
    statusbar->addWidget(info_label);
    statusbar->addWidget(error_label);

    setStatusBar(statusbar);
}

void MapEditWindow::createActions()
{
    QMenu *viewsMenu = menuBar()->addMenu(tr("Views"));
    QToolBar *viewsToolBar = addToolBar(tr("Views"));

    dockMapTree = new DockMapTree(&oneMap);
    dockProperty = new DockProperty(&oneMap);
    dockView = new DockView(&oneMap);

    addDockWidget(Qt::LeftDockWidgetArea,dockMapTree);
    splitDockWidget(dockMapTree,dockProperty,Qt::Vertical);
    splitDockWidget(dockMapTree,dockView,Qt::Horizontal);

    viewsMenu->addAction(dockMapTree->toggleViewAction());
    viewsToolBar->addAction(dockMapTree->toggleViewAction());

    viewsMenu->addAction(dockProperty->toggleViewAction());
    viewsToolBar->addAction(dockProperty->toggleViewAction());

    viewsMenu->addAction(dockView->toggleViewAction());
    viewsToolBar->addAction(dockView->toggleViewAction());

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MapEditWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &HRG"), this, &MapEditWindow::aboutHrg);
    aboutQtAct->setStatusTip(tr("Show the HRG's About box"));

}

void MapEditWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>HRG Dispatch client</b> connect to dispatch server,<br>"
                          "and show the ui for user to watch the status of agvs and<br>"
                          "other information about the agv dispatch system."));
}

void MapEditWindow::aboutHrg()
{
    QMessageBox::about(this, tr("About Hit Robot Group"),
                       tr("<b>Hit Robot Group</b><br>"
                          "we have the top robotics research scientists in<br>"
                          "the country and we are in the robot technology <br>"
                          "subject frountirer struggle,unity,hard work of<br>"
                          "high level research team.<br>"
                          "To get more information about us,you can visit our<br>"
                          " website: <a href=\'http://www.hrgrobotics.com/\'>HRG</a>"));
}

void MapEditWindow::statusbar_info(QString msg)
{
    info_label->setText(QStringLiteral("信息:")+msg);
}

void MapEditWindow::statusbar_err(QString msg)
{
    error_label->setText(QStringLiteral("错误:")+msg);
}