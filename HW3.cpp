// 
// OS_HW2
// ��T�T�A 10827234 �^�ۺ�
//

/* ���ѨM���D:
 * fixed! method6 �g�ɰ��D(�n�ੵ��g�ɡA���n�`�N��l��)
 * fixed! reference string �u���@��ƾ��(0-9)?
 * 
 * 
 */

#include <iostream>
#include <string.h> // strcmp 
#include <vector>
#include <fstream>	// open, is_open, close, write, ignore
#include <stdlib.h>  

using namespace std;

typedef struct pF{
    char page;
    string framContain;
    char isFault;
}printFormat;

string inFileName = "";
vector<pF> allToPrint; // �O���U�� page �����n��X������ ( page, which page(s) in frame, Fault)

class FIFO{
public:
    void initialize(){ pageFault = 0; pageReplace = 0;  pageQueue.clear();}
    void pageReplacement(string referenceString, int capacity);
    void getValue(int & nPageFault, int & nPageReplace ){ nPageFault = pageFault; nPageReplace = pageReplace;};

private:
    int pageFault, pageReplace;
    vector<char> pageQueue;
};

class LRU{
public:
    void initialize(){ pageFault = 0; pageReplace = 0; pageQueue.clear(); }
    void pageReplacement(string referenceString, int capacity);
    void getValue(int & nPageFault, int & nPageReplace ){ nPageFault = pageFault; nPageReplace = pageReplace;};
    
private:
    int pageFault, pageReplace;
    vector<char> pageQueue;
};

class LFU{
public:
    void initialize(){ 
        pageFault = 0; pageReplace = 0; pageQueue.clear();  
        for( int i = 0 ; i < 10 ; i++)
            counter[i] = 0 ;
    } // initialize()

    void pageReplacement_Used_LRU(string referenceString, int capacity);
    void getValue(int & nPageFault, int & nPageReplace ){ nPageFault = pageFault; nPageReplace = pageReplace;};
    char findLeastFreqUsed(); // ��� pagequeue ���ϥΦ��Ƴ̤֪� �^��>page number<

private:
    int pageFault, pageReplace;
    vector<char> pageQueue;
    int counter[10] ={0};
};

class MFU{
public:
    void initialize(){ 
        pageFault = 0; pageReplace = 0; pageQueue.clear();  
        for( int i = 0 ; i < 10 ; i++)
            counter[i] = 0 ;
    } // initialize()

    void pageReplacement_Used_LRU(string referenceString, int capacity);
    void pageReplacement_Used_FIFO(string referenceString, int capacity);
    void getValue(int & nPageFault, int & nPageReplace ){ nPageFault = pageFault; nPageReplace = pageReplace;};
    char findMostFreqUsed(); // ��� pagequeue ���ϥΦ��Ƴ̤֪� �^��>page number<

private:
    int pageFault, pageReplace;
    vector<char> pageQueue;
    int counter[10] ={0};
};

bool openFile( fstream &inFile ) ; // read records from a file
void writeFile( fstream &outFile, string methods, int pageFault, int pageReplace, int capacity ) ; // write datas  into a file
int findIndex( vector<char> source, char target);

