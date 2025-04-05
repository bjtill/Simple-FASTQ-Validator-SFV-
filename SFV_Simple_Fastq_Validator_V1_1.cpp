//SFC Simple Fastq Validator, Version 1.0 BT. April 4, 2025
//To compile in Linux:  g++ -o SFC_Simple_fastq_validator SFC_Simple_fastq_validator_V1_1.cpp
//Run without arguments to check for duplicate sequence IDs add the argument 0 to skip this analysis (if the program is runningly slowly on your system)
//Usage with bgzipped files with all checks:  bgzip -dc 1902_subset1.fastq.gz | ./SFC_Simple_fastq_validator
//Usage with bgzipped files with no dup check:  bgzip -dc 1902_subset1.fastq.gz | ./SFC_Simple_fastq_validator 0
// Usage with uncompressed files with all checks: cat 1902_subset1.fastq | ./SFC_Simple_fastq_validator 
// Usage with uncompressed files with no dup check: cat1902_subset1.fastq | ./SFC_Simple_fastq_validator 0 
//todo: get public fastq files and subset and tweak.  

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

// Function to split a string by a delimiter
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char *argv[]) {
    int check_dups = 1; // Default value for duplicate check
    if (argc > 1) {
        check_dups = stoi(argv[1]); // Set check_dups based on command line argument
    }

    string line;
    int line_count = 0;
    int reads = 0;
    int seq_qual_errors = 0;
    int dup_errors = 0;
    bool seq_qual_valid = true;
    bool dup_check_valid = true;

    unordered_map<string, int> id_seen; // Store seen sequence IDs

    int seq_len = 0;
    int qual_len = 0;
    bool lines_valid = true;

    while (getline(cin, line)) {
        line_count++;
        int line_num = line_count % 4;

        if (line_num == 1) {  // Header line
            if (check_dups) {
                // Extract sequence ID (remove '@' and get the first part)
                vector<string> header_parts = split(line, ' ');
                string seq_id = header_parts[0].substr(1);  // Remove the '@' character

                // Check for duplicate IDs
                if (id_seen.find(seq_id) != id_seen.end()) {
                    dup_check_valid = false;
                    dup_errors++;
                    if (dup_errors <= 10) {
                        cout << "Duplicate ID found: " << seq_id << " (first seen at read " << id_seen[seq_id] << ", again at read " << (reads + 1) << ")" << endl;
                    } else if (dup_errors == 11) {
                        cout << "Too many duplicate errors found. Stopping duplicate checking..." << endl;
                    }
                }
                id_seen[seq_id] = reads + 1;
            }
        } else if (line_num == 2) {  // Sequence line
            seq_len = line.length();
        } else if (line_num == 0) {  // Quality line
            qual_len = line.length();
            reads++;

            // Check if sequence length matches quality length
            if (seq_len != qual_len) {
                seq_qual_valid = false;
                seq_qual_errors++;
                if (seq_qual_errors <= 10) {
                    cout << "Error at read " << reads << ": Sequence length (" << seq_len << ") != Quality length (" << qual_len << ")" << endl;
                } else if (seq_qual_errors == 11) {
                    cout << "Too many sequence-quality errors found. Stopping error reporting..." << endl;
                }
            }
        }
    }

    // Check if line count is divisible by 4 (valid FASTQ format)
    if (line_count % 4 != 0) {
        lines_valid = false;
    }

    // Print results


    // Print summary
    cout << "\nSummary:" << endl;
    cout << "- Processed " << reads << " reads (" << line_count << " lines)" << endl;
    cout << "- Expected line count divisible by 4: " << (lines_valid ? "Yes" : "No") << endl;
    cout << "- All sequence lengths match quality lengths: " << (seq_qual_valid ? "Yes" : "No") << endl;

    if (check_dups) {
        cout << "- No duplicate sequence IDs: " << (dup_check_valid ? "Yes" : "No") << endl;
    } else {
        cout << "- No duplicate sequence IDs: NA (check skipped)" << endl;
    }

    // Set exit code - 0 if all checked validations passed, 1 if any failed
    if (seq_qual_valid && lines_valid && (dup_check_valid || !check_dups)) {
        return 0;
    } else {
        return 1;
    }
}

