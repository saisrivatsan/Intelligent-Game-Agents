#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <Qtcore>
#include <QGridLayout>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //freopen("A.txt","w",stdout);


    setWindowTitle("Warfare Game");
    AssignUI();
    setValue();
    loadUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Given the location of the move, make suitable updates to the board
void MainWindow::makeMove(int status[6][6],int idx,int idy,int playNo)
{
    status[idx][idy] = playNo;

    //blitz checking
    int canBlitz = 0;
    if(idx+1<6)
        if(status[idx+1][idy]==playNo)
            canBlitz = 1;
    if(idx-1>=0)
        if(status[idx-1][idy]==playNo)
            canBlitz = 1;
    if(idy+1<6)
        if(status[idx][idy+1]==playNo)
            canBlitz = 1;
    if(idy-1>=0)
        if(status[idx][idy-1]==playNo)
            canBlitz = 1;

    //blitz assignment
    if(canBlitz==1)
    {
        if(idx+1<6)
            if(status[idx+1][idy]==-playNo)
                status[idx+1][idy]= playNo;

        if(idx-1>=0)
            if(status[idx-1][idy]== -playNo)
                status[idx-1][idy]= playNo;

        if(idy+1<6)
            if(status[idx][idy+1]== -playNo)
                status[idx][idy+1]= playNo;

        if(idy-1>=0)
            if(status[idx][idy-1]== -playNo)
                status[idx][idy-1]= playNo;
    }
}

//Detects user input. Call function for Human Moves
void MainWindow::detect()
{
    int idx,idy;
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            if(sender() == button[i][j])
               idx=i,idy=j;


    makeMove(status,idx,idy,turn);
    moves[turnCnt-1][0] = idx;
    moves[turnCnt-1][1] = idy;
    GUIupdate(idx,idy);
    //qDebug() << idx+1 <<" "<< (char)(idy+65);

    if(turn == 1)
    {
        ui->label_45->setText("Player 1\'s turn");
        ui->label_45->setStyleSheet("QLabel { background-color : blue}");
    }
    else
    {
        ui->label_45->setText("Player 2\'s turn");
        ui->label_45->setStyleSheet("QLabel { background-color : green}");
    }

    if(turnCnt>=37)
    {
        endGameStatus();
        return;
    }

    if( (turn == 1 && player1 == 1) || (turn == -1 && player2 == 1) )
        AIminimaxMove(turn);
    else if( (turn == 1 && player1 == 2) || (turn == -1 && player2 == 2) )
        AIABpruningMove(turn);

}

void MainWindow::endGameStatus()
{
    ui->label_45->setText("Game Over");
    ui->label_48->show();
    if(computeScore(status,1)>computeScore(status,-1))
    {
        ui->label_48->setText("Player 1 wins");
        ui->label_48->setStyleSheet("QLabel { background-color : blue}");
    }
    else if(computeScore(status,1)==computeScore(status,-1))
    {
        ui->label_48->setText("Tie");
        ui->label_48->setStyleSheet("QLabel { background-color : yellow}");
    }
    else
    {
        ui->label_48->setText("Player 2 wins");
        ui->label_48->setStyleSheet("QLabel { background-color : green}");
    }

    ui->label_45->setStyleSheet("QLabel { background-color : red}");
    isGameOn = 0;
    ui->pushButton_37->setText("New Game");
    log();
}

//Updates and Makes relelvant GUI changes
void MainWindow::GUIupdate(int idx,int idy)
{
    button[idx][idy]->hide();
    lbl[idx][idy]->show();
    displayBoard(status);
    turn = -turn;
    ui->lcdNumber->display(computeScore(status,1));
    ui->lcdNumber_2->display(computeScore(status,-1));
    turnCnt++;


}

//Given the status, display the board
void MainWindow::displayBoard(int status[6][6])
{
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
        {
            if(status[i][j]==1)
                lbl[i][j]->setStyleSheet("QLabel { background-color : blue}");
            else
                lbl[i][j]->setStyleSheet("QLabel { background-color : green}");
        }
}

//Computes score
int MainWindow::computeScore(int status[6][6],int playNo)
{
    int score = 0;
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            if(status[i][j]==playNo)
                score+=board[i][j];

    return score;
}

