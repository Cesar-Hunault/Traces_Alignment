#include <string>
#include <random>
#include <vector>
#include <regex>

using namespace std;

//*************************************************************
//Variables globales
vector<char> suffix_Ev = {'1','2','3','4','5','6','7','8','9'};
//*************************************************************

//*************************************************************
//Description fonction
string generate_expression_type_none(int min, int max){
    string output;
    
    uniform_int_distribution<int> distrib( min, max );
    random_device rd;
    default_random_engine re(rd());
    
    int rand_int = distrib(re); 
    
    for (int i=0; i<rand_int;i++){
        output.push_back('.');
    }
    
    return(output);
}
//*************************************************************


//*************************************************************
//Description fonction
string generate_expression_type_star_or_plus(int min, int max, int selector){
    uniform_int_distribution<int> distrib_nb_pts( min, max );
    random_device rd; //pour que les valeurs retourné ne soient pas les memes a chaque fois
    default_random_engine re(rd());
    int nb_pts = distrib_nb_pts(re); int a=0;
    
    if (selector==1){ //si selecteur =1 on est en type plus donc a =1
        a=1;
        if (nb_pts==0){
            nb_pts=1;
        }
    }
    uniform_int_distribution<int> distrib_nb_Ev( a, nb_pts );
    int nb_Ev = distrib_nb_Ev(re);
    if (nb_Ev==0){
        return(generate_expression_type_none(min,max)); // si le nb d'evenement est 0 on apelle la fct de base
    }

    vector<int> index_Ev;
    vector<int> tabl_index;
    for (int n=0; n<nb_pts; n++){ //on créé un tableau contenant des index que l'on va supprimer quand il est utiliser
        tabl_index.push_back(n);  //pour éviter que 2 évènements est le meme index car peut conduire a des erreurs
    }

    for (int i=0; i<nb_Ev; i++){
        uniform_int_distribution<int> distrib( 0, tabl_index.size()-1 ); int test = distrib(re);
        index_Ev.push_back(tabl_index[test]); tabl_index.erase(tabl_index.begin()+test);
    }
    
    string output; bool trouve;
    for (int j=0; j<nb_pts;j++){
        for (int o=0; o<nb_Ev; o++){
            if (j==index_Ev[o]){
                trouve = true; int m = o;             
            }
        }
        if (trouve==true){
            output.push_back('E'); output.push_back(suffix_Ev[0]); suffix_Ev.erase(suffix_Ev.begin());
            trouve=false;
        }else{
            output.push_back('.');
        }
    }
    suffix_Ev = {'1','2','3','4','5','6','7','8','9'};
    return(output);
}
//*************************************************************


//*************************************************************
//Description fonction
string choose_expression_type(string item){
    //item exemple : '<(3-6)+>' '<(3-6)*>' '<(3-6)>'
    //Variables locales 
    string range, output, expression_type;
    int selector;  
    smatch sm;
    regex pattern { R"(<\(([0-9]+)(-)([0-9]+)\)([\+\*]*)>)" };
    
    if (!regex_match(item,sm,pattern)){
        cout<<"Erreur de synthaxe dans \'"<<item<<"\'"<<endl; //Faire la gestion  d'erreur ici c'est que le format ne correspond pas
        exit(1);
    }

    int p1 = item.find('(');
    int p2 = item.find(')');
    int n = item.find('-');
    string s1 = item.substr(p1+1,n-1);
    string s2 = item.substr(n+1,p2-n-1);
    int min = stoi(s1);
    int max = stoi(s2);

    if (min>max) {
        cout<<"Erreur de synthaxe dans \'"<<item<<"\', la valeur minimale doit etre en première"<<endl; // faire la gestion derreur ici
        exit(2);
    }

    if ((item.find('*')<7)) {
        output = generate_expression_type_star_or_plus(min,max,2);
    } else {
        if ((item.find('+')<7)) {
            output = generate_expression_type_star_or_plus(min,max,1);
        } else {
            output = generate_expression_type_none(min,max);
        }
    } 

    return(output);
}