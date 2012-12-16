#ifndef LOCALMAP
#define LOCALMAP

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

/* Attention aux coordonnées : 
 * y x->
 * |
 * v
 */

const int LSIZE=1025; //(2^n+1)

class LocalMap
{
    public:
    
    LocalMap(int var, LocalMap* ul, LocalMap* uu, LocalMap* ur, LocalMap* rr, LocalMap* dr, LocalMap* dd, LocalMap* dl, LocalMap* ll) : 
    m_var(var), m_ul(ul), m_uu(uu), m_ur(ur), m_rr(rr), m_dr(dr), m_dd(dd), m_dl(dl), m_ll(ll)
    {
		seed(5000);
		diamondSquare();
		
		//show();
		cout << "Mean:" << getMean() << "  Max:" << getMax();
		cout << "  Min:" << getMin() << "  Delta:";
		cout << getMax()-getMin() << "." << endl;
    }
    short getHeight(int x, int y)
    {	
		respectLocalBorders(x,y);
		return m_heightMap[x][y];
	}
	short getMean()
	{
		long long m=0; // 32767*1025*1025= 34 426 880 000
		for(int i=0; i<LSIZE; i++)
			for(int j=0;j<LSIZE;j++)
				m+=m_heightMap[i][j];
		return short(m/(1025*1025));
	}
	short getMax()
	{
		int max=m_heightMap[0][0];
		for(int i=0; i<LSIZE; i++)
			for(int j=0;j<LSIZE;j++)
				if(max<m_heightMap[i][j])
					max=m_heightMap[i][j];
		return max;
	}
	short getMin()
	{
		int min=m_heightMap[0][0];
		for(int i=0; i<LSIZE; i++)
			for(int j=0;j<LSIZE;j++)
				if(min>m_heightMap[i][j])
					min=m_heightMap[i][j];
		return min;	
	}
	