// Plays Minimax Move for playNo
void MainWindow::AIminimaxMove(int playNo)
{

    QElapsedTimer timer;
    timer.start();
    minimax(status,0,turnCnt,turn,playNo);
    makeMove(status,optIdx,optIdy,playNo);
    moves[turnCnt-1][0] = optIdx;
    moves[turnCnt-1][1] = optIdy;
    timeElapsed[turnCnt-1] = timer.elapsed();
    GUIupdate(optIdx,optIdy);

    if(turn == 1)
    {
        ui->label_45->setStyleSheet("QLabel { background-color : blue}");
        ui->label_45->setText("Player 1\'s turn");
    }
    else
    {
        ui->label_45->setStyleSheet("QLabel { background-color : green}");
        ui->label_45->setText("Player 2\'s turn");
    }

    if(turnCnt>=37)
    {
        endGameStatus();
        return;
    }


    if((turn==1 && player1==1)||(turn == -1 && player2==1))
            AIminimaxMove(turn);
    else if((turn==1 && player1==2)||(turn == -1 && player2==2))
            AIABpruningMove(turn);
}

// Sets the value for best minimax move
int MainWindow::minimax(int status[6][6],int depth,int turnCnt,int turn,int playNo)
{
    nodeExpaned[MainWindow::turnCnt-1]++;
    if(turnCnt==37 || depth == MAXdepth)
    {

        return computeScore(status,playNo)-computeScore(status,-playNo);
    }


    int i,j,k,l,Best;

    if(turn==playNo)
        Best = -10000;
    else
        Best = 10000;

    for(i=0;i<6;i++)
    {
        for(j=0;j<6;j++)
        {
            if(status[i][j]==0)
            {
                int temp[6][6];
                for(k=0;k<6;k++)
                    for(l=0;l<6;l++)
                        temp[k][l]=status[k][l];

                makeMove(temp,i,j,turn);
                int val = minimax(temp,depth+1,turnCnt+1,-turn,playNo);
                if(turn==playNo && val>Best)
                        Best = val;
                else if(turn!=playNo && val<Best)
                        Best = val;


                if(depth==0 && val == Best)
                    optIdx = i,optIdy = j;

            }
        }
    }

    return Best;
}

//Make move using alpha-beta pruning for playNo
void MainWindow::AIABpruningMove(int playNo)
{

    //qDebug() << turnCnt;
    QElapsedTimer timer;
    timer.start();
    ABpruning(status,0,turnCnt,turn,playNo,-10000,10000);
    makeMove(status,optIdx,optIdy,playNo);
    moves[turnCnt-1][0] = optIdx;
    moves[turnCnt-1][1] = optIdy;
    timeElapsed[turnCnt-1] = timer.elapsed();
    GUIupdate(optIdx,optIdy);

    if(turn == 1)
    {
        ui->label_45->setText("Player 1\'s turn");
        ui->label_45->setStyleSheet("QLabel { background-color : blue}");
    }
    else
    {
        ui->label_45->setText("Player 2\'s turn");
        ui->label_45->setStyleSheet("QLabel { background-color : green}");
    }

    if(turnCnt>=37)
    {
        endGameStatus();
        return;
    }

    if((turn==1 && player1==1)||(turn == -1 && player2==1))
            AIminimaxMove(turn);
    else if((turn==1 && player1==2)||(turn == -1 && player2==2))
            AIABpruningMove(turn);

}

//Determines optimal move using alpha-beta pruning
int MainWindow::ABpruning(int status[6][6],int depth,int turnCnt,int turn,int playNo,int alpha,int beta)
{
    nodeExpaned[MainWindow::turnCnt-1]++;
    if(turnCnt==37 || depth == MAXdepth)
    {
        //nodeExpaned[MainWindow::turnCnt-1]++;
        return computeScore(status,playNo)-computeScore(status,-playNo);
    }
    int i,j,k,l;

    if(turn==playNo)
    {
        for(i=0;i<6;i++)
        {
            for(j=0;j<6;j++)
            {
                if(status[i][j]==0)
                {
                    int temp[6][6];
                    for(k=0;k<6;k++)
                        for(l=0;l<6;l++)
                            temp[k][l]=status[k][l];

                    makeMove(temp,i,j,turn);
                    int val = ABpruning(temp,depth+1,turnCnt+1,-turn,playNo,alpha,beta);
                    if(val>alpha)
                    {
                        alpha = val;
                        if(depth==0)
                            optIdx = i,optIdy = j;
                    }

                    if(beta<=alpha)
                        return alpha;
                }
            }
        }

        return alpha;
    }

    else
    {
        for(i=0;i<6;i++)
        {
            for(j=0;j<6;j++)
            {
                if(status[i][j]==0)
                {
                    int temp[6][6];
                    for(k=0;k<6;k++)
                        for(l=0;l<6;l++)
                            temp[k][l]=status[k][l];

                    makeMove(temp,i,j,turn);
                    int val = ABpruning(temp,depth+1,turnCnt+1,-turn,playNo,alpha,beta);
                    if(val<beta)
                    {
                        beta = val;
                        if(depth==0)
                            optIdx = i,optIdy = j;
                    }

                    if(beta<=alpha)
                        return beta;
                }
            }
        }

        return beta;
    }
}

