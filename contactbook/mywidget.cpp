#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

QString mFilename = "myfile.csv";

void Write(QString Filename,QString str){
    QFile mFile(Filename);
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"could not open file for write";
        return;
    }
    QTextStream out(&mFile);
    out.setEncoding(QStringConverter::System);
    out<<str;
    mFile.flush();
    mFile.close();
}

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    QStringList ColTotle;
    ui->tableWidget->setColumnCount(4);
    ColTotle<<QStringLiteral("學號")<<QStringLiteral("班級")<<QStringLiteral("姓名")<<QStringLiteral("電話");
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;
    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);



}


void MyWidget::on_pushButton_2_clicked()
{
    QString savePath = QFileDialog::getSaveFileName(
        this,
        "選擇儲存位置",
        QDir::currentPath(),
        "CSV Files (*.csv)"
        );

    // 若使用者取消選擇則不執行寫入
    if (savePath.isEmpty()) return;

    QString saveFile="";
    for(int i = 0; i<ui->tableWidget->rowCount();i++){
        for (int j = 0; j<ui->tableWidget->columnCount();j++) {
            saveFile+=ui->tableWidget->item(i,j)->text();//去掉後方逗號
            if (j < ui->tableWidget->columnCount() - 1) {//判斷是否最後一行
                saveFile += ",";
            }
        }
        saveFile+="\n";
    }
    Write(savePath, saveFile);
}


void MyWidget::on_pushButton_3_clicked()
{
    QString openPath = QFileDialog::getOpenFileName(
        this,
        "選擇要匯入的 CSV 檔案",
        QDir::currentPath(),
        "CSV Files (*.csv)"
        );

    // 如果使用者按下取消，則不執行後續動作
    if (openPath.isEmpty()) return;

    QFile file(openPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "錯誤", "無法開啟該檔案！");
        return;
    }

    // 2. 匯入前先清空現有表格內容，保持乾淨
    ui->tableWidget->setRowCount(0);

    QTextStream in(&file);
    in.setEncoding(QStringConverter::System); // 強制指定以 UTF-8 讀取

    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue; // 跳過空白行

        // 過濾檔案開頭可能帶有的 UTF-8 BOM 標記
        if (row == 0 && line.startsWith("\xEF\xBB\xBF")) {
            line = line.mid(3);
        }

        // 以逗號分隔欄位
        QStringList fields = line.split(",");

        // 在 Table 中新增一行並填入資料
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < fields.size() && col < ui->tableWidget->columnCount(); ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(fields[col].trimmed()));
        }
        row++;
    }

    file.close();
    QMessageBox::information(this, "成功", "資料已成功匯入！");
}

