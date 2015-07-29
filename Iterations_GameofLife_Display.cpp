#include<iostream>
#include<vector>
#include<fstream>
#include<iterator>
#include<algorithm>
#include<cmath>
using namespace std;
void outVec(vector< vector <int> > grid);//outputs automaton
vector< vector <int> > updateVec(vector< vector <int> > grid);//generates automaton at t+1
vector< vector <int> > checkVec(vector< vector <int> > grid);//check to see if the grid needs to be expanded. If it does make sure to increase the size of prev. grids as well
bool test(vector< vector <int> > grid); //make sure to take into account the increased size of vector. THis tests to see if the automaton has reached still life

void outVec2(vector< vector <int> > grid);//outputs automaton to file


//create a new automaton (once val_n is updated)
vector< vector <int> > newAutomaton();

void out2file(void);//outputs data to two separate text files

int rows=0;

int num_gens=0;//represents the number of generations of a specific automata
int val_n=59;//n represents the intitial configuration of the nxn square. Namely, it specifies the square root of the number of cells turned on

//storage grids to compare with current in order to determine if still life has been reached
vector< vector <int> > prev2grid;
vector< vector <int> > prevgrid;



int prelim=0;
int counter=0;
//create a program to generate output representing the number of 
//generations it takes for an nxn grid of cells to reach still life
int main()
{
	system("MODE 1000,100");
	int k=60;
	vector< vector <int> > grid;
	//	grid=newAutomaton();
	//	outVec(grid);
	int x=0;
	
	while(k<80)
	{
		grid=newAutomaton();
		outVec(grid);
		while(!test(grid))
		{
			while(x<229)
			{
				grid=updateVec(grid);
				grid=checkVec(grid);
				x++;
			}
			
			outVec2(grid);
			grid=updateVec(grid);
			grid=checkVec(grid);
			system("PAUSE");
		}
		out2file();
		grid.clear();
		k++;
	}
		
	//while(counter<scenarios)	
	/*
	vector<int> a;
	a.push_back(90);
	a.insert(a.begin(),10);
	grid.push_back(a);
	cout<< grid[0][0]<<grid[0][1];
	cin.get();
	*/
	return 0;	
}




void outVec(vector< vector <int> > grid)
{
	if(grid.size()!=grid[0].size())
		cout<<"Error. Grid is no longer a square.";
		
	int size=grid.size();
	int counter1=0;
	int counter2=0;
	while(counter1<size)
	{
		while(counter2<size)
		{
			if(grid[counter1][counter2]==1)
				cout<<"*";
			else if(grid[counter1][counter2]==0)
				cout<<" ";
			if(counter2+1==size)
				cout<<endl;
			counter2++;
		}
		counter2=0;
		counter1++;
	}
	cout<<endl<<endl;
	cout<<"T= "<<num_gens<<endl;
}

void outVec2(vector< vector <int> > grid)
{
	if(grid.size()!=grid[0].size())
		cout<<"Error. Grid is no longer a square.";
		
	ofstream generations("graphic.txt");
	
	int size=grid.size();
	int counter1=0;
	int counter2=0;
	while(counter1<size)
	{
		while(counter2<size)
		{
			if(grid[counter1][counter2]==1)
				generations<<"*";
			else if(grid[counter1][counter2]==0)
				generations<<" ";
			if(counter2+1==size)
				generations<<"\n";
							
			counter2++;
		}
		counter2=0;
		counter1++;
	}
	generations<<"\n";
	generations<<"T= "<<num_gens<<endl;
	generations.close();
}




vector < vector<int> > updateVec(vector< vector <int> > grid)
{
	if(grid.size()!=grid[0].size())
		cout<<"Error. Grid is no longer a square.";
	int size=grid.size();
	vector< vector<int> > newgrid(size, vector<int>(size));//create an updated automaton
	int c1=1;
	int c2=1;
	while(c1<size-1)
	{
		while(c2<size-1)
		{
			int sum_neighborhood=grid[c1+1][c2]+grid[c1][c2+1]+grid[c1+1][c2+1]+grid[c1-1][c2-1]+grid[c1-1][c2]+grid[c1][c2-1]+grid[c1+1][c2-1]+grid[c1-1][c2+1];
			
			//apply the transition rules
			if(grid[c1][c2]==0&&sum_neighborhood==3)
				newgrid[c1][c2]=1;
			
			else if(grid[c1][c2]==0&&sum_neighborhood!=3)
				newgrid[c1][c2]=0;
			else if(grid[c1][c2]==1&&(sum_neighborhood==2||sum_neighborhood==3))
				newgrid[c1][c2]=1;
			else if(grid[c1][c2]==1&&(sum_neighborhood!=2&&sum_neighborhood!=3))
				newgrid[c1][c2]=0;
			c2++;
		}
		c2=1;
		c1++;
	}
	num_gens++;
	return newgrid;
}

