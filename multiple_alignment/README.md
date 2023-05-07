# Generate a Multiple alignment of event traces.

## Description

This C++ software allows to align a set of event traces. The method used is the multiple sequence alignment method, this method is based on the construction of a tree from a matrix of dissimilarities between sequences. The source code of the multiple alignment algorithm is entirely implemented in C++.
The launching code of the software is implemented in Pyhton3 (see section Example of use).</br>

>Authors: César HUNAULT, Romain LEVERGEOIS </br>
>Last update: 26/01/2023</br>
>>Based on the course of Pr C.Sinoquet 'Models Methods and Algorithms for Bioinformatics'.

The software folder contains :
* the executable file of the multiple alignment algorithm: '**alignement_multiple**' * the parameters file of the multiple alignment algorithm
* the alignment parameters file : '**alignment_params.txt**' whose synthax is presented below.
>
```
## Scores for insertions
# Score for insertion of a tick ('.')
ins_point 1
>
# Score for insertion of a dash ('-')
ins_dash 2
>
# Score for insertion of an event ('Ei' with i in [1-9])
ins_event 3
```
* the python file to launch the algorithm : '**launch_alignement.py**' which allows to launch the multiple alignment the datasets presents in '**data_generation/Data**'.
* the installation file of the python libraries : '**setup.sh**'.
* the folder containing the C++ source files of the multiple alignment algorithm : '**src/**'
* the file with the toy example : '**toy_example/**' this folder contains the multiple alignment parameters file with default values, the python script to launch the multiple alignment method and the executable file of the multiple alignment algorithm.

****************************************

## Setup

To use the python files '**launch_alignement_variante.py**' you must first check that all packages are installed on your machine.
To do this, run :

```
$ ./setup.sh
```

****************************************

## C++ Libraries
*  string.h
*  vector.h
*  fstream.h   
*  time.h
*  vector.h
*  filesystem.h
*  regex.h
*  cmath.h

****************************************

## Compilation

The executable file of the multiple alignment is present in the software folder, however if you need to recompile the source code of the multiple alignment algorithm please follow the instructions presented below.

You need the g++ compiler (installed by default on Linux systems).
```
$ g++ src/alignement_multiple.cpp -o toy_example/alignement_multiple
$ g++ src/alignement_multiple.cpp -o alignement_multiple
```
   
****************************************
    
## Example of use with a toy example (in a Linux shell)

When you are in the directory of the multiple alignment '**multiple_alignment/**', move to the toy example directory:
```
$ cd toy_example/
```

Then you just have to run the method launch file '**launch_alignement_toy_example.py**': 
```
$ python3 launch_alignement_toy_example.py
```
This will run the method with default settings. </br>

Result :
```
Reading toy_example.txt in process......OK
Reading alignment_params.txt in process......OK
****************************************
Multiple Alignment in process......OK
****************************************
Saving......OK
```

Display the alignment :
```
$ cat msa_toy_example.txt 
TN01   E1- - E2. . E3. . . - E4. . . . . . E7. . 
TN05   E1. . E2E8. E3. . . - E4. . . . . . E7. . 
TN02   E1. . E2. . . . . - - E4. . . . . - E7. - 
TN14   E1. . E2. . E3. . . - E4E5. . - - - E7. - 
TN03   E1- - - - - E3- . . . E4. . . . - - E7- - 
TN04   E1- - - - - E3- E2. - E4. . . - - - E7- - 
TN06   E1. . . - . E3E8E2. . E4. . . . . . E7- - 
TN07   E1. . . - - E3- E2E8- E4. . . . . . E7. . 
TN08   E1. . . . - E3- E2E8E2E4- - - - - - E7- - 
TN09   E1. . E8- - E3- E2E8E2E4- - - - - - E7- - 
TN10   - - - - - - - - - - - E4. E5. E6. . E7. - 
TN12   - - - - - - - - - - - E4. . - E6. . E7. - 
TN13   - - - - - - - - - - - E4. . - E6. E5E7. . 
TN11   - - - - - - - - - - - E4- E5. . - - E7. . 

```

Display the scores of the alignment :
```
$ cat scores.csv 
identifiant_fichier	score_nw	score_e	match_e	score_g proj_length rtime
toy_example.txt	1461	66	961	114	21	0.435733

```
- score_nw : sum of Needleman-Wunch scores of the global alignment of each pair of final projections
- score_e : sum of the number of events, for each event present at least twice in each column of the alignment
  example : score_e(toy_example) = 10 (E1) + 11 (E2) + 9 (E3) + 14 (E4) + 2 (E5) + 3 (E6) + 14 (E7) + 3 (E8)</br>
                                = 66
- match_e : sum of the number of matches for each pair of final projections
- score_g : total number of gaps on all projections

To print the help run :

```
$ python3 launch_alignement_toy_example.py -h
usage: launch_alignement_toy_example.py [-h] [-oa OUTPUT_ALIGNED]
                                        [-os OUTPUT_SCORES] [-t TRACE_FILE]
                                        [-p PARAMS_FILE]

options:
  -h, --help            show this help message and exit
  -oa OUTPUT_ALIGNED, --output_aligned OUTPUT_ALIGNED
                        Output file path of the alignment [default =
                        MSA_toy_example.txt]
  -os OUTPUT_SCORES, --output_scores OUTPUT_SCORES
                        Output file path of the alignment scores [default =
                        scores.csv]
  -t TRACE_FILE, --trace_file TRACE_FILE
                        path of the trace file [default = toy_example.txt]
  -p PARAMS_FILE, --params_file PARAMS_FILE
                        Path of the alignment parameters file [default =
                        alignment_params.txt]

```

****************************************

## Example of use with datasets (in a Linux shell)
When you are in the directory of the multiple alignment '**multiple_alignment/**', run the method launch file '**launch_alignement.py**' :
```
$ python3 launch_alignement.py <path_to_input_directory/> <path_to_output_directory/>
```

To print the help run :
```
$ python3 launch_alignement.py -h
usage: launch_alignement.py [-h] [-os OUTPUT_SCORES] [-p PARAMS_FILE]
                            trace_file output_aligned
                            
Arguments required for multiple alignment

positional arguments:
  trace_file            Path of the trace files directory
  output_aligned        Path of the output alignment directory

options:
  -h, --help            show this help message and exit
  -os OUTPUT_SCORES, --output_scores OUTPUT_SCORES
                        Output file path of the alignment scores [default =
                        scores.csv]
  -p PARAMS_FILE, --params_file PARAMS_FILE
                        Path of the alignment parameters file [default =
                        alignment_params.txt]
```
