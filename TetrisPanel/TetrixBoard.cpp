#include "TetrixBoard.h"
#include "TetrixPiece.h"

#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QPointer>
#include <QSize>

//��ɫ�������붨�����״˳��һֱ
static const QRgb colorTable[8] = {
    0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
    0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
};

TetrixBoard::TetrixBoard(QWidget *parent) :
    QFrame(parent)
{
    setFrameStyle(QFrame::NoFrame);
    setFocusPolicy(Qt::StrongFocus);
    clearBoard();
    isPaused = false;
    isStarted = false;
    nextPiece.setRandomShape();
}


void TetrixBoard::start(){
    if(isPaused){
        return;
    }
    isStarted = true;
    clearBoard();
    newPiece();

    level = 1;
    score = 0;

    emit levelChanged(level);
    emit scoreChanged(score);

    timer.start(1000/level,this);
}

void TetrixBoard::setNextPieceLabel(QLabel *label){
    nextPieceLabel = label;
}

void TetrixBoard::showNextPiece(){
    if(!nextPieceLabel){
        return;
    }

    QPixmap pixmap(nextPiece.getWidth() * SquareWidth , nextPiece.getHeight() * SquareHeight);
    QPainter painter(&pixmap);
   // painter.fillRect(pixmap.rect(),nextPieceLabel->palette().background());
	painter.fillRect(pixmap.rect(), QColor(220, 220, 220));
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(nextPiece.value(i,j) == 0){
                continue;
            }
            int x = (j - nextPiece.x()) * SquareWidth;
            int y = (i - nextPiece.y())  * SquareHeight;
            drawSquare(painter,x,y,nextPiece.shape());
        }
    }
    nextPieceLabel->setPixmap(pixmap);
}

void TetrixBoard::paintEvent(QPaintEvent *event){

    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::gray);
    //��Ϸ�߿�
    painter.drawRect(GameBoardBorder - 1,GameBoardBorder - 1,SquareWidth * BoardWidth + 1,SquareHeight * BoardHeight + 1);

    //�����Ѿ����ڵ���״
    for(int i = 0; i < BoardHeight; i++){
        for(int j = 0; j < BoardWidth; j++){
            TetrixShape shape = shapeAt(j,i);
            if(shape != NOShape){
                int x = GameBoardBorder + j * SquareWidth;
                int y = GameBoardBorder + i * SquareHeight;
                drawSquare(painter, x, y,shape);
            }
        }
    }

    //������ǰ����״
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(currentPiece.value(i,j) == 0){
                continue;
            }
            int x = (j - currentPiece.x() + curX) * SquareWidth + GameBoardBorder;
            int y = (i - currentPiece.y() + curY) * SquareHeight + GameBoardBorder;
            drawSquare(painter,x,y,currentPiece.shape());
        }
    }
    //��ͣ
    if(isPaused){
        painter.setPen(QColor(Qt::green).light());
        painter.drawText(contentsRect(),Qt::AlignCenter,tr("Pause"));
        return;
    }

 }

void TetrixBoard::keyPressEvent(QKeyEvent *event){
    if(!isStarted || isPaused || currentPiece.shape() == NOShape){
        return;
    }
    switch(event->key()){
    case Qt::Key_Left:
        tryMove(currentPiece,curX - 1,curY);
        break;
    case Qt::Key_Right:
        tryMove(currentPiece,curX + 1,curY);
        break;
    case Qt::Key_Down:
        if(!tryMove(currentPiece,curX,curY+1)){
            pieceDroped();
        }
        break;
    case Qt::Key_J:
        tryMove(currentPiece.rotateLeft(),curX,curY);
        break;
    case Qt::Key_Up:
        tryMove(currentPiece.rotateRight(),curX,curY);
        break;
    case Qt::Key_Space:
        dropDown();
        break;
    default:
        //�¼�����
        QFrame::keyPressEvent(event);
    }
}

void TetrixBoard::timerEvent(QTimerEvent *event){
    if(timer.timerId() == event->timerId()){
        if(!tryMove(currentPiece,curX,curY+1)){
            pieceDroped();
        }
    }else{
        QFrame::timerEvent(event);
    }
}

