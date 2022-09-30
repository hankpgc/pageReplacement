// 
// OS_HW2
// 資訊三乙 10827234 彭桂綺
//

/* 未解決問題:
 * fixed! method6 寫檔問題(要能延續寫檔，但要注意初始化)
 * fixed! reference string 只有一位數整數(0-9)?
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
vector<pF> allToPrint; // 記錄各個 page 對應要輸出的項目 ( page, which page(s) in frame, Fault)

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
    char findLeastFreqUsed(); // 找到 pagequeue 中使用次數最少的 回傳>page number<

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
    char findMostFreqUsed(); // 找到 pagequeue 中使用次數最少的 回傳>page number<

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
        
        wantExe = openFile(inputFile); // 輸入 "0" return false 否則持續讀入直到正確開啟檔案

        if ( wantExe ) { 
            inputFile >> method >> frameSize ;// read records from a file
 
            getline(inputFile, referenceString) ; // '\n'
            getline(inputFile, referenceString) ; // real data
            cout << endl << method << " $ " << frameSize << " end >"<< referenceString << endl ;

            /* Initialize */
            string outFileName = "out_" + inFileName + ".txt" ;
            outFile.open( outFileName, ios::out | ios :: trunc ); // 刪除所有內容
            outFile.close() ;

            switch (method) {
            case 1 :
                cout << "╔══════════════════════════╗" << endl;
                cout << "║     Method 1: [FIFO]     ║" << endl;
                cout << "╚══════════════════════════╝" << endl;
                FIFOItem.pageReplacement(referenceString, frameSize) ;
                FIFOItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------FIFO-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;

            case 2 :
                cout << "╔═══════════════════════════╗" << endl;
                cout << "║      Method 2: [LRU]      ║" << endl;
                cout << "╚═══════════════════════════╝" << endl;
                LRUItem.pageReplacement(referenceString, frameSize) ;
                LRUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------LRU-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;

            case 3:
                cout << "╔═══════════════════════════════╗" << endl;
                cout << "║     Method 3: [LFU + LRU]     ║" << endl;
                cout << "╚═══════════════════════════════╝" << endl;
                LFUItem.pageReplacement_Used_LRU(referenceString, frameSize) ;
                LFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Least Frequently Used LRU Page Replacement-----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;

            case 4:
                cout << "╔════════════════════════════════╗" << endl;
                cout << "║     Method 4: [MFU + FIFO]     ║" << endl;
                cout << "╚════════════════════════════════╝" << endl;
                MFUItem.pageReplacement_Used_FIFO(referenceString, frameSize) ;
                MFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Most Frequently Used Page Replacement -----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;

            case 5:
                cout << "╔═══════════════════════════════╗" << endl;
                cout << "║     Method 5: [MFU + LRU]     ║" << endl;
                cout << "╚═══════════════════════════════╝" << endl;
                MFUItem.pageReplacement_Used_LRU(referenceString, frameSize) ;
                MFUItem.getValue(pageFalts, pageReplaces) ;
                methodFormat = "--------------Most Frequently Used LRU Page Replacement -----------------------";
                writeFile( outFile, methodFormat, pageFalts, pageReplaces, frameSize);
                break;
            
            case 6:
                cout << "╔══════════════════════════╗" << endl;
                cout << "║     Method 6: [ALL]      ║" << endl;
                cout << "╚══════════════════════════╝" << endl;
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
                MFUItem.initialize(); // 執行完一個 class 的東西 要初始化 把變數清空

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

    } while (method != 0 && wantExe); // method != 0 & 想繼續執行

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
 
	outFile.open( fileName, ios :: out | ios::app ) ;  // 從檔案結尾接續寫入資料
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
    allToPrint.clear(); // 初始化

	return ;
} // 

void FIFO::pageReplacement(string refString, int capacity){
    bool inQueue[10] = {false}; // 記錄此 頁 有沒有在 queue 中 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // 目前頁沒有在queue 中 // - '0' char 轉 int
            this->pageFault++; // 因為在 queue 中找不到此頁 所以 page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame 還沒滿
                this->pageQueue.push_back(refString.at(i)); // 將此 page push 進 queue 中 
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } // if
            else{ //  page frame 已經滿了，要將page 置換掉
                this->pageReplace++; // 因為 page frame 已經滿了，要將舊的page 置換掉
                inQueue[this->pageQueue.front() - '0'] = false; // 將最舊的踢出 設定此page 不在 queue 中
                this->pageQueue.erase(this->pageQueue.begin()); //將最舊的(第一個 | 頭的) page 踢出

                this->pageQueue.push_back(refString.at(i)) ; // 將目前 page 放進 queue 中
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } //else

        } // if
        else{
            // 目前頁在queue 中 不用做事
            tempPF.isFault = 'N'; // 沒有遇到 pageFault 所以設為 N 之後不印出
        }

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame 要倒著印
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // 將記錄好要印的東西存進 vector 中
    } // for()

} // FIFO::pageReplacement()

