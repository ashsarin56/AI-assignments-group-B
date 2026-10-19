#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
#define int long long
using ld=long double;
#define all(v) v.begin(),v.end()
const int INF=1e18;

// Problem Statement
/*
   Develop a game-playing agent using Minimax for Tic-Tac-Toe.
   The AI plays X and the human player plays O.
*/

vector<char>board(9,' ');

void printBoard(){
    cout<<"\n";
    for(int i=0;i<9;i++){
        cout<<" "<<(board[i]==' '?char('1'+i):board[i])<<" ";
        if(i%3!=2)cout<<"|";
        if(i%3==2&&i!=8)cout<<"\n---+---+---\n";
    }
    cout<<"\n\n";
}

bool winner(char player){
    vector<vector<int>>lines={
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for(auto&line:lines)
        if(board[line[0]]==player&&board[line[1]]==player&&board[line[2]]==player)return true;
    return false;
}

bool full(){
    return all_of(all(board),[](char cell){return cell!=' ';});
}

int minimax(bool maximizing,int depth){
    if(winner('X'))return 10-depth;
    if(winner('O'))return depth-10;
    if(full())return 0;
    if(maximizing){
        int best=-INF;
        for(int i=0;i<9;i++){
            if(board[i]!=' ')continue;
            board[i]='X';
            best=max(best,minimax(false,depth+1));
            board[i]=' ';
        }
        return best;
    }
    int best=INF;
    for(int i=0;i<9;i++){
        if(board[i]!=' ')continue;
        board[i]='O';
        best=min(best,minimax(true,depth+1));
        board[i]=' ';
    }
    return best;
}

int bestMove(){
    int score=-INF,move=-1;
    for(int i=0;i<9;i++){
        if(board[i]!=' ')continue;
        board[i]='X';
        int current=minimax(false,0);
        board[i]=' ';
        if(current>score){score=current;move=i;}
    }
    return move;
}

void solve(){
    cout<<"=== Tic-Tac-Toe using Minimax ===\n";
    cout<<"AI: X | Human: O\n";
    cout<<"Choose a position from 1 to 9 as shown on the board.\n";
    printBoard();
    while(true){
        int choice;
        cout<<"Enter your move (1-9): ";
        if(!(cin>>choice)||choice<1||choice>9||board[choice-1]!=' '){
            cout<<"Invalid move. Choose an empty position from 1 to 9.\n";
            cin.clear();
            cin.ignore(10000,'\n');
            continue;
        }
        board[choice-1]='O';
        printBoard();
        if(winner('O')){cout<<"You win! The AI could not find a winning move.\n";break;}
        if(full()){cout<<"The game is a draw.\n";break;}
        int move=bestMove();
        board[move]='X';
        cout<<"AI chooses position "<<move+1<<" using Minimax.\n";
        printBoard();
        if(winner('X')){cout<<"AI wins! Minimax found the best move.\n";break;}
        if(full()){cout<<"The game is a draw.\n";break;}
    }
}

signed main(){
    ios_base::sync_with_stdio(0);cin.tie(0);
    int tt=1;
    for(int i=1;i<=tt;i++)solve();
}
