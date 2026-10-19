#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
#define int long long
#define all(v) v.begin(),v.end()
// Problem Statement
/*
   Build a simple rule-based chatbot using predefined facts and rules.
   The chatbot remembers statements, derives new facts, and answers
   questions using pattern matching and forward chaining.
*/

struct Fact{
    string subject,predicate;
};

struct Rule{
    string condition,conclusion;
};

string lower(string s){
    transform(all(s),s.begin(),[](unsigned char c){return tolower(c);});
    return s;
}

string trim(string s){
    while(!s.empty()&&isspace(s.front()))s.erase(s.begin());
    while(!s.empty()&&isspace(s.back()))s.pop_back();
    return s;
}

bool same(const Fact&a,const Fact&b){
    return a.subject==b.subject&&a.predicate==b.predicate;
}

bool contains(const vector<Fact>&facts,const Fact&fact){
    return any_of(all(facts),[&](const Fact&x){return same(x,fact);});
}

void addFact(vector<Fact>&facts,Fact fact){
    fact.subject=lower(trim(fact.subject));
    fact.predicate=lower(trim(fact.predicate));
    if(!fact.subject.empty()&&!fact.predicate.empty()&&!contains(facts,fact))facts.push_back(fact);
}

bool readFact(string sentence,Fact&fact){
    sentence=lower(trim(sentence));
    if(!sentence.empty()&&sentence.back()=='.')sentence.pop_back();
    string marker=" is ";
    auto pos=sentence.find(marker);
    if(pos==string::npos)return false;
    fact={trim(sentence.substr(0,pos)),trim(sentence.substr(pos+marker.size()))};
    return !fact.subject.empty()&&!fact.predicate.empty();
}

bool isQuestion(string sentence,string&subject,string&predicate){
    sentence=lower(trim(sentence));
    if(!sentence.empty()&&sentence.back()=='?')sentence.pop_back();
    string prefix="is ";
    if(sentence.rfind(prefix,0)!=0)return false;
    sentence=trim(sentence.substr(prefix.size()));
    string marker=" a ";
    auto pos=sentence.find(marker);
    if(pos==string::npos)marker=" an ",pos=sentence.find(marker);
    if(pos==string::npos)return false;
    subject=trim(sentence.substr(0,pos));
    predicate=trim(sentence.substr(pos+marker.size()));
    return !subject.empty()&&!predicate.empty();
}

void infer(vector<Fact>&facts,const vector<Rule>&rules){
    bool changed=true;
    while(changed){
        changed=false;
        for(auto&rule:rules){
            int currentSize=facts.size();
            for(int i=0;i<currentSize;i++){
                auto fact=facts[i];
                if(fact.predicate!=rule.condition)continue;
                Fact result{fact.subject,rule.conclusion};
                if(!contains(facts,result)){facts.push_back(result);changed=true;}
            }
        }
    }
}

string answer(string input,vector<Fact>&facts,const vector<Rule>&rules){
    string subject,predicate;
    if(isQuestion(input,subject,predicate)){
        Fact wanted{subject,predicate};
        if(contains(facts,wanted))return "Yes, "+subject+" is "+predicate+".";
        vector<Fact>derived=facts;
        infer(derived,rules);
        for(auto&rule:rules){
            if(rule.conclusion==predicate){
                Fact condition{subject,rule.condition};
                if(contains(derived,condition))
                    return "Yes, "+subject+" might be "+predicate+" because it is "+rule.condition+".";
            }
        }
        return "I cannot conclude that "+subject+" is "+predicate+" from my current knowledge.";
    }
    Fact fact;
    if(readFact(input,fact)){
        addFact(facts,fact);
        return "I learned that "+fact.subject+" is "+fact.predicate+".";
    }
    return "Please use a statement such as 'apple is sweet' or a question such as 'is apple a fruit?'.";
}

void solve(){
    vector<Fact>facts={
        {"apple","sweet"},{"banana","sweet"},{"carrot","healthy"},
        {"rose","beautiful"},{"dog","loyal"},{"dog","animal"},
        {"cat","animal"},{"water","liquid"},{"ice","cold"},{"sun","hot"}
    };
    vector<Rule>rules={
        {"sweet","fruit"},{"healthy","vegetable"},{"loyal","friendly"},
        {"liquid","drinkable"},{"cold","solid"}
    };
    cout<<"=== Rule-Based AI Chatbot ===\n";
    cout<<"I remember statements and use rules to answer later questions.\n";
    cout<<"Type 'help' for examples or 'exit' to stop.\n\n";
    string input;
    while(true){
        cout<<"You: ";
        getline(cin,input);
        string command=lower(trim(input));
        if(command=="exit"||command=="quit"){cout<<"Bot: Goodbye!\n";break;}
        if(command=="help"){
            cout<<"Bot: Tell me facts, for example 'apple is sweet'.\n";
            cout<<"Bot: Then ask 'is apple a fruit?'.\n";
            continue;
        }
        cout<<"Bot: "<<answer(input,facts,rules)<<"\n";
    }
}

signed main(){
    ios_base::sync_with_stdio(0);cin.tie(0);
    int tt=1;
    for(int i=1;i<=tt;i++)solve();
}
// run command 
// clang++ -std=c++17 assignment-11/chatbot.cpp -o chatbot
// ./chatbot