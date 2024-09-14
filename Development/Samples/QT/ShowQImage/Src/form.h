#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QTimer>
#include "IMVAPI/IMVApi.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
	void initUi();

private slots:
	void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();
	void on_pushButton_Open_clicked();
	void on_pushButton_Close_clicked();
	void on_comboBox_currentIndexChanged(int nIndex);
	void closeEvent(QCloseEvent * event);
	void onTimerStreamStatistic();// 定时刷新状态栏信息 | Refresh status bar information regularly

private:
    Ui::Form *ui;

	IMV_DeviceList m_deviceInfoList;	// 发现的相机列表 | List of cameras found
	QTimer m_staticTimer;	// 定时器，定时刷新状态栏信息 | Timer, refresh status bar information regularly
};

#endif // FORM_H
