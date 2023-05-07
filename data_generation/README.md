# Data Generation for multiple sequence alignment

The 'data_generation.cpp' script allows you to generate structured data so that you can use the provided 'alignement_multiple.cpp' script. 

## Content of the Data generation directory

* A folder of 20 Datasets already generated
* The source files 'data_generation.cpp' and 'functions.h' in the src folder
* The binary file 'data_generation'
* The guide file to control the generation 'guide.txt'
* The executable script 'launch_data_generation_expr1.py' allowing to launch the generation

## Setup

To use the python files 'launch_data_generation_expr1.py' you must first check that all packages are installed on your machine.
To do this, run :

```
./setup.sh
```

## C++ Libraries
* iostream
* filesystem
* string
* fstream
* vector
* regex
* algorithm 
* random

## How the program works

The generation of the data is carried out according to the rules specified in an expression, present in the file 'guide.txt'. This file also allows to define the number of sequences to generate per dataset and their length. These sequences are also called traces. 

The binary file 'data_generation' is launched via the python script 'launch_data_generation_expr1.py'. This script allows to specify several parameters:
* The name of the directory containing the generated datasets
* The number of datasets to generate
* the prefix of each generated file
* the guide file containing the information defined above

To display the help message to see the use of the script and the values of the various default parameters: 

```
$ python3 launch_data_generation_expr1.py -h

usage: launch_data_generation_expr1.py [-h] [-o OUTPUT] [-n NUMBER] [-p PREFIX] [-g GUIDE]

Arguments required for data generation

options:
  -h, --help            show this help message and exit
  -o OUTPUT, --output OUTPUT
                        Output repertory name [default = Data]
  -n NUMBER, --number NUMBER
                        Number of files to generate [default = 1]
  -p PREFIX, --prefix PREFIX
                        Common prefix of file identifiers [default = Dataset_]
  -g GUIDE, --guide GUIDE
                        File guiding the generation [default = guide.txt]
```

## Example of use

Here are the steps used to generate the dataset provided with the script in the Data directory:

Initialization of the parameters in the file 'guide.txt':
```
#Description of param "expression"
<expression>;<(0-9)+> E4 <(0-6)*> E7 <(0-2)>

#Description of param "traces_number"
<traces_number>;20

#Description of param "traces_len"
<traces_len>;20
```

Launching the script 'launch_data_generation_expr1.py':
```
$ ./launch_data_generation_expr1.py -n 20 
```
With the default parameters -o, -p and -g. 