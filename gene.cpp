#include <string>
#include <iostream>
#include <vector>

using namespace std;

//g++ -Wall -Werror -pedantic -std=c++1y -o gene gene.cpp

struct elem {
	string original_protein;
	string changed_protein;
	string polypeptide_original;
	string polypeptied_changed;
	int index;
};

vector<vector<string> > codon_matrix { { "Phe", "Ser", "Tyr", "Cys"},
										{"Phe", "Ser", "Tyr", "Cys"},
										{"Leu", "Ser", "Stop", "Stop"},
										{"Leu", "Ser", "Stop", "Stop"},
										{"Leu", "Pro", "His", "Arg"},
										{"Leu", "Pro", "His", "Arg"},
										{"Leu", "Pro", "Gln", "Arg"},
										{"Leu", "Pro", "Gln", "Arg"},
										{"Ile", "Thr", "Asn", "Ser"},
										{"Ile", "Thr", "Asn", "Ser"},
										{"Ile", "Thr", "Lys", "Arg"},
										{"Met", "Thr", "Lys", "Arg"},
										{"Val", "Ala", "Asp", "Gly"},
										{"Val", "Ala", "Asp", "Gly"},
										{"Val", "Ala", "Glu", "Gly"},
										{"Val", "Ala", "Glu", "Gly"} };

string transcription(string single_strand) {
	string transcribed_strand = "";

	for (int i = 0; i < single_strand.size(); i++) {
		if (single_strand[i] == 'A') {
			transcribed_strand += 'U';
		}
		else if (single_strand[i] == 'T') {
			transcribed_strand += 'A';
		}
		else if (single_strand[i] == 'G') {
			transcribed_strand += 'C';
		}
				else if (single_strand[i] == 'C') {
			transcribed_strand += 'G';
		}
		else {
			return "Error in transcription. Single Strand DNA must contain only 'A', 'G', 'C', or 'T'";
		}
	}
	return transcribed_strand;
}

string determines_protein(string codon) {
	if (codon.length() != 3) {
		return "Error";
	}

	int row = 0;
	int col = 0;

	if (codon[0] == 'U') {
		row = 3;
	}
	else if (codon[0] == 'C') {
		row = 7;
	}
	else if (codon[0] == 'A') {
		row = 11;
	}
	else if (codon[0] == 'G') {
		row = 15;
	}
	else {
		cout << "Error unexpected base" << '\n';
		exit(0);
	}

	if (codon[2] == 'U') {
		row -= 3;
	}
	else if (codon[2] == 'C') {
		row -= 2;
	}
	else if (codon[2] == 'A') {
		row -= 1;
	}
	else if (codon[2] == 'G') {
		row -= 0;
	}
	else {
		cout << "Error unexpected base" << '\n';
		exit(0);
	}


	if (codon[1] == 'U') {
		col = 0;
	}
	else if (codon[1] == 'C') {
		col = 1;
	}
	else if (codon[1] == 'A') {
		col = 2;
	}
	else if (codon[1] == 'G') {
		col = 3;
	}
	else {
		cout << "Error unexpected base" << '\n';
		exit(0);
	}

	return codon_matrix[row][col];

}


string read_transcription(string transcribed_strand) {
	string proteins = "";
	string codon = "";

	for (int i = 0; i < transcribed_strand.size(); i++) {
		codon += transcribed_strand[i];
		if (codon.size() == 3) {
			proteins += determines_protein(codon) + " ";
			codon = "";
		}
	}
	return proteins;
}




elem compare_read_transcription(string strand1, string strand2) {
	string polypep1 = "";
	string polypep2 = "";
	string codon1 = "";
	string codon2 = "";

	string orig_prot = "";
	string change_prot = "";
	int index = 0;

	int size = 0;
	if (strand1.size() > strand2.size()) {
		size = strand2.size();
	}
	else {
		size = strand1.size();
	}

	for (int i = 0; i < size; i++) {
		codon1 += strand1[i];
		codon2 += strand2[i];
		if (codon1.size() == 3 && codon2.size() == 3) {
			polypep1 += determines_protein(codon1) + " ";
			polypep2 += determines_protein(codon2) + " ";

			if (determines_protein(codon1) != determines_protein(codon2)) {
				index = i;
				orig_prot = determines_protein(codon1);
				change_prot = determines_protein(codon2);
			}

			codon1 = "";
			codon2 = "";
		}
	}

	elem final_proteins;
	final_proteins.polypeptide_original = polypep1;
	final_proteins.polypeptied_changed = polypep2;
	final_proteins.original_protein = orig_prot;
	final_proteins.changed_protein = change_prot;
	final_proteins.index = index - 2;

	return final_proteins;
}


