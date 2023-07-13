#ifndef FACE
#define FACE
//
// face.h: header file for face class
//
// Author:  Nourhan Berjawi
//
// A face object represents a 3D triangle, defined by 3 nodes.
// These faces are outputted by TetGen's tetrahedralize method into a .face file.
//
// Constructors:
//
//      face()
//          default constructor
//
// Public Member Functions:
//
//      bool equals(node b);
//          this function checks if the node object is equal to another one b.
//          input:  node type
//          output: boolean value
//      
//      the indexing [ ] operator has been overloaded to access the memeber coords
//
// Public Members:
//
//      double coords [3]
//          this member stores doubles representing the 3D coordinates of a point
//

class face {
    public:

        // array of node indeces in correspoing node array
        int nodes[3] = {0};

        // default constructor
        face(){}
        ~face(){}

};


// function to read faces from a .face file.
// input: 
//      - file_name: the name of the .face file as a string 
//      - face_list: pointer to array of face objects
//      - face_count: size of array
// output:
//      - none

// function to read faces from a .face file.
void read_faces(string file_name, face* face_list, int face_count);

// count the total number of faces in a file
int count_faces(string filename);

// write faces to a file
void write_faces(vector<face> &face_list, string name);
#endif