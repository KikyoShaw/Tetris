#include "TetrisPanel.h"
#include <QMouseEvent>

TetrisPanel::TetrisPanel(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	ui.widget_tetrixBoard->setNextPieceLabel(ui.label_nextPiece);

	connect(ui.pushButton_Close, &QPushButton::clicked, this, &QWidget::close);
	connect(ui.pushButton_Start, &QPushButton::clicked, ui.widget_tetrixBoard, &TetrixBoard::start);
	connect(ui.pushButton_Pause, &QPushButton::clicked, ui.widget_tetrixBoard, &TetrixBoard::pause);

	connect(ui.widget_tetrixBoard, &TetrixBoard::levelChanged, ui.lcdNumber_level, QOverload<int>::of(&QLCDNumber::display));
	connect(ui.widget_tetrixBoard, &TetrixBoard::scoreChanged, ui.lcdNumber_score, QOverload<int>::of(&QLCDNumber::display));
}

TetrisPanel::~TetrisPanel()
{
}

void TetrisPanel::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void TetrisPanel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void TetrisPanel::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
