#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

void encrypt(string, string);
void decrypt(string, string);
void error(string);

int main(int argc, char *argv[]){
	string filename;
	string passkey;

	cout<<"Enter filename: ";
	getline(cin, filename);
	cout<<"Enter passkey: ";
	getline(cin, passkey);

	char ed;
	cout<<"Do you want to encrypt or decrypt? (e/d): ";
	cin>>ed;

	if (tolower(ed) == 'e'){
		encrypt(filename, passkey);
	}
	else if (tolower(ed) == 'd'){
		decrypt(filename, passkey);
	}
	else {
		cout<<"Invalid input, exiting..."<<endl;
	}
}

void encrypt(string filename, string passkey){
	//open input file
	ifstream fin(filename.c_str(), ios::binary);
	if (!fin){
		error("File not found");
	}

	//create output binary file
	ofstream fenc( (filename + ".enc").c_str(), ios::binary);
	if (!fenc){
		error("Cannot create output file");
	}

	char c;
	//create interator that iterates thru passkey string
	string::const_iterator pi;
	pi = passkey.begin();


	//using fin.eof() to check for end of file is dirty
	while ( fin.read(&c, sizeof(c)) ){
		c ^= *pi;		//xor the original character
		fenc.write(&c, sizeof(c));

		pi++;
		if (pi == passkey.end())
			pi = passkey.begin();
	}

	//close all files
	fenc.close();
	fin.close();

	//delete original file
	remove(filename.c_str());

	cout<<"Encrypted file "<<filename;
	cout<<", removed original file..."<<endl;
}

void decrypt(string filename, string passkey){
	//check if it is a .enc file
	size_t encpos = filename.rfind(".enc");
	if (encpos == string::npos){
		//not .enc
		error("Only .enc files can be decrypted!");
	}
	else {
		cout<<"Decrypting "<<filename<<" using passkey "<<passkey<<"..."<<endl;
	}

	//open input file
	ifstream fenc(filename.c_str(), ios::binary);
	if (!fenc){
		error("Unable to open file");
	}

	//create output binary file
	filename.replace(encpos, 4, "");
	//remove .enc from filename

	ofstream fout( filename.c_str() );
	if (!fout){
		error("Cannot create output file");
	}

	char c;
	string::const_iterator pi;
	pi = passkey.begin();

	while ( fenc.read(&c, sizeof(c)) ){
		c ^= *pi;		//xoring twice will get the original
		fout.put(c);

		pi++;
		if (pi == passkey.end())
			pi = passkey.begin();
	}

	fenc.close();
	fout.close();

	cout<<"Decrypted contents to file "<<filename<<"..."<<endl;

	remove((filename+".enc").c_str());
	cout<<"Removed original encrypted file..."<<endl;
}

void error(string errmsg){
	cout<<"Error: "<<errmsg<<endl;
	exit(1);
}
