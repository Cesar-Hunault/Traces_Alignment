#!/usr/bin/env python3

import subprocess
import argparse

parser = argparse.ArgumentParser()

parser.add_argument('-oa', '--output_aligned', type=str, help='Path of the output alignment file [default = msa_toy_example.txt]', default="msa_toy_example.txt")
parser.add_argument('-os', '--output_scores', type=str, help='Output file path of the alignment scores [default = scores.csv]', default="scores.csv")
parser.add_argument('-t', '--trace_file', type=str, help='Path of the trace file [default = toy_example.txt]', default="toy_example.txt")
parser.add_argument('-p', '--params_file', type=str, help='Path of the alignment parameters file [default = alignment_params.txt]', default="alignment_params.txt")

args = parser.parse_args()

output_aligned_file_name = args.output_aligned
output_scores_file_name = args.output_scores
trace_file_name = args.trace_file
params_file_name = args.params_file



subprocess.run(["./alignement_multiple", output_aligned_file_name, output_scores_file_name, trace_file_name, params_file_name])
