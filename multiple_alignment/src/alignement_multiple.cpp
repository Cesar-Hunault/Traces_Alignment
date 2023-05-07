#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <filesystem>
#include "functions_alignment.h"

#define GREEN "\033[32m"
#define RESET "\033[0m"
#define RED "\033[31m"

using namespace std;

//**************************************************************
// ___  ___        _        
// |  \/  |       (_)       
// | .  . |  __ _  _  _ __  
// | |\/| | / _` || || '_ \ 
// | |  | || (_| || || | | |
// \_|  |_/ \__,_||_||_| |_|

int main(int argc, char** argv) {
    // Parameters of the algorithm
    // - the path of the event traces file,                                   argv[3]
    // - the path of the file of parameters specific to the alignment method, argv[4]
    // - the path of the alignment file,                                      argv[1]
    // - the path to the score file.                                          argv[2]
    string path_traces=argv[3];
    string path_params=argv[4];
    string path_MSA=argv[1];
    string path_align_scores=argv[2];

    // Reduction of the path size of the trace file.
    // We remove the parent directories to keep only the file name.
    string path_traces_aux=path_traces;
    while(path_traces_aux.find('/')<path_traces_aux.size() && path_traces_aux.find('/')>=0){
        path_traces_aux.erase(begin(path_traces_aux),begin(path_traces_aux)+path_traces_aux.find('/')+1);
    }

    //**********************************************************
    // Parsing of the dataset path_traces
    vector<vector <string>> seqs;         // This matrix stores the traces, the first dimension of the matrix 
                                          // corresponds to a set of items for example 'E1 ' or '. and the second 
                                          // dimension corresponds to the set of traces.
    cout<<"Reading ";
    cout<<RED<<path_traces_aux;
    cout<<RESET<<" in process...";

    read_traces_file(path_traces, &seqs);
    if(seqs.size()<2){
        cerr<<endl<<"Error : you need at least 2 sequences in the file \'"<<path_traces<<"\'."<<endl;
        exit(4);
    }

    cout<<"..."<<GREEN<<"OK"<<RESET<<endl;

    //**********************************************************
    // Parsing of the parameters file

    // Initialization of parameters with aberrant values.
    vector<pair<string, int>> params_map {{"ins_point",-1,},
                                          {"ins_dash",-1,},
                                          {"ins_event",-1,},
                                          {"del_point",-1,},
                                          {"del_dash",-1,},
                                          {"del_event",-1,},
                                          {"subst_events",-1,},
                                          {"subst_event_tick",-1,},
                                          {"subst_event_dash",-1,},
                                          {"subst_dash_tick",-1,},
                                          {"threshold_msa",-1}};
    cout<<"Reading ";
    cout<<RED<<path_params;
    cout<<RESET<<" in process...";

    read_param_file(path_params, params_map);

    cout<<"..."<<GREEN<<"OK"<<RESET<<endl;

    //**********************************************************
    //Multiple Sequence Alignment
    cout<<"****************************************"<<endl;
    cout<<"Multiple Alignment in process..."; 

    //Declaration of variables 
    float threshold=params_map[10].second; // Threshold of the difference between D_prec and D.

    int n=seqs.size();;                    // Integer corresponding to the number of iterations of 
                                           // the hierarchical ascending classification.

    vector<vector <int>> D_prec, D;        // 2D matrices, square of integers storing the values of the 
                                           // pairwise alignments of the first iteration of the multiple alignment.

    vector <pair <int, int>> Tree;         // List of pairs to agglomerate to form the multiple alignment.

    vector<vector <vector <string>>> MSA;  // The variable 'MSA' is of the same type as the varibal 'seqs' but 
                                           // it has an extra dimension to agglomerate the sequences.

    vector<vector <int>> al_order;         // 2D matrix that stores the order of the multiple alignment to identify 
                                           // the sequences in the alignment file, as for the variable 'MSA', we have 
                                           // an additional dimension to agglomerate the indices.
    
    bool convergence=false;                // Boolean that becomes true when the calculated difference between the D
                                           // of the last iteration of MSA and the current D. Output clause of the algorithm.

    // Initialization of the alignment order (TN01 is the first trace in the trace file (1st line)).
    for (int i=0 ; i<seqs.size(); i++){
        vector <int> TMP;
        TMP.push_back(i+1);
        al_order.push_back(TMP);
    }
        
    // Set the timer for the 'rtime' score for the alignement.
    float time = 0;

    // Initialization to 0 of the pairwise alignment matrices.
    init_square_vect2D_int(&D_prec, seqs.size());
    init_square_vect2D_int(&D, seqs.size());

    while (!convergence){
        // Calculate dissimilarities between each trace.
        vector<vector <int>> tmp;
        for (int i=0; i<seqs.size(); i++){
            vector<vector <string>> seq1; // When building 'D', 'seq1' is simply a trace but the function 
                                          // alignment_global_two_sequences_or_projections takes this type.
                                          // See the 'functions_alignment.cpp' file for more details on this function typing.
            seq1.push_back(seqs[i]);
            for (int j=i+1; j<seqs.size(); j++){
                vector<vector <string>> seq2;
                seq2.push_back(seqs[j]);
                alignement_global_deux_sequences_ou_projections(seq1, seq2, seq1[0].size(), seq2[0].size(), params_map, tmp);
                D[i][j]=tmp[seqs[i].size()][seqs[j].size()];
            }
        }     

        // Multiple alignment loop exit clause.
        if (difference(D_prec, D)<=threshold){ // See the 'functions_alignment.cpp' file for more details on this function.
            convergence = true;
        }

        // We "build" the tree for the classification of the traces.
        Tree = CAH(D,n-1);

        vector<vector <int>> ordre_al_tmp; // Temporary vector for pushing back in the main vector.
        for (int i=0 ; i<seqs.size(); i++){
            vector <vector <string>> tmp;
            tmp.push_back(seqs[i]);
            MSA.push_back(tmp);
        }

        if(al_order.size()<2){
            for (int i=0; i<al_order[0].size(); i++){
                vector <int> TMP;
                TMP.push_back(al_order[0][i]);
                ordre_al_tmp.push_back(TMP);
            }
        } else {
            ordre_al_tmp = al_order;
        }

        vector <vector <string>> seq1, seq2;

        // Building of the multiple alignment by agglomeration of pairs of traces
        // or projections following the pairs given by the HAC.
        for (int i=0; i<Tree.size(); i++) {
            int k = min({Tree[i].first,Tree[i].second});
            int r = max({Tree[i].first,Tree[i].second});
            seq1 = MSA[Tree[i].first];
            seq2 = MSA[Tree[i].second];
            vector<vector <int>> M;
            vector<vector <string>> ali;
            alignement_global_deux_sequences_ou_projections(seq1,seq2,seq1[0].size(),seq2[0].size(), params_map, M);
            affichage_un_meilleur_alignement_global(seq1,seq2,seq1[0].size(),seq2[0].size(),M, params_map, ali);
            MSA.erase(begin(MSA)+r);
            MSA[k]=ali;

            for (int j=0; j<ordre_al_tmp[r].size(); j++){
                ordre_al_tmp[k].push_back(ordre_al_tmp[r][j]);
            }
            ordre_al_tmp.erase(begin(ordre_al_tmp)+r);
        }

        D_prec = D;
        seqs = MSA[0];
        MSA.clear();

        al_order = ordre_al_tmp;
    }

    cout<<"..."<<GREEN<<"OK"<<RESET<<endl;
    cout<<"****************************************"<<endl;
    cout<<"Saving...";

    //**********************************************************
    // Saving the result of the multiple sequence alignment

    // Opening the alignement file.
    fstream msa_fstream(path_MSA, std::ios::out );
    if (!msa_fstream.is_open()) {
        cerr <<endl<< "Error : saving failed (failed to open file) " << path_MSA << endl;
        exit(1);
    }

    // Writing to the alignment file.
    for (int i=0; i < seqs.size(); i++){
        string nom_seq;
        if(al_order[0][i]<10){
            nom_seq="TN0"+to_string(al_order[0][i]); //'TN' for 'Trace Number'
        } else {
            nom_seq="TN"+to_string(al_order[0][i]);
        }
        msa_fstream<<nom_seq<<"   ";
        for (int j=0 ; j< seqs[i].size(); j++){
            msa_fstream<<seqs[i][j];
        }
        msa_fstream<<endl;
    }

    al_order.clear();
    msa_fstream.close(); 

    //**********************************************************
    // Alignement scores
    int score_nw=calcul_score_nw(D), score_e=calcul_score_e(seqs), match_e=calcul_match_e(seqs), score_g=calcul_score_g(seqs), proj_length = seqs[0].size();
    time = clock();
    float temps=time/1000000;

    //**********************************************************
    // Saving the Alignement scores
    string contenu_fichier, ligne;

    // Checking the existence of the score file.
    if (!filesystem::exists(path_align_scores)){
        fstream score_fstream(path_align_scores, std::ios::out );
        if (!score_fstream.is_open()) {
            cerr <<endl<< "Failed to save (failed to open file) " << path_align_scores << endl;
            exit(1);
        }
        // Writing headers if file doesn't exist.
        score_fstream<<"identifiant_fichier\tscore_nw\tscore_e\tmatch_e\tscore_g proj_length rtime"<<endl;
        score_fstream.close();
    }

    // Opening the score file.
    fstream score_fstream(path_align_scores, std::ios::app);

    // Writing in the score file. 
    stringstream oss;
    oss<< path_traces_aux + "\t" + to_string(score_nw) + "\t" + to_string(score_e) + "\t" + to_string(match_e) + "\t" + to_string(score_g) + "\t" + to_string(proj_length) + "\t" + to_string(temps) + "\n" <<score_fstream.rdbuf();
    score_fstream<<oss.str();
    score_fstream.close();
    seqs.clear();

    cout<<"..."<< GREEN << "OK"<<endl;

    return(EXIT_SUCCESS);
}