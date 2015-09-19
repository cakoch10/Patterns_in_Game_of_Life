#include<iostream>
#include<vector>
#include<fstream>
#include<iterator>
#include<algorithm>
#include<cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
void outVec(vector< vector <int> > grid);//outputs automaton
vector< vector <int> > updateVec(vector< vector <int> > grid);//generates automaton at t+1
vector< vector <int> > checkVec(vector< vector <int> > grid);//check to see if the grid needs to be expanded. If it does make sure to increase the size of prev. grids as well
bool test(vector< vector <int> > grid, int, char); //check to see if there is a one in the given row/column

void numLive(vector< vector <int> > grid);//num live in total
void numLiveC(vector< vector <int> > grid);//check number of live cells in corner
void numLiveir(vector< vector <int> > grid);//check num of live cells in inner region
void numLiveb(vector< vector <int> > grid);//chekc live in border

long long combination(int, int);//compute the combination of two numbers
unsigned long long int factorial(unsigned long long int); //compute the factorial of a number

//create a new automaton (once val_n is updated)
vector< vector <int> > newAutomaton();

void out2file(void);//outputs data to two separate text files

int rows=0;

int total_alive=0; //gives the total number of cells alive currently in the cellular automaton
int num_gens=0;//represents the number of generations of a specific automata
int val_n=50;//n represents the intitial configuration of the nxn square. Namely, it specifies the square root of the number of cells turned on

//storage grids to compare with current in order to determine if still life has been reached
vector< vector <int> > prev2grid;
vector< vector <int> > prevgrid;

int numLength=8;

