# Simple-FASTQ-Validator-SFV-
A tool to quickly check for major errors in FASTQ file format
___________________________________________________________________________________________________________________________________________________________________

Use at your own risk.
I cannot provide support. All information obtained/inferred with this script is without any implied warranty of fitness for any purpose or use whatsoever.

ABOUT:

This program provides a quick way to validate a FASTQ file. This is useful for cases where checksums are not available, or you are merging several FASTQ files into one (e.g. if a sample was split across different flow cells). The program checks for truncated files, and also duplicated information that can indicate issues with merging data. A complete FASTQ file should have a total number of lines that is divisible by 4. File truncation may also result in the length of raw sequence letters not matching the length of quality values. The SFV program will report Yes or No if these conditions are met. If an error in merging data results in duplicated data, some FASTQ data will share the same sequence identifier. SFV also checks for this. However, this check can consume a large amount of memory depending on the size of the input file. This check can be turned off with a command line option if processing times become prohibitively slow.  

PREREQUISITES:  

1. bgzip 
2. C++ compiler (for example GCC/G++)

INSTALLATION:  

This program should work on all systems.  Dowload the .cpp file and compile according to your system.  For Linux Ubuntu, compile with g++ (g++ -o SFV_Simple_FASTQ_validator SFV_Simple_FASTQ_validator_V1_1.cpp).  

TO RUN:  
(examples below are using a short FASTQ file provided in the example data directory)
Note that a properly formatted file will result in a Yes result for all checks. Errors are indicated by No. 

Caution: This program is designed to be a quick and fast tool to check a FASTQ file. There are other programs that provide much more extensive evaluations, such as FastQC.  


1) If using a bgzip compressed file:  

To perform all three checks:

bgzip -dc SFV_shorttestOK.fastq.gz | ./SFV_Simple_fastq_validator 

To perform truncation but not duplication checks: 

bgzip -dc SFV_shorttestOK.fastq.gz | ./SFV_Simple_fastq_validator 0

2) If using an uncompressed file:

To perform all three checks: 
cat SFV_shorttestOK.fastq | ./SFV_Simple_fastq_validator 

To perform truncation but not duplication checks: 

cat SFV_shorttestOK.fastq | ./SFV_Simple_fastq_validator 0 

EXAMPLE DATA:

The example data directory contains example FASTQ files that are correctly formatted and with errors.  The .txt file in that directory provides examples of expected outputs for each file/scenario.   
