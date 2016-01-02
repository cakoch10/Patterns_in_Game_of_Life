#include<iostream>
#include<vector>
#include<fstream>
#include<iterator>
#include<algorithm>
#include<cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

using namespace std;
void outVec(vector< vector <int> > grid);//outputs automaton
vector< vector <int> > updateVec(vector< vector <int> > grid);//generates automaton at t+1
bool test(vector< vector <int> > grid, int, char); //check to see if there is a one in the given row/column

void numLive(vector< vector <int> > grid); //num live in total
void numLiveC(vector< vector <int> > grid); //check number of live cells in corner
void numLiveir(vector< vector <int> > grid); //check num of live cells in inner region
void numLiveb(vector< vector <int> > grid); //chekc live in border
void numLives2(vector< vector <int> > grid); //check in s2 region
void numLives1(vector< vector <int> > grid); //check in s1 region
void numLiveo(vector< vector <int> > grid); //check outer region

long long combination(int, int);//compute the combination of two numbers
unsigned long long int factorial(unsigned long long int); //compute the factorial of a number

//create a new automaton (once val_n is updated)
vector< vector <int> > newAutomaton();

void out2file(int correctb,int ranb,int correcti,int rani,int correctc,int ranc,int correcto,int rano); //output data to file

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
	
	vector< vector <int> > grid;

	int P,p,Q,q,Lc,Lb,Li,delta,L;
	double gammai, gammac, gammab, delta2;
	bool guess, answer;
	
	int correctc=0,correcti=0,correctb=0;
	int corrects2=0;
	int corrects1=0;
	int correcto=0;
	
	int ranc=0,rani=0,ranb=0,rano=0;
	int total=0;
	
	int decreasec=0, decreasei=0;
	int decreaseb=0;
	
	bool g=true;
	
	//	grid=newAutomaton();
	//	outVec(grid);
	for(int xy=0; xy<100; xy++)
	{
	
	for(int j=0; j<100; j++)
	{
		Sleep(1000);
		grid=newAutomaton();
		//checkVec(grid);
		//outVec(grid);
				
		delta=(numLength-2)*(numLength-2);
		//find the true gamma values
		//find g_b,_i,_c
		
		numLive(grid);
		L=total_alive;
		
		numLiveC(grid);
		Lc=total_alive;	
		gammac=((double) Lc)/4;

		numLiveb(grid);
		Lb=total_alive;	
		gammab=((double) Lb)/((numLength-4)*4);
		
		numLiveir(grid);
		Li=total_alive;	
		gammai=((double) Li)/((numLength-4)*(numLength-4));
		
		numLives2(grid);
		int Ls2=total_alive;
		
		numLives1(grid);
		int Ls1=total_alive;
		
					
		//calculate individual probabilities
		
		//NOTE this is individual sections first, there will not be one composite probability of all weighted probabilities yet
		double ps1,ps2,ps3,ps4,pi,pc,pb,po;
		
		//outer region
		int tot_out = numLength*4 - 12;
		double frac = (double) 8/tot_out;
		ps4 = gammac*pow(gammab,2)*frac;
		frac = 1 - frac;
		po = pow(gammab,3)*frac+ps4;
		
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
		3*pow((gammai),2)*gammac*pow((1-gammab),4)*(1-gammai)+
		12*pow((gammai),2)*gammab*(1-gammac)*pow((1-gammab),3)*(1-gammai)+
		18*gammai*pow((gammab),2)*(1-gammai)*pow((1-gammab),2)*pow((1-gammai),2)+
		4*pow((gammab),3)*(1-gammab)*pow((1-gammai),3)*(1-gammac)+
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
		
		pi = frac*(gammai*pcase1+pcase2)+ps2+ps3;//must incorporate other probabilities
		
		
		//BORDER REGION
		int tot_b = (numLength-4)*4;
	
		//S1 subregion
		frac=(double) 8/tot_b;
		
		pcase1=gammac*pow((gammai),2)*pow((1-gammab),2)+
		4*gammab*gammac*gammai*(1-gammai)*(1-gammab)+
		pow((gammab),2)*gammac*pow((1-gammai),2)+
		2*gammai*pow((gammab),2)*(1-gammac)*(1-gammai)+
		2*pow((gammai),2)*gammab*(1-gammab)*(1-gammac);
		
		pcase2=pow((gammab),2)*pow((1-gammai),3)+
		6*gammai*gammab*(1-gammab)*pow((1-gammai),2)+
		3*pow((gammai),2)*(1-gammai)*pow((1-gammab),2);
		
		ps1 = frac*(gammab*pcase2+pcase1);
		
		//Regular
		
		frac=1-frac;
		
		pcase1=3*pow((gammab),2)*gammai*pow((1-gammai),2)+
		6*gammab*pow((gammai),2)*(1-gammab)*(1-gammai)+
		pow((gammai),3)*pow((1-gammab),2);
		
		pb = frac*(gammab*pcase2+pcase1)+ps1;
		
		//corner
		pc = (1-gammac)*pow(gammab,2)*gammai+gammac*pow(gammab,2)*gammai+2*gammac*(1-gammab)*gammab*gammai+gammac*gammab*gammab*(1-gammai);
		
		
		//calculate extended probabilities
		//inner
		double sumi=0;
		for(int i=1; i<=Li; i++)
		{
			double comb=combination(tot_in,Li-i);
			sumi=sumi+comb*pow(pi,(Li-i))*pow((1-pi),(tot_in-Li+i));
		}
		
		//border
		double sumb=0;
		for(int i=1; i<=Lb; i++)
		{
			double comb=combination(tot_b,Lb-i);
			sumb=sumb+comb*pow(pb,(Lb-i))*pow((1-pb),(tot_b-Lb+i));
		}
		
		//corner
		double sumc=0;
		for(int i=1; i<=Lc; i++)
		{
			double comb=combination(4.0,Lc-i);
			sumc=sumc+comb*pow(pc,(Lc-i))*pow((1-pc),(4-Lc+i));
		}
		
		//s2 subregion
		double sums2=0;
		for(int i=1; i<=Ls2; i++)
		{
			double comb=combination(4.0,Ls2-i);
			sums2+=comb*pow(ps2,(Ls2-i))*pow((1-ps2),(4-Ls2+i));
		}
		
		//s1 subregion
		double sums1=0;
		for(int i=1; i<=Ls1; i++)
		{
			double comb=combination(8.0,Ls1-i);
			sums1+=comb*pow(ps1,(Ls1-i))*pow((1-ps1),(8-Ls1+i));
		}
		
		//probability that there are 0 dead cells
		//at time t+1
		double sumo=pow((1-po),tot_b);
		
			
		grid = updateVec(grid);
		
	//	grid = checkVec(grid);
	
//		for (int c=1; c<=327; c++)
//      		for (int d=1; d<=32; d++)
//       		{}
       	
		
		bool guessc=0,guessi=0,guessb=0,guess2=0,guess1=0,guesso=0;//start off as false
		if(sumc>0.5)
			guessc=1;//num will decrease
		
//		srand (time(NULL));	
//		int num = rand() % 2;
		bool guesscr=g;
		
//		for (int c=1; c<=327; c++)
//      		for (int d=1; d<=32; d++)
//       		{}
       		
		if(sumi>0.5)
			guessi=1;
		
		if(sums2>0.5)
			guess2=1;
			
		if(sums1>0.5)
			guess1=1;
		
		if(sumo>0.5)
			guesso=1;
		
		g=!g;
		bool guessir=g;
		

		if(sumb>0.5)
			guessb=1;
//		srand (time(NULL));
//		num = rand() % 2;
		
		g=!g;		
		bool guessbr=g;
		
		g=!g;
		bool guessor;
			
		int nLc,nLi,nLb;//find new values
		
		numLiveC(grid);
		nLc=total_alive;	
		
		numLiveb(grid);
		nLb=total_alive;	
		
		numLiveir(grid);
		nLi=total_alive;	
		
		numLives2(grid);
		int nLs2=total_alive;	
		
		numLives1(grid);
		int nLs1=total_alive;
		
		numLiveo(grid);
		int nLo=total_alive;
		
	//	g=!g;
		
		if(nLc<Lc)
		{
			decreasec++;
			if(guessc==true)
				correctc++;
			
			if(guesscr==true)
				ranc++;
		}
		else
		{
			if(guessc==false)
				correctc++;
		
			if(guesscr==false)
				ranc++;
		}
		
		
		if(nLi<Li)
		{
			decreasei++;
			if(guessi==true)
				correcti++;
			
			if(guessir==true)
				rani++;
		}
		else
		{
			if(guessi==false)
				correcti++;

			if(guessir==false)
				rani++;
		}
		
		if(nLb<Lb)
		{
			decreaseb++;
			if(guessb==true)
				correctb++;
			
			if(guessbr==true)
				ranb++;
		}
		
		else
		{
			if(guessb==false)
				correctb++;
	
			if(guessbr==false)
				ranb++;
		}
		
		if(nLs2<Ls2)
		{
			if(guess2==true)
				corrects2++;
		}
		else
		{
			if(guess2==false)
				corrects2++;
		}
		
		
		if(nLs1<Ls1)
		{
			if(guess1==true)
				corrects1++;
		}
		else
		{
			if(guess1==false)
				corrects1++;
		}
		
		if(nLo==0)
		{
			if(guesso)
				correcto++;
			if(guessor)
				rano++;
			
		}
		else
		{
			if(!guesso)
				correcto++;
			if(!guessor)
				rano++;
		}
		
		total++;		
	}
	
	
	cout<<"Correct Border: "<<correctb<<endl;
	cout<<"Random Border: "<<ranb<<endl;
	cout<<"Num decrease: "<<decreaseb<<endl;
	cout<<endl;
	
	cout<<"Correct Inner: "<<correcti<<endl;
	cout<<"Random Inner: "<<rani<<endl;
	cout<<"Num decrease: "<<decreasei<<endl;
	cout<<endl;
	
	cout<<"Correct Corner: "<<correctc<<endl;
	cout<<"Random Corner: "<<ranc<<endl;
	cout<<"Num decrease: "<<decreasec<<endl;
	cout<<endl;
	
	cout<<"Correct Outer: "<<correcto<<endl;
	cout<<"Random Outer: "<<rano<<endl;
	
	
	cout<<"Total: "<<total<<endl;
	
	out2file(correctb,ranb,correcti,rani,correctc,ranc,correcto,rano);
	
	total=0;
	correctb=0;
	ranb=0;
	decreaseb=0;
	correcti=0;
	rani=0;
	decreasei=0;
	correctc=0;
	ranc=0;
	correcto=0;
	rano=0;
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
	
	for(int i=2; i<numLength-2; i++)
	{
		if((grid[1][i-1]+grid[1][i]+grid[1][i+1])==3)
			newgrid[0][i]=1;
	}
	
	for(int i=2; i<numLength-2; i++)
	{
		if((grid[i-1][1]+grid[i][1]+grid[i+1][1])==3)
			newgrid[i][0]=1;
	}
	
	for(int i=2; i<numLength-2; i++)
	{
		if((grid[numLength-2][i-1]+grid[numLength-2][i]+grid[numLength-2][i+1])==3)
			newgrid[numLength-1][i]=1;
	}
	
	for(int i=2; i<numLength-2; i++)
	{
		if((grid[i-1][numLength-2]+grid[i][numLength-2]+grid[i+1][numLength-2])==3)
			newgrid[i][numLength-1]=1;
	}
	
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

void out2file(int correctb,int ranb,int correcti,int rani,int correctc,int ranc,int correcto,int rano)
{

	ofstream results("GoLresults.txt", ios_base::out | ios_base::app);
	results<<correctb<<" "<<ranb<<" ";
	results<<correcti<<" "<<rani<<" ";
	results<<correctc<<" "<<ranc<<" ";
	results<<correcto<<" "<<rano<<endl;
		
	results.close();
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
					for (int c=1; c<=300; c++)
      					for (int d=1; d<=32; d++){}
				
					int num = rand() % 2;
					grid[i][k]=num;
				}
			}
		}
	}		
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
	
	//inner region is all cells excluding those in the outer two layer
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
	
	for(int i=2; i<size-2; i++)
	{
		total_alive+=grid[1][i];
		total_alive+=grid[i][1];
		total_alive+=grid[size-2][i];
		total_alive+=grid[i][size-2];
	}
}



