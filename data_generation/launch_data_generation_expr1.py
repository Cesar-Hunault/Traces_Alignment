#!/usr/bin/env python3

import subprocess
import argparse

parser = argparse.ArgumentParser(description='Arguments required for data generation')

parser.add_argument('-o', '--output', type=str, help='Output repertory name [default = Data]', default="Data")
parser.add_argument('-n', '--number', type=int, help='Number of files to generate [default = 1]', default=1)
parser.add_argument('-p', '--prefix', type=str, help='Common prefix of file identifiers [default = Dataset_]', default="Dataset_")
parser.add_argument('-g', '--guide', type=str, help='File guiding the generation [default = guide.txt]', default="guide.txt")


args = parser.parse_args()

output_repertory_name = args.output
file_number = args.number
common_file_prefix = args.prefix
name_guide_file = args.guide



subprocess.run(["./data_generation", output_repertory_name, str(file_number), common_file_prefix, name_guide_file])
