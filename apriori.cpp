// Apriori Implementation
//./<RollNo>.sh -apriori <dataset_name> X <outputFilename>

#include<bits/stdc++.h>


using namespace std;


void prune(map<vector<int>, int> ck, map<vector<int>,int>&lk_1,map <vector<int>, int>&ckp )
{
	
	for(auto x: ck)
	{
		for(int i=0; i< x.first.size(); i++)
		{
			vector <int> m;

			for(int j=0; j<x.first.size();j++)
			{
				if(	j!=i)
				{
					m.push_back(x.first[j]);
				}
			}

			if (lk_1.count(m) <= 0)
			{
				ckp.erase(x.first);
				break;
			}

		}
	}

}

void freq_count(map<vector<int>,int>&ckp,string database)
{
	ifstream file(database);
	if (file.is_open()) 
	{
	    string line;
	    while (getline(file, line)) 
	    {
	    	//cout<<line<<endl;
	    	vector <int> v;
	    	string t = "";
	    	for(int i=0; i<line.size(); i++)
	    	{
	    		if(line[i]==' ')
	    		{
	    			if(t.size()>0)
	    			{
	    				//cerr<<" \n hj \n  "<<t<<endl;
	    				v.push_back(stoi(t));		
	    			}
	    			t = "";
	    		}
	    		else
	    		{
	    			t.push_back(line[i]);
	    		}
	    	}
	    	if(t.size()>0){
		    	v.push_back(stoi(t));
		    }

	    	for(auto x: ckp)
	    	{
	    		
	    		bool inc_count = true;
	    		bool arr[x.first.size()];
	    		memset(arr,0,sizeof(arr));
	    		for(int i=0; i<x.first.size(); i++)
	    		{
	    			for(int j=0; j<v.size();j++)
	    			{
	    				if(x.first[i] == v[j])
	    				{
	    					arr[i] = 1;
	    				}
	    			}
	    			if(arr[i]==0)
	    			{
	    				inc_count = false;
	    				break;
	    			}
	    		}
	    		 
	    		if (inc_count)
	    		{
	    			ckp[x.first]++;
	    		}
	    	}
	        
    	}
   		file.close();

	}
}

map <vector<int>,int> compare_support(map<vector<int>,int>&ckp, int min_sup)
{
	
	map <vector<int>,int> lk;
	for(auto x: ckp)
	{
		if(x.second*100 >= min_sup)
		{
			lk[x.first] = x.second;
		}
	}
	return lk;
}

void join(map<vector<int>,int>&lk,map<vector<int>,int>&ckp1)
{
	
	
	for(auto it=lk.begin(); it!=lk.end(); it++)
	{
		auto x = *it;
		auto it1 = it;
		it1++;
		int k = x.first.size();
		for(;it1!=lk.end();it1++)
		{
			auto y = (*it1);

			bool joinable = true;

			
			for(int i=0; i<k-1; i++)
			{
				if(x.first[i] != y.first[i])
				{
					joinable = false;
				}

			}

			if(joinable)
			{
				if(x.first[k-1] < y.first[k-1])
				{
					vector<int> v;
					v = x.first;
					v.push_back(y.first[k-1]);
					ckp1[v] = 0;
					//cout<<"Hjhjkh\n";
					//for(auto x : v) cout<< x <<" ";
					//cout<<"\n";
				}

			}
			else
			{
				break;
			}
		}
	}
}


int main(int argc, char** argv)
{
	int min_sup; 
	int tot_trans = 0;
	min_sup = stoi(argv[2]);
	string database = argv[1];
	string outfile = argv[3];

	ofstream fout(outfile);

	map <vector<int>,int> c1;

	ifstream file(database);
	if (file.is_open()) {
	    string line;
	    while (getline(file, line)) {
	    	tot_trans++;
	    	//cout<<line<<endl;
	    	vector <int> v;
	    	string t = "";
	    	for(int i=0; i<line.size(); i++)
	    	{
	    		if(line[i]==' ')
	    		{
	    			if(t.size()>0)
	    			{
	    				//cerr<<" \n hj \n"<<t<<endl;
	    				v.push_back(stoi(t));
	    			}
	    			t = "";
	    		}
	    		else
	    		{
	    			t.push_back(line[i]);
	    		}
	    	}
	    	if(t.size()>0)
	    	{
		    	v.push_back(stoi(t));
		    }

	    	for(int i=0; i < v.size(); i++)
	    	{
	    		vector<int> v1;
	    		v1.push_back(v[i]);
	    		c1[v1]++;
	    	}
	        
	    }
	    file.close();


	}


	map <vector<int>,int> l1;
	min_sup = min_sup*tot_trans;

	for(auto x: c1)
	{
		if(x.second*100 >= min_sup)
		{
			l1[x.first] = x.second;
		}
	}

	//cout<<l1.size()<<endl;
	map <vector<int>,int> lk;
	lk = l1;
	//cout<<lk.size()<<endl;

	vector<string> v;
	for(auto x : lk){
		vector<string> v1;
		for(auto y : x.first){
			v1.push_back(to_string(y));
		}
		sort(v1.begin(),v1.end());
		string t = v1[0];
		for(int i=1;i<v1.size();i++)
			t += " " + v1[i];
		v.push_back(t);
	}

	while(lk.size()>0)
	{
		map <vector<int>,int> ckp1;
		join(lk,ckp1);
		prune(ckp1,lk,ckp1);
		//cout<<"hkj: "<<lk.size()<<endl;
		freq_count(ckp1, database);
		lk = compare_support(ckp1, min_sup);
		for(auto x : lk){
			vector<string> v1;
			for(auto y : x.first){
				v1.push_back(to_string(y));
			}
			sort(v1.begin(),v1.end());
			string t = v1[0];
			for(int i=1;i<v1.size();i++)
				t += " " + v1[i];
			v.push_back(t);
		}

	}
	sort(v.begin(),v.end());
	for(auto x : v) fout <<x << "\n";
	fout.close();

}