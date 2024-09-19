#include <iostream>
#include <string>
#include <algorithm> // for string manipulation like remove and unique

using namespace std;

void createkeymatrix(string key, char matrix[5][5]){

    //creating the 5x5 matrix
    string alphabet = "abcdefghiklmnopqrstuvwxyz"; // j is omitted
    bool used[26] = {false};

    key.erase(remove(key.begin(), key.end(), 'j'), key.end()); // removes j
    key.erase(unique(key.begin(), key.end()), key.end()); // removes duplicates

    int index = 0; // position where the next unused char will be used

    //finds free spaces and adds unique chars
    for (char c : key){
        if (!used[c-'a']){ // if the character hasnt been used
            used[c-'a'] = true; //mark it as used
            matrix[index/5][index%5] = c; // position it in the next free space
            index ++; // increment to the next free space
        }
    }

    //fills the remaining matrix with unused letters
    for (char c : alphabet){
        if (!used[c-'a']){
            matrix[index/5][index%5] = c;
            index ++;
        }
    }
}

int processmessageintopairs(string message, pair<char, char> pairs[]){

    string cleanmessage= "";

    //cleaning message- removing numbers and replacing j ->i
    for (char c: message){
        if (isalpha(c)){ // checks it is an alphabetical letter
            if (c =='j') c = 'i';
            cleanmessage += tolower(c);
        }
    }

    int paircount = 0;
    for (size_t i = 0; i < cleanmessage.length(); i++){
        char first = cleanmessage[i];
        char second = (i + 1 < cleanmessage.length() && cleanmessage[i+1] != first) ? cleanmessage[i+1] : 'x';
        //if the second is the same as first, or the message has an odd length, it inserts x as the filler alphabet

        pairs[paircount++] = {first, second};
        if (first == second){
            second = 'x'; // use x as filler for repeating letters
        }
        else{
            i++; // skip the charcter if not needed
        }

    }

    //handle odd length message by adding filler x at the end
    if (cleanmessage.length() %2 != 0){
        pairs[paircount++] = {cleanmessage.back(), 'x'}; // if cleaned message has odd chars adds x at end
    }
    return paircount; // returns the char pairs
}

//finds the position of c in matrix and stores row and col for ref
void findposition(char c, char matrix[5][5], int &row, int &col){
    for (int i = 0; i < 5; i++){ // rows
        for (int j = 0; j < 5; j++){ //columns
            if (matrix[i][j] == c){
                row = i;
                col = j;
                return;
            }            
        } 
    }
}

string encryptpairs(pair<char, char> pairs[], int paircount, char matrix[5][5]){

    string ciphertext = "";

    for (int k = 0; k< paircount; k++){
        char first = pairs[k].first;
        char second = pairs[k].second;

        int row1, col1, row2, col2;
        findposition(first, matrix, row1, col1);
        findposition(second, matrix, row2, col2);

        //applyind playfair cipher rules
         
        if (row1 == row2){
            //same row shift right
            ciphertext += matrix[row1][(col1 + 1) % 5];
            ciphertext += matrix[row2][(col2 + 1) % 5];
        }
        else if (col1 == col2){
            //same column shift down
            ciphertext += matrix[(row1 + 1) %5][col1];
            ciphertext += matrix[(row2 + 1) %5][col2];
        }
        else{
            //rectangle rule- swap columns
            ciphertext += matrix[row1][col2];
            ciphertext += matrix[row2][col1];
        }
    }
    return ciphertext;
}
string playfaircipher(string key, string message){
    if (key.empty() || message.empty()){
        return "please input a key or message!! :<";
    }
    char matrix[5][5];
    createkeymatrix(key, matrix);
    pair<char, char> pairs[100];
    int paircount = processmessageintopairs(message, pairs);
    return encryptpairs(pairs, paircount, matrix);
}

int main(){
    string key, message;

    cout << " ---  WELCOME TO PLAYFAIR MATRIX  ---\n "<< endl ;
    cout << "enter key: " << endl;
    cin >> key;
    cout << "enter message to encrypt: " << endl;
    cin.ignore(); // clears the input buffer beforer reading the entire message
    getline(cin, message);

    string ciphertext = playfaircipher(key, message);
    cout << "\nthe encrypted message is " << ciphertext << endl;
    cout << "\nthank u for using playfair cipher!! :)\n\n";

    return 0;

}