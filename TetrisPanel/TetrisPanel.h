#pragma once

#include <QtWidgets/QWidget>
#include "ui_TetrisPanel.h"

class TetrisPanel : public QWidget
{
    Q_OBJECT

public:
    TetrisPanel(QWidget *parent = Q_NULLPTR);
	~TetrisPanel();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::TetrisPanelClass ui;
	//窗口移动属性值
	QPoint m_point;
	bool m_bMove = false;
};
