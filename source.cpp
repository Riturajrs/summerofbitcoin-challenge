#include <bits/stdc++.h>
#define ll long long
#define nl "\n" 
using namespace std;

struct coins{
    string txid;
    vector<string> ptxid;
    ll weight;
    ll fee;
    int f;
    ll total_fee;
    ll total_weight;
};

// txid,weight,parent_txids,fee,total_fee
vector<coins> id;
vector<string> sol;
ll weight = 0;
// Converting string to integer
ll stringTointeger(string str)
{
    ll temp = 0;
    for (ll i = 0; i < str.length(); i++) {
        temp = temp * 10 + (str[i] - '0');
    }
    return temp;
}
void solve(coins a ){
    weight += a.weight;
    if(a.ptxid.empty() || weight > 4000000 )
        return ;
    for( auto it : a.ptxid ){
        for( auto it1 : id ){
            if( it1.txid == it ){
                solve(it1);
                sol.push_back(it1.txid);
            }
        }
    }

}
int main(){
    fstream fin;
    fin.open("mempool.csv",ios::in);
    string temp,s1,s2,s3;
    ll i,j;
    coins temp1;
    // Parsing the CSV file
    // in the form of a graph
    fin >> temp;
    while( fin >> temp ){
        temp += ", ";
        int c = 0;
        vector<string> v;
        for( i = 0 ; i < temp.size() ; i++ ){
            if( temp[i] == ','){
                if( c == 0 ){
                    temp1.txid = s1;
                }
                else if( c == 1 ){
                    temp1.fee = stringTointeger(s1);
                }
                else if( c == 2 ){
                    temp1.weight = stringTointeger(s1);
                }
                else if( c == 3 ){
                    s1 += ";";
                    for( j = 0 ; j < s1.size() ; j++ ){
                        if( s1[j] == ';' ){
                            v.push_back(s2);
                            s2 = "";
                        }
                        else{
                            s2 += s1[j];
                        }
                    }
                    i = s1.size();
                }
                s1 = "";
                i++;
                c++;
            }
            else{
                s1 += temp[i];
            }
        }
       if( temp1.ptxid.size() > 0 ){
        temp1.f = 1;
        }
        else{
        temp1.f = 0;
        }
        temp1.total_fee = 0;
        temp1.total_weight = 0;
        v.clear();
        id.push_back(temp1);
    }

    int f;
    // Calculating total fees for every transaction using breadth first search
    for( i = 0 ; i < id.size() ; i++ ){
        queue<coins> q;
        q.push(id[i]);
        while( !q.empty() ){
            id[i].total_fee += q.front().fee;
            id[i].total_weight += q.front().weight;
            for( auto it : q.front().ptxid ){
                f = 0;
                for( j = 0 ; j < i ; j++ ){
                    if( id[j].txid == it ){
                        q.push(id[j]);
                        f = 1;
                        break;
                    }
                }
                if( f == 0 ){
                    while( q.size() > 1 ){
                        q.pop();
                    }
                    id[i].f = 0;
                    break;
                }
            }
            q.pop();
        }
    }
    ll mind= 0 ;
    //Sorting transaction using Greedy algortihim
    for (i = 0; i < id.size()-1; i++)
    {
        mind = i;
        for (j = i+1; j < id.size(); j++){
        if ( id[j].total_fee*id[mind].total_weight < id[mind].total_fee*id[j].total_weight )
            mind = j;
        }
        temp1 = id[i];
        id[i] = id[mind];
        id[mind] = temp1;
    }
    //Storing the transaction ids
    for( auto it : id ){
        if( weight + it.weight > 4000000 ){
            break;
        }
        if( it.f )
        solve(it);
        sol.push_back(it.txid);
        weight += it.weight;
    }
    // Creating block.txt file
    ofstream file;
    file.open("block.txt");
    // Writing in block.txt file
    for( i = 0 ; i < sol.size() ; i++ ){
        f = 0;
        for( j = 0 ; j < i ; j++ ){
            if( sol[i] == sol[j] ){
                f++;
                break;
            }
        }
        if( f == 0 ){
            file << sol[i] << nl;
        }
    }
    file.close();
    return 0;
}