#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QPushButton* button[6][6];
    QLabel* lbl[6][6];
    int status[6][6];
    int turn = 1;
    int board[6][6];
    int MAXdepth=3;
    int optIdx,optIdy;
    int turnCnt = 1;
    int isGameOn = 0;
    int player1 = 0;
    int player2 = 0;
    int moves[36][2];
    long long int nodeExpaned[36];
    int timeElapsed[36];
    QFile *file;


    void log();
    void AssignUI();
    void displayBoard(int status[6][6]);
    void makeMove(int status[6][6],int idx,int idy,int playNo);
    void GUIupdate(int idx,int idy);
    void AIminimaxMove(int playNo);
    void AIABpruningMove(int playNo);
    void setValue();
    void loadUI();
    void startGame();
    void boardInit(int t);
    void setReset();
    void reset();
    int minimax(int status[6][6],int depth,int turnCnt,int turn,int playNo);
    int ABpruning(int status[6][6],int depth,int turnCnt,int turn,int playNo,int alpha,int beta);
    int computeScore(int status[6][6], int playNo);
    void endGameStatus();
    void difficultySlider();
    ~MainWindow();

private slots:
    void detect();


    void on_pushButton_37_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_horizontalSlider_sliderMoved(int position);


    void on_pushButton_38_clicked();

    void on_pushButton_39_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
