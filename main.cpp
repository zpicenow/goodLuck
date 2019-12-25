#include"NeuronDataReader.h"
#include<stdio.h>
#include <stdlib.h>
//#include <mmsystem.h>
#include <winbase.h>
#include <ctime>
//
#include<iostream>
#include <winsock2.h>
#include <time.h>

using namespace std;

//#pragma comment(lib, "ws2_32.lib")
SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

SOCKADDR_IN addrSrv;
float *exdata;


clock_t start,mend;

void show(void *pVoid, BvhDataHeader *pHeader, float *pDouble);

void showCal(void *sender, CalcDataHeader *header, float *data);

void dataRecive(void* customedObj, void* sender, BvhDataHeader* header, float* data){
    //????????????

//    printf("%d", header);
//    printf("comein");
    mend = clock();
    double endtime=(double)(mend-start)/CLOCKS_PER_SEC;
    printf("Use Time:%f\n",endtime);
    show(sender, header, data);
//    printf("goout");
//    printf("**%d**", data);
//    printf("++%d++", header);

//    itoa(header->FrameIndex, buffer, 10);
//    printf("---%s---", buffer);


}
void calRecive(void* customedObj, void* sender, CalcDataHeader * header, float* data){
    printf("infun");
    showCal(sender, header, data);

}

void showCal(void *sender, CalcDataHeader *header, float *data) {
    printf("inffff");
    int index = 7;
    printf("-x:%dy:%dz:%d--", data[index*16],data[index*16+1],data[index*16+2]);

}

void show(void *sender, BvhDataHeader *header, float *data) {
    char *buffer;

    float dx[10], dy[10], dz[10], ry[10], rx[10], rz[10];
    int flag = 0;
    int cnt[10]={13,14,15,16,18,36,37,38,39,41};
    /**
     * 0 RightShoulder 13
     * 1 RightArm 14
     * 2 RightForeArm 15
     * 3 RightHand 16
     * 4 RightHandThumb2 18
     * 5 LiftShoulder 36
     * 6 LeftArm 37
     * 7 LeftForeArm 38
     * 8 LeftHand 39
     * 9 LeftThumb2 41
     */
//    printf("**%d**", data);
//    printf("++%d++", header);
    char str[1000];
    if (header->WithDisp) {

        for (int i = 0; i < 10; ++i) {
            dx[i] = data[cnt[i] * 6];
            dy[i] = data[cnt[i] * 6 + 1];
            dz[i] = data[cnt[i] * 6 + 2];
            ry[i] = data[cnt[i] * 6 + 3];
            rx[i] = data[cnt[i] * 6 + 4];
            rz[i] = data[cnt[i] * 6 + 5];
        }
        flag = 1;

    } else {
        for (int i = 0; i < 10; ++i) {
            rx[i] = data[cnt[i] * 3 + 1];
            ry[i] = data[cnt[i] * 3 + 0];
            rz[i] = data[cnt[i] * 3 + 2];

        }
        flag = 0;
    }


    sprintf(str,"%d;%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f;"
                "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f;"
                "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f;"
                "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f;"
                "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f;"
                "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",flag,
                dx[0],dx[1],dx[2],dx[3],dx[4],dx[5],dx[6],dx[7],dx[8],dx[9],
                dy[0],dy[1],dy[2],dy[3],dy[4],dy[5],dy[6],dy[7],dy[8],dy[9],
                dz[0],dz[1],dz[2],dz[3],dz[4],dz[5],dz[6],dz[7],dz[8],dz[9],
                ry[0],ry[1],ry[2],ry[3],ry[4],ry[5],ry[6],ry[7],ry[8],ry[9],
                rx[0],rx[1],rx[2],rx[3],rx[4],rx[5],rx[6],rx[7],rx[8],rx[9],
                rz[0],rz[1],rz[2],rz[3],rz[4],rz[5],rz[6],rz[7],rz[8],rz[9]);
    printf("%s\n--------------------------------------------\n", str);

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(1, 1);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return ;
    }

    if (LOBYTE(wsaData.wVersion) != 1 ||
        HIBYTE(wsaData.wVersion) != 1) {
        WSACleanup();
        return ;
    }



//    char ccc[10000]={};
//    sprintf(ccc, "%f,%f,%f", data[6],data[7],data[8]);
//    printf("---%s---", ccc);
//    printf("===%c===", ccc[0]);
    send(sockClient,  str,strlen(str), 0);





    WSACleanup();




}

int main() {

    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本机
    //addrSrv.sin_addr.S_un.S_addr = inet_addr("192.168.1.107");
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(8888);

    int sec = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));



    printf("%d", BRConnectTo("127.0.0.1", 7001));
    start = clock();


//     printf("--%d--", BRGetSocketStatus(BRStartUDPServiceAt(7003)));
    BRRegisterFrameDataCallback(0 , (FrameDataReceived)dataRecive);
//    BRRegisterCalculationDataCallback(0, (CalculationDataReceived)calRecive);
    while (true)
    {
        /* code */
    }

    Sleep(1000);
    printf("---%d---", exdata);
    return 0;
}