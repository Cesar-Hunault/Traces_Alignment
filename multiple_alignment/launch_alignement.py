#!/usr/bin/env python3

import subprocess
import argparse
from os import listdir
import os

parser = argparse.ArgumentParser()

parser.add_argument('-t','--trace_file', type=str, help='Path of the trace files directory with \'/\' at the end', required=True)
parser.add_argument('-oa','--output_aligned', type=str, help='Path of the output alignment directory with \'/\' at the end', required=True)
parser.add_argument('-os', '--output_scores', type=str, help='Output file path of the alignment scores [default = scores.csv]', default="scores.csv")
parser.add_argument('-p', '--params_file', type=str, help='Path of the alignment parameters file [default = alignment_params.txt]', default="alignment_params.txt")



args = parser.parse_args()

output_aligned_file_name = args.output_aligned
output_scores_file_name = args.output_scores
trace_file_name = args.trace_file
params_file_name = args.params_file

if(os.path.isdir(output_aligned_file_name)==False):
    os.mkdir(output_aligned_file_name)


for trace_file2 in listdir(trace_file_name):
    subprocess.run(["./alignement_multiple", output_aligned_file_name+"MSA_"+trace_file2, output_scores_file_name, args.trace_file+trace_file2, params_file_name])