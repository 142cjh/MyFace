#include "selectwin.h"
#include "ui_selectwin.h"

selectwin::selectwin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectwin)
{
    ui->setupUi(this);
    model=new QSqlTableModel();
}

selectwin::~selectwin()
{
    delete ui;
}

void selectwin::on_selectBt_clicked()
{
    if(ui->empRb->isChecked())
    {
        //显示员工注册的表格
        model->setTable("employee");
    }
    if(ui->attRb->isChecked())
    {
        //员工考勤的表格
        model->setTable("attendance");
    }

    //设置过滤器
    //model->setFilter("name=''");
    //查询
    model->select();

    ui->tableView->setModel(model);
}

