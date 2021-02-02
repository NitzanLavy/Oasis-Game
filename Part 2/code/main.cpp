#include <iostream>
#include "library2.h"
#include <cassert>
#include <ctime>
#include <thread>

void* testInit1(){
    int arr[]={5,2,1,4,3};
    int bad_arr[]={5,2,-1};
    assert(init(3,NULL)==NULL);
    assert(init(1,arr)==NULL);
    assert(init(0,arr)==NULL);
    assert(init(-1,arr)==NULL);
    assert(init(3,bad_arr)==NULL);
    bad_arr[1]=0; //just for csl..
    return init(5,arr);
}
void* testInit2(){ //this is the main init we will use
    int arr[20];
    for(int i=0; i<20; i++){
        arr[19-i]=i;
    }
    std::cout<<"Passed initial test..."<<std::endl;
    return init(20,arr);
}
void testAddClan1(void* oasis){
    assert(addClan(oasis,1)==FAILURE);
    assert(addClan(oasis,2)==FAILURE);
    assert(addClan(oasis,3)==FAILURE);
    assert(addClan(oasis,4)==FAILURE);
    assert(addClan(oasis,5)==FAILURE);
    assert(addClan(NULL,1)==INVALID_INPUT);
    assert(addClan(oasis,-1)==INVALID_INPUT);
}
void testAddClan2(void* oasis){
    for(int i=0;i<20;i++){
        assert(addClan(oasis,i)==FAILURE);
    }
    for(int i=100;i<110;i++){
        assert(addClan(oasis,i)==SUCCESS);
    }
    std::cout<<"Passed add clan test..."<<std::endl;
}
void testAddPlayer(void* oasis){
    assert(addPlayer(NULL, 1, 1, 1)==INVALID_INPUT);
    assert(addPlayer(oasis, -1, 1, 1)==INVALID_INPUT);
    assert(addPlayer(oasis, 1, -1, 1)==INVALID_INPUT);
    assert(addPlayer(oasis, 1, 1, -1)==INVALID_INPUT);
    assert(addPlayer(oasis, 1, 1, 20)==FAILURE);
    for (int i = 0; i < 10; ++i) {
        assert(addPlayer(oasis,60+i,55+i,0)==SUCCESS);
        //  player(i) with id 60+i score 55+i clan0
    }
    for (int i = 0; i < 10; ++i) {
        assert(addPlayer(oasis,100+i,100+i,1)==SUCCESS);
        //  player(i) with id 100+i score 100+i clan1
    }
    for (int i = 0; i < 5; ++i) {
        assert(addPlayer(oasis,20+i,20+i,2)==SUCCESS);
        //  player(i) with id 20+i score 20+i clan2
    }
    //check if the players are in the system
    for (int i = 0; i < 10; ++i) {
        assert(addPlayer(oasis,60+i,55+i,0)==FAILURE);
        assert(addPlayer(oasis,100+i,100+i,1)==FAILURE);
        if(i<5)
            assert(addPlayer(oasis,20+i,20+i,2)==FAILURE);
    }
    for (int i = 3; i < 19; ++i) {
        assert(addPlayer(oasis,i,i,i)==SUCCESS);
        //the clans 3-18 get one player each that have id as the clan and score as the clan
        assert(addPlayer(oasis,i,i,i==FAILURE));
    }
    std::cout<<"Passed add player test..."<<std::endl;
}
void testFightAndMin(void* oasis){
    assert(clanFight(NULL, 0, 1, 1, 1)==INVALID_INPUT);
    assert(clanFight(oasis, -1, 1, 1, 1)==INVALID_INPUT);
    assert(clanFight(oasis, 0, -1, 1, 1)==INVALID_INPUT);
    assert(clanFight(oasis, 0, 1,0, 1)==INVALID_INPUT);
    assert(clanFight(oasis, 0, 1, 1, 0)==INVALID_INPUT);
    assert(clanFight(oasis, 0, 1, 11, 1)==FAILURE);
    assert(clanFight(oasis, 0, 1, 1, 11)==FAILURE);
    assert(clanFight(oasis,0,90,1,1)==FAILURE);
    assert(clanFight(oasis,90,0,1,1)==FAILURE);
    assert(getMinClan(oasis,NULL)==INVALID_INPUT);
    int min_alive;
    assert(getMinClan(NULL,&min_alive)==INVALID_INPUT);
    assert(getMinClan(oasis,&min_alive)==SUCCESS);
    assert(min_alive==0);
    assert(clanFight(oasis, 0, 1, 5, 5)==SUCCESS);    //first fight clan 1 won
    assert(getMinClan(oasis,&min_alive)==SUCCESS);
    assert(min_alive==1);
    assert(addClan(oasis,0)==FAILURE);
    assert(clanFight(oasis, 2, 1, 5, 1)==SUCCESS);    //clan2 won
    assert(getMinClan(oasis,&min_alive)==SUCCESS);
    assert(min_alive==2);
    assert(addClan(oasis,1)==FAILURE);
    assert(addPlayer(oasis,500,1000,19)==SUCCESS);
    assert(addPlayer(oasis,456,2,18)==SUCCESS);
    assert(clanFight(oasis,19,18,1,1)==SUCCESS);
    for (int i = 2; i < 17; ++i) {
        assert(clanFight(oasis, i, 19, 1, 1)==SUCCESS);    //clan19 won
        assert(clanFight(oasis,i-2,i-1,1,1)==FAILURE);      //they cant fight
        assert(getMinClan(oasis,&min_alive)==SUCCESS);
        assert(min_alive==i+1);
        assert(addClan(oasis,i)==FAILURE); //check clan exist
        assert(addPlayer(oasis,900+i,900+i,i)==SUCCESS); //check we can add players to clan
    }
    assert(clanFight(oasis, 17, 19, 1, 1)==SUCCESS); //19 win
    //clan 19 the only able to fight
    assert(addClan(oasis,50)==SUCCESS);
    assert(addClan(oasis,51)==SUCCESS);
    assert(addPlayer(oasis,777,9900,50)==SUCCESS);
    assert(clanFight(oasis,19,50,1,1)==SUCCESS);    //50 won
    assert(getMinClan(oasis,&min_alive)==SUCCESS);
    assert(min_alive==50);
    assert(addPlayer(oasis,776,9900,51)==SUCCESS);
    assert(clanFight(oasis,51,50,1,1)==SUCCESS);    //50 won - check who win if the have the same score
    assert(clanFight(oasis,51,50,1,1)==FAILURE);
    assert(getMinClan(oasis,&min_alive)==SUCCESS);
    assert(min_alive==50);
    std::cout<<"Passed fight and minimum clan alive test..."<<std::endl;
    min_alive=0; //just for csl..
}
void testQuit(void* oasis){
    void** tmp=NULL;
    void* temp=NULL;
    quit(tmp);
    quit(&temp);
    quit(&oasis);
    assert(oasis==NULL);
}

