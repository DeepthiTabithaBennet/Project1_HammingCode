#include <iostream>
#include <cmath>
#include <string>
#include <unistd.h>

using namespace std;
/*-------------------------------------------------------------------------*/
class Hamming{

    public:
        string message;
        int codeword[100], temp[100];
        int n, check;
        char parity;
/*-------------------------------------------------------------------------*/
        Hamming(){
            parity = 'E';
            message = "";
            n = check = 0;
            
            for(int i = 0; i < 100; i++){
                temp[i] = codeword[i] = 0;
            }
        }
/*-------------------------------------------------------------------------*/
        int findr(){
            for(int i = 1 ;; i++){
                if((n + i + 1) <= pow(2, i))
                    return i;
            }
        }
};
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
class Transmitter : public Hamming{
    
    public:

        void generate(){
            do{
                cout << "Enter the message to be Transmitted, in Binary (LSB to MSB) : ";
                cin >> message;
            }while(message.find_first_not_of("01") != string::npos);
            
            n = message.size();
            cout << "Enter the Parity Type - Odd(O) / Even(E) : ";
            cin >> parity;
            
            for(unsigned int i = 0; i < message.size(); i++){
                if(message[i] == '1'){
                    temp[i+1] = 1;
                }
                else{
                    temp[i+1] = 0;
                }
            }
            computeCode();
        }
/*-------------------------------------------------------------------------*/
        void computeCode(){
            check = findr();
            cout << "\nNumber of Parity Bits : " << check << endl;
            cout << "Total Number of Bits to be Transmitted : " << (n+check) << endl << endl;
     
            for(int i = (n+check), j = n; i > 0; i--){
                if((i & (i - 1)) != 0){
                    codeword[i] = temp[j--];
                }
                else{
                    codeword[i] = setParity(i);
                }
            }
            
            cout << "PARITY BITS :\n";
     
            for(int i = 0; i < check; i++){
                cout << "P" << pow(2, i) << " : " << codeword[(int)pow(2, i)] << endl;
            }
            
            cout << "\nHamming Encoded Output : " << endl;
            
            for(int i = 1; i <= (n+check); i++){
                cout << codeword[i];
            }
     
            cout << endl;
        }        
/*-------------------------------------------------------------------------*/
        int setParity(int x){
            bool flag = true;
            int bit;
            
            if(x == 1){
                bit = codeword[x+2];
                for(int j = (x + 3); j <= (n+check); j++){
                    if(j % 2){
                        bit ^= codeword[j];
                    }
                }
            }
            
            else{
                bit = codeword[x+1];
                for(int i = x; i <= (n + check); i++){
                    if(flag){
                       if((i == x) || (i == (x+1))){
                           bit = codeword[x+1];
                       }
                       else{
                           bit ^= codeword[i];
                       }
                    }
                   if(((i+1) % x) == 0){
                       flag = !flag;
                   }
               }
           }
           
           if(parity == 'O' || parity == 'o'){
               return (!(bit));
           }
           else{
               return (bit);
           }
        }
};
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
class Receiver : public Hamming{

    public:
    
        void correct(){
            do{
                cout << "Enter the Received Encoded Data (LSB to MSB) : ";
                cin >> message;
            }while(message.find_first_not_of("01") != string::npos);
            
            n = message.size();
            cout << "Enter the Parity Type - Odd(O) / Even(E) : ";
            cin >> parity;
            
            for(unsigned int i = 0; i < message.size(); i++){
                if(message[i] == '1'){
                    codeword[i+1] = 1;
                }
                else{
                    codeword[i+1] = 0;
                }
            }
            detect();
        }
/*-------------------------------------------------------------------------*/
        void detect(){
            int position = 0;
            check = findr();
            
            for(int i = 0; i < (n-check); i++){
                bool flag = true;
                int x = pow(2, i);
                int bit = codeword[x];
                
                if(x == 1){
                    for(int j = (x+1); j <= (n+check); j++){
                        if(j % 2){
                            bit ^= codeword[j];
                        }
                    }
                }
                
                else{
                    for(int k = (x+1); k <= (n+check); k++){
                        if(flag){
                            bit ^= codeword[k];
                        }
                        if((k+1) % x == 0){
                            flag = !flag;
                        }
                    }
                }
                
                if((parity == 'E' || parity == 'e') && (bit == 1)){
                    position += x;
                }
                    
                if((parity == 'O' || parity == 'o') && (bit == 0)){
                    position += x;
                }
            }
            
            cout << "\nPARITY BITS :\n";
     
            for(int i = 0; i < check; i++){
                cout << "P" << pow(2,i) << " : " << codeword[(int)pow(2,i)] << endl;
            }
            
            cout << "\nReceived Data : " << endl;
     
            for(int i = 1; i <= (n); i++){
                cout << codeword[i];
            }
                
            cout << endl;
     
            if(position != 0){
                cout << "\nError Detected in the Transmitted Data at the Position : " << position << endl;
                codeword[position] = !codeword[position];
         
                cout << "\nReceived Data Sequence After Correction : " << endl;
                for(int i = 1; i <= (n); i++){
                    cout << codeword[i];
                }
         
                cout << endl;
            }
            
            else{
                cout << "\nNo Error in the Transmitted Data." << endl;
            }
            
            cout << "\nHamming Decoded Output : ";
            for(int i = 1; i <= (n); i++){
                if((i & (i - 1)) != 0){
                    cout << codeword[i];
                }
            }
            cout << endl;
        }
};
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int main(){
    char ch;
    string choice;
    
    Transmitter t;
    Receiver r;
    
    cout << "HAMMING ERROR DETECTION AND CORRECTION CODE\n\n";

    sleep(1.5);

    cout << "Hamming Code is a Linear Error Detection and Correction Code used to Detect and Correct Bit Errors that can occur when data is Transmitted or Received.";
    
    sleep(1.5);
    
    cout << "\n\nIt can Detect upto Two simultaneous Bit Errors and is capable of Accurately Correcting Single Bit Errors.";
    
    sleep(1.5);
    
    cout << "\n\nThe Transmitter generates Parity Bits, which are added to Data, so that the validity of the Data can be checked when it is received.\n\n";
    
    sleep(1.5);
    
    cout << "\nAre you the Transmitter or the Receiver ? ";
    cin >> choice;
        
    if ((choice == "transmitter")||(choice == "Transmitter")||(choice == "t")||(choice == "T")){
        cout << endl;
        t.generate();
    }
        
    if ((choice == "receiver")||(choice == "Receiver")||(choice == "r")||(choice == "R")){
        cout << endl;
        r.correct();
    }
    
    cout << endl << "Enter another code ? (Y / N) : ";
    cin >> ch;
    
    return 0;
}