bool test(vector< vector <int> > grid)
{
	/*
	if((prelim>1)&&(prev2grid.size()!=grid.size()||prevgrid.size()!=grid.size()))
	{
		if(prev2grid.size()!=grid.size())
		{
			int side_big=grid.size();
			int side_small=prev2grid.size();
			int diff=side_big-side_small;
			//I think I need to take half and add amount of rows/columns needed
			
			
		}
	}
	*/
	if(prelim==0)
	{
		prevgrid=grid;
		prelim++;
		return false;
	}
	else if(prelim==1)
	{
		prev2grid=prevgrid;
		prevgrid=grid;
		prelim++;
		return false;
	}
	else if(prelim>1)
	{
		if(prevgrid==grid)
		{
			num_gens=num_gens-1;
			return true;
		}
		else if(prev2grid==grid)
		{
			num_gens=num_gens-2;
			return true;
		}
		
		prev2grid=prevgrid;
		prevgrid=grid;
		return false;
	}
	
}

void out2file()
{
	if(val_n%2==0)
	{
		ofstream generations("generationseven.txt", ios_base::out | ios_base::app);
		generations<<num_gens<<endl;
		generations.close();
	
		ofstream size_init("value_of_neven.txt", ios_base::out | ios_base::app);
		size_init<<val_n<<endl;
		size_init.close();
	}
	//reset values
	prev2grid.clear();
	prevgrid.clear();
	num_gens=0;
	prelim=0;
	val_n=val_n+2;
}


vector< vector <int> > checkVec(vector< vector <int> > grid)
{
	if(grid.size()!=grid[0].size())
		{cout<<"Error. Grid is no longer a square.";}
	int size=grid.size();
	
	//vector< vector <int> >::iterator it;
	//for(it=grid.begin(); it<=grid.end(); it++)
	//create loops to search vector for 1 in the outer two layers of grid
	
	//add up the sum of states in the outer layers
	int sumElementsOuter=0;
	
	for(vector<int>::iterator j=grid[0].begin();j!=grid[0].end();++j)
    		sumElementsOuter += *j;
	for(vector<int>::iterator k=grid[size-1].begin();k!=grid[size-1].end();++k)
    		sumElementsOuter += *k;
	for(int m=1; m<size-1; m++)
		sumElementsOuter=grid[m][0]+grid[m][size-1]+sumElementsOuter;
	
	//add up the sum of the states in the layer right before the outer
	int sumElementsIn=0;
	
	for(vector<int>::iterator x=grid[1].begin();x!=grid[1].end();++x)
    		sumElementsIn += *x;
	
	for(vector<int>::iterator y=grid[size-2].begin();y!=grid[size-2].end();++y)
    		sumElementsIn += *y;
	
	for(int m=2; m<size-2; m++)
		sumElementsIn=grid[m][1]+grid[m][size-2]+sumElementsIn;

			if(sumElementsOuter>0)
			{
				//gird needs to be updated to have two new outer layers
				grid.insert(grid.begin(), vector<int> (size, 0));
				grid.insert(grid.begin(), vector<int> (size, 0));
				grid.push_back(vector<int> (size, 0));
				grid.push_back(vector<int> (size, 0));
				
				//do the same to grid state holders
				prev2grid.insert(prev2grid.begin(), vector<int> (size, 0));
				prev2grid.insert(prev2grid.begin(), vector<int> (size, 0));
				prev2grid.push_back(vector<int> (size, 0));
				prev2grid.push_back(vector<int> (size, 0));
				
				prevgrid.insert(prevgrid.begin(), vector<int> (size, 0));
				prevgrid.insert(prevgrid.begin(), vector<int> (size, 0));
				prevgrid.push_back(vector<int> (size, 0));
				prevgrid.push_back(vector<int> (size, 0));
				
				int rows2=0;
				int col2=0;
				while(rows2<size+4)
				{
					grid[rows2].insert(grid[rows2].begin(), 0);
					grid[rows2].insert(grid[rows2].begin(), 0);
					grid[rows2].push_back(0);
					grid[rows2].push_back(0);
					
					//do the same to grid state holders
					prev2grid[rows2].insert(prev2grid[rows2].begin(), 0);
					prev2grid[rows2].insert(prev2grid[rows2].begin(), 0);
					prev2grid[rows2].push_back(0);
					prev2grid[rows2].push_back(0);
					
					prevgrid[rows2].insert(prevgrid[rows2].begin(), 0);
					prevgrid[rows2].insert(prevgrid[rows2].begin(), 0);
					prevgrid[rows2].push_back(0);
					prevgrid[rows2].push_back(0);
					
					rows2++;
				}
			
			}
			
			else if(sumElementsIn>0)
			{
				//gird needs to be updated to have one new outer layers
				grid.insert(grid.begin(), vector<int> (size, 0));
				grid.push_back(vector<int> (size, 0));
				
				//do the same to the previous grids
				prev2grid.insert(prev2grid.begin(), vector<int> (size, 0));
				prev2grid.push_back(vector<int> (size, 0));
				
				prevgrid.insert(prevgrid.begin(), vector<int> (size, 0));
				prevgrid.push_back(vector<int> (size, 0));
				
				int rows2=0;
				while(rows2<size+2)
				{
					grid[rows2].insert(grid[rows2].begin(), 0);
					grid[rows2].push_back(0);
					
					//do the same to the previous grids
					if(prelim>1)
					{
						prev2grid[rows2].insert(prev2grid[rows2].begin(), 0);
						prev2grid[rows2].push_back(0);
					}
					
					if(prelim>0)
					{
						prevgrid[rows2].insert(prevgrid[rows2].begin(), 0);
						prevgrid[rows2].push_back(0);
					}
					rows2++;
				}
			
			}
	return grid;
}