void TetrixBoard::dropDown(){
    int newY = curY;
    while(newY < BoardHeight){
        if(!tryMove(currentPiece,curX,curY+1)){
            break;
        }
        ++newY;
    }
    pieceDroped();
}

void TetrixBoard::pieceDroped(){
    //�޷��ƶ�������ݵ�ǰλ�ý����鸳ֵ��board
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(currentPiece.value(i,j) == 0){
                continue;
            }
            shapeAt(j - currentPiece.x() + curX,i - currentPiece.y() + curY) = currentPiece.shape();
        }
    }
    removeFullLines();
    newPiece();
}

void TetrixBoard::removeFullLines(){

    int numFullLines = 0;

    for(int i = 0; i < BoardHeight; i++){
        bool isFullLine = true;
        for(int j = 0; j < BoardWidth; j++){
            if(shapeAt(j,i) == NOShape){
                isFullLine = false;
                break;
            }
        }
        if(isFullLine){
            //������ǰ������
            ++ numFullLines;
            for(int j = 0; j < BoardWidth; j++){
                shapeAt(j,i) = NOShape;
            }
            //����һ�е����������ƶ�һ��
            for(int k = i; k > 0; k--){
                for(int j = 0; j < BoardWidth; j++){
                    shapeAt(j,k) = shapeAt(j,k - 1);
                }
            }
        }
    }

    //�ؿ���������
    if(numFullLines > 0){
        score += numFullLines;
        emit scoreChanged(score);
        if( score - (level - 1) * 25 >=  25){
            ++level;
            timer.start(1000/level,this);
            emit levelChanged(level);
        }
        update();
    }


}

void TetrixBoard::newPiece(){
    currentPiece = nextPiece;
    nextPiece.setRandomShape();
    showNextPiece();
    curX = BoardWidth / 2 - 1 ;
    curY = 0;

    //����޷��ƶ���ʾ�Ѿ����ﶥ��
    if(!tryMove(currentPiece,curX,curY)){
        currentPiece.setShape(NOShape);
        timer.stop();
        isStarted = false;
    }
}

void TetrixBoard::clearBoard(){
    for(int i = 0; i < BoardHeight; i++){
        for(int j = 0; j < BoardWidth; j++){
            coordsBoard[j][i] = NOShape;
        }
    }
}

//��ͣ
void TetrixBoard::pause(){
    if(!isStarted){
        return;
    }
    isPaused = !isPaused;
    if(isPaused){
        timer.stop();
    }else{
        timer.start(1000/2,this);
    }
    update();
}

bool TetrixBoard::tryMove(const TetrixPiece &newPiece, int newX, int newY){
    //newPiece ��const ���Σ���ֻ�ܷ���ʵ��const ���εĳ�Ա�������߷���
    //�Ƿ�Խ��
    if((newPiece.getHeight() + newY) > BoardHeight){
        return false;
    }else if((newPiece.getWidth() + newX) > BoardWidth || newX < 0){
        return false;
    }else{
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                //���ģ���е�ֵΪ�㣬������
                if(newPiece.value(i,j) == 0){
                    continue;
                }
                if(shapeAt(j - currentPiece.x() + newX , i - currentPiece.y() + newY) != NOShape){
                    return false;
                }
            }
        }
    }

    currentPiece = newPiece;
    curX = newX;
    curY = newY;
    update();
    return true;
}

void TetrixBoard::drawSquare(QPainter &painter, int x, int y, TetrixShape shape){
    QColor color = colorTable[(int)shape];
    //ʵ����ɫ��䷽��
    painter.fillRect(x + 1 , y + 1 , SquareWidth - 2, SquareHeight -2,color);
    //������ɫΪǳɫ
    painter.setPen(color.light());
    painter.drawLine(x , y + SquareHeight - 1 ,x , y);
    painter.drawLine(x , y , x + SquareWidth - 1 , y);
    painter.setPen(color.dark());
    //x + 1 y + 1�������Ѿ�����ǳɫ������ -1 ��ʾ�����ؿ��Ϊ1���������ڷ����ڲ�
    painter.drawLine(x + 1, y + SquareHeight - 1,x + SquareWidth - 1 , y + SquareHeight - 1);
    painter.drawLine(x + SquareWidth - 1 , y + SquareHeight -1 ,x + SquareWidth - 1, y + 1);

}

