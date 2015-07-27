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

//create a new automaton (once val_n is updated)
vector< vector <int> > newAutomaton();

void out2file(void);//outputs data to two separate text files

int rows=0;

int total_alive=0; //gives the total number of cells alive currently in the cellular automaton
int num_gens=0;//represents the number of generations of a specific automata
int val_n=63;//n represents the intitial configuration of the nxn square. Namely, it specifies the square root of the number of cells turned on

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
	
	int k=2;
	vector< vector <int> > grid;
	

	
	//	grid=newAutomaton();
	//	outVec(grid);
	
	while(k<101)
	{
		grid=newAutomaton();
		outVec(grid);
		while(!test(grid))
		{
			grid=updateVec(grid);
			grid=checkVec(grid);
			outVec(grid);
			system("PAUSE");
		}
		outVec(grid);
		out2file();
		grid.clear();
		k++;
	}
	
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
			cout<<grid[counter1][counter2];
			if(counter2+1==size)
			{
				cout<<endl;
			}
			
			counter2++;
		}
		counter2=0;
		counter1++;
	}
	cout<<endl<<endl;
}




vector < vector<int> > updateVec(vector< vector <int> > grid)
{
	//if(grid.size()!=grid[0].size())
//		cout<<"Error. Grid is no longer a square.";
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
			{
				newgrid[c1][c2]=1;
			}
			else if(grid[c1][c2]==0&&sum_neighborhood!=3)
			{
				newgrid[c1][c2]=0;
			}
			else if(grid[c1][c2]==1&&(sum_neighborhood==2||sum_neighborhood==3))
			{
				newgrid[c1][c2]=1;
			}
			else if(grid[c1][c2]==1&&(sum_neighborhood!=2&&sum_neighborhood!=3))
			{
				newgrid[c1][c2]=0;
			}
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

	ofstream generations("generationssquare.txt", ios_base::out | ios_base::app);
	generations<<num_gens<<endl;
	generations.close();
	
	ofstream size_init("value_of_nsquare.txt", ios_base::out | ios_base::app);
	size_init<<val_n<<endl;
	size_init.close();
	
	//reset values
	prev2grid.clear();
	prevgrid.clear();
	num_gens=0;
	prelim=0;
	val_n++;
}


vector< vector <int> > checkVec(vector< vector <int> > grid)
{
//	if(grid.size()!=grid[0].size())
//		{cout<<"Error. Grid is no longer a square.";}
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
	{
		sumElementsOuter=grid[m][0]+grid[m][size-1]+sumElementsOuter;
	}
	
	//add up the sum of the states in the layer right before the outer
	int sumElementsIn=0;
	
	for(vector<int>::iterator x=grid[1].begin();x!=grid[1].end();++x)
    	sumElementsIn += *x;
	
	for(vector<int>::iterator y=grid[size-2].begin();y!=grid[size-2].end();++y)
    	sumElementsIn += *y;
	
	for(int m=2; m<size-2; m++)
	{
		sumElementsIn=grid[m][1]+grid[m][size-2]+sumElementsIn;
	}
	
	
	
	
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
	
	int j=2;
	int k=2;
	
	while(j<sideLen-2)
	{
		grid[2][j]=1;
		j++;
	}
	j=2;
	
	while(j<sideLen-2)
	{
		grid[sideLen-3][j]=1;
		j++;
	}
	j=2;
	
	while(j<sideLen-2)
	{
		grid[j][2]=1;
		j++;
	}
	j=2;
	
	while(j<sideLen-2)
	{
		grid[j][sideLen-3]=1;
		j++;
	}
	
	return grid;
}


void numLive(vector< vector <int> > grid)
{
	//give the total number of live cells
	int size=grid.size();
	
	for(int i=0; i<size; i++)
	{
		for(vector<int>::iterator j=grid[i].begin();j!=grid[i].end();++j)
    		total_alive += *j;
	}
	
}