//GUI Functions

// Assigns array of buttons and labels to UIwidgets
void MainWindow::AssignUI()
{
    //Buttons on Board
    button[0][0] = ui->pushButton;   button[2][0] = ui->pushButton_13;button[4][0] = ui->pushButton_25;
    button[0][1] = ui->pushButton_2; button[2][1] = ui->pushButton_14;button[4][1] = ui->pushButton_26;
    button[0][2] = ui->pushButton_3; button[2][2] = ui->pushButton_15;button[4][2] = ui->pushButton_27;
    button[0][3] = ui->pushButton_4; button[2][3] = ui->pushButton_16;button[4][3] = ui->pushButton_28;
    button[0][4] = ui->pushButton_5; button[2][4] = ui->pushButton_17;button[4][4] = ui->pushButton_29;
    button[0][5] = ui->pushButton_6; button[2][5] = ui->pushButton_18;button[4][5] = ui->pushButton_30;
    button[1][0] = ui->pushButton_7; button[3][0] = ui->pushButton_19;button[5][0] = ui->pushButton_31;
    button[1][1] = ui->pushButton_8; button[3][1] = ui->pushButton_20;button[5][1] = ui->pushButton_32;
    button[1][2] = ui->pushButton_9; button[3][2] = ui->pushButton_21;button[5][2] = ui->pushButton_33;
    button[1][3] = ui->pushButton_10;button[3][3] = ui->pushButton_22;button[5][3] = ui->pushButton_34;
    button[1][4] = ui->pushButton_11;button[3][4] = ui->pushButton_23;button[5][4] = ui->pushButton_35;
    button[1][5] = ui->pushButton_12;button[3][5] = ui->pushButton_24;button[5][5] = ui->pushButton_36;

    //Labels on Board
    lbl[0][0] = ui->label;   lbl[2][0] = ui->label_13;lbl[4][0] = ui->label_25;
    lbl[0][1] = ui->label_2; lbl[2][1] = ui->label_14;lbl[4][1] = ui->label_26;
    lbl[0][2] = ui->label_3; lbl[2][2] = ui->label_15;lbl[4][2] = ui->label_27;
    lbl[0][3] = ui->label_4; lbl[2][3] = ui->label_16;lbl[4][3] = ui->label_28;
    lbl[0][4] = ui->label_5; lbl[2][4] = ui->label_17;lbl[4][4] = ui->label_29;
    lbl[0][5] = ui->label_6; lbl[2][5] = ui->label_18;lbl[4][5] = ui->label_30;
    lbl[1][0] = ui->label_7; lbl[3][0] = ui->label_19;lbl[5][0] = ui->label_31;
    lbl[1][1] = ui->label_8; lbl[3][1] = ui->label_20;lbl[5][1] = ui->label_32;
    lbl[1][2] = ui->label_9; lbl[3][2] = ui->label_21;lbl[5][2] = ui->label_33;
    lbl[1][3] = ui->label_10;lbl[3][3] = ui->label_22;lbl[5][3] = ui->label_34;
    lbl[1][4] = ui->label_11;lbl[3][4] = ui->label_23;lbl[5][4] = ui->label_35;
    lbl[1][5] = ui->label_12;lbl[3][5] = ui->label_24;lbl[5][5] = ui->label_36;
}

void MainWindow::on_pushButton_37_clicked()
{
    setReset();
}

void MainWindow::reset()
{
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
        {
            button[i][j]->hide();
            lbl[i][j]->show();
            lbl[i][j]->setStyleSheet("QLabel { background-color : yellow}");
            status[i][j] = 0;
        }
    turn = 1;
    turnCnt = 1;
    isGameOn = 0;
    ui->pushButton_37->setText("Start Game");
    ui->lcdNumber->display(0);
    ui->lcdNumber_2->display(0);
    ui->label_48->hide();
    ui->label_45->setText("Begin Warfare");
}