string determine_mutation(string original, string transcripted) {
	string compare = transcription(original);
	if (compare == transcripted) {
		cout << "NO MUTATION" << '\n';
		cout << "No mutations exist in trasncription; however, there could exist mutations elsewhere in the replication process" << '\n';
	}
	else {
		bool first_position_identified = false;
		int save_index = 0;
		int num_diff = 0;

		for (int i = 0; i < compare.size(); i++) {
			if (compare[i] != transcripted[i]) {
				if (first_position_identified == false) {
					save_index = i;
				}
				first_position_identified = true;
				num_diff++;
			}
		}
		elem protein;
		protein = compare_read_transcription(compare, transcripted);

		if (compare.size() == transcripted.size()) {
			//Point Mutation
			if (num_diff == 1) {

				if (protein.original_protein == protein.changed_protein) {
					cout << "SILENT MUTATION" << '\n';
					cout << "There was a point mutation in the transcription process; however, the new codon maps to the original, meaning there is no change in polypeptide chain." << '\n';
				}
				else if (protein.original_protein != "Stop" && protein.changed_protein == "Stop") {
					cout << "NONSENSE MUTATION" << '\n';
					cout << "The polypeptide chain ended before it was supposed to. A point mutation caused a stop codon to appear too early at position " << protein.index << '\n' << '\n';
					cout << "The chain is supposed to be: " << protein.polypeptide_original << '\n';
					cout << "The chain is actually: " << protein.polypeptied_changed << '\n';

				}
				else if (protein.original_protein != protein.changed_protein) {
					cout << "MISSENSE MUTATION" << '\n';
					cout << "Protein has been changed from " << protein.original_protein << " to " << protein.changed_protein << " at index " << protein.index << '\n' << '\n';
					cout << "The chain is supposed to be: " << protein.polypeptide_original << '\n';
					cout << "The chain is actually: " << protein.polypeptied_changed << '\n';
				}
			}

		}
		

		else if (num_diff > 1) {
			//Insertion
			//The original is smaller than the transcripted
			if (compare.size() + 1 == transcripted.size()) {
				for (int i = save_index; i < compare.size(); i++) {
					if (transcripted[i + 1] != compare[i]) {
						cout << "ERROR: Unidentified Mutation in sequence. Make sure sequence was entered correctly";
						return " ";
					}
				}
				cout << "INSERTION MUTATION: " << '\n';
				cout << "An unexpected base has been inserted inside the DNA sequence resulting in a completely different polypeptide chain " << '\n' << '\n';
				cout << "The chain is supposed to be: " << protein.polypeptide_original << '\n';
				cout << "The chain is actually: " << protein.polypeptied_changed << '\n';

			}

			//Deletion
			//compare is larger than transcripted
			else if (compare.size() == transcripted.size() + 1) {
				for (int i = save_index; i < transcripted.size(); i++) {
					if (compare[i + 1] != transcripted[i]) {
						cout << "ERROR: Unidentified Mutation in sequence. Make sure sequence was entered correctly";
						return " ";
					}
				}
				cout << "DELETION MUTATION: " << '\n';
				cout << "An unexpected base has been deleted inside the DNA sequence resulting in a completely different polypeptide chain " << '\n' << '\n';
				cout << "The chain is supposed to be: " << protein.polypeptide_original << '\n';
				cout << "The chain is actually: " << protein.polypeptied_changed << '\n';
			}
		}
		else {
			cout << "ERROR: Unidentified Mutation in sequence. Make sure sequence was entered correctly";
		}

		
	}
	return " ";
	
}

void user_interface() {
	int input;
	cout << "PRESS 1 TO TRANSCRIBE DNA" << '\n';
	cout << "PRESS 2 TO RETURN POLYPEPTIDE CHAIN OF A SINGLE STRAND OF DNA" << '\n';
	cout << "PRESS 3 TO COMPARE SINGLE STRAND TO TRANSCRIPTED STRAND AND DETERMINE MUTATION" << '\n';
	cin >> input;

	if (input == 1) {
		cout << "Enter in your DNA strand (Ex. ATGCATGC) " << '\n';
		string strand;
		cin >> strand;
		cout << "Your transcribed strand is: " << '\n';
		cout << transcription(strand) << '\n';

	}
	else if (input == 2) {
		cout << "Enter in your DNA strand (Ex. ATGCATGC) " << '\n';
		string strand;
		cin >> strand;
		string transcribed_strand = transcription(strand);
		cout << "Your Polypeptide chain is: " << '\n';
		cout << read_transcription(transcribed_strand) << '\n';

	}
	else if (input == 3) {
		cout << "Enter in your DNA strand (Ex. ATGCATGC) " << '\n';
		string strand;
		cin >> strand;
		cout << "Enter in your Transcribed DNA strand (Ex. AUGCAUGC) " << '\n';
		string transcribe;
		cin >> transcribe;
		cout << '\n';
		determine_mutation(strand, transcribe);
	}
	else {
		cout << "Unidentified option";
	}
}


int main() {
	user_interface();

	return 0;
}