void LRU::pageReplacement(string refString, int capacity){
    bool inQueue[10] = {false}; // 記錄此 頁 有沒有在 queue 中 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // 目前頁沒有在queue 中 // - '0' char 轉 int
            this->pageFault++; // 因為在 queue 中找不到此頁 所以 page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame 還沒滿
                this->pageQueue.push_back(refString.at(i)); // 將此 page push 進 queue 中 
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } // if
            else{ //  page frame 已經滿了 ，要將page 置換掉
                this->pageReplace++; // 因為 page frame 已經滿了，要將舊的page 置換掉
                inQueue[this->pageQueue.front() - '0'] = false; // 將最舊的踢出 設定此page 不在 queue 中
                this->pageQueue.erase(this->pageQueue.begin()); //將最舊的(第一個 | 頭的) page 踢出

                this->pageQueue.push_back(refString.at(i)) ; // 將目前 page 放進 queue 中
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } //else

        } // if
        else{
            // 目前頁在queue 中 renew the stamp 將此page 移到尾巴(尾巴為最新的)
            tempPF.isFault = 'N'; // 沒有遇到 pageFault 所以設為 N 之後不印出
            char eraseStuff = refString.at(i); // 
            int offset = findIndex(this->pageQueue, eraseStuff); // 找到此 page 在 vector 中的 index(距離 0 的偏移量)
            this->pageQueue.erase(this->pageQueue.begin() + offset); // 刪除此 page
            this->pageQueue.push_back(eraseStuff); // 將此page 移到尾巴(尾巴為最新的)
        } // else

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame 要倒著印
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // 將記錄好要印的東西存進 vector 中
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
    bool inQueue[10] = {false}; // 記錄此 頁 有沒有在 queue 中 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        this->counter[refString.at(i) - '0'] ++;  // 每次參考此 page 就將此 page 計數 + 1
        
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // 目前頁沒有在queue 中 // - '0' char 轉 int
            this->pageFault++; // 因為在 queue 中找不到此頁 所以 page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame 還沒滿
                this->pageQueue.push_back(refString.at(i)); // 將此 page push 進 queue 中 
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } // if
            else{ //  page frame 已經滿了 ，要將page 置換掉
                this->pageReplace++; // 因為 page frame 已經滿了，要將舊的page 置換掉

                // 換掉 counter 最小的
                char leastUsedChar = this->findLeastFreqUsed();
                inQueue[leastUsedChar - '0'] = false; // 將counter 最小的的踢出 設定此page 不在 queue 中
                this->pageQueue.erase(this->pageQueue.begin() + findIndex(this->pageQueue, leastUsedChar)); //將counter 最小的的 page 踢出
                this->counter[leastUsedChar - '0'] = 0; // 被踢出要記得歸零

                this->pageQueue.push_back(refString.at(i)) ; // 將目前 page 放進 queue 中
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } //else

        } // if
        else{
            // 目前頁在queue 中 renew the stamp 將此page 移到尾巴(尾巴為最新的)
            // 只需將counter+1(上面已經做到)
            tempPF.isFault = 'N'; // 沒有遇到 pageFault 所以設為 N 之後不印出
            char eraseStuff = refString.at(i); // 
            int offset = findIndex(this->pageQueue, eraseStuff); // 找到此 page 在 vector 中的 index(距離 0 的偏移量)
            this->pageQueue.erase(this->pageQueue.begin() + offset); // 刪除此 page
            this->pageQueue.push_back(eraseStuff); // 將此page 移到尾巴(尾巴為最新的)
        } // else

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame 要倒著印
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // 將記錄好要印的東西存進 vector 中
    } // for()

} // LFU::pageReplacement()

