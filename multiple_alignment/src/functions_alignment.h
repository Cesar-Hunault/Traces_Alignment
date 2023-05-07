#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include "functions.h"

using namespace std;

//**************************************************************
//            _ _                                  _       __                  _   _                 
//      /\   | (_)                                | |     / _|                | | (_)                
//     /  \  | |_  __ _ _ __  _ __ ___   ___ _ __ | |_   | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//    / /\ \ | | |/ _` | '_ \| '_ ` _ \ / _ \ '_ \| __|  |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//   / ____ \| | | (_| | | | | | | | | |  __/ | | | |_   | | | |_| | | | | (__| |_| | (_) | | | \__ \
//  /_/    \_\_|_|\__, |_| |_|_| |_| |_|\___|_| |_|\__|  |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                 __/ |                                                                            
//                |___/    


//**************************************************************
// Function called when insertion                  
int ins(string base, vector<pair<string, int>> map){ 
    int res;
    switch(base[0]){
        case '.':
            res = map[0].second;
            break;
        case '-':
            res = map[1].second;
            break;
        case 'E':
            res = map[2].second;
            break;                
    }
    return(res);
}


//**************************************************************
// Function called when deletion                
int del(string base, vector<pair<string, int>> map){
    int res;
    switch(base[0]){
        case '.':
            res = map[3].second;
            break;
        case '-':
            res = map[4].second;
            break;
        case 'E':
            res = map[5].second;
            break;                
    }
    return(res);
}


//**************************************************************
// Function called when substitution
int subst(string base1, string base2, vector<pair<string, int>> map){
    int res;
    if (base1==base2){
        // Match.
        res=0;
    } else {
        // Mismatch.
        switch(base1[0]){
            case 'E' :
                switch(base2[0]){
                    case 'E':
                        res=map[6].second;
                        break;
                    case '-' :
                        res=map[8].second;
                        break;
                    case '.':
                        res=map[7].second;
                        break;
                }
                break;
            case '-' : 
                switch(base2[0]){
                    case 'E':
                        res=map[8].second;
                        break;
                    case '.' :
                        res=map[9].second;
                        break;
                }
            case '.' : 
                switch(base2[0]){
                    case 'E' :
                        res=map[7].second;
                        break;
                    case '-' :
                        res=map[9].second;
                        break;
                }
        }
    }
    return(res);
}


//**************************************************************
// Function for display best pairwise alignement from an alignement matrix.
// Can fit sequences or projections of aligned sequences,
// Return the graphical representation of the alignment
// from a score alignement matrix.
void affichage_un_meilleur_alignement_global(vector<vector <string>> seq1, 
                                             vector<vector <string>> seq2,
                                             int n,
                                             int m,
                                             vector<vector <int>> M,
                                             vector<pair<string, int>> map, // for calling the functions ins,del,subst.
                                             vector<vector <string>> &ali){
    int i=n;
    int j=m;

    vector<vector <string>> al;
    vector<vector <string>> string1;
    vector<vector <string>> string2;
    vector <string> ss_string1;
    vector <string> ss_string2;

    for (int i=0;i<seq1.size(); i++){
        string1.push_back(ss_string1);
    }

    for (int i=0;i<seq2.size(); i++){
        string2.push_back(ss_string2);
    }

    while (i!=0 && j!=0){
        int somme_ins=0;
        for (int k=0; k<seq2.size(); k++){
            somme_ins = somme_ins + ins(seq2[k][j-1], map);
        }
        somme_ins = somme_ins/seq2.size();  
        if (M[i][j]==M[i][j-1] + somme_ins){
            for (int k=0; k<string1.size(); k++){
                string1[k].insert(string1[k].begin(),"- ");
            }
            for (int l=0; l<string2.size(); l++){
                string2[l].insert(string2[l].begin(),seq2[l][j-1]);
            }
            j--;
            continue;
        }

        int somme_dels=0;
        for (int k=0; k<seq1.size(); k++){
            somme_dels = somme_dels + del(seq1[k][i-1], map);
        }
        somme_dels = somme_dels/seq1.size();
        if (M[i][j]==M[i-1][j] + somme_dels){
            for (int k=0; k<string1.size(); k++){
                string1[k].insert(string1[k].begin(),seq1[k][i-1]);
            }
            for (int l=0; l<string2.size(); l++){            
                string2[l].insert(string2[l].begin(),"- ");
            }
            i--;
            continue;
        }
        // M[i,j] is necessarily equal to n[i-1,j-1] + subst(S1[i-1], S2[j-1]).
        for (int k=0; k<string1.size(); k++){
            string1[k].insert(string1[k].begin(),seq1[k][i-1]);
        }
        for (int l=0; l<string2.size(); l++){ 
            string2[l].insert(string2[l].begin(),seq2[l][j-1]);
        }
        i--;j--;
    }
    // The previous loop stopped at the northern border (j=0).
    // The previous loop stopped at the western border (i=0).
    // Post condition : i=0 or j=0.
    while (i!=0){
        for (int k=0; k<string1.size(); k++){
            string1[k].insert(string1[k].begin(),seq1[k][i-1]);
        }
        for (int l=0; l<string2.size(); l++){ 
            string2[l].insert(string2[l].begin(),"- ");
        }
        i--;
    }
    while (j!=0){
        for (int k=0; k<string1.size(); k++){
            string1[k].insert(string1[k].begin(),"- ");
        }
        for (int l=0; l<string2.size(); l++){ 
            string2[l].insert(string2[l].begin(),seq2[l][j-1]);
        }       
        j--;
    }   

    for (int k=0; k<string1.size(); k++){
        al.push_back(string1[k]);
    }
    for (int l=0; l<string2.size(); l++){ 
        al.push_back(string2[l]);
    }   

    // The previous loop stopped at the northern border (j=0).
    ali=al;
}


