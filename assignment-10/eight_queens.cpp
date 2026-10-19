#include <iostream>
#include <vector>
using namespace std;
#define int long long
using ld=long double;
#define all(v) v.begin(),v.end()
const int INF=1e18;

// Problem Statement
/*
   Place eight queens on an 8*8 chessboard such that no two queens
   attack each other using backtracking search.
*/

int n=8,totalSolutions=0,totalBacktracks=0,firstBacktracks=0;
int rejected=0,placed=0,positions=0;
bool firstSolutionFound=false;
vector<int>queen,firstSolution,colUsed,diag1,diag2;

bool safe(int row,int c){
    return !colUsed[c]&&!diag1[row+c]&&!diag2[row-c+n-1];
}

void backtrack(int row){
    if(row==n){
        totalSolutions++;
        if(!firstSolutionFound){
            firstSolution=queen;
            firstSolutionFound=true;
            firstBacktracks=totalBacktracks;
        }
        return;
    }
    for(int c=0;c<n;c++){
        positions++;
        if(!safe(row,c)){rejected++;continue;}
        placed++;
        queen[row]=c;colUsed[c]=diag1[row+c]=diag2[row-c+n-1]=1;
        backtrack(row+1);
        queen[row]=-1;colUsed[c]=diag1[row+c]=diag2[row-c+n-1]=0;
        totalBacktracks++;
    }
}

void printBoard(){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)cout<<(firstSolution[i]==j?'Q':'.')<<" ";
        cout<<"\n";
    }
}

void solve(){
    queen.assign(n,-1);firstSolution.assign(n,-1);
    colUsed.assign(n,0);diag1.assign(2*n-1,0);diag2.assign(2*n-1,0);
    backtrack(0);
    cout<<"=== Backtracking Solution for 8-Queens ===\n\n";
    cout<<"1. One valid board solution:\n";
    printBoard();
    cout<<"\n2. Backtracking steps for this solution: "<<firstBacktracks<<"\n";
    cout<<"3. Total solutions found: "<<totalSolutions<<"\n";
    cout<<"4. Total number of backtracks: "<<totalBacktracks<<"\n";
    cout<<"5. Total rejected positions: "<<rejected<<"\n";
    cout<<"6. Total queens placed: "<<placed<<"\n";
    cout<<"7. Total positions examined: "<<positions<<"\n";
}

signed main(){
    ios_base::sync_with_stdio(0);cin.tie(0);
    int tt=1;
    for(int i=1;i<=tt;i++)solve();
}
