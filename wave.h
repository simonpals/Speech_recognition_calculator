// wave.h "������� ������ � wave-������" 
#ifndef WAVE_H
#define WAVE_H

#include <vcl.h>
#include <mmsystem.h>
#include <stdio.h>
#include <math.h>

void Write16BitsLowHigh(FILE *fp, int i);   // ������ 2 ����
void Write32BitsLowHigh(FILE *fp, int i);   // ������ 4 ����
// �������� ����������� ������ str � ����� fp
void CheckString(FILE *fp,char *str) throw(Exception);
double ToLog(double v);     // ������� � ��������������� �������
void ErrorMessage(AnsiString message);  // ������ ��������� �� ������
TColor ToRGB(int c);    // ������� ������������� 0..255 � TColor

// �������� ��������� �����
class TWave {
public:
    int  Channels;		    // ����� ������� (����/������)
    int  Bits;			    // �����������
    long SampleRate;	    // ������� �������������
    long TotalSamples;	    // ������ � �������
    long Length;		    // ������ � ��������
    char Name[50];          // ��� �����
    char Description[256];  // ��������
    FILE *fp;               // ��������� �� ����
    bool ForWriting;        // ������� ��� ������ �����

    // ��������� ���� ��� ������
    TWave(char *name,int  channels,int  bits,long samplerate);
	// ����������� ����
    TWave(char *name);
    ~TWave();
    // �������� �����
	void 	Close();
    // ������ � ������ sfirst ������ �������� samples � ����� buffer
    // ���������� true � ������ ��������� ������
	bool GetData(long pos,short *buffer,int samples);
    // ���������� ����� � ����
    void WriteData(short *buffer,unsigned long samples);
};



/*----------------------------------------------------------//
  ���� WaveRecorder. ������������ ��� ������ �
  ��������������� ����� � ������� WAVE_PCM 44100Hz 16bit mono
  � ������������������.
/*-----------------------------------------------------------*/

#define NBUF 3	// ����� ������� ������/���������������

// ������� ��������� ������
typedef void (*waveInProc)(DWORD pinstance,DWORD pbuffer);

class TWaveRecorder:public TThread{
private:
	HWAVEIN hWaveIn;            // ���������� ���������� �����
	WAVEFORMATEX  waveFormat;   // ������ �����
	WAVEHDR WaveHdr[NBUF];      // ��������� ��� �������
    int curdata;                // ������� �����
    HANDLE hEvent;			    // ������-�������

    void __fastcall ProcessCallback(void); // ������������� � ����� �����������
    waveInProc Proc;            // ������� ��������� ������
    DWORD pinstance;            // ���. ��������
    bool synchronize;           // ������������� �������������
public:
	DWORD       dwDataSize;     // ������ ������ ������
    bool recording;             // ��������� ������
	TWaveRecorder(long samples,int sampleRate,
   			waveInProc Proc_v=0,DWORD pinstance_v=0,bool synchronize_v=true);
    virtual void __fastcall Execute();  // �������������� ������� �����
    void Stop();                // ��������� ������
};

/*  ����� WavePlayer
������������ ��� ��������������� ����������� ������
*/
/*
class TWavePlayer {
private:
	HANDLE hData[NBUF];
	HPSTR  lpData[NBUF];
	HGLOBAL hWaveHdr[NBUF];
	LPWAVEHDR lpWaveHdr[NBUF];
	HWAVEOUT    hWaveOut;
    WAVEFORMATEX  Format;
    HWND hWnd;
	DWORD       dwDataSize;
    int curdata;
    bool playing;

public:
	long blockSize;	// ������ ����� � �������

    void Start();
    void Stop();
    void AddBuffer();
    TWavePlayer(long v_blockSize,int sampleRate,HWND hw=0);
    ~TWavePlayer();
    short *GetBufer();
};

//---------------------------------------------------------------------------
*/
#endif