int main() {
    int method = 0, frameSize = 0;
    int pageFalts = 0, pageReplaces = 0 ;
    bool wantExe = false ;
    string referenceString, methodFormat;

    FIFO FIFOItem;
    LRU LRUItem;
    LFU LFUItem;
    MFU MFUItem;

    do {
        cout << endl << "***************  Page Replacement  ****************";
        cout << endl << "* 1. [FIFO] -------> First In First Out           *";
        cout << endl << "* 2. [LRU] --------> Least Recently Used          *";
        cout << endl << "* 3. [LFU + LRU] --> Least Frequently Used + LRU  *";
        cout << endl << "* 4. [MFU + FIFO] -> Most Frequently Used + FIFO  *";
        cout << endl << "* 5. [MFU + LRU] --> MFU + LRU                    *";
        cout << endl << "* 6. [ALL] --------> Execute ALL method           *";
        cout << endl << "***************************************************" << endl;

        fstream inputFile, outFile;

        /* Initialize */ FIFOItem.initialize() ; allToPrint.clear() ; LRUItem.initialize();
        /* Initialize */ LFUItem.initialize() ; MFUItem.initialize() ;
        
        wantExe = openFile(inputFile); // ��J "0" return false �_�h����Ū�J���쥿�T�}���ɮ�

        if ( wantExe ) { 
            inputFile >> method >> frameSize ;// read records from a file
 
            getline(inputFile, referenceString) ; // '\n'
            getline(inputFile, referenceString) ; // real data
            cout << endl << method << " $ " << frameSize << " end >"<< referenceString << endl ;

            /* Initialize */
            string outFileName = "out_" + inFileName + ".txt" ;
            outFile.open( outFileName, ios::out | ios :: trunc ); // �R���Ҧ����e
            outFile.close() ;

            switch (method) {
            case 1 :
                cout << "�ݢ�����������������������������������������������������" << endl;
                cout << "��     Method 1: [FIFO]     ��" << endl;
                cout << "�㢤����������������������������������������������������" << endl;
                FIFOItem.pageReplacement(referenceString, frameSize) ;
                FIFOItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------FIFO-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;

            case 2 :
                cout << "�ݢ�������������������������������������������������������" << endl;
                cout << "��      Method 2: [LRU]      ��" << endl;
                cout << "�㢤������������������������������������������������������" << endl;
                LRUItem.pageReplacement(referenceString, frameSize) ;
                LRUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------LRU-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;

            case 3:
                cout << "�ݢ���������������������������������������������������������������" << endl;
                cout << "��     Method 3: [LFU + LRU]     ��" << endl;
                cout << "�㢤��������������������������������������������������������������" << endl;
                LFUItem.pageReplacement_Used_LRU(referenceString, frameSize) ;
                LFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Least Frequently Used LRU Page Replacement-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;

            case 4:
                cout << "�ݢ�����������������������������������������������������������������" << endl;
                cout << "��     Method 4: [MFU + FIFO]     ��" << endl;
                cout << "�㢤����������������������������������������������������������������" << endl;
                MFUItem.pageReplacement_Used_FIFO(referenceString, frameSize) ;
                MFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Most Frequently Used Page Replacement -----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;

            case 5:
                cout << "�ݢ���������������������������������������������������������������" << endl;
                cout << "��     Method 5: [MFU + LRU]     ��" << endl;
                cout << "�㢤��������������������������������������������������������������" << endl;
                MFUItem.pageReplacement_Used_LRU(referenceString, frameSize) ;
                MFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Most Frequently Used LRU Page Replacement -----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;
            
            case 6:
                cout << "�ݢ�����������������������������������������������������" << endl;
                cout << "��     Method 6: [ALL]      ��" << endl;
                cout << "�㢤����������������������������������������������������" << endl;
                FIFOItem.pageReplacement(referenceString, frameSize) ;
                FIFOItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------FIFO-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);

                LRUItem.pageReplacement(referenceString, frameSize) ;
                LRUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------LRU-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);

                LFUItem.pageReplacement_Used_LRU(referenceString, frameSize) ;
                LFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Least Frequently Used LRU Page Replacement-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);

                MFUItem.pageReplacement_Used_FIFO(referenceString, frameSize) ;
                MFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Most Frequently Used Page Replacement -----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                MFUItem.initialize(); // ���槹�@�� class ���F�� �n��l�� ���ܼƲM��

                MFUItem.pageReplacement_Used_LRU(referenceString, frameSize) ;
                MFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Most Frequently Used LRU Page Replacement -----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);

                break;

            case 0:break;
            default: cout << endl << "Command does not exist!" << endl;
            } // switch(command)
        } // if

        inputFile.close();

    } while (method != 0 && wantExe); // method != 0 & �Q�~�����

    return 0 ;
} // main()

