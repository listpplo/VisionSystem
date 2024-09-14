#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::Form)
{
    ui->setupUi(this);

	connect(&m_staticTimer, &QTimer::timeout, this, &Form::onTimerStreamStatistic);

	initUi();
}

Form::~Form()
{
	delete ui;
}

void Form::initUi()
{
	int ret = IMV_OK;

	ui->label_Statistic->setText("");// 连接相机之前不显示状态栏 | Don't show status bar before connecting camera

	ret = IMV_EnumDevices(&m_deviceInfoList, interfaceTypeAll);
	if (IMV_OK != ret)
	{
		printf("Enumeration devices failed! ErrorCode[%d]\n", ret);
		return;
	}
	if (m_deviceInfoList.nDevNum < 1)
	{
		ui->comboBox->setEnabled(false);
		ui->pushButton_Open->setEnabled(false);
	}
	else
	{
		ui->comboBox->setEnabled(true);
		ui->pushButton_Open->setEnabled(true);

		for (unsigned int i = 0; i < m_deviceInfoList.nDevNum; i++)
		{
			ui->comboBox->addItem(m_deviceInfoList.pDevInfo[i].cameraKey);
		}

		ui->widget->SetCamera(m_deviceInfoList.pDevInfo[0].cameraKey);
	}

	ui->pushButton_Close->setEnabled(false);
	ui->pushButton_Start->setEnabled(false);
	ui->pushButton_Stop->setEnabled(false);
}

// 设置要连接的相机
// set camera which need to connect
void Form::on_comboBox_currentIndexChanged(int nIndex)
{
	ui->widget->SetCamera(m_deviceInfoList.pDevInfo[nIndex].cameraKey);
}

// 连接
// connect
void Form::on_pushButton_Open_clicked()
{
	if (!ui->widget->CameraOpen())
	{
		return;
	}

	ui->pushButton_Open->setEnabled(false);
	ui->pushButton_Close->setEnabled(true);
	ui->pushButton_Start->setEnabled(true);
	ui->pushButton_Stop->setEnabled(false);
	ui->comboBox->setEnabled(false);

	// 连接相机之后显示统计信息，所有值为0
	// Show statistics after connecting camera, all values are 0
	ui->widget->resetStatistic();
	QString strStatic = ui->widget->getStatistic();
    ui->label_Statistic->setText(strStatic);
}

// 断开
// disconnect
void Form::on_pushButton_Close_clicked()
{
	on_pushButton_Stop_clicked();
	ui->widget->CameraClose();

    ui->label_Statistic->setText("");// 断开相机以后不显示状态栏 | Do not display the status bar after disconnecting the camera

	ui->pushButton_Open->setEnabled(true);
	ui->pushButton_Close->setEnabled(false);
	ui->pushButton_Start->setEnabled(false);
	ui->pushButton_Stop->setEnabled(false);
	ui->comboBox->setEnabled(true);
}

// 开始
// start 
void Form::on_pushButton_Start_clicked()
{
	// 设置连续拉流
	// set continue grabbing
	// ui->widget->CameraChangeTrig(CammerWidget::trigContinous);

	ui->widget->CameraStart();

	ui->pushButton_Start->setEnabled(false);
	ui->pushButton_Stop->setEnabled(true);

	ui->widget->resetStatistic();
	m_staticTimer.start(100);
}

// 停止
// stop
void Form::on_pushButton_Stop_clicked()
{
	m_staticTimer.stop();

    ui->widget->CameraStop();

	ui->pushButton_Start->setEnabled(true);
	ui->pushButton_Stop->setEnabled(false);
}

void Form::onTimerStreamStatistic()
{
	QString strStatic = ui->widget->getStatistic();
    ui->label_Statistic->setText(strStatic);
}

void Form::closeEvent(QCloseEvent * event)
{
	on_pushButton_Stop_clicked();
	ui->widget->CameraClose();
}