void numLives2(vector< vector <int> > grid)
{
	//give the total number of live cells in s2 region
	int size=grid.size();
	total_alive=0;
	
	if(grid[2][2]==1)
		total_alive++;
	
	if(grid[2][size-3]==1)
		total_alive++;
		
	if(grid[size-3][2]==1)
		total_alive++;
		
	if(grid[size-3][size-3]==1)
		total_alive++;
}

void numLives1(vector< vector <int> > grid)
{
	//give the total number of live cells in s2 region
	int size=grid.size();
	total_alive=0;
	
	if(grid[2][1]==1)
		total_alive++;
	
	if(grid[1][2]==1)
		total_alive++;
	
	if(grid[size-3][size-2]==1)
		total_alive++;
		
	if(grid[size-2][size-3]==1)
		total_alive++;
		
	if(grid[1][size-3]==1)
		total_alive++;
	
	if(grid[size-3][1]==1)
		total_alive++;	
		
	if(grid[2][size-2]==1)
		total_alive++;
	
	if(grid[size-2][2]==1)
		total_alive++;
}


void numLiveo(vector< vector <int> > grid)
{
	//number of live outer cells
	int size=grid.size();
	total_alive=0;
	
	for(int i=0; i<size; i++)
	{
		if(grid[0][i]==1)
			total_alive++;
		if(grid[i][0]==1)
			total_alive++;
		if(grid[i][size-1]==1)
			total_alive++;
		if(grid[i][size-1]==1)
			total_alive++;
	}
}