bool openFile( fstream &inFile ) { // read records from a file
	string fileName = "";

    do {
        cout << endl << "Input a file number(eg.input<int>{_method<1-6>}) ([0]:quit!):";
        cin >> inFileName;
        
        if(strcmp(inFileName.c_str(), "0") == 0 )
            break ;

        fileName = inFileName + ".txt";
        inFile.open(fileName, ios::in); 
        if( inFile )
            return true ;
        else 
            cout << endl << "### " << fileName << " does not exist! ###" << endl ;

    }while( !inFile ) ;

	return false ; // return  
} // openInputFile() 

void writeFile( fstream &outFile, string methods, int pageFault, int pageReplace, int capacity ) { // write datas  into a file
    string fileName = "out_" + inFileName + ".txt" ;
 
	outFile.open( fileName, ios :: out | ios::app ) ;  // �q�ɮ׵�������g�J���
    //outFile.open( fileName, ios :: out );
    outFile << methods << endl ;
    for( int i = 0 ; i < allToPrint.size() ; i++) {
        outFile << allToPrint.at(i).page << "\t" << allToPrint.at(i).framContain ;
        if(allToPrint.at(i).isFault == 'F')
            outFile << "\t" <<  allToPrint.at(i).isFault << endl ;
        else
            outFile << endl ;
    } // for()

    outFile << "Page Fault = " << pageFault << "  Page Replaces = " << pageReplace << "  Page Frames = " << capacity << endl << endl ;
    //outFile << "Hank" ;
    outFile.close() ;
    allToPrint.clear(); // ��l��

	return ;
} // 

void FIFO::pageReplacement(string refString, int capacity){
    bool inQueue[10] = {false}; // �O���� �� ���S���b queue �� 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // �ثe���S���bqueue �� // - '0' char �� int
            this->pageFault++; // �]���b queue ���䤣�즹�� �ҥH page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame �٨S��
                this->pageQueue.push_back(refString.at(i)); // �N�� page push �i queue �� 
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } // if
            else{ //  page frame �w�g���F�A�n�Npage �m����
                this->pageReplace++; // �]�� page frame �w�g���F�A�n�N�ª�page �m����
                inQueue[this->pageQueue.front() - '0'] = false; // �N���ª���X �]�w��page ���b queue ��
                this->pageQueue.erase(this->pageQueue.begin()); //�N���ª�(�Ĥ@�� | �Y��) page ��X

                this->pageQueue.push_back(refString.at(i)) ; // �N�ثe page ��i queue ��
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } //else

        } // if
        else{
            // �ثe���bqueue �� ���ΰ���
            tempPF.isFault = 'N'; // �S���J�� pageFault �ҥH�]�� N ���ᤣ�L�X
        }

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame �n�˵ۦL
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // �N�O���n�n�L���F��s�i vector ��
    } // for()

} // FIFO::pageReplacement()

void LRU::pageReplacement(string refString, int capacity){
    bool inQueue[10] = {false}; // �O���� �� ���S���b queue �� 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // �ثe���S���bqueue �� // - '0' char �� int
            this->pageFault++; // �]���b queue ���䤣�즹�� �ҥH page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame �٨S��
                this->pageQueue.push_back(refString.at(i)); // �N�� page push �i queue �� 
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } // if
            else{ //  page frame �w�g���F �A�n�Npage �m����
                this->pageReplace++; // �]�� page frame �w�g���F�A�n�N�ª�page �m����
                inQueue[this->pageQueue.front() - '0'] = false; // �N���ª���X �]�w��page ���b queue ��
                this->pageQueue.erase(this->pageQueue.begin()); //�N���ª�(�Ĥ@�� | �Y��) page ��X

                this->pageQueue.push_back(refString.at(i)) ; // �N�ثe page ��i queue ��
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } //else

        } // if
        else{
            // �ثe���bqueue �� renew the stamp �N��page �������(���ڬ��̷s��)
            tempPF.isFault = 'N'; // �S���J�� pageFault �ҥH�]�� N ���ᤣ�L�X
            char eraseStuff = refString.at(i); // 
            int offset = findIndex(this->pageQueue, eraseStuff); // ��즹 page �b vector ���� index(�Z�� 0 �������q)
            this->pageQueue.erase(this->pageQueue.begin() + offset); // �R���� page
            this->pageQueue.push_back(eraseStuff); // �N��page �������(���ڬ��̷s��)
        } // else

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame �n�˵ۦL
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // �N�O���n�n�L���F��s�i vector ��
    } // for()

} // LRU::pageReplacement()