void MainWindow::loadUI()
{
    boardInit(0);

    // Load Buttons
    for(int i=0;i<6;i++)
        for(int j = 0;j<6;j++)
            {
                //board[i][j] = 1;
                button[i][j]->setGeometry(j*55+55,i*55+55,50,50);
                QString text = QString("%1").arg(board[i][j]);
                button[i][j]->setText(text);
                button[i][j]->hide();
            }

    // Load Labels
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
        {
            lbl[i][j]->setGeometry(j*55+55,i*55+55,50,50);
            QString text = QString("%1").arg(board[i][j]);
            lbl[i][j]->setText(text);
            lbl[i][j]->setAlignment(Qt::AlignCenter);
            lbl[i][j]->setStyleSheet("QLabel { background-color : yellow}");
        }

    //Connect Buttons with SLOTS
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            connect(button[i][j], SIGNAL(released()), this, SLOT(detect()));

   //Board Label
    ui->label_39->setGeometry(55*8,55*1,150,25);
    ui->label_39->setText("Select Board");


   //Player 1 select
    ui->label_42->setGeometry(55*8,55*2,150,25);
    ui->label_42->setText("Select Player 1");

   //Player 2 select
    ui->label_43->setGeometry(55*8,55*3,150,25);
    ui->label_43->setText("Select Player 2");

    //Player 2 select
    ui->label_46->setGeometry(55*8,55*4,150,25);
    ui->label_46->setText("Select Difficulty");

    //Design
    ui->label_38->setGeometry(55*8-20,55*1,15,50);
    ui->label_38->setStyleSheet("QLabel { background-color : red}");
    ui->label_38->setText("");
    ui->label_40->setGeometry(55*8-20,55*2,15,50);
    ui->label_40->setStyleSheet("QLabel { background-color : blue}");
    ui->label_40->setText("");
    ui->label_41->setGeometry(55*8-20,55*3,15,50);
    ui->label_41->setStyleSheet("QLabel { background-color : green}");
    ui->label_41->setText("");
    ui->label_37->setGeometry(55*8-20,55*5,15,50);
    ui->label_37->setStyleSheet("QLabel { background-color : blue}");
    ui->label_37->setText("");
    ui->label_44->setGeometry(55*8-20,55*6,15,50);
    ui->label_44->setStyleSheet("QLabel { background-color : green}");
    ui->label_44->setText("");

    ui->label_47->setGeometry(55*8-20,55*4,15,50);
    ui->label_47->setStyleSheet("QLabel { background-color : red}");
    ui->label_47->setText("");

    ui->label_48->setGeometry(55*8-20,55*7,170,105);
    ui->label_48->setStyleSheet("QLabel { background-color : yellow}");
    ui->label_48->setAlignment(Qt::AlignCenter);
    ui->label_48->hide();


    // LCD score boards
    ui->lcdNumber->setGeometry(55*8,55*5,150,50);
    ui->lcdNumber_2->setGeometry(55*8,55*6,150,50);

    // Start Button
    ui->pushButton_37->setGeometry(55*1,55*7,160,50);
    ui->pushButton_37->setText("Start Game");

    //Auto Button
    ui->pushButton_38->setGeometry(55*1,55*8,160,50);
    ui->pushButton_38->setText("RunAllCombo");

    //Quit Button
    ui->pushButton_39->setGeometry(55*4,55*8,160,50);
    ui->pushButton_39->setText("Quit");

    // Combo Box 1
    ui->comboBox->setGeometry(55*8,55*1+25,150,25);
    ui->comboBox->addItem("Keren");
    ui->comboBox->addItem("Narvik");
    ui->comboBox->addItem("Sevastopol");
    ui->comboBox->addItem("Smolensk");
    ui->comboBox->addItem("Westerplatte");
    ui->comboBox->addItem("Random");

    // Combo Box 2
    ui->comboBox_2->setGeometry(55*8,55*2+25,150,25);
    ui->comboBox_2->addItem("Human");
    ui->comboBox_2->addItem("Minimax");
    ui->comboBox_2->addItem("A-B Pruning");

    //Combo Box 3
    ui->comboBox_3->setGeometry(55*8,55*3+25,150,25);
    ui->comboBox_3->addItem("Human");
    ui->comboBox_3->addItem("Minimax");
    ui->comboBox_3->addItem("A-B Pruning");

    //Slider
    ui->horizontalSlider->setGeometry(55*8,55*4+25,150,25);
    ui->horizontalSlider->setRange(1,5);
    ui->horizontalSlider->setValue(MAXdepth);

    //status Label
    ui->label_45->setGeometry(55*4,55*7,160,50);
    ui->label_45->setAlignment(Qt::AlignCenter);
    ui->label_45->setStyleSheet("QLabel { background-color : red}");
    ui->label_45->setText("Begin Warfare");




    difficultySlider();



}

