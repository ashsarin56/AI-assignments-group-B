#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
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
   Find the shortest path from S to G in a grid using BFS.
   X represents an obstacle and O represents a free cell.
*/

using Position=pair<int,int>;
struct Step{vector<string>grid;Position robot;string message;};
const array<Position,4>directions={Position{-1,0},Position{0,1},Position{1,0},Position{0,-1}};

bool inside(int r,int c,int n,int m){return r>=0&&r<n&&c>=0&&c<m;}

void printGrid(const vector<string>&grid,Position robot){
    for(auto&row:grid){
        for(auto&cell:row)cout<<cell<<" ";
        cout<<"\n";
    }
    cout<<"Robot position: ("<<robot.first<<", "<<robot.second<<")\n";
}

void solve(){
    int n,m;
    cout<<"=== BFS Robot Path Planning ===\n";
    cout<<"Enter rows and columns: ";
    if(!(cin>>n>>m)||n<=0||m<=0){cerr<<"Invalid grid dimensions.\n";return;}
    vector<string>grid(n);
    Position src{-1,-1},dest{-1,-1};
    cout<<"Enter the grid using S, G, X, and O:\n";
    for(int i=0;i<n;i++){
        cin>>grid[i];
        if((int)grid[i].size()!=m){cerr<<"Each row must contain exactly "<<m<<" characters.\n";return;}
        for(int j=0;j<m;j++){
            if(grid[i][j]=='S')src={i,j};
            else if(grid[i][j]=='G')dest={i,j};
            else if(grid[i][j]!='X'&&grid[i][j]!='O'){cerr<<"Invalid cell at ("<<i<<", "<<j<<"). Use S, G, X, or O.\n";return;}
        }
    }
    if(src.first==-1||dest.first==-1){cerr<<"The grid must contain exactly one S and one G.\n";return;}
    queue<Position>q;
    vector<vector<int>>vis(n,vector<int>(m)),parr(n,vector<int>(m,-1)),parc(n,vector<int>(m,-1));
    vector<Step>steps;
    q.push(src);vis[src.first][src.second]=1;
    steps.push_back({grid,src,"Initial state: start position added to the queue."});
    bool found=false;
    while(!q.empty()){
        auto cur=q.front();q.pop();
        if(cur==dest){steps.push_back({grid,cur,"Goal reached. BFS stops because the shortest path was found."});found=true;break;}
        steps.push_back({grid,cur,"Exploring the next cell from the front of the queue."});
        for(auto [dr,dc]:directions){
            int nr=cur.first+dr,nc=cur.second+dc;
            if(!inside(nr,nc,n,m)||vis[nr][nc]||grid[nr][nc]=='X')continue;
            vis[nr][nc]=1;parr[nr][nc]=cur.first;parc[nr][nc]=cur.second;q.push({nr,nc});
            steps.push_back({grid,{nr,nc},"Discovered ("+to_string(nr)+", "+to_string(nc)+") and added it to the queue."});
        }
    }
    vector<Position>path;
    if(found){
        for(Position cur=dest;cur.first!=-1;cur={parr[cur.first][cur.second],parc[cur.first][cur.second]})path.push_back(cur);
        reverse(all(path));
        for(auto [r,c]:path)if(grid[r][c]!='S'&&grid[r][c]!='G')grid[r][c]='*';
        steps.push_back({grid,dest,"Shortest path highlighted with * (each move has equal cost)."});
    }
    cout<<"\nInitial grid:\n";printGrid(steps.front().grid,src);
    cout<<"\nStep-by-step BFS trace:\n";
    for(int i=1;i<(int)steps.size();i++){cout<<"\nStep "<<i<<": "<<steps[i].message<<"\n";printGrid(steps[i].grid,steps[i].robot);}
    if(!found)cout<<"\nNo path exists from S to G.\n";
    else{cout<<"\nShortest path length: "<<path.size()-1<<" move(s)\n";cout<<"Final grid (* marks the shortest path):\n";printGrid(grid,dest);}
}

signed main(){
    ios_base::sync_with_stdio(0);cin.tie(0);
    int tt=1;
    for(int i=1;i<=tt;i++)solve();
}
