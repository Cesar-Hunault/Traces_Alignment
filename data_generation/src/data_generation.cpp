#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm> 
#include "functions.h"

using namespace std;

int main(int argc, char *argv[]){

    string output_repertory_name = argv[1];
    int file_number = stoi(argv[2]);
    string common_file_prefix = argv[3];
    string name_guide_file = argv[4];

    string output;

    //Lecture et parsing du guide_file****************************
    ifstream guide_file(name_guide_file);

    string line, expression;
    vector<string> expression_items_list;
    int traces_number, traces_len;

    smatch sm;
    regex pattern { R"(<(.+)>;(.+))" };

    while (getline(guide_file, line)){
        if (line[0]!='#' && line[0]!='\0'){
            if (!regex_match(line,sm,pattern)){
                cout<<"Erreur de synthaxe dans \'"<<line<<"\'"<<endl; //Faire la gestion  d'erreur ici c'est que le format ne correspond pas
                exit(1);
            }
            int a1 = line.find('<');
            int a2 = line.find('>');
            string s1 = line.substr(a1+1,a2-1);
            string s2 = line.substr(a2+2,line.length()-1);
            if (s1=="expression") {
                expression = s2;
            } else {
                if (s1=="traces_number"){
                    traces_number = stoi(s2);
                } else {
                    if (s1=="traces_len") {
                        traces_len = stoi(s2);
                    }
                }
            }
        }
    }
    //**********************************************************

    //Parsing de l'expression***********************************
    int a3=expression.find(' ');
    while (a3!=-1){
        expression_items_list.push_back(expression.substr(0,a3));
        expression.erase(expression.begin(),expression.begin()+a3+1);
        a3=expression.find(' ');
    }
    expression_items_list.push_back(expression);
    //**********************************************************

    // Ici il faut vérifier que les parties de l'expression n'étant  pas <truc> sont bien 'En' ou 'S'
    // si <min-max> max>9 alors il n'y aura pas d'évènement entre les ancres (du au événement limité à 9) mettre warning


    //Création répertoire de sortie*****************************
    filesystem::create_directory(output_repertory_name)?
    cout << "created directory - " << output_repertory_name << endl :
    cout << "create_directory() failed" << endl;
    //**********************************************************

    string res1, res2;
    fstream output_fstream;
    for (int q=0;q<file_number;q++){
        string filename = output_repertory_name+"/"+common_file_prefix+to_string(q)+".txt";
        output_fstream.open(filename, std::ios_base::out);
        if (!output_fstream.is_open()) {
            cerr << "Failed to open " << filename << '\n';
            exit(1);
        } else {
            vector<int> index_ancres,index_ancres_princ;
            for (int j=0; j<traces_number; j++){
                for (int k=0; k<expression_items_list.size(); k++){
                    res1 = expression_items_list[k];
                    if (res1[0]=='E' || res1[0]=='S'){
                        for (int l=0;l<res1.length();l++){
                            output.push_back(res1[l]);
                        }
                        index_ancres_princ.push_back(output.length()-2);
                        
                    }else{ 
                        //attention gérer erreur ici forcément type 'variable' = <truc>
                        res2 = choose_expression_type(res1);
                        for (int m=0; m<res2.length(); m++){
                            output.push_back(res2[m]);
                            if (res2[m]=='E') {
                                index_ancres.push_back(output.length()-1);
                            }
                        }
                    }
                }

                //Gestion longueur traces***********************************
                if (output.length()>(traces_len+index_ancres.size()+index_ancres_princ.size())){
                    int diff=output.length()-(traces_len+index_ancres.size()+index_ancres_princ.size());
                    int v=1;
                    for (int n=0; n<diff;n++){
                        if (output[output.length()-v]=='.') {
                            output.erase(output.begin()+output.length()-v);
                        } else {
                            for (int m=0; m<index_ancres_princ.size();m++) {
                                if (index_ancres_princ[m]==output.length()-(v+1)) {
                                    v = v+2;
                                    break;
                                }
                            }
                            output.erase(output.begin()+output.length()-(v+1),output.begin()+output.length()-(v-1));            
                        }
                    }
                }
                //**********************************************************
                
                output_fstream << output << endl;
                output.clear();
                index_ancres.clear();
                index_ancres_princ.clear();
            }
        }
        output_fstream.close();
    }
    //return(0);
}