#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
#define int long long
using ld=long double;
#define all(v) v.begin(),v.end()
const int INF=1e18;

// Problem Statement
/*
   Solve a maze from S to G using DFS and backtracking.
   X represents a wall and O represents an open cell.
*/

using Position=pair<int,int>;
struct Step{vector<string>grid;Position robot;string message;};
const array<Position,4>directions={Position{-1,0},Position{0,1},Position{1,0},Position{0,-1}};

bool inside(Position p,int n,int m){return p.first>=0&&p.first<n&&p.second>=0&&p.second<m;}

void printGrid(const vector<string>&grid,Position robot){
    for(auto&row:grid){
        for(auto&cell:row)cout<<cell<<" ";
        cout<<"\n";
    }
    cout<<"Robot position: ("<<robot.first<<", "<<robot.second<<")\n";
}

bool dfs(Position cur,Position dest,vector<string>&grid,vector<vector<int>>&vis,vector<Step>&steps){
    int n=grid.size(),m=grid[0].size();
    if(!inside(cur,n,m)||grid[cur.first][cur.second]=='X'||vis[cur.first][cur.second])return false;
    vis[cur.first][cur.second]=1;
    steps.push_back({grid,cur,"DFS visits this cell and marks it visited."});
    if(cur==dest){steps.push_back({grid,cur,"Goal reached. DFS returns through the call stack."});return true;}
    for(auto [dr,dc]:directions){
        Position nxt={cur.first+dr,cur.second+dc};
        if(!inside(nxt,n,m)||vis[nxt.first][nxt.second]||grid[nxt.first][nxt.second]=='X')continue;
        steps.push_back({grid,cur,"DFS moves forward to ("+to_string(nxt.first)+", "+to_string(nxt.second)+")."});
        if(dfs(nxt,dest,grid,vis,steps)){
            if(grid[cur.first][cur.second]!='S'&&grid[cur.first][cur.second]!='G')grid[cur.first][cur.second]='*';
            steps.push_back({grid,cur,"This cell is part of the solution path."});
            return true;
        }
        steps.push_back({grid,cur,"Dead end found; DFS backtracks to this cell."});
    }
    return false;
}

void solve(){
    int n,m;
    cout<<"=== DFS Maze Solving ===\n";
    cout<<"Enter rows and columns: ";
    if(!(cin>>n>>m)||n<=0||m<=0){cerr<<"Invalid maze dimensions.\n";return;}
    vector<string>maze(n);
    Position src{-1,-1},dest{-1,-1};
    cout<<"Enter the maze using S, G, X, and O:\n";
    for(int i=0;i<n;i++){
        cin>>maze[i];
        if((int)maze[i].size()!=m){cerr<<"Each row must contain exactly "<<m<<" characters.\n";return;}
        for(int j=0;j<m;j++){
            if(maze[i][j]=='S')src={i,j};
            else if(maze[i][j]=='G')dest={i,j};
            else if(maze[i][j]!='X'&&maze[i][j]!='O'){cerr<<"Invalid cell at ("<<i<<", "<<j<<"). Use S, G, X, or O.\n";return;}
        }
    }
    if(src.first==-1||dest.first==-1){cerr<<"The maze must contain one S and one G.\n";return;}
    vector<vector<int>>vis(n,vector<int>(m));
    vector<Step>steps{{maze,src,"Initial maze. DFS starts at S."}};
    bool found=dfs(src,dest,maze,vis,steps);
    cout<<"\nInitial maze:\n";printGrid(steps.front().grid,src);
    cout<<"\nStep-by-step DFS trace:\n";
    for(int i=1;i<(int)steps.size();i++){cout<<"\nStep "<<i<<": "<<steps[i].message<<"\n";printGrid(steps[i].grid,steps[i].robot);}
    if(found){cout<<"\nMaze solved. * marks the DFS path.\n";printGrid(maze,dest);}
    else cout<<"\nNo path exists from S to G.\n";
}

signed main(){
    ios_base::sync_with_stdio(0);cin.tie(0);
    int tt=1;
    for(int i=1;i<=tt;i++)solve();
}