void MainWindow::setValue()
{
    //boardInit();

    for(int i=0;i<36;i++)
        timeElapsed[i]=0,nodeExpaned[i]=0;

    //Set Status Values
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            status[i][j] = 0;

    turn = 1;
    turnCnt = 1;
    ui->lcdNumber->display(0);
    ui->lcdNumber->display(0);

}

void MainWindow::boardInit(int t)
{

     int board1[6][6] = { 1,1,1,1,1,1,
     1,1,1,1,1,1,
     1,1,1,1,1,1,
     1,1,1,1,1,1,
     1,1,1,1,1,1,
     1,1,1,1,1,1};

     int board2[6][6] = {99,1,99,1,99,1,
     1,99,1,99,1,99,
     99,1,99,1,99,1,
     1,99,1,99,1,99,
     99,1,99,1,99,1,
     1,99,1,99,1,99};

     int board3[6][6]={1,1,1,1,1,1,
     2,2,2,2,2,2,
     4,4,4,4,4,4,
     8,8,8,8,8,8,
     16,16,16,16,16,16,
     32,32,32,32,32,32};


     int board4[6][6]={66,76,28,66,11,9,
     31,39,50,8,33,14,
     80,76,39,59,2,48,
     50,73,43,3,13,3,
     99,45,72,87,49,4,
     80,63,92,28,61,53};

     int board5[6][6]={1,1,1,1,1,1,
     1,3,4,4,3,1,
     1,4,2,2,4,1,
     1,4,2,2,4,1,
     1,3,4,4,3,1,
     1,1,1,1,1,1 };



    if(t==0)
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                board[i][j] = board1[i][j];
    if(t==1)
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                board[i][j] = board2[i][j];
    if(t==2)
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                board[i][j] = board3[i][j];
    if(t==3)
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                board[i][j] = board4[i][j];
    if(t==4)
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                board[i][j] = board5[i][j];

    if(t==5)
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                board[i][j] = qrand()%1000;

    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
        {
            QString text = QString("%1").arg(board[i][j]);
            lbl[i][j]->setText(text);
            button[i][j]->setText(text);
        }

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{

    boardInit(index);
    reset();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    player1 = index;
    difficultySlider();

}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    player2 = index;
    difficultySlider();

}

void MainWindow::startGame()
{

    if(player1==1)
        AIminimaxMove(turn);
    if(player1==2)
        AIABpruningMove(turn);



}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    MAXdepth = position;
}

void MainWindow::difficultySlider()
{
    if(player1 + player2 == 0)
    {
        ui->horizontalSlider->hide();
        ui->label_47->hide();
        ui->label_46->hide();
    }
    else if((player1==0 && player2==2 )||(player1==2 && player2==0 ))
    {
        ui->horizontalSlider->show();
        ui->label_47->show();
        ui->label_46->show();
        ui->horizontalSlider->setRange(1,7);
     }
    else
    {
        ui->horizontalSlider->show();
        if(ui->horizontalSlider->value()>5)
            ui->horizontalSlider->setValue(3);
        ui->horizontalSlider->setRange(1,5);
        ui->label_47->show();
        ui->label_46->show();

    }
}

void MainWindow::setReset()
{

    if(isGameOn == 0)
    {
       reset();

        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
            {
                button[i][j]->show();
                lbl[i][j]->hide();
                ui->label_45->show();
                ui->label_45->setText("Player 1\'s turn");
                ui->label_45->setStyleSheet("QLabel { background-color : blue}");
            }

        ui->pushButton_37->setText("Reset");
        isGameOn = 1;

        startGame();
    }

    else
        reset();
}