//this is an extra - not the test, erase if you want to run on csl
/*const int DONE =-1;
void printSomeChar(char c, int amount){
    for(int i=0; i<amount*2; i++)
        std::cout<<c;
}
void printLine(int *square_num, char first, char second){
    bool first_char=true;
    for(int i=0; square_num[i]!= DONE; i++){
        if(first_char){
            printSomeChar(first,square_num[i]);
            first_char= false;
        }else{
            printSomeChar(second,square_num[i]);
            first_char= true;
        }
    }
    std::cout << std::endl;
}
void pDown(char c, char space, int side){
    int move=6+side;
    int l1[] = {move+8,7,8+move,DONE};printLine(l1,space,c);
    int l2[] = {move+6,11,6+move,DONE};printLine(l2,space,c);
    int l3[] = {move+5,13,5+move,DONE};printLine(l3,space,c);
    int l4[] = {move+4,15,4+move,DONE};printLine(l4,space,c);
    int l5[] = {move+4,3,4,1,4,3,4+move,DONE};printLine(l5,space,c);
    int l6[] = {move+3,3,11,3,3+move,DONE};printLine(l6,space,c);
    int l7[] = {move+3,3,12,2,3+move,DONE};printLine(l7,space,c);
    int l8[] = {move+3,2,13,2,3+move,DONE};printLine(l8,space,c);
    int l9[] = {move+3,2,3,2,3,2,3,3,2+move,DONE};printLine(l9,space,c);
    int l10[] = {move+2,3,3,2,3,2,3,3,2+move,DONE};printLine(l10,space,c);
    int l11[] = {move+2,3,13,3,2+move,DONE};printLine(l11,space,c);
    int l12[] = {move+2,3,5,3,5,3,2+move,DONE};printLine(l12,space,c);
    int l13[] = {move+2,3,6,1,6,3,2+move,DONE};printLine(l13,space,c);
    int l14[] = {move+2,3,13,3,2+move,DONE};printLine(l14,space,c);
    int l15[] = {move+1,4,13,4,1+move,DONE};printLine(l15,space,c);
    int l16[] = {move+1,4,13,4,1+move,DONE};printLine(l16,space,c);
    int l17[] = {move,5,13,5,move,DONE};printLine(l17,space,c);
    int l18[] = {move,5,13,5,move,DONE};printLine(l18,space,c);
    int l19[] = {move,5,13,5,move,DONE};printLine(l19,space,c);
    int l20[] = {move,6,11,6,move,DONE};printLine(l20,space,c);
    int l21[] = {move,2,1,4,9,4,1,2,move,DONE};printLine(l21,space,c);
    int l22[] = {move,1,3,5,5,5,3,1,move,DONE};printLine(l22,space,c);
    int l23[] = {move+6,11,6+move,DONE};printLine(l23,space,c);
    int l24[] = {move+3,17,3+move,DONE};printLine(l24,space,c);
    int l25[] = {move+3,7,3,7,3+move,DONE};printLine(l25,space,c);
}
void pSide(char c, char space,int side){
    int move=6+side;
    int l1[] = {move+8,7,8+move,DONE};printLine(l1,space,c);
    int l2[] = {move+6,11,6+move,DONE};printLine(l2,space,c);
    int l3[] = {move+5,13,5+move,DONE};printLine(l3,space,c);
    int l4[] = {move+4,15,4+move,DONE};printLine(l4,space,c);
    int l5[] = {move+4,3,4,1,4,3,4+move,DONE};printLine(l5,space,c);
    int l6[] = {move+3,3,11,3,3+move,DONE};printLine(l6,space,c);
    int l7[] = {move+3,3,12,2,3+move,DONE};printLine(l7,space,c);
    int l8[] = {move+3,2,13,2,3+move,DONE};printLine(l8,space,c);
    int l9[] = {move+3,2,3,2,3,2,3,3,2+move,DONE};printLine(l9,space,c);
    int l10[] = {move+2,3,3,2,3,2,3,3,2+move,DONE};printLine(l10,space,c);
    int l11[] = {move+2,3,13,3,2+move,DONE};printLine(l11,space,c);
    move=side;
    int l12[] = {move+5,6,5,3,5,6,5+move,DONE};printLine(l12,space,c);
    int l13[] = {move+2,9,6,1,6,9,2+move,DONE};printLine(l13,space,c);
    int l14[] = {move,11,13,11,move,DONE};printLine(l14,space,c);
    int l15[] = {move+1,5,2,3,13,3,2,5,1+move,DONE};printLine(l15,space,c);
    int l16[] = {move+8,3,13,3,8+move,DONE};printLine(l16,space,c);
    int l17[] = {move+8,3,13,3,8+move,DONE};printLine(l17,space,c);
    int l18[] = {move+8,3,13,3,8+move,DONE};printLine(l18,space,c);
    int l19[] = {move+8,3,13,3,8+move,DONE};printLine(l19,space,c);
    int l20[] = {move+9,3,11,3,9+move,DONE};printLine(l20,space,c);
    int l21[] = {move+9,4,9,4,9+move,DONE};printLine(l21,space,c);
    int l22[] = {move+10,5,5,5,10+move,DONE};printLine(l22,space,c);
    move=6+side;
    int l23[] = {move+6,11,6+move,DONE};printLine(l23,space,c);
    int l24[] = {move+3,17,3+move,DONE};printLine(l24,space,c);
    int l25[] = {move+3,7,3,7,3+move,DONE};printLine(l25,space,c);
}
void pUp(char c, char space,int side){
    int move=6+side;
    int l1[] = {move+8,7,8+move,DONE};printLine(l1,space,c);
    int l2[] = {move+6,11,6+move,DONE};printLine(l2,space,c);
    int l3[] = {move+5,13,5+move,DONE};printLine(l3,space,c);
    int l4[] = {move+4,15,4+move,DONE};printLine(l4,space,c);
    int l5[] = {move+4,3,4,1,4,3,4+move,DONE};printLine(l5,space,c);
    int l6[] = {move+3,3,11,3,3+move,DONE};printLine(l6,space,c);
    move=4+side;
    int l7[] = {move,1,4,3,12,2,4,1,move,DONE};printLine(l7,space,c);
    int l8[] = {move,2,3,2,13,2,3,2,move,DONE};printLine(l8,space,c);
    int l9[] = {move,3,2,2,3,2,3,2,3,2,2,3,move,DONE};printLine(l9,space,c);
    int l10[] = {move,7,3,2,3,2,3,7,move,DONE};printLine(l10,space,c);
    int l11[] = {move+1,6,13,6,1+move,DONE};printLine(l11,space,c);
    int l12[] = {move+1,6,5,3,5,6,1+move,DONE};printLine(l12,space,c);
    int l13[] = {move+2,5,6,1,6,5,2+move,DONE};printLine(l13,space,c);
    int l14[] = {move+2,5,13,5,2+move,DONE};printLine(l14,space,c);
    int l15[] = {move+3,4,13,4,3+move,DONE};printLine(l15,space,c);
    move=4+side;
    int l16[] = {move+4,3,13,3,4+move,DONE};printLine(l16,space,c);
    int l17[] = {move+4,3,13,3,4+move,DONE};printLine(l17,space,c);
    int l18[] = {move+4,3,13,3,4+move,DONE};printLine(l18,space,c);
    int l19[] = {move+4,3,13,3,4+move,DONE};printLine(l19,space,c);
    int l20[] = {move+5,3,11,3,5+move,DONE};printLine(l20,space,c);
    int l21[] = {move+5,4,9,4,5+move,DONE};printLine(l21,space,c);
    int l22[] = {move+6,5,5,5,6+move,DONE};printLine(l22,space,c);
    move=6+side;
    int l23[] = {move+6,11,6+move,DONE};printLine(l23,space,c);
    int l24[] = {move+3,17,3+move,DONE};printLine(l24,space,c);
    int l25[] = {move+3,7,3,7,3+move,DONE};printLine(l25,space,c);
}
void printMovingP(char c, char space){
    system("cls");
    std::cout << "Get ready...";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    system("cls");
    int k=7, side=0;
    while(k--){
        pDown(c,space,side);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        system("cls");
        pSide(c,space,side);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        system("cls");
        pUp(c,space,side);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        system("cls");
        side+=2;
    }
    k=7; side-=2;
    while(k--){
        pDown(c,space,side);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        system("cls");
        pSide(c,space,side);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        system("cls");
        pUp(c,space,side);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        system("cls");
        side+=-2;

    }
}
void coverP(){
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    printMovingP(178,' ');
    while(true){
        std::cout<<"Once again?"<<std::endl<<"Press y/n"<<std::endl;
        char c;
        std::cin>>c;
        if(c=='n'){
            return;
        }else if(c=='y'){
            printMovingP(178,' ');
        }else{
            std::cout<<"Bad input :/"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            return;
        }
    }
}*/
//till here