//**************************************************************
// Function for pairwise alignement
// Can fit sequences or projections of aligned sequences,
// Return the score alignement matrix.
void alignement_global_deux_sequences_ou_projections(vector<vector <string>> seq1, 
                                                     vector<vector <string>> seq2, 
                                                     int n,
                                                     int m, 
                                                     vector<pair<string, int>> map, // for calling the functions ins,del,subst.
                                                     vector<vector <int>> &Mat){
    // Declaration of the 2D matrix.
    vector<vector <int>> M;
    
    // We initialize to 0 all the matrix of dim (n+1)*(m+1).
    for (int i = 0; i < n+1; i++){
        vector<int> v1;
        for (int j = 0; j<m+1; j++){
            v1.push_back(0);
        }
        // Pushing back above 1D vector to create the 2D vector.
        M.push_back(v1);
    }
    M[0][0] = 0;

    for (int i=1; i<n+1; i++){
        int somme_dels=0;
        for (int j=0; j<seq1.size(); j++){
            somme_dels = somme_dels + del(seq1[j][i-1], map);
        }
        somme_dels = somme_dels/seq1.size();
        M[i][0] = M[i-1][0] + somme_dels; //deletion
    }

    for (int j=1; j<m+1;j++){
        int somme_ins=0;
        for (int k=0; k<seq2.size(); k++){
            somme_ins = somme_ins + ins(seq2[k][j-1], map);
        }
        somme_ins = somme_ins/seq2.size();        
        M[0][j] = M[0][j-1] + somme_ins; //insertion
        
        for (int i=1; i<n+1; i++){
            int somme_ins2=0, somme_subst=0;
            for (int l=0; l<seq2.size(); l++){
                somme_ins2 = somme_ins2 + ins(seq2[l][j-1], map);
                for (int m=0; m<seq1.size(); m++){
                    somme_subst = somme_subst + subst(seq1[m][i-1],seq2[l][j-1], map);
                }
            }
            somme_ins2 = somme_ins2/seq2.size();        
            somme_subst = somme_subst/(seq2.size()*seq1.size());
            int somme_dels=0;
            for (int n=0; n<seq1.size(); n++){
                somme_dels = somme_dels + del(seq1[n][i-1], map);
            }
            somme_dels = somme_dels/seq1.size();
            M[i][j] = min({M[i][j-1]+somme_ins2,M[i-1][j]+somme_dels, M[i-1][j-1]+somme_subst});
            // The cell n[n+1,m+1] contains the score of the best alignment or alignments.
        }
    }
    Mat=M;
}


//**************************************************************
// Function for calculating dissimilarities
// We use the exact Solution developed as shown in the example:
// dissim(((27,43),((1,2),18)), (5,6)) = (dissim(27,5) + dissim(27,6) + dissim(43,5) + dissim(43,6) + dissim(1,5) + dissim(1,6) + dissim(2,5) + dissim(2,6) + dissim(18,5) + dissim(18,6) )/ 10
// where the dissimilarity between the trace at index 27 and the trace at index 5 is the score of the best alignment between these two traces.
float dissim(int i0, int j0, int I, vector<vector <int>> D, vector <vector<int>> vect){
    int mini=min({i0,j0});
    float res=0.0;
    vector<int> G1=vect[mini];
    vector <int> G2=vect[I];

    for (int i=0; i<G1.size(); i++){

        for (int j=0; j<G2.size(); j++){
            int ent1 = G1[i];
            int ent2 = G2[j];
            if (ent1<=ent2){
                res = res + D[ent1][ent2];
            } else {
                res = res + D[ent2][ent1];
            }
        }
    }

    res = res / (G1.size()*G2.size());
 
    return(res);
} 


