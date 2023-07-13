#ifndef ELE
#define ELE


//
// ele.h: header file for ele class
//
// Author:  Nourhan Berjawi
//
// An ele object represents a 3D element, defined by 4 nodes.
// These elements are outputted by TetGen's tetrahedralize method into a .ele file.
//
// Constructors:
//      ele()
//          default constructor
//
// Public Member Functions:
//
//      bool equals(ele b);
//          this function checks if the ele object is equal to another one b.
//          input:  ele type
//          output: boolean value
//
// Public Members:
//
//      int nodes[4]
//          this member stores integers that point to the locations in the node list/no file
//          for the 4 nodes of each element
//

class ele{
    public:
        
        int nodes[4] = {0};
        bool equals(ele b);
        ele(){};
        ~ele(){};
};


// function to read ele objects from a .ele file.
// input: 
//      - file_name: the name of the .ele file as a string 
//      - ele_list: pointer to array of ele objects
// output:
//      - none

// function to read nodes from a .node file.
void read_eles(string file_name, ele* ele_list);

// counts the number of elements in an .ele file, given the name of the file
int count_eles(string filename);

// writes a vector of elements to a file, given its name
void write_eles(vector<ele> &ele_list, string name);

#endif