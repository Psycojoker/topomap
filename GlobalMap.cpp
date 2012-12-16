#ifndef GLOBALMAP
#define GLOBALMAP

#include <iostream>
#include <fstream>

#include "LocalMap.cpp"

/* GlobalMap est un tableau qui contient des pointeurs vers les LocalMap.
 * Chaque LocalMap contient une heightMap, mais contiendra d'autres cartes
 * caractérisant la zone.
 * 
 * Attention aux coordonnées :
 * yx->
 * |
 * v
 */

const int SIZEX = 3;
const int SIZEY = 3;

class GlobalMap
{
    public:
    
    GlobalMap()
    {
		//Initialise all the pointers to zero.
        for(int x=0; x<SIZEX; x++)
            for(int y=0; y<SIZEY; y++)
                m_globalMap[x][y]=0;
    }
    ~GlobalMap()
    {
        for(int x=0; x<SIZEX; x++)
            for(int y=0; y<SIZEY; y++)
                delete m_globalMap[x][y];
    }
    bool genLocalMap(int x, int y)
    {	
		respectGlobalBorders(x,y);
		if(m_globalMap[x][y]==0)
		{
			//Send pointers to adjacents local maps. u=up, l=left, etc.
			LocalMap *ul, *uu, *ur, *rr, *dr, *dd, *dl, *ll;
			ul=getLocalMap(x-1, y-1); uu=getLocalMap(x, y-1); ur=getLocalMap(x+1, y-1);
			dl=getLocalMap(x-1, y+1); dd=getLocalMap(x, y+1); dr=getLocalMap(x+1, y+1);
			rr=getLocalMap(x+1, y); ll=getLocalMap(x-1, y);

			int var=1; //A fignoler en fonction de x,y.
			
			m_globalMap[x][y] = new LocalMap(var, ul, uu, ur, rr, dr, dd, dl, ll);
			return true;
		}
		else
		{
			cout << "There is already a LocalMap here (" << x << "," << y << ")." << endl;
			return false;
		}
		
    }
    LocalMap* getLocalMap(int x, int y)
    {	
		respectGlobalBorders(x,y);
        return m_globalMap[x][y];
    }
    void exportTxt(int x, int y)
	{
		LocalMap* lm=getLocalMap(x,y);
		
		char fileName[4]={'h', 'm', x+48, y+48};
		ofstream file(fileName);
		
		if(lm!=0)
		{
			for(int i=0; i<1025; i++)
			{
				for(int j=0;j<1025;++j)
					file << lm->getHeight(j,i) << " ";
				file << endl;
			}
			file << endl;
		}
		else
			file << "There is no LocalMap here (" << x << "," << y << ")." << endl;
				
	}

    private:
    
    void respectGlobalBorders(int &x, int &y)
    {
		if(x<0) x=(x%SIZEX)+SIZEX;
		else x=x%SIZEX;
		
		if(y<0) y=(y%SIZEY)+SIZEY;
		else y=y%SIZEY;
	}

	//ATTRIBUTS
    LocalMap* m_globalMap[SIZEX][SIZEY];
};

#endif //GLOBALMAP