//**************************************************************
// Procedure for the Hierarchical Ascending Classification (CAH in french)
vector <pair <int, int>> CAH(vector<vector <int>> &D, int n){
    
    // Initialization of matrices 
    vector<vector <int>> D_ini = D;
    vector<vector <float>> D_aux;           // We pass on a matrix of real because the dissim function 
                                            // returns an average thus a real.
    for (int i=0; i<D_ini.size(); i++){
        vector <float> tmp;
        for (int j=0; j<D_ini[i].size(); j++){
            int i2 = D_ini[i][j];
            float f = (float)i2;
            tmp.push_back(f);
        }
        D_aux.push_back(tmp);
    }

    // Initialization of the tree used for the dissim function.
    int k,r,iter = 0;
    vector<vector <int>> tree;
    for (int i=0; i<D_aux.size();i++){
        vector <int> tmp;
        tmp.push_back(i);
        tree.push_back(tmp);
    }

    // Initialization of the outuput.
    // It's a vector of pairs to agglomerate to form the multiple alignment.
    vector <pair <int, int>> res;

    // Starting classification
    while (iter<n){
        // Searching for the minimum in the matrix.
        pair<int, int> case_min = case_min_upper_triangle(D_aux);

        // The new item obtained by agglomeration of items i0=case_min.first and j0=case_min.second
        // will be positioned at index k in D_aux updated.
        k = min(case_min.first,case_min.second);
        r = max(case_min.first,case_min.second);
        
        for (int i=0; i<tree[r].size();i++){
            tree[k].push_back(tree[r][i]);
        }

        tree.erase(tree.begin()+r);
        res.push_back(case_min);
        D_aux.erase(D_aux.begin()+r);
        delete_column(D_aux, r);
    
        // Update of the values of column k and row k.
        for (int i=0; i<k; i++){
            D_aux[i][k] = dissim(case_min.first,case_min.second,i,D_ini, tree);
        }
        for (int j=k+1; j<D_aux[k].size(); j++){
            D_aux[k][j] = dissim(case_min.first,case_min.second,j, D_ini, tree);
        }
        iter++;
    }
    return(res);
}


//**************************************************************
// Fonction for calculating the sum of the needlemann-wunch scores 
// for each pair of aligned traces (including gaps).
int calcul_score_nw(vector<vector <int>> D){
    //alignement scores
    // initializing the score.
    int score_nw=0;           

    // For each sequence i:
    for (int i=0; i<D.size(); i++){
        // For each sequence j > i :
        for (int j=i+1; j<D[i].size(); j++){
            score_nw=score_nw+D[i][j];
        }
    }
    return(score_nw);
}


//**************************************************************
// Procedure for calculating event score
// If a column of the multiple alignment contains at least two occurrences 
// of an event, we count the number of occurrences of this event on the whole column.
// The number of occurrences of this event on the whole column. The "events" score is 
// obtained by summing up the results obtained on all the concerned columns.
int calcul_score_e(vector<vector <string>> seqs){
    // initializing the score.
    int cpt=0, score_e=0;

    // For each event (Ei with i in [1-9]):
    for (int k=1; k<10; k++){
        // For each column of a sequence:
        for (int i=0; i < seqs[0].size(); i++){
            for (int j=0; j<seqs.size(); j++){
                if (seqs[j][i]=="E"+to_string(k)){
                    cpt++;
                }
            }
            if (cpt>=2) {
                score_e=score_e+cpt;
            }
            cpt=0;
        }
    }
    return(score_e);
}


//**************************************************************
// Procedure for calculating event match score
// The event match score is the sum of the number of match_events for all pairs of projections.
int calcul_match_e(vector<vector <string>> seqs){
    // initializing the score.
    int match_e=0;

    // For each sequence i :
    for(int i=0; i<seqs.size()-1; i++){ 
        // For each sequence j > i :
        for (int j=i+1; j<seqs.size(); j++){
            // For each items of aligned sequences i et j :
            for (int k=0; k<seqs[0].size(); k++){
                // If match :
                if (seqs[i][k]==seqs[j][k]){
                    match_e++;
                }
            }
        }
    }
    return(match_e);
}


//**************************************************************
// Procedure for calculating gap score
// The gap score is the total number of gaps on all projections.
int calcul_score_g(vector<vector <string>> seqs){
    // initializing the score.    
    int score_g=0;

    // For each sequences :
    for (int i=0; i<seqs.size(); i++){
        // For each items of the sequence :
        for (int j=0; j<seqs[i].size(); j++){
            // If the item is a gap :
            if (seqs[i][j]=="- "){
                score_g++;
            }
        }
    }
    return(score_g);
}