void MainWindow::log()
{
    QString text = QString("B_%1 D_%2 G_%3 vs %4.txt").arg(ui->comboBox->currentText()).arg(MAXdepth+1).arg(ui->comboBox_2->currentText()).arg(ui->comboBox_3->currentText());
    qDebug() << text;
    freopen(text.toLocal8Bit().data(),"w",stdout);

    cout<<"Board: "<<ui->comboBox->currentText().toStdString()<<"\n";
    cout<<ui->comboBox_2->currentText().toStdString()<<" vs "<<ui->comboBox_3->currentText().toStdString()<<"\n";
    cout<<"Depth: "<<MAXdepth+1<<"\n";

    cout<<"-------------------------------------\n";
    cout<<"Move Sequence\n";
    cout<<"-------------------------------------\n";
    for(int i=0;i<36;i++)
        if(i%2)
            cout<<"Green : "<<(char)(65+moves[i][1])<<moves[i][0]+1<<"\n";
        else
            cout<<"Blue : "<<(char)(65+moves[i][1])<<moves[i][0]+1<<"\n";
    cout<<"\n\n";

    cout<<"-------------------------------------\n";
    cout<<"Board state\n";
    cout<<"-------------------------------------\n";
    for(int i=0;i<6&&cout<<"\n";i++)
        for(int j=0;j<6;j++)
            if(status[i][j]==+1)
                cout<<"B ";
            else
                cout<<"G ";
    cout<<"\n\n";

    cout<<"-------------------------------------\n";
    cout<<"Scores And Status\n";
    cout<<"-------------------------------------\n";
    cout<<"BLUE :"<<computeScore(status,1)<<"\n";
    cout<<"GREEN :"<<computeScore(status,-1)<<"\n";
    if(computeScore(status,1)>computeScore(status,-1))
        cout<<"BLUE WINS\n";
    else if(computeScore(status,1)==computeScore(status,-1))
        cout<<"TIE\n";
    else
        cout<<"GREEN WINS\n";
    cout<<"\n\n";

    cout<<"-------------------------------------\n";
    cout<<"Nodes Expanded\n";
    cout<<"-------------------------------------\n";
    cout<<"BLUE :\n";
    //cout<<"Nodes Expanded on the ith move:\n";
    long long int tot=0;
    for(int i=0;i<36;i+=2)
    {
        cout<<nodeExpaned[i]<<" ";
        tot +=nodeExpaned[i];
    }
    cout<<"\n";
    cout<<"Total :"<<tot<<"\n";
    cout<<"Average :"<<tot/18<<"\n\n";
    cout<<"GREEN :\n";
    tot=0;
    //cout<<"Nodes Expanded on the ith move:\n";
    for(int i=1;i<36;i+=2)
    {
        cout<<nodeExpaned[i]<<" ";
        tot+=nodeExpaned[i];
    }
    cout<<"\n";
    cout<<"Total :"<<tot<<"\n";
    cout<<"Average :"<<tot/18<<"\n";
    cout<<"\n\n";

    cout<<"-------------------------------------\n";
    cout<<"Time Elapsed (in ms)\n";
    cout<<"-------------------------------------\n";
    cout<<"BLUE :\n";
    //cout<<"Time Elapsed on the ith move:\n";
    tot=0;
    for(int i=0;i<36;i+=2)
    {
        //cout<<timeElapsed[i]<<" ";
        tot +=timeElapsed[i];
    }
    cout<<"\n";
    cout<<"Total :"<<tot<<"\n";
    cout<<"Average :"<<tot/18<<"\n\n";
    cout<<"GREEN :\n";
    tot=0;
    //cout<<"Time Elapsed on the ith move:\n";
    for(int i=1;i<36;i+=2)
    {
        //cout<<timeElapsed[i]<<" ";
        tot +=timeElapsed[i];
    }
    cout<<"\n";
    cout<<"Total :"<<tot<<"\n";
    cout<<"Average :"<<tot/18<<"\n";

    fclose (stdout);
}

void MainWindow::on_pushButton_38_clicked()
{
    int i,j,k;
    for(MAXdepth=2;MAXdepth<7;MAXdepth++)
        for(i=0;i<5;i++)
            for(j=1;j<=2;j++)
                for(k=1;k<=2;k++)
                {
                    if(MAXdepth>=5 && (j==1 || k==1))
                        continue;

                    ui->comboBox->setCurrentIndex(i);
                    ui->comboBox_2->setCurrentIndex(j);
                    ui->comboBox_3->setCurrentIndex(k);
                    setValue();
                    setReset();
                   // setReset();
                }
}

void MainWindow::on_pushButton_39_clicked()
{
    close();

}
