#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <sstream> // std::stringstream
#include <iostream>
#include <map>
#include <iterator>
#include <queue>
#include <set>
#include <cstdlib>
using namespace std;
struct pr{
    int i;
    int j;
    string psf;  //path so far 

    pr(int a,int b,string c):i(a),j(b),psf(c){}

};

void read_record(map<string,vector<string>>& mp,map<string,pair<int,int>>& id_f_w,map<string,bool>& came_or_not){
	fstream fin;
	fin.open("mempool.csv", ios::in);
	string line, word;
    string tx_id;
    int fee,weight;
    bool column = false;
	while (!fin.eof()) {
        int countCol = 0; //counting columns
		getline(fin,line);
        if (!column){
            column = true;
            continue;
        }

		stringstream s(line);
		while (getline(s, word,',')) {
            //count columns for diffrent column and capturing different cells
            if (countCol == 0){
                tx_id = word;
            }else if (countCol == 1){
                fee = stoi(word);
            }else if(countCol == 2){
                weight = stoi(word);
                id_f_w[tx_id] = make_pair(fee,weight);
                came_or_not[tx_id] = false;
            }else if (countCol == 3){
                string pId;
                stringstream w(word);
                while(getline(w,pId,';')){
                    mp[tx_id].push_back(pId);
                }
            }
            countCol++;
		}
	}
}

//this fuction arranges the tx_ids so that the child id comes before parent id
void arrange_record(vector<pair<string,pair<int,int>>>& arranged,map<string,vector<string>>& mp,map<string,pair<int,int>>& id_f_w,map<string,bool>& came_or_not,map<string,pair<int,int>>::iterator it1){
    if (it1 == id_f_w.end()) return;
    string S = it1->first;
    int fee = it1->second.first;
    int wgt = it1->second.second;
    if (mp.find(S) == mp.end() ){
        if (!came_or_not[S]) {
            arranged.push_back(make_pair(S,make_pair(fee,wgt)));
            came_or_not[S] = true;

        }
    }else {
        for (auto it2 = mp.find(S)->second.begin() ; it2 != mp.find(S)->second.end() ; it2++){
            string str = *it2;
            map<string,pair<int,int>>::iterator it3 = id_f_w.find(str);
            arrange_record(arranged,mp,id_f_w,came_or_not,it3);
        }
        if (!came_or_not[S]) {
            arranged.push_back(make_pair(S,make_pair(fee,wgt)));
            came_or_not[S] = true;
        }
    }
    return;
}

int main(){
    const int weight = 40000;
    map<string,vector<string>> mp;
    map<string,pair<int,int>> id_f_w;
    map<string,bool> came_or_not;

    vector<pair<string,pair<int,int>>> arranged;
    read_record(mp,id_f_w,came_or_not);
    int size = id_f_w.size();
    map<string,pair<int,int>>::iterator it1 = id_f_w.begin();
    for (auto it1 = id_f_w.begin() ; it1 != id_f_w.end() ; it1++){
        arrange_record(arranged,mp,id_f_w,came_or_not,it1);
    }
    /*
    for(auto it = mp.begin(); it != mp.end() ; it++){
        cout<<it->first<<"::";
        for(auto it1 = it->second.begin(); it1 != it->second.end() ; it1++){
            cout<<*it1<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

    for(auto i = id_f_w.begin(); i != id_f_w.end() ; i++){
        cout<<i->first<<"::"<<i->second.first<<" "<<i->second.second<<endl;
    }
    cout <<endl;

    for (auto it = arranged.begin() ; it != arranged.end() ; it++){
        cout<<it->first<<" "<<it->second.first<<" "<<it->second.second<<endl;
    }
    cout<<endl;
    */
    

    //i considered the problem as modified 0/1 knapsack where printing is involved
    //standard 0/1 knapsack problem
    vector<vector<int>> dp(size+1,vector<int>(weight+1,0));
    for (int i = 1 ; i < dp.size() ; i++){
        for (int j = 1 ; j < dp[0].size() ; j++){
            dp[i][j] = dp[i-1][j];
            if (j >= arranged[i-1].second.second){
                if(dp[i-1][j - arranged[i-1].second.second] + arranged[i-1].second.first > dp[i-1][j]){
                    dp[i][j] = dp[i-1][j - arranged[i-1].second.second] + arranged[i-1].second.first;
                }
            }
        }
    }

    /*
    for (int i = 0 ; i < dp.size() ; i++){
        for (int j = 0; j < dp[0].size() ; j++){
            cout<<dp[i][j]<<" ";
        }
        cout<<endl;
    }
    */
    

    int ans = dp[size][weight];
    cout<<ans<<endl;

    //finding answer in every weight if previous weight didn't give the answer
    for(int i = weight ; i >= 0 ; i--){
        //finding valid
        //using bfs for every weight
        bool isValid = true;
        queue<pr> q;
        q.push(pr(size,i,""));
        while(!q.empty()){
            pr rem = q.front();
            q.pop();

            if (rem.i == 0 || rem.j == 0){
                // for (auto it = rem.psf.begin() ; it != rem.psf.end() ; it++){
                //     cout<<*it<<" ";
                // }
                // cout<<endl;
                //cout<<rem.psf<<" "<<i<<endl;
                string word;
                stringstream s(rem.psf);
                isValid = true;
                vector<string> psf;
                set<string,greater<string>> pppOrNot; //previous parents present or not
		        while (getline(s, word,' ')) {
                    psf.push_back(word);
                    pppOrNot.insert(word);
                    if (mp.find(word) != mp.end()){
                        for (auto it = mp.find(word)->second.begin(); it != mp.find(word)->second.end() ; it++){
                            if (pppOrNot.find(*it) == pppOrNot.end()){
                                isValid = false;
                                break;
                            }
                        }
                    }
                    if (!isValid) break;  
                }

                //outputing the valid output
                if (isValid){
                    ofstream outdata;
                    outdata.open("block.txt"); // opens the file
                    if( !outdata ) { // file couldn't be opened
                        cerr << "Error: file could not be opened" << endl;
                        exit(1);
                    }

                    for (auto it = psf.begin() ; it != psf.end() ; it++)
                        outdata <<*it<< endl;
                    outdata.close();
                    break;
                }
            }else {
                int exc = dp[rem.i - 1][rem.j];

                if (rem.j >= arranged[rem.i-1].second.second){
                    int inc = dp[rem.i - 1][rem.j - arranged[rem.i-1].second.second] + arranged[rem.i-1].second.first;
                    if (dp[rem.i][rem.j] == inc){
                        q.push(pr(rem.i-1,rem.j - arranged[rem.i-1].second.second,arranged[rem.i - 1].first + " " + rem.psf));
                    }
                }

                if (dp[rem.i][rem.j] == exc){
                    q.push(pr(rem.i - 1,rem.j ,rem.psf));
                }
            }
        }
        
        if(isValid) break;
    }
    
    return 0;

}