vector< vector<int> > newAutomaton()
{
	int sideLen;
	sideLen=4+val_n;
	vector< vector<int> > grid(sideLen, vector<int>(sideLen,0));
	
	//create a vector for each row a part of grid
	int col=0;
	/*
	while(col<sideLen)
	{
		grid[col].push_back(vector<int> (sideLen,0));
		col++;
	}
	*/
	
	/*
	if(n_val==1)
	{
		int count=0;
		while(count<sideLen)
		{
			if(count!=2)
			{
				for(int i=0;i<5;i++)
				{
					grid[count][i]=0;
					
				}
			}
			else
			{
				grid[2][0]=0;
				grid[2][1]=0;
				grid[2][2]=1;
				grid[2][3]=0;
				grid[2][4]=0;
			}
			count++;
		}
	}
	*/
	
	int count2=0;
	int count3=0;
	
	//initialize all the values to 0 at first
	while(count2<sideLen)
	{
		while(count3<sideLen)
		{
			grid[count2][count3]=0;
			count3++;
		}
		count3=0;
		count2++;
	}
	
	//	
	int j=2;
	int k=2;
	
	while(j<val_n+2)
	{
		while(k<val_n+2)
		{
			grid[j][k]=1;
			k++;
		}
		k=2;
		j++;
	}
	return grid;
}




//a.insert(a.begin(),10);

/*


	int scenarios;
	//cout<< "How many scenarious do you want to run? :";
	//cin>> scenarios;
	
	vector< vector<int> > grid(6, vector<int>(6)); //create a 2-D vector representing the grid whose elements make up the horizontal rows
	//start by creating a grid of 6x6 cells with four cells at center turned on
	//fill vector
	int counter1=0;
	int counter2=0;
	while(counter1<6)
	{
		while(counter2<6)
		{
			if((counter2==2 && (counter1==2||counter1==3))||(counter2==3&&(counter1==2||counter1==3)))
			{
				grid[counter1][counter2]=1;
			}
			else
			{
				grid[counter1][counter2]=0;
			}
			counter2++;
		}
		counter2=0;
		counter1++;
	}
	




while(rows<2||rows>size-2)
	{
		while(columns<2||columns>size-2)
		{
			for(it=grid[rows].begin(); it<=grid[rows].end(); it++)
			{
				
			}
		}
	}


for(it=grid.begin(); it<=grid.end(); it++)
	{
		if(*it<2||*it>=size-1)
		{
			vector <int>::iterator it2;
			int count_columns=0;
			for(it=grid.begin(); it<=grid.end(); it++)
			{
				if(*it2==1&&(count_columns<2||count_columns))
			}
			
outVec(grid);
	grid=updateVec(grid);
	cout<<endl<<endl;
	outVec(grid);
	cout<<"\n__________________________________________________________________\n";
	while(!test(grid))
	{
		grid=updateVec(grid);
		cout<<"false\n";
	}
	cout<<"The following is true: \n";
	outVec(grid);

			
*/			

			
			
			
			
			
			
			
	