int findIndex( vector<char> source, char target){
    for(int i = 0 ; i < source.size() ; i++){
        if( target == source.at(i) )
            return i;
    }// for

    return -1;
} // findIndex()

void LFU::pageReplacement_Used_LRU(string refString, int capacity){
    bool inQueue[10] = {false}; // �O���� �� ���S���b queue �� 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        this->counter[refString.at(i) - '0'] ++;  // �C���ѦҦ� page �N�N�� page �p�� + 1
        
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // �ثe���S���bqueue �� // - '0' char �� int
            this->pageFault++; // �]���b queue ���䤣�즹�� �ҥH page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame �٨S��
                this->pageQueue.push_back(refString.at(i)); // �N�� page push �i queue �� 
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } // if
            else{ //  page frame �w�g���F �A�n�Npage �m����
                this->pageReplace++; // �]�� page frame �w�g���F�A�n�N�ª�page �m����

                // ���� counter �̤p��
                char leastUsedChar = this->findLeastFreqUsed();
                inQueue[leastUsedChar - '0'] = false; // �Ncounter �̤p������X �]�w��page ���b queue ��
                this->pageQueue.erase(this->pageQueue.begin() + findIndex(this->pageQueue, leastUsedChar)); //�Ncounter �̤p���� page ��X
                this->counter[leastUsedChar - '0'] = 0; // �Q��X�n�O�o�k�s

                this->pageQueue.push_back(refString.at(i)) ; // �N�ثe page ��i queue ��
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } //else

        } // if
        else{
            // �ثe���bqueue �� renew the stamp �N��page �������(���ڬ��̷s��)
            // �u�ݱNcounter+1(�W���w�g����)
            tempPF.isFault = 'N'; // �S���J�� pageFault �ҥH�]�� N ���ᤣ�L�X
            char eraseStuff = refString.at(i); // 
            int offset = findIndex(this->pageQueue, eraseStuff); // ��즹 page �b vector ���� index(�Z�� 0 �������q)
            this->pageQueue.erase(this->pageQueue.begin() + offset); // �R���� page
            this->pageQueue.push_back(eraseStuff); // �N��page �������(���ڬ��̷s��)
        } // else

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame �n�˵ۦL
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // �N�O���n�n�L���F��s�i vector ��
    } // for()

} // LFU::pageReplacement()

char LFU::findLeastFreqUsed(){ // ��� pagequeue ���ϥΦ��Ƴ̤֪� �^��>page number<
    int minimize = this->counter[this->pageQueue.at(0) - '0'];
    char minPage = this->pageQueue.at(0);

    for( int i = 1 ; i < this->pageQueue.size() ; i++ ){
        if( this->counter[this->pageQueue.at(i) - '0'] < minimize){
            minimize = this->counter[this->pageQueue.at(i) - '0'];
            minPage = this->pageQueue.at(i);         
        } // if
    } // for

    return minPage;
} // 