int prelim=0;
int counter=0;
//create a program to generate output representing the number of 
//generations it takes for an nxn grid of cells to reach still life
int main()
{

//	system("MODE 1000,100");
	
	int k=2;
	vector< vector <int> > grid;

	int P,p,Q,q,Lc,Lb,Li,delta;
	double gammai, gammac, gammab, delta2;
	bool guess, answer;
		
	//	grid=newAutomaton();
	//	outVec(grid);
	for(int j=0; j<10000; j++)
	{
		grid=newAutomaton();
				
		int sLength = numLength;
		//outVec(grid);
		//check for P and p
		for(int b=sLength-2; b>0; b--)
		{
			if(test(grid, b, 'R'))
			{
				P=b;
				break;
			}
		}
		
		for(int b=1; b<sLength; b++)
		{
			if(test(grid, b, 'r'))
			{
				p=b;
				break;
			}
		}
		//check for Q and q
		for(int b=sLength-2; b>0; b--)
		{
			if(test(grid, b, 'C'))
			{
				Q=b;
				break;
			}
		}
		for(int b=1; b<sLength; b++)
		{
			if(test(grid, b, 'c'))
			{
				q=b;
				break;
			}
		}
		
		delta=(numLength-2)*(numLength-2);
		//find the true gamma values
		//find g_b,_i,_c
		
		
		numLivec(grid);
		Lc=total_alive;	
		gammac=((double) Lc)/delta;
		
		numLiveb(grid);
		Lb=total_alive;	
		gammab=((double) Lb)/delta;
		
		numLivei(grid);
		Li=total_alive;	
		gammai=((double) Li)/delta;
		
//		if(P>p)
//		{
//			cout<<P<<endl;
//			cout<<p<<endl;
//			system("pause>nul");
//		}
		
		//find the value of alpha and delta2 (which is prob that a cell is dead)
		int swhole = numLength*numLength;
		
//		P = numLength - 2;
//		p = 1;
//		
//		Q = numLength - 2;
//		q = 1;
		
		//calculate individual probabilities
		
		//NOTE this is individual sections first, there will not be one composite probability of all weighted probabilities yet
		double ps1,ps2,ps3,ps4,pi,pc,pb,po;
		
		//outer region
		int tot_out = numLength*4 - 12
		double frac = 8/tot_out;
		ps4 = gammac*pow(gammab,2)*frac;
		frac = 1 - frac;
		po = pow(gammab,3)*frac;
		
		//inner region
		int tot_in = (numLength-4)*(numLength-4);
		
		//s2 subregion
		double pcase1, pcase2;
		frac = 4/tot_in;

		
		pcase1=4*gammac*gammab*pow((1-gammab),3)*pow((1-gammai),3)+
		3*gammai*gammac*pow((1-gammai),2)*pow((1-gammab),4)+
		3*pow((gammai),2)*(1-gammac)*pow((1-gammab),4)*(1-gammai)+
		12*gammab*gammai*pow((1-gammai),2)*pow((1-gammab),3)*(1-gammac)+
		6*gammab*gammab*pow((1-gammab),2)*pow((1-gammai),3)*(1-gammac);
		
		pcase2=6*gammac*pow((gammab),2)*pow((1-gammab),2)*pow((1-gammai),3)+
		12*gammai*gammac*gammab*pow((1-gammab),3)*pow((1-gammai),2)+
		3*pow((gammai),2)*gammacpow((1-gammab),4)*(1-gammai)+
		12*pow((gammai),2)*gammab*(1-gammac)*pow((1-gammab),3)*(1-gammai)+
		18*gammai*pow((gammab),2)*(1-gammai)*pow((1-gammab),2)*pow((1-gammai),2)+
		4*(gammab),3)*(1-gammab)*pow((1-gammai),3)*(1-gammac)+
		pow((gammai),3)*pow((1-gammab),4)*(1-gammac);
		
		ps2 = frac*(gammai*pcase1+pcase2);
		
		//s3 subregion
		double nums3 = 4*((double) numLength-6);
		double frac2=nums3/tot_in;//can be more than just 8
		
		pcase1=10*pow((gammai),2)*pow((1-gammai),3)*pow((1-gammab),3)+
		15*gammai*gammab*pow((1-gammai),3)*pow((1-gammab),2)+
		3*pow((gammab),2)*pow((1-gammai),5)*(1-gammab);
		
		pcase2=10*pow((gammai),2)*pow((1-gammai),2)*pow((1-gammab),3)+
		pow((gammab),3)*pow((1-gammai),5)+
		30*pow((gammai),2)*gammab*pow((1-gammai),3)*pow((1-gammab),2)+
		15*gammai*pow((gammab),2)*(1-gammab)*pow((1-gammai),4);
		
		ps3 = frac*(gammai*pcase1+pcase2);
		
		//regular
		frac=1-frac-frac2;
		
		pcase1=28*pow(gammai,2)*pow((1-gammai),6);
		pcase2=56*pow(gammai,3)*pow((1-gammai),5);
		
		
		delta2 = 1 - alpha;
		
		//find P(Lt<Lt+1)
		int deltaf = (numLength)*(numLength)-12;
		if(deltaf==0)
			return 0;
		int n = 1;
		long double sum = 0;
		
		while(n<(deltaf-L))
		{
			if(deltaf<(L+n))
			{
				cout<<"error\n";
				system("pause>nul");	
			}
			double comb = (double) combination(deltaf, (L+n));
			long double pos = pow(alpha, (L+n));
			long double neg = pow(delta2, (deltaf-L-n));
			
			if((deltaf-L-n)<=0)
			{
				cout<<"ErroR\n";
				system("pause>nul");
			}
			
			if(comb<=0 || pos<=0)
			{
				cout<<"ERRor\n\n";
				if(comb<=0)
				{
					cout<<"deltaf: "<<deltaf<<" L+n: "<<L+n<<endl;
					cout<<"comb: "<<comb<<endl;
					system("pause>nul");
				}
				
				if(pos<=0)
				{
					cout<<"alpha: "<< alpha<<" L+n: "<<L+n<<endl;
					system("pause>nul");
				}
			}
				
//			cout<<pos<<endl;
//			cout<<comb<<endl<<endl;
			if(pos==0)
				pos=0.00000001;
			sum = sum + (comb*pos*neg);
			n++;
		}
		
//		if(sum!=0)
//		{
//			cout<<sum<<endl;
//			system("pause>nul");
//		}
		
		if(sum > 0.5)
			guess = true; //number of cells should increase
		else
			guess = false;
			
		//update grid
		grid = updateVec(grid);
		grid = checkVec(grid);
		numLive(grid);
		int L2=total_alive;
		answer = L<L2;
		
		
//		cout<<sum;
//		cout<<endl<<alpha<<endl;
//		system("pause>nul");

		
//		cout<<"Lt: "<<L;
//		cout<<endl<<"Lt+1: "<<L2;
//		cout<<endl<<"Value of Sum: "<<sum;
//		cout<<endl<<"Guess: "<<guess;
//		cout<<endl<<"Answer (is it true/false that the number of live cells increased?): "<<answer;
		if(guess == answer)
			cout<<1<<endl;
		
		else
		{
			cout<<0<<endl;
			cout<<"Guess: "<<guess<<" Actual: "<<answer<<endl;
			cout<<"L: "<<L<<endl;
			cout<<"L2: "<<L2<<endl;
			cout<<"Alpha: "<<alpha<<endl;
			cout<<"Sum: "<<sum<<endl;
			cout<<"delta: "<<delta<<endl;
			cout<<"Alpha2: "<<alpha2<<endl;
			cout<<"Alpha4: "<<alpha4<<endl;
			cout<<"P: "<<P<<endl;
			cout<<"p: "<<p<<endl;
			cout<<"Q: "<<Q<<endl;
			cout<<"q: "<<q<<endl;

			system("pause>nul");
		}
		
		if(guess==1)
		{
			cout<<"You guessed 1\n";
			system("pause>nul");

		}
		
		if(j%100==0)
			cout<<"check: "<<j<<endl;		
	}
	

	return 0;	
}

//		while(!test(grid))
//		{
//			grid=updateVec(grid);
//			grid=checkVec(grid);
//			outVec(grid);
//			system("PAUSE");
//		}
//		outVec(grid);
//		out2file();
//		grid.clear();
//		k++;
	

