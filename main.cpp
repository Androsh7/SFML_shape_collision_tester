#include <vector>

// creates the point notation
// (x,y)
struct point {
  float x;
  float y;
};
// creates the line notation
// ((x,y),(x,y))
struct line {
  point p1;
  point p2;
};

// create the poly class
class poly {
    public:
    int length = 0;
    std::vector<line> polygon;
    std::vector<line> backup;
    
    // sorts the polygon based on the first point
    void sortPoly (){
        
        // copies the polygon array to the backup array
        for (int i = 0; i < polygon.size();i++){
            backup.push_back(polygon[i]);
        }
        
        // wipes the polygon array
        polygon.clear();
        
        // places the array in the polygon sorted
        int PolygonWriteNum = 0;
        for (int BackupReadNum) {
            while ( PolygonWriteNum < backup.size()){
                if ((polygon.length - 1) < i){
                    polygon.push_back(backup[0]);    
                } else {
                    
                }
                PolygonWriteNum++;
            }
            }
            BackupReadNum++;
            PolygonWriteNum = BackupReadNum;
        }
    }
    
    // adds a line at the end of the array
    void addLine (line inputLine){
        polygon.push_back(inputLine);
    }
    
    // removes the last line of the array
    void rmLast (){
        polygon.pop_back();
    }
    
    // 
    void rmLineNum (int lineNum){
        polygon.erase(polygon.begin() + lineNum);
    }
};

int main(){
    poly player;
    poly entity;
}