void MFU::pageReplacement_Used_LRU(string refString, int capacity){
    bool inQueue[10] = {false}; // �O���� �� ���S���b queue �� 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        this->counter[refString.at(i) - '0'] ++;  // �C���ѦҦ� page �N�N�� page �p�� + 1
        
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // �ثe���S���bqueue �� // - '0' char �� int
            this->pageFault++; // �]���b queue ���䤣�즹�� �ҥH page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame �٨S��
                this->pageQueue.push_back(refString.at(i)); // �N�� page push �i queue �� 
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } // if
            else{ //  page frame �w�g���F �A�n�Npage �m����
                this->pageReplace++; // �]�� page frame �w�g���F�A�n�N�ª�page �m����

                // ���� counter �̤j��
                char mostFreqChar = this->findMostFreqUsed();
                inQueue[mostFreqChar - '0'] = false; // �Ncounter �̤j������X �]�w��page ���b queue ��
                this->pageQueue.erase(this->pageQueue.begin() + findIndex(this->pageQueue, mostFreqChar)); //�Ncounter �̤j���� page ��X
                this->counter[mostFreqChar - '0'] = 0; // �Q��X�n�O�o�k�s

                this->pageQueue.push_back(refString.at(i)) ; // �N�ثe page ��i queue ��
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } //else

        } // if
        else{
            // �ثe���bqueue �� renew the stamp �N��page �������(���ڬ��̷s��)
            // �u�ݱNcounter+1(�W���w�g����)
            tempPF.isFault = 'N'; // �S���J�� pageFault �ҥH�]�� N ���ᤣ�L�X
            char eraseStuff = refString.at(i); // 
            int offset = findIndex(this->pageQueue, eraseStuff); // ��즹 page �b vector ���� index(�Z�� 0 �������q)
            this->pageQueue.erase(this->pageQueue.begin() + offset); // �R���� page
            this->pageQueue.push_back(eraseStuff); // �N��page �������(���ڬ��̷s��)
        } // else

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame �n�˵ۦL
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // �N�O���n�n�L���F��s�i vector ��

    } // for()

} // MFU::pageReplacement_Used_LRU()

char MFU::findMostFreqUsed(){ // ��� pagequeue ���ϥΦ��Ƴ̤֪� �^��>page number<
    int maximum = this->counter[this->pageQueue.at(0) - '0'];
    char maxPage = this->pageQueue.at(0);
    for( int i = 1 ; i < this->pageQueue.size() ; i++ ){
        if( this->counter[this->pageQueue.at(i) - '0'] > maximum){
            maximum = this->counter[this->pageQueue.at(i) - '0'];
            maxPage = this->pageQueue.at(i);         
        } // if
    } // for

    return maxPage;
} // 

void MFU::pageReplacement_Used_FIFO(string refString, int capacity){
    bool inQueue[10] = {false}; // �O���� �� ���S���b queue �� 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        this->counter[refString.at(i) - '0'] ++;  // �C���ѦҦ� page �N�N�� page �p�� + 1
        
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // �ثe���S���bqueue �� // - '0' char �� int
            this->pageFault++; // �]���b queue ���䤣�즹�� �ҥH page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame �٨S��
                this->pageQueue.push_back(refString.at(i)); // �N�� page push �i queue �� 
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } // if
            else{ //  page frame �w�g���F �A�n�Npage �m����
                this->pageReplace++; // �]�� page frame �w�g���F�A�n�N�ª�page �m����

                // ���� counter �̤j��
                char mostFreqChar = this->findMostFreqUsed();
                inQueue[mostFreqChar - '0'] = false; // �Ncounter �̤j������X �]�w��page ���b queue ��
                this->pageQueue.erase(this->pageQueue.begin() + findIndex(this->pageQueue, mostFreqChar)); //�Ncounter �̤j���� page ��X
                this->counter[mostFreqChar - '0'] = 0; // �Q��X�n�O�o�k�s

                this->pageQueue.push_back(refString.at(i)) ; // �N�ثe page ��i queue ��
                inQueue[refString.at(i) - '0'] = true; // �ثe�� page �b queue �� 
            } //else

        } // if
        else{
            // �ثe���bqueue �� ���ΰ���
            // �u�ݱNcounter+1(�W���w�g����)
            tempPF.isFault = 'N'; // �S���J�� pageFault �ҥH�]�� N ���ᤣ�L�X
        } // else

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame �n�˵ۦL �ӵۦѮv���榡
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // �N�O���n�n�L���F��s�i vector ��
    } // for()

} // MFU::pageReplacement_Used_FIFO()