long long combination(int n, int r)
{
    if(r > n/2)
		r = n - r; //because C(n, r) == C(n, n - r)
	
    unsigned long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
	{
        ans *= (n - r + i);
        if((n-r+i)<=0)
        {
        	cout<<"n-r+i is neg\n";
        	cout<<"ANS: "<<ans<<" i: "<<i<<" n: "<<n<<" r: "<<r<<endl;
			system("pause>nul");
        }
        ans /= i;
        if(ans<=0)
        {
        	cout<<"ANS is neg\n";
        	cout<<"ANS: "<<ans<<" i: "<<i<<" n: "<<n<<" r: "<<r<<endl;
			system("pause>nul");
        }
    }
    
    return ans;
}
//
//unsigned long long int combination(int n, int r)
//{
//	cout<<n<<" "<<r<<endl;
//	unsigned long long int nf = factorial(n);
//	unsigned long long int rf = factorial(r);
//	unsigned long long int thirdf = factorial(n-r);
//	cout<<(nf/(rf*thirdf))<<endl;
//	return (nf/(rf*thirdf));
//}

unsigned long long int factorial(unsigned long long int m)
{
	if(m==0 || m==1)
		return 1;
	else
		return m*factorial(m-1);
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

bool test(vector< vector <int> > grid, int colrow, char cr)
{	
	if(cr=='r')
	{
		for(int zz=1; zz<numLength; zz++)
		{
			if(grid[colrow][zz]==1)
			{
				return true;
			}
		}
		return false;
	}
	
	if(cr=='R')
	{
		for(int zz=numLength-2; zz>0; zz--)
		{
			if(grid[colrow][zz]==1)
				return true;
		}
		return false;
	}
	
	if(cr=='c')
	{
		for(int zz=1; zz<numLength; zz++)
		{
			if(grid[zz][colrow]==1)
				return true;
		}
		return false;
	}
	
	if(cr=='C')
	{
		for(int zz=numLength-2; zz>0; zz--)
		{
			if(grid[zz][colrow]==1)
				return true;
		}
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
	int sideLen=numLength;
	vector< vector<int> > grid(sideLen, vector<int>(sideLen,0));
	srand (time(NULL));

	for(int i=0; i<sideLen; i++)
	{
		if(i!=0&&i!=sideLen-1)
		{
			for(int k=0; k<sideLen; k++)
			{
				if(k!=0 && k!=sideLen-1)
				{
					int num = rand() % 2;
					grid[i][k]=num;
				}
			}
		}
	}
	
	
//	int check = 0;
//	for(int i=1; i<sideLen-2)
//	{
//		if(grid[1][i]==1)
//		{
//			check = 1;
//			break;
//		}
//		
//	}
	
	int num = rand() % (sideLen-1);
	if(num==0)
		num == 1;
	
	grid[1][num]==1;
	
//	for(int i=1; i<sideLen-2)
//	{
//		if(grid[sideLen-2][i]==1)
//		{
//			check = 1;
//			break;
//		}
//	}
//	
//	
	num = rand() % (sideLen-1);
	if(num==0)
		num == 1;
	
	grid[sideLen-2][num]==1;
	
	num = rand() % (sideLen-1);
	if(num==0)
		num == 1;
	
	grid[sideLen-2][num]==1;
	
	num = rand() % (sideLen-1);
	if(num==0)
		num == 1;
	
	grid[num][sideLen-2]==1;
	
	num = rand() % (sideLen-1);
	if(num==0)
		num == 1;
	
	grid[1][num]==1;
		
	return grid;
}


void numLive(vector< vector <int> > grid)
{
	//give the total number of live cells in corners
	int size=grid.size();
	total_alive=0;
	
	for(int i=0; i<size; i++)
	{
		for(vector<int>::iterator j=grid[i].begin();j!=grid[i].end();++j)
    		total_alive += *j;
	}
	
}



void numLiveC(vector< vector <int> > grid)
{
	//give the total number of live cells in corners
	int size=grid.size();
	total_alive=0;
	
	if(grid[1][1]==1)
		total_alive++;
	
	if(grid[1][size-2]==1)
		total_alive++;
		
	if(grid[size-2][1]==1)
		total_alive++;
		
	if(grid[size-2][size-2]==1)
		total_alive++;
}



void numLiveir(vector< vector <int> > grid)
{
	//give the total number of live cells in inner region
	int size=grid.size();
	total_alive=0;
	//inner region is all cells excluding those in the outer two layers
	
	for(int i=2; i<size-2; i++)
	{
		int count=2;
		
		while(count>1&&count<size-2)
		{
    		total_alive += grid[i][count];
			count++;
    	}
	}
	
}


void numLiveb(vector< vector <int> > grid)
{
	//give the total number of live cells in border
	int size=grid.size();
	total_alive=0;
	
	for(int i=1; i<size-1; i++)
	{
		total_alive+=grid[1][i];
		total_alive+=grid[size-2][i];
	}	
}























