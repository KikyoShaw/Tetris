#ifndef TETRIXBOARD_H
#define TETRIXBOARD_H

#include <QFrame>
#include <QPainter>
#include <QBasicTimer>
#include <QLabel>
#include <QPointer>

#include "TetrixPiece.h"

class TetrixBoard : public QFrame
{
    Q_OBJECT
public:
    TetrixBoard(QWidget *parent = 0);
    void setNextPieceLabel(QLabel *label);

public slots:
    void pause();
    void start();

signals:
    void levelChanged(int level);
    void scoreChanged(int score);

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    //���巽�����������������
    enum {BoardWidth = 18 ,BoardHeight = 30};
    enum {GameBoardBorder = 1};
    //����һ������
    void drawSquare(QPainter &painter,int x,int y,TetrixShape shape);
    //�ƶ�����
    bool tryMove(const TetrixPiece &newPiece,int newX,int newY);
    void pieceDroped();
    TetrixShape &shapeAt(int x, int y){return coordsBoard[x][y];}
    void newPiece();
    void clearBoard();
    void removeFullLines();
    void dropDown();
    void showNextPiece();

    TetrixPiece currentPiece;
    TetrixPiece nextPiece;
    QPointer<QLabel> nextPieceLabel;
    int curX;
    int curY;
    bool isPaused;
    bool isStarted;
    TetrixShape coordsBoard[BoardWidth][BoardHeight];
    QBasicTimer timer;
    int level;
    int score;
};

#endif // TETRIXBOARD_H