    private:
    void seed(short initValue=rand()%65536-32768)
	{
		//Si aucune LocalMap adjacente n'est remplie, la LocalMap est isolée et les valeurs sont arbitraires.

		if(m_uu==0 && m_rr==0 && m_dd==0 && m_ll==0 && m_ul==0 && m_ur==0 && m_dr==0 && m_dl==0)
		{// The LocalMap is isolated
			
			if(initValue >  32767) initValue= 32767;
			if(initValue < -32768) initValue=-32768;
		
			m_heightMap[0      ][0      ]=keinOverflow(initValue, 1024);
			m_heightMap[LSIZE-1][0      ]=keinOverflow(initValue, 1024);
			m_heightMap[LSIZE-1][LSIZE-1]=keinOverflow(initValue, 1024);
			m_heightMap[0      ][LSIZE-1]=keinOverflow(initValue, 1024);
			
			cout << m_heightMap[0      ][0      ] << " ";
			cout << m_heightMap[LSIZE-1][0      ] << " ";
			cout << m_heightMap[LSIZE-1][LSIZE-1] << " ";
			cout << m_heightMap[0      ][LSIZE-1] << endl;
		}
		else
		{// The LocalMap is not isolated	
			bool cornerIsIsolated[4]={0,0,0,0};
			
			seedNotIsolatedCorners(cornerIsIsolated);
			seedIsolatedCorners(cornerIsIsolated);
			seedBorders();
		}
	}
	void seedNotIsolatedCorners(bool cornerIsIsolated[])
    {
		//Sinon, pour chaque coin interne, on vérifie d'abord si les coins externes adjacents sont remplis ou pas.
		//Le coin interne devient la moyenne des coins externes adjacents remplis.
		
		int emptyAdjacentLMaps;	
		short ul_dr, ur_dl, dr_ul, dl_ur;
		short uu_dl, uu_dr, rr_ul, rr_dl, dd_ur, dd_ul, ll_dr, ll_ur;
		
		//up left corner
		emptyAdjacentLMaps=0;
		if(m_ll==0) {emptyAdjacentLMaps++; ll_ur=0;}
		else ll_ur=m_ll->getHeight(LSIZE-1,0);
		if(m_ul==0) {emptyAdjacentLMaps++; ul_dr=0;}
		else ul_dr=m_ul->getHeight(LSIZE-1,LSIZE-1);
		if(m_uu==0) {emptyAdjacentLMaps++; uu_dl=0;}
		else uu_dl=m_uu->getHeight(0,LSIZE-1);
		
		if(emptyAdjacentLMaps==3) 
			cornerIsIsolated[0]=1;
		else
			m_heightMap[0][0]=(ll_ur+ul_dr+uu_dl)/(3-emptyAdjacentLMaps);
		
		//up right corner			
		emptyAdjacentLMaps=0;
		if(m_uu==0) {emptyAdjacentLMaps++; uu_dr=0;}
		else uu_dr=m_uu->getHeight(LSIZE-1,LSIZE-1);
		if(m_ur==0) {emptyAdjacentLMaps++; ur_dl=0;}
		else ur_dl=m_ur->getHeight(0,LSIZE-1);
		if(m_rr==0) {emptyAdjacentLMaps++; rr_ul=0;}
		else rr_ul=m_rr->getHeight(0,0);
					
		if(emptyAdjacentLMaps==3) 
			cornerIsIsolated[1]=1;
		else
			m_heightMap[LSIZE-1][0]=(uu_dr+ur_dl+rr_ul)/(3-emptyAdjacentLMaps);
		
		//down right corner
		emptyAdjacentLMaps=0;
		if(m_rr==0) {emptyAdjacentLMaps++; rr_dl=0;}
		else rr_dl=m_rr->getHeight(0,LSIZE-1);
		if(m_dr==0) {emptyAdjacentLMaps++; dr_ul=0;}
		else dr_ul=m_dr->getHeight(0,0);
		if(m_dd==0) {emptyAdjacentLMaps++; dd_ur=0;}
		else dd_ur=m_dd->getHeight(LSIZE-1,0);
					
		if(emptyAdjacentLMaps==3) 
			cornerIsIsolated[2]=1;
		else
			m_heightMap[LSIZE-1][LSIZE-1]=(rr_dl+dr_ul+dd_ur)/(3-emptyAdjacentLMaps);
			
		//down left corner
		emptyAdjacentLMaps=0;
		if(m_dd==0) {emptyAdjacentLMaps++; dd_ul=0;}
		else dd_ul=m_dd->getHeight(0,0);
		if(m_dl==0) {emptyAdjacentLMaps++; dl_ur=0;}
		else dl_ur=m_dl->getHeight(LSIZE-1,0);
		if(m_ll==0) {emptyAdjacentLMaps++; ll_dr=0;}
		else ll_dr=m_ll->getHeight(LSIZE-1,LSIZE-1);
					
		if(emptyAdjacentLMaps==3) 
			cornerIsIsolated[3]=1;
		else
			m_heightMap[0][LSIZE-1]=(dd_ul+dl_ur+ll_dr)/(3-emptyAdjacentLMaps);
			
	}
    void seedIsolatedCorners(bool cornerIsIsolated[])
    {
		//Si aucun coin adjacent externe n'est rempli, le coin interne devient la moyenne des coins internes remplis.
			
		int isolatedCorners=0;
			for(int i=0; i<4; i++)
				if(cornerIsIsolated[i])
					isolatedCorners++;
				
		if(cornerIsIsolated[0]) //up left
		{
			m_heightMap[0][0]=keinOverflow((m_heightMap[LSIZE-1][0]+m_heightMap[LSIZE-1][LSIZE-1]+m_heightMap[0][LSIZE-1])/(4-isolatedCorners), 1024);
			isolatedCorners--;
		}
		if(cornerIsIsolated[1]) //up right
		{
			m_heightMap[LSIZE-1][0]=keinOverflow((m_heightMap[LSIZE-1][LSIZE-1]+m_heightMap[0][LSIZE-1]+m_heightMap[0][0])/(4-isolatedCorners), 1024);
			isolatedCorners--;
		}
		if(cornerIsIsolated[2]) //down right
		{
			m_heightMap[LSIZE-1][LSIZE-1]=keinOverflow((m_heightMap[0][LSIZE-1]+m_heightMap[0][0]+m_heightMap[LSIZE-1][0])/(4-isolatedCorners), 1024);
			isolatedCorners--;
		}
		if(cornerIsIsolated[3]) //down left
		{
			m_heightMap[0][LSIZE-1]=keinOverflow((m_heightMap[0][0]+m_heightMap[LSIZE-1][0]+m_heightMap[LSIZE-1][LSIZE-1])/(4-isolatedCorners), 1024);
			isolatedCorners--;
		}
	}
	void seedBorders()
	{
		//Pour assurer la continuité entre deux cartes adjacentes, 
		//il faut copier la ligne frontalière dans la nouvelle carte.
		//Il ne faut pas copier les coins.
		if(m_ll!=0)
			for(int y=1; y<LSIZE-1; y++)
				m_heightMap[0][y]=keinOverflow(m_ll->getHeight(LSIZE-1,y),1);
		if(m_uu!=0)
			for(int x=1; x<LSIZE-1; x++)
				m_heightMap[x][0]=keinOverflow(m_uu->getHeight(x,LSIZE-1),1);
		if(m_rr!=0)
			for(int y=1; y<LSIZE-1; y++)
				m_heightMap[LSIZE-1][y]=keinOverflow(m_rr->getHeight(0,y),1);
		if(m_dd!=0)
			for(int x=1; x<LSIZE-1; x++)
				m_heightMap[x][LSIZE-1]=keinOverflow(m_dd->getHeight(x,0),1);
	}
    