int main(){
    int d = 0;
    int arr[]={0, 1};
    void* DS = init(2,arr);
    addClan(DS, 8);
    getMinClan(DS, &d);
    clanFight(DS, 6, 6, 5, 9);
    clanFight(DS, 2, 7, 2, 4);
    addPlayer(DS, 784, 326, -1);
    clanFight(DS, 7, 8, 2, -1);
    addPlayer(DS, 334, 323, 8);
    addClan(DS, 2);
    clanFight(DS, 3, 7, 8, 10);
    getMinClan(DS, &d);
    clanFight(DS, 6, 7, 4, 3);
    addPlayer(DS, 690, 65, 4);
    clanFight(DS, 3, 0, 4, 2);
    getMinClan(DS, &d);
    addPlayer(DS, 641, 584, 8);
    addPlayer(DS, 404, 322, 7);
    addClan(DS, 6);
    clanFight(DS, 8, 7, 7, 2);
    addClan(DS, -1);
    addClan(DS, 4);
    clanFight(DS, 3, 6, 7, 7);
    clanFight(DS, 6, 3, 5, 0);
    addPlayer(DS, 229, 833, 6);
    clanFight(DS, 6, 6, 6, 2);
    clanFight(DS, -1, 8, 4, 4);
    addClan(DS, 7);
    addPlayer(DS, 91, 418, 6);
    clanFight(DS, 4, 8, 8, 9);
    addPlayer(DS, 971, 692, 1);
    addPlayer(DS, 417, 193, -2);
    getMinClan(DS, &d);
    addPlayer(DS, 814, 316, 3);
    addClan(DS, 5);
    getMinClan(DS, &d);
    addPlayer(DS, 601, 403, 8);
    addPlayer(DS, 767, 182, 7);
    getMinClan(DS, &d);
    addClan(DS, 5);
    addPlayer(DS, 420, 849, 6);
    addPlayer(DS, 316, 734, -2);
    addClan(DS, 3);
    addPlayer(DS, 452, 720, -2);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 0, 8, -1, 9);
    addClan(DS, 4);
    clanFight(DS, 7, 7, 10, 6);
    getMinClan(DS, &d);
    clanFight(DS, 4, 8, 9, 7);
    clanFight(DS, 4, 2, 1, 2);
    getMinClan(DS, &d);
    addPlayer(DS, 692, 392, 5);
    addClan(DS, 2);
    addPlayer(DS, 160, 115, 4);
    clanFight(DS, 8, -1, 10, 7);
    addPlayer(DS, 827, 629, 8);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 5, 1, 6, 7);
    addClan(DS, 0);
    clanFight(DS, 0, 3, 3, 7);
    clanFight(DS, 1, 8, 2, 7);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 6);
    addClan(DS, 1);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 5);
    addPlayer(DS, 52, 94, 6);
    getMinClan(DS, &d);
    clanFight(DS, 4, 5, 0, 3);
    addPlayer(DS, 214, 505, -1);
    addPlayer(DS, 629, 218, 4);
    clanFight(DS, 8, 3, 7, 10);
    getMinClan(DS, &d);
    addPlayer(DS, 562, 500, 1);
    addPlayer(DS, 903, 565, 1);
    clanFight(DS, 8, 7, 5, 5);
    addPlayer(DS, 949, 762, 5);
    getMinClan(DS, &d);
    addPlayer(DS, 184, 206, -2);
    addClan(DS, -2);
    clanFight(DS, -1, 2, 2, 4);
    getMinClan(DS, &d);
    clanFight(DS, 3, 3, 7, 5);
    addPlayer(DS, 419, 871, 7);
    getMinClan(DS, &d);
    clanFight(DS, 5, 0, 6, 4);
    addClan(DS, 5);
    addClan(DS, 7);
    getMinClan(DS, &d);
    clanFight(DS, 4, 0, 6, -1);
    addPlayer(DS, 560, 782, -2);
    addClan(DS, -1);
    addClan(DS, 6);
    addClan(DS, -2);
    addClan(DS, 0);
    clanFight(DS, 1, 6, -1, 3);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 2, 3, 4, 6);
    getMinClan(DS, &d);
    addPlayer(DS, 462, 167, -2);
    addPlayer(DS, 441, 842, 3);
    addClan(DS, 4);
    addPlayer(DS, 654, 27, 0);
    addClan(DS, 0);
    getMinClan(DS, &d);
    clanFight(DS, 2, 0, 7, 2);
    getMinClan(DS, &d);
    clanFight(DS, 4, 5, 4, 1);
    addPlayer(DS, 417, 311, 4);
    getMinClan(DS, &d);
    clanFight(DS, 1, 8, 10, 6);
    addPlayer(DS, 124, 388, 3);
    clanFight(DS, 6, 4, 10, 7);
    addClan(DS, 3);
    getMinClan(DS, &d);
    addClan(DS, 6);
    addClan(DS, 0);
    addClan(DS, 4);
    getMinClan(DS, &d);
    addClan(DS, 5);
    addPlayer(DS, 857, 894, -2);
    addClan(DS, 0);
    addClan(DS, 1);
    addPlayer(DS, 103, 667, 6);
    clanFight(DS, 2, 3, 5, 9);
    getMinClan(DS, &d);
    addClan(DS, 3);
    addPlayer(DS, 477, 47, 1);
    clanFight(DS, 0, 7, 3, 2);
    addClan(DS, 2);
    addClan(DS, 5);
    addClan(DS, 7);
    clanFight(DS, 0, 6, 7, 1);
    getMinClan(DS, &d);
    clanFight(DS, 3, 1, -1, 9);
    getMinClan(DS, &d);
    addClan(DS, 7);
    addPlayer(DS, 559, 558, 8);
    addClan(DS, 6);
    getMinClan(DS, &d);
    clanFight(DS, 8, 5, 9, 7);
    addClan(DS, -1);
    addClan(DS, -2);
    clanFight(DS, 6, 3, 5, 8);
    addClan(DS, -2);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, -1);
    getMinClan(DS, &d);
    addClan(DS, 3);
    getMinClan(DS, &d);
    addClan(DS, 3);
    addPlayer(DS, 627, 519, 8);
    addClan(DS, 7);
    addPlayer(DS, 491, 535, 7);
    getMinClan(DS, &d);
    addPlayer(DS, 911, 23, 6);
    clanFight(DS, 5, 6, 8, 7);
    addClan(DS, 4);
    addClan(DS, 0);
    clanFight(DS, 8, 2, -1, -1);
    clanFight(DS, 5, 8, 6, 7);
    addPlayer(DS, 628, 651, -1);
    addClan(DS, 7);
    addPlayer(DS, 883, 699, 2);
    addClan(DS, 0);
    addClan(DS, 5);
    addPlayer(DS, 297, 174, 8);
    getMinClan(DS, &d);
    addClan(DS, 7);
    getMinClan(DS, &d);
    addPlayer(DS, 7, 798, 3);
    getMinClan(DS, &d);
    clanFight(DS, 0, 1, 9, -1);
    clanFight(DS, 2, 8, 3, 5);
    addClan(DS, 1);
    clanFight(DS, 3, 7, -1, 2);
    addClan(DS, 3);
    clanFight(DS, 0, 2, 0, 2);
    addPlayer(DS, 852, 505, 7);
    getMinClan(DS, &d);
    addClan(DS, 6);
    getMinClan(DS, &d);
    clanFight(DS, 1, 5, 6, 3);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 4);
    addPlayer(DS, 899, 595, 3);
    addClan(DS, 5);
    addPlayer(DS, 265, 817, 0);
    clanFight(DS, 5, 7, 6, 8);
    addClan(DS, -1);
    clanFight(DS, 4, 2, 6, 5);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 1);
    addPlayer(DS, 29, 675, 7);
    addClan(DS, 7);
    addPlayer(DS, 273, 229, 8);
    clanFight(DS, -1, 1, 0, 7);
    getMinClan(DS, &d);
    addClan(DS, 8);
    clanFight(DS, 1, 8, 5, 6);
    getMinClan(DS, &d);
    addPlayer(DS, 406, 843, 5);
    getMinClan(DS, &d);
    clanFight(DS, 5, 3, 3, 3);
    addPlayer(DS, 644, 303, 0);
    addPlayer(DS, 238, 328, -2);
    clanFight(DS, 4, 7, 5, -1);
    getMinClan(DS, &d);
    clanFight(DS, 7, 4, 1, 9);
    addPlayer(DS, 291, 880, 3);
    addClan(DS, 8);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, -1);
    addClan(DS, 6);
    clanFight(DS, 7, 8, 3, 4);
    addClan(DS, 3);
    addPlayer(DS, 539, 519, 0);
    clanFight(DS, 4, 8, -1, -1);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addPlayer(DS, 708, 294, 7);
    clanFight(DS, 2, 6, 5, 4);
    clanFight(DS, 5, 6, 0, 7);
    addPlayer(DS, 532, 856, 8);
    addClan(DS, -1);
    addPlayer(DS, 149, 572, 6);
    addPlayer(DS, 661, 714, 8);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 4, 1, 8, 3);
    addClan(DS, 3);
    clanFight(DS, 5, 3, 9, 9);
    clanFight(DS, 3, 7, 9, -1);
    addClan(DS, 1);
    getMinClan(DS, &d);
    addPlayer(DS, 89, 812, -1);
    addPlayer(DS, 943, 52, 4);
    clanFight(DS, 0, 7, 9, 9);
    addClan(DS, 0);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addPlayer(DS, 566, 356, 6);
    addPlayer(DS, 460, 889, 8);
    getMinClan(DS, &d);
    addPlayer(DS, 220, 441, 8);
    addClan(DS, 8);
    addPlayer(DS, 58, 293, 1);
    addPlayer(DS, 21, 286, 7);
    clanFight(DS, -1, 0, 7, 0);
    getMinClan(DS, &d);
    addPlayer(DS, 147, 946, 8);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, -1, -1, 10, 4);
    addPlayer(DS, 748, 4, 5);
    clanFight(DS, 0, 6, 4, 0);
    addClan(DS, 3);
    getMinClan(DS, &d);
    addPlayer(DS, 775, 602, 4);
    getMinClan(DS, &d);
    addPlayer(DS, 242, 701, 8);
    clanFight(DS, 3, 5, 5, 9);
    clanFight(DS, 3, 0, -1, 7);
    addPlayer(DS, 444, 448, -1);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 6, 4, -1, 0);
    addClan(DS, 0);
    getMinClan(DS, &d);
    addClan(DS, 3);
    addClan(DS, 8);
    getMinClan(DS, &d);
    addClan(DS, -2);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 4, 4, 1, 0);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 5);
    addPlayer(DS, 171, 340, 3);
    addClan(DS, -1);
    addPlayer(DS, 547, 539, 5);
    getMinClan(DS, &d);
    addPlayer(DS, 249, 899, -1);
    clanFight(DS, 7, 0, 6, 0);
    clanFight(DS, 7, 8, 1, 0);
    getMinClan(DS, &d);
    clanFight(DS, 5, -1, 2, 10);
    clanFight(DS, -1, 6, 1, 6);
    clanFight(DS, 0, 2, 7, -1);
    getMinClan(DS, &d);
    addPlayer(DS, 30, 169, 2);
    addPlayer(DS, 188, 445, 6);
    getMinClan(DS, &d);
    addClan(DS, 1);
    addClan(DS, 6);
    clanFight(DS, 0, 3, 5, 9);
    clanFight(DS, 6, 8, 6, 10);
    addClan(DS, 4);
    clanFight(DS, 6, 4, 6, 7);
    getMinClan(DS, &d);
    clanFight(DS, 5, -1, 8, 9);
    clanFight(DS, 5, 6, 10, 1);
    clanFight(DS, 5, 0, 7, -1);
    addPlayer(DS, 52, -4, 5);
    addPlayer(DS, 688, 311, 0);
    getMinClan(DS, &d);
    clanFight(DS, 0, 0, 6, 2);
    addClan(DS, 7);
    addPlayer(DS, 675, 895, 4);
    getMinClan(DS, &d);
    clanFight(DS, 4, 0, 10, 4);
    addPlayer(DS, 625, 357, -2);
    clanFight(DS, 0, 3, 10, 4);
    addClan(DS, 4);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 7, 0, 3, 9);
    addClan(DS, 7);
    addClan(DS, 5);
    clanFight(DS, 5, -1, 6, 8);
    addClan(DS, 1);
    clanFight(DS, 8, 4, 1, -1);
    getMinClan(DS, &d);
    addPlayer(DS, 0, 779, 0);
    addPlayer(DS, 602, 813, 6);
    clanFight(DS, -1, 7, -1, 8);
    clanFight(DS, 8, 8, 9, 0);
    getMinClan(DS, &d);
    clanFight(DS, 0, -1, 6, 1);
    addClan(DS, 0);
    addPlayer(DS, 82, 647, 5);
    addPlayer(DS, 755, 763, 3);
    addPlayer(DS, 643, 962, 5);
    addPlayer(DS, 629, 125, 5);
    getMinClan(DS, &d);
    addPlayer(DS, 479, 834, 8);
    getMinClan(DS, &d);
    addClan(DS, 2);
    addClan(DS, 4);
    getMinClan(DS, &d);
    addPlayer(DS, 957, 505, 5);
    getMinClan(DS, &d);
    addClan(DS, 5);
    getMinClan(DS, &d);
    clanFight(DS, 3, 0, 5, 6);
    getMinClan(DS, &d);
    addPlayer(DS, 866, 907, 7);
    addPlayer(DS, 468, 500, 2);
    clanFight(DS, 2, 5, 0, 6);
    getMinClan(DS, &d);
    addPlayer(DS, 327, 889, 8);
    getMinClan(DS, &d);
    addPlayer(DS, 919, 868, 4);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 5);
    clanFight(DS, 2, -1, 8, 6);
    getMinClan(DS, &d);
    addPlayer(DS, 247, 630, 5);
    addPlayer(DS, 889, 446, 2);
    clanFight(DS, 4, 1, 10, 0);
    addClan(DS, 5);
    addClan(DS, 4);
    getMinClan(DS, &d);
    addPlayer(DS, 360, 310, 4);
    addPlayer(DS, 390, 807, 2);
    addPlayer(DS, 979, 692, 8);
    addClan(DS, 5);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addPlayer(DS, 405, 797, 3);
    clanFight(DS, 6, 0, -1, 9);
    getMinClan(DS, &d);
    clanFight(DS, 0, -1, 10, 3);
    addClan(DS, 8);
    addClan(DS, 6);
    getMinClan(DS, &d);
    addClan(DS, -2);
    clanFight(DS, -1, 3, 9, 8);
    addClan(DS, 0);
    clanFight(DS, 3, 4, 9, 3);
    clanFight(DS, 7, 4, 9, 1);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 7, -1, 4, 9);
    addClan(DS, 2);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 6, 3, 10, -1);
    clanFight(DS, 1, 4, 5, 10);
    clanFight(DS, 4, 6, 3, 5);
    addClan(DS, 6);
    addClan(DS, -2);
    addClan(DS, 2);
    addClan(DS, 8);
    getMinClan(DS, &d);
    clanFight(DS, 7, 6, 5, 8);
    clanFight(DS, 3, 8, 9, 10);
    clanFight(DS, -1, 2, 6, 10);
    clanFight(DS, 3, 7, 9, 6);
    addPlayer(DS, 6, 603, -1);
    addPlayer(DS, 20, 775, 6);
    addPlayer(DS, 46, 522, 4);
    addPlayer(DS, 474, 697, -1);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 7, 3, 5, -1);
    getMinClan(DS, &d);
    clanFight(DS, 7, -1, 0, 10);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 1, 6, 3, 2);
    addPlayer(DS, 887, 415, 4);
    getMinClan(DS, &d);
    addClan(DS, 2);
    clanFight(DS, 7, 5, 0, 6);
    clanFight(DS, 6, -1, 7, 8);
    getMinClan(DS, &d);
    addClan(DS, 8);
    getMinClan(DS, &d);
    addClan(DS, 2);
    getMinClan(DS, &d);
    addClan(DS, 0);
    clanFight(DS, 6, 5, 9, 5);
    clanFight(DS, 5, 7, 0, 3);
    clanFight(DS, 5, 6, 1, 5);
    getMinClan(DS, &d);
    addClan(DS, 1);
    clanFight(DS, 5, 0, 5, 2);
    clanFight(DS, 2, 6, 6, 5);
    addPlayer(DS, 564, 897, 2);
    clanFight(DS, 3, 1, 5, 7);
    addClan(DS, 6);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 8);
    addPlayer(DS, 102, 515, 4);
    addClan(DS, 3);
    addClan(DS, 1);
    clanFight(DS, 5, 0, 0, 1);
    clanFight(DS, -1, 2, 7, 3);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addPlayer(DS, 833, 295, 8);
    addPlayer(DS, 346, 880, 6);
    addClan(DS, 1);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, -1);
    addClan(DS, 7);
    addPlayer(DS, -10, 568, 8);
    addPlayer(DS, 29, 585, -2);
    clanFight(DS, 8, 7, 1, 10);
    getMinClan(DS, &d);
    addPlayer(DS, 410, 635, -2);
    clanFight(DS, -1, 1, 0, 3);
    addClan(DS, 5);
    clanFight(DS, 1, -1, 4, 0);
    clanFight(DS, 3, -1, -1, 4);
    addPlayer(DS, 240, 1, 3);
    getMinClan(DS, &d);
    addPlayer(DS, 67, 620, 8);
    addPlayer(DS, 783, 620, 2);
    getMinClan(DS, &d);
    clanFight(DS, 4, 5, 3, 3);
    addPlayer(DS, 668, 65, 4);
    addClan(DS, 1);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 2, -1, 2, 10);
    addPlayer(DS, 756, 753, 0);
    addPlayer(DS, 411, 618, 2);
    addClan(DS, 3);
    addPlayer(DS, 828, 297, -1);
    addPlayer(DS, 785, 863, -2);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 5);
    clanFight(DS, 6, 5, 9, 0);
    addPlayer(DS, 415, 339, 6);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 1);
    getMinClan(DS, &d);
    addClan(DS, 7);
    getMinClan(DS, &d);
    clanFight(DS, 6, 3, 2, 10);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    clanFight(DS, 7, 6, 2, 7);
    addClan(DS, -2);
    clanFight(DS, 4, 2, 2, 5);
    clanFight(DS, 0, 7, 0, 9);
    getMinClan(DS, &d);
    addPlayer(DS, 382, 354, 6);
    addPlayer(DS, 425, 989, 1);
    getMinClan(DS, &d);
    addPlayer(DS, 979, 719, 4);
    addPlayer(DS, 781, 1000, 8);
    getMinClan(DS, &d);
    addClan(DS, -2);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, 0);
    getMinClan(DS, &d);
    addClan(DS, 6);
    getMinClan(DS, &d);
    clanFight(DS, 3, 6, 0, 7);
    addClan(DS, 1);
    addClan(DS, 5);
    getMinClan(DS, &d);
    clanFight(DS, 6, 7, 8, 5);
    addPlayer(DS, 436, 317, 4);
    clanFight(DS, 5, 5, 1, 1);
    addPlayer(DS, 630, 944, -2);
    addPlayer(DS, 264, 530, 1);
    addClan(DS, 2);
    addClan(DS, -2);
    clanFight(DS, 2, 5, 0, 8);
    addClan(DS, 0);
    addClan(DS, 6);
    getMinClan(DS, &d);
    addClan(DS, 5);
    clanFight(DS, 6, 6, 8, 8);
    getMinClan(DS, &d);
    addClan(DS, -2);
    addClan(DS, -2);
    getMinClan(DS, &d);
    addPlayer(DS, 922, 76, 2);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addPlayer(DS, 766, 291, 6);
    getMinClan(DS, &d);
    addPlayer(DS, 416, 929, -1);
    addPlayer(DS, 991, 400, 5);
    clanFight(DS, 3, 8, 4, 8);
    addClan(DS, 7);
    getMinClan(DS, &d);
    addClan(DS, 3);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, -1);
    getMinClan(DS, &d);
    addPlayer(DS, 588, 89, 8);
    clanFight(DS, 4, 3, 5, 7);
    clanFight(DS, 5, 8, 3, 2);
    addClan(DS, -2);
    clanFight(DS, 1, 4, 10, 2);
    addClan(DS, 5);
    addClan(DS, 0);
    addPlayer(DS, 537, 44, 4);
    addPlayer(DS, 158, 461, 2);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addPlayer(DS, 655, 546, 5);
    addPlayer(DS, 524, 199, -2);
    getMinClan(DS, &d);
    clanFight(DS, 6, 5, 2, 0);
    addPlayer(DS, 457, 397, 5);
    clanFight(DS, 7, 7, -1, 10);
    getMinClan(DS, &d);
    getMinClan(DS, &d);
    addClan(DS, -2);
    clanFight(DS, 6, 6, 8, 6);
    getMinClan(DS, &d);
    addPlayer(DS, 172, 166, 3);
    clanFight(DS, 0, 1, 2, 1);
    getMinClan(DS, &d);
    addClan(DS, 1);
    addClan(DS, 4);
    addClan(DS, 4);
    addPlayer(DS, 578, 202, -1);
    clanFight(DS, 8, 2, 10, 3);
    addClan(DS, 8);
    clanFight(DS, 4, 6, 4, 1);
    clanFight(DS, 1, 8, 9, 7);
    clanFight(DS, 7, -1, 5, 10);
    clanFight(DS, 8, 2, 0, 7);
    getMinClan(DS, &d);
    addPlayer(DS, 987, 240, 7);
    addPlayer(DS, 189, 63, 4);
    clanFight(DS, 1, 8, 6, 8);
    quit(&DS);
    /*
    void* oasis=testInit1();
    assert(oasis);
    testAddClan1(oasis);
    quit(&oasis);
    oasis=testInit2(); //on oasis we got 0-19 clans till now
    assert(oasis);
    testAddClan2(oasis); //add some more clans that we wont use and check if all 0-19 are in
    testAddPlayer(oasis);
    testFightAndMin(oasis);
    testQuit(oasis);
*/
    //coverP();   //penguin fun :0
    return 0;
}