char LFU::findLeastFreqUsed(){ // 找到 pagequeue 中使用次數最少的 回傳>page number<
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
    bool inQueue[10] = {false}; // 記錄此 頁 有沒有在 queue 中 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        this->counter[refString.at(i) - '0'] ++;  // 每次參考此 page 就將此 page 計數 + 1
        
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // 目前頁沒有在queue 中 // - '0' char 轉 int
            this->pageFault++; // 因為在 queue 中找不到此頁 所以 page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame 還沒滿
                this->pageQueue.push_back(refString.at(i)); // 將此 page push 進 queue 中 
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } // if
            else{ //  page frame 已經滿了 ，要將page 置換掉
                this->pageReplace++; // 因為 page frame 已經滿了，要將舊的page 置換掉

                // 換掉 counter 最大的
                char mostFreqChar = this->findMostFreqUsed();
                inQueue[mostFreqChar - '0'] = false; // 將counter 最大的的踢出 設定此page 不在 queue 中
                this->pageQueue.erase(this->pageQueue.begin() + findIndex(this->pageQueue, mostFreqChar)); //將counter 最大的的 page 踢出
                this->counter[mostFreqChar - '0'] = 0; // 被踢出要記得歸零

                this->pageQueue.push_back(refString.at(i)) ; // 將目前 page 放進 queue 中
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } //else

        } // if
        else{
            // 目前頁在queue 中 renew the stamp 將此page 移到尾巴(尾巴為最新的)
            // 只需將counter+1(上面已經做到)
            tempPF.isFault = 'N'; // 沒有遇到 pageFault 所以設為 N 之後不印出
            char eraseStuff = refString.at(i); // 
            int offset = findIndex(this->pageQueue, eraseStuff); // 找到此 page 在 vector 中的 index(距離 0 的偏移量)
            this->pageQueue.erase(this->pageQueue.begin() + offset); // 刪除此 page
            this->pageQueue.push_back(eraseStuff); // 將此page 移到尾巴(尾巴為最新的)
        } // else

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame 要倒著印
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // 將記錄好要印的東西存進 vector 中

    } // for()

} // MFU::pageReplacement_Used_LRU()

char MFU::findMostFreqUsed(){ // 找到 pagequeue 中使用次數最少的 回傳>page number<
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
    bool inQueue[10] = {false}; // 記錄此 頁 有沒有在 queue 中 0-9
    int refStringLength = refString.length() ;

    for( int i  = 0 ; i < refStringLength ; i++){
        this->counter[refString.at(i) - '0'] ++;  // 每次參考此 page 就將此 page 計數 + 1
        
        printFormat tempPF;
        tempPF.page = refString.at(i);
        if( inQueue[refString.at(i)- '0'] == false ){ // 目前頁沒有在queue 中 // - '0' char 轉 int
            this->pageFault++; // 因為在 queue 中找不到此頁 所以 page Fault
            tempPF.isFault = 'F';

            if( this->pageQueue.size() < capacity){ // page frame 還沒滿
                this->pageQueue.push_back(refString.at(i)); // 將此 page push 進 queue 中 
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } // if
            else{ //  page frame 已經滿了 ，要將page 置換掉
                this->pageReplace++; // 因為 page frame 已經滿了，要將舊的page 置換掉

                // 換掉 counter 最大的
                char mostFreqChar = this->findMostFreqUsed();
                inQueue[mostFreqChar - '0'] = false; // 將counter 最大的的踢出 設定此page 不在 queue 中
                this->pageQueue.erase(this->pageQueue.begin() + findIndex(this->pageQueue, mostFreqChar)); //將counter 最大的的 page 踢出
                this->counter[mostFreqChar - '0'] = 0; // 被踢出要記得歸零

                this->pageQueue.push_back(refString.at(i)) ; // 將目前 page 放進 queue 中
                inQueue[refString.at(i) - '0'] = true; // 目前此 page 在 queue 中 
            } //else

        } // if
        else{
            // 目前頁在queue 中 不用做事
            // 只需將counter+1(上面已經做到)
            tempPF.isFault = 'N'; // 沒有遇到 pageFault 所以設為 N 之後不印出
        } // else

        for(int x = this->pageQueue.size() ; x > 0 ; x--) { // page frame 要倒著印 照著老師的格式
            tempPF.framContain.push_back( this->pageQueue.at(x-1) );
        } //for

        allToPrint.push_back(tempPF); // 將記錄好要印的東西存進 vector 中
    } // for()

} // MFU::pageReplacement_Used_FIFO()
