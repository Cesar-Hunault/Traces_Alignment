#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <cmath>

using namespace std;

//**************************************************************
// Procedure for reading the trace file
void read_traces_file(string path, vector<vector <string>> *seqs){
    
    // Opening the trace file.
    ifstream traces_file(path);
    if(!traces_file.is_open()){
        cerr<<endl<<"Error : can't open  "<<path<<endl; 
        exit(1);
    }

    // Reading the trace file line by line.
    string line;
    while (getline(traces_file, line)){
        vector <string> tmp;
        string s;

        //
        for(int i=0; i<line.length(); i++){
            if (line[i]=='.'){
                s = ". ";
                tmp.push_back(s);
            } else {
                if (line[i]=='E'){
                    string inter="E"+line.substr(i+1,1);
                    tmp.push_back(inter);
                } else {
                    if (!isdigit(line[i])){
                        cerr<<endl<<"Error : when reading the trace file: Unauthorized characters are used"<<endl;
                        exit(2);
                    }
                }
            }
        }
        seqs->push_back(tmp);
    }
    traces_file.close();
}


//**************************************************************
// Procedure for reading the data file
void read_param_file(string path, vector<pair<string, int>> &map ){

    // Opening the parameters file.
    ifstream param_file(path);
    if(!param_file.is_open()){
        cerr<<"Error : can't open  "<<path<<endl; 
        exit(1);
    }

    string line;
    smatch sm;
    int cpt=0;
    regex pattern { R"(([a-zA-Z_]+)([ ])([0-9]+))" };
    string verif = "ins_point ins_dash ins_event del_point del_dash del_event subst_events subst_event_tick subst_event_dash subst_dash_tick threshold_msa";
    // 'verif' is a string that includes the names of the admissible parameters in the alignment.
    
    // Reading the parameters file line by line.
    while (getline(param_file, line)){
        // If the line starts with a hash or if the line is empty, the line is ignored
        if (line[0]!='#' && line[0]!='\0'){
            // Verification de la synthaxe exemple : 'word_word 0' il faut que le séparateur soit un espace.
            if (!regex_match(line,sm,pattern)){
                cerr<<"Error : synthax error in \'"<<line<<"\'"<<endl;
                  exit(2);
            }

            int a1 = line.find(' ');
            string s1 = line.substr(0,a1);
            string s2 = line.substr(a1+1,line.length()-1);
            
            if (verif.find(s1)>verif.length()){
                cerr<<"Error : id of the parameter \'"<<s1<<"\' unknown."<<endl;
                exit(3);
            }

            // Counter that counts the number of updated parameters.
            for(int i=0; i<map.size(); i++){
                if (s1==map[i].first){
                    map[i].second = stoi(s2);
                    cpt++;
                } 
            }
        }
    }
    // As the parameters have been initialized with aberrant values, the user must be warned
    // if he has not filled in the value of a parameter in the alignment parameters file.
    if(cpt<11){
        cout<<endl<<"Warning: Please note that some parameters have not been found in the parameter file and will be initialized with aberrant values."<<endl;
    }
    param_file.close();
}

//**************************************************************
// Function finding the minimum of a real numbers matrix
// Search only in the upper rigth triangle.
pair<int, int> case_min_upper_triangle(vector<vector <float>> D_aux){
    pair<int, int> pair1;
    vector<int> index_min_lignes;
    vector<float> min_lignes;
    int i0,j0,cpt=1;

    for (int i=0; i<D_aux.size()-1;i++){
        index_min_lignes.push_back(distance(begin(D_aux[i]), min_element(begin(D_aux[i])+cpt, end(D_aux[i]))));
        min_lignes.push_back(*min_element(begin(D_aux[i])+cpt, end(D_aux[i])));
        cpt++;
    }

    pair1.first = distance(begin(min_lignes), min_element(begin(min_lignes), end(min_lignes)));
    pair1.second = index_min_lignes[pair1.first];

    return(pair1);
}


//**************************************************************
// Procedure for delete a column of a real numbers matrix 
void delete_column(vector<vector<float>> &Daux, int r){
    for (int i=0; i<Daux.size(); i++){
        Daux[i].erase(Daux[i].begin()+r);
    }
}


//**************************************************************
// Procedure for Initialization of a 2D square matrix of integers
void init_square_vect2D_int(vector<vector <int>> *D, int n){
    for (int i = 0; i < n; i++){
            vector<int> v1;
            for (int j = 0; j< n; j++){
                v1.push_back(0);
            }
            // Pushing back above 1D vector to create the 2D vector.
            D->push_back(v1);
    }
}


//**************************************************************
// Procedure for calculating the difference between two matrix of integers
float difference(vector<vector <int>> &D_prec, vector<vector <int>> &D){
    // initializing the difference.
    float d=0;

    for (int i=0; i<D.size()-1; i++){
        for (int j=i+1; j<D.size()-1; j++){
            d = d + ((D_prec[i][j]-D[i][j])*(D_prec[i][j]-D[i][j]));
        }
    }

    // The number of distinct pairs that can be formed 
    // from r elements is: r(r-1)/2 (sum of first integers).
    d = (2*d)/(D.size()*(D.size()-1));
    d = sqrt(d); 

    return(d);
}