	short squareStep(int x, int y, int delta)
	{	
		int l=x-delta, r=x+delta, u=y-delta, d=y+delta;
		return (m_heightMap[l][u]+m_heightMap[l][d]+m_heightMap[r][d]+m_heightMap[r][u])/4;
	}
	short diamondStep(int x,int y,int delta)
	{
		int l=x-delta, r=x+delta, u=y-delta, d=y+delta;
		short ll, uu, rr, dd;
		bool emptyAdjacentLMap=0;
		
		if(l<0)
		{
			if(m_ll==0) {ll=0; emptyAdjacentLMap=1;}
			else ll=m_ll->getHeight(l+LSIZE,y);
		}
		else ll=m_heightMap[l][y];
					
		if(u<0)
		{	
			if(m_uu==0) {uu=0; emptyAdjacentLMap=1;}
			else uu=m_uu->getHeight(x,u+LSIZE);
		}
		else uu=m_heightMap[x][u];
		
		if(r>LSIZE-1)
		{
			if(m_rr==0) {rr=0; emptyAdjacentLMap=1;}
			else rr=m_rr->getHeight(r-LSIZE,y);
		}
		else rr=m_heightMap[r][y];
		
		if(d>LSIZE-1)
		{	
			if(m_dd==0) {dd=0; emptyAdjacentLMap=1;}
			else dd=m_dd->getHeight(x,d-LSIZE);
		}
		else dd=m_heightMap[x][d];
		
		return (ll+uu+rr+dd)/(4-emptyAdjacentLMap);
	}
	void diamondSquare()
    {
		int step = LSIZE-1;
		int halfStep, jStart;
		
		while(step>1)
		{
			halfStep=step/2;
					
			for(int x=halfStep; x<LSIZE; x+=step)
				for(int y=halfStep; y<LSIZE; y+=step)
						m_heightMap[x][y]=keinOverflow(squareStep(x,y,halfStep), step);

			for(int x=0; x<LSIZE; x+=halfStep)
			{
				jStart = ( (x/halfStep)%2 == 0 ) ? halfStep : 0;
				for(int y=jStart; y<LSIZE; y+=step)
					if((x!=0 || m_ll==0) && (y!=0 || m_uu==0) && (x!=LSIZE-1 || m_rr==0) && (y!=LSIZE-1 || m_dd==0))//Do not overwrite seeded borders.
						m_heightMap[x][y]=keinOverflow(diamondStep(x,y,halfStep), step);
			}
			
			step = halfStep;
		}
	}
	short keinOverflow(short tmpMean, int step)
	{
		if( tmpMean > (32767-9*step/m_var) )
			return tmpMean - step/m_var*(rand()%10);
			
		else if ( tmpMean < (-32768 + 9*step/m_var) )
			return tmpMean + step/m_var*(rand()%10);
			
		else					
			return tmpMean + step/m_var*(rand()%19-9);
	}	
	void show()
	{
		for(int y=0; y<LSIZE; y++)
		{
			for(int x=0; x<LSIZE; x++)
				cout << m_heightMap[x][y] << " ";
			cout << endl;
		}
		cout << endl;
	}
    void respectLocalBorders(int &x, int &y)
    {
		if(x<0) x=(x%LSIZE)+LSIZE;
		else x=x%LSIZE;
		
		if(y<0) y=(y%LSIZE)+LSIZE;
		else y=y%LSIZE;
	}
    
    //ATTRIBUTS
    int m_var;
    short m_heightMap[LSIZE][LSIZE];    
    LocalMap *m_ul, *m_uu, *m_ur, *m_rr, *m_dr, *m_dd, *m_dl, *m_ll;
};

#endif //LOCALMAP
