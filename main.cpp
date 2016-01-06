#include <iostream>

using namespace std;

void incrementString(string &s, int offset = 1){
    int l = s.length();
    char c = s.at(l-offset);
    if(c >= 'a' && c<='y'){
        s[l-offset] = c+1;
    }
    if(c == 'z'){
        s[l-offset] = '9';
    }
    if(c == '9' ){
        s[l-offset] = 'a';
        incrementString(s,offset+1);
    }
}

int INDICES[1000];
int leftPart[1000];
int rightPart[1000];
int i,j,k;
int F[9] = {0, -1, 1, 0, 1, -1, -1, 1, 0};
int TRYTE_TO_TRITS_MAPPING[27][3] = {{0, 0, 0}, {1, 0, 0}, {-1, 1, 0}, {0, 1, 0}, {1, 1, 0}, {-1, -1, 1}, {0, -1, 1}, {1, -1, 1}, {-1, 0, 1}, {0, 0, 1}, {1, 0, 1}, {-1, 1, 1}, {0, 1, 1}, {1, 1, 1}, {-1, -1, -1}, {0, -1, -1}, {1, -1, -1}, {-1, 0, -1}, {0, 0, -1}, {1, 0, -1}, {-1, 1, -1}, {0, 1, -1}, {1, 1, -1}, {-1, -1, 0}, {0, -1, 0}, {1, -1, 0}, {-1, 0, 0}};

int f(int a, int b) {
    return F[a * 3 + b + 4];
}

void transform(int (&state)[1000]){


    int round = 9, i, index = 0, a,b;
    while (round-- > 0) {
        for (i = 0; i < 729; i++) {
            leftPart[i] = f(a = state[index], b = state[INDICES[i]]);
            index = INDICES[i];
            rightPart[i] = f(b, a);
        }
        for (i = 0; i < 729; i++) {
            state[i] = f(leftPart[index], rightPart[INDICES[i]]);
            index = INDICES[i];
        }
    }
}

string getAddress(string _seed){

    int state[1000];
    string seed = _seed;
    int tryte;
    int key[10000];
    string address = "";
    string charset = "9ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (i = 0; i < 729; i++) {
        state[i] = 0;
    }
    if (!seed.empty()) {
        if (seed.length() > 81) {
            cout << "Only first 81 chars will be used!";
            seed = seed.substr(0, 81);
        }
        for (i = 0; i < seed.length(); i++) {
            tryte = 0;
            if(seed.at(i) >= 'a' &&  seed.at(i) <= 'z'){
                tryte = seed.at(i);
                tryte -= 96;
            }

            state[i * 3] = TRYTE_TO_TRITS_MAPPING[tryte][0];
            state[i * 3 + 1] = TRYTE_TO_TRITS_MAPPING[tryte][1];
            state[i * 3 + 2] = TRYTE_TO_TRITS_MAPPING[tryte][2];
        }
        transform(state);
        for (i = 243; i < 729; i++) {
            state[i] = 0;
        }
        transform(state);
        for (i = 243; i < 729; i++) {
            state[i] = 0;
        }



        for (i = 0; i < 9; i++) {
            transform(state);
            for (j = 0; j < 243; j++) {
                key[i * 243 + j] = state[j];
            }
        }
        for (i = 0; i < 27; i++) {
            for (j = 0; j < 27; j++) {
                for (k = 0; k < 81; k++) {
                    state[k] = key[i * 81 + k];
                }
                for (; k < 729; k++) {
                    state[k] = 0;
                }
                transform(state);
                for (k = 0; k < 81; k++) {
                    key[i * 81 + k] = state[k];
                }
            }
        }



        for (i = 243; i < 729; i++) {
            state[i] = 0;
        }
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 243; j++) {
                state[j] = key[i * 243 + j];
            }
            transform(state);
        }



        for (i = 0; i < 243; ) {
            j = state[i++] + state[i++] * 3 + state[i++] * 9;
            if (j < 0) {
                j += 27;
            }
            //address += ("9ABCDEFGHIJKLMNOPQRSTUVWXYZ").charAt(j);
            address = address + charset.at(j);

        }
        //prompt("Address", address);
        return address;
    }
    cout << "Seed is emtpy";
}




int main() {
    string seed = "aaaaaaaaaabbbbbbbbbbccccccccccddddddddddeeeeeeeeeeffffffffffggggggggghhhhhhhhhhh";

    // initialize
    int index = 0, ii;
    for(ii=0;ii<729;ii++){
        INDICES[ii] = index += index <= 364 ? 364 : -365;
    }

    while(1) {
        string ad = getAddress(seed);
        cout << seed << "\t" << ad << "\n";
        incrementString(seed);
    }

    return 0;
}







