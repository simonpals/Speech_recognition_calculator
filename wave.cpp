// wave.cpp "??????? ?????? ? wave-??????"
//#include <vcl.h>
#pragma hdrstop

#include "wave.h"

//--------------------------------------------------------
double ToLog(double v)
{
    return (2.00+log10(0.01+v))/2.00;
}

//--------------------------------------------------------
void CheckString(FILE *fp,char  *str) throw(Exception)
{
    char temp[1024];
    unsigned int  length = strlen(str);
    if(fread(temp,1,length,fp)!=length)
        throw Exception("???? ?????????? ??? ???????? ?????? ?????");
    temp[length] = (char)0;
    if(strcmp(str,temp))
        throw Exception("???? ?????????? ??? ???????? ?????? ?????");
}

//--------------------------------------------------------
void Write16BitsLowHigh(FILE *fp,int i)
{
    putc(char(i&0xff),fp);
    putc(char((i>>8)&0xff),fp);
}
//--------------------------------------------------------
void Write32BitsLowHigh(FILE *fp,int i)
{
    Write16BitsLowHigh(fp,(int)(i&0xffffL));
    Write16BitsLowHigh(fp,(int)((i>>16)&0xffffL));
}
//--------------------------------------------------------

void ErrorMessage(AnsiString message)
{
    Application->MessageBox(message.c_str(),"???????",MB_OK|MB_ICONSTOP);
}
//--------------------------------------------------------

TColor ToRGB(int c)
{
    return TColor((c<<16)+(c<<8)+c);
}
//--------------------------------------------------------


TWave::TWave(char *name,int  channels, int  bits,
                       long samplerate)
{
	Channels = channels;
    Bits = bits;
    SampleRate = samplerate;
    TotalSamples = 0;
    Length = 0;
    strcpy(Name,name);
    ForWriting = true;
    fp=fopen(Name,"wb");
    if(!fp)
   		throw Exception("??????? ????????? ?????");

    fseek(fp,44L,SEEK_SET);

    static char    *channel_mappings[] = {NULL,"mono","stereo"};
    sprintf(Description,"PCM, %s %ldHz %dbit, %2ld:%2ld:%2ld",
		channel_mappings[Channels], SampleRate,
		 Bits,(Length/3600),(Length/60)%60,(Length%60));

}

TWave::TWave(char *name)
{
    strcpy(Name,name);
    ForWriting = false;
    fp=fopen(Name,"rb");
    if(!fp)
   		throw Exception("??????? ????????? ?????");

	// ?????? ????????? ?????
	fseek(fp,8L,SEEK_SET);
    CheckString(fp,"WAVEfmt ");

    DWORD header_size;
	WAVEFORMATEX wfx;
    fread(&header_size,4,1,fp);
	if(header_size>sizeof(wfx))
    	throw Exception("????????? ??????");
    fread(&wfx,header_size,1,fp);
    if(wfx.wFormatTag!=WAVE_FORMAT_PCM)
    	throw Exception("?? ????????????? ???????? ????");
    CheckString(fp,"data");
    DWORD datasize;
    fread(&datasize,4,1,fp);
    // ????????? ????????

    Channels   = wfx.nChannels;
    SampleRate = wfx.nSamplesPerSec;
    Bits       = wfx.wBitsPerSample;
    if(Bits!=16)
        throw Exception("??????? ?????? ?????? ? 16 ????????? ??????");

    TotalSamples = datasize/(2*Channels);
    Length = TotalSamples/SampleRate;

    static char    *channel_mappings[] = {NULL,"mono","stereo"};
    sprintf(Description,"PCM, %s %ldHz %dbit, %2ld:%2ld:%2ld",
		channel_mappings[Channels], SampleRate,
		 Bits,(Length/3600),(Length/60)%60,(Length%60));
}

TWave::~TWave()
{
    Close();
}

void TWave::Close()
{
	long file_size;

    // ?????????? ????????? ?????
    if(ForWriting) {
   		fseek(fp,0L,SEEK_END);
		file_size = ftell(fp);
    	fseek(fp,0L,SEEK_SET);

    	fputs("RIFF",fp);
    	// write file size
		Write32BitsLowHigh(fp,file_size);
    	fputs("WAVE",fp);
    	fputs("fmt ",fp);
    	// write fmt_ chunk size
    	Write32BitsLowHigh(fp,16L);
    	// write format tag
	    Write16BitsLowHigh(fp,0x0001);
    	// write channels
    	Write16BitsLowHigh(fp,Channels);
    	// write samplerate
    	Write32BitsLowHigh(fp,SampleRate);
    	// write average bytes per second
    	Write32BitsLowHigh(fp,SampleRate*Channels*Bits/8);
    	// write block align
    	Write16BitsLowHigh(fp,2);
    	// write bits per sample
    	Write16BitsLowHigh(fp,Bits);
    	// data begins
    	fputs("data",fp);
    	// data chunk size
		Write32BitsLowHigh(fp,file_size-44);
    }
   	fclose(fp);
    fp = NULL;
}


bool TWave::GetData(long smplpos,short *buffer,int samples)
{
    short			*temp_buf = new short[samples*Channels];
    int				samples_read;
    int				j;
	// ????????? ??????? ????????????? ????? ? ??????? ??????? ?????????????

    long pos=(long)smplpos*Channels*2+44;
    fseek(fp,pos,SEEK_SET);

	// ?????? ? ?????
	samples_read = fread(temp_buf,Channels*2,samples, fp);
    if(samples_read == 0) {
    	// ????????? ??????
		for(j=0;j < samples;j++)
	   		buffer[j] = 0;
    	return false;
    }
	// ???????? ????? ?? ??????????
	for(j=0;j < samples_read;j++)
    {
			if(Channels == 1)
			    // ???? ????
				buffer[j] = temp_buf[j];
			else
				// ???? ??????, ?????????? ??? ??????
				buffer[j] = (char)((temp_buf[2*j]+temp_buf[2*j+1])/2);
    }
	// ????????? ????????????? ?????? ??????
	for(j=samples_read;j < samples;j++)
   		buffer[j] = 0;

    delete temp_buf;
    return true;
}

void TWave::WriteData(short *buffer,unsigned long samples)
{
	if(!ForWriting)
        throw Exception("???? ????????? ?????? ??? ???????");

	if(fwrite(buffer,Bits*Channels/8,samples,fp)!=samples)
        throw Exception("??????? ???????????");

    TotalSamples+=samples;
}

//----------------------------------------------------------------
// ????? TWaveRecorder - ???? ?????
//----------------------------------------------------------------

// ??????? ?????? ????????? ?????? Windows
char msg[300];
void __fastcall Win32Check(bool isOk) throw(Exception)
{
	if(isOk)
    	return;
    FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
    				0,GetLastError(),0,msg,sizeof(msg)-1,0);
    throw Exception(msg);
}

// ??????? ?????? ?????? ?????????? ??????
void __fastcall WaveInCheck(MMRESULT res) throw (Exception)
{
	if(MMSYSERR_NOERROR == res)
    	return;
    char ErrText[MAXERRORLENGTH+1] = "";
    waveInGetErrorText(res,ErrText,sizeof(ErrText));
    throw Exception(ErrText);
}


TWaveRecorder::TWaveRecorder(long samples,int sampleRate,
   			waveInProc Proc_v,DWORD pinstance_v,bool synchronize_v)
                :TThread(true)
{
		Proc = Proc_v;
        pinstance = pinstance_v;
        synchronize = synchronize_v;

		Priority = tpHigher;
    	waveFormat.wFormatTag=WAVE_FORMAT_PCM;
	    waveFormat.nChannels=1;
	    waveFormat.nSamplesPerSec=sampleRate;
	    waveFormat.wBitsPerSample=16;
	    waveFormat.nBlockAlign=WORD(waveFormat.wBitsPerSample/8*waveFormat.nChannels);
	    waveFormat.nAvgBytesPerSec=waveFormat.nSamplesPerSec*waveFormat.wBitsPerSample*waveFormat.nChannels/8;
	    waveFormat.cbSize=0;

    	dwDataSize=samples*waveFormat.wBitsPerSample/8*waveFormat.nChannels;
        recording = false;
        Resume();
}

// ??????? ??????? ??????
void __fastcall TWaveRecorder::Execute()
{
    hWaveIn=0;
    // ????? ???????? ?????? ?????
    int WaitTime = dwDataSize*1000*NBUF/waveFormat.nAvgBytesPerSec;

	__try
    {
    try
    {
	    // ????????? ??????????
	    ZeroMemory(WaveHdr,sizeof(WaveHdr));
        for(int i=0;i<NBUF;++i) {
		    // ????????? ?????? ??? ??????
		    Win32Check((WaveHdr[i].lpData = (LPSTR)new BYTE[dwDataSize])!=0);
		    WaveHdr[i].dwBufferLength = dwDataSize;
		    WaveHdr[i].dwFlags = 0L;
		    WaveHdr[i].dwLoops = 0L;
        }
        // ??????? ??????-???????
        hEvent = CreateEvent(0,TRUE,FALSE,0);
        Win32Check(hEvent!=0);
	    // ????????? ?????????? ??????
	    WaveInCheck(waveInOpen((LPHWAVEIN)&hWaveIn, WAVE_MAPPER,
	                    (LPWAVEFORMATEX)&waveFormat,
	                    (DWORD)hEvent, 0, CALLBACK_EVENT));

        // ????????? ??? ?????? ? ??????? ??????
        for(int i=0;i<NBUF;++i) {
		    WaveInCheck(waveInPrepareHeader(hWaveIn,WaveHdr+i,
            			sizeof(WAVEHDR)));
	    	WaveInCheck(waveInAddBuffer(hWaveIn,WaveHdr+i,
            			sizeof(WAVEHDR)));
        }
        curdata=0;

        // ??????-??????? ??? ???????? ?????????? ??? ????????? ? ????. "????????"
        // ?????????? ???.
        ResetEvent(hEvent);

        // ?????? ??????
	    WaveInCheck(waveInStart(hWaveIn));
        recording = true;

        while(true)
        {
        	// ??????? ?????? ?????????? ?????
//            if(WAIT_TIMEOUT==WaitForSingleObject(hEvent,WaitTime))
//            	throw Exception("????? ????? ???????");
			WaitForSingleObject(hEvent,WaitTime);
            ResetEvent(hEvent);

            // ???? ???? ????????? ??????
            if(Terminated)
              {
              break;
              }

            // ???????? ???????-?????????? ?????
            if(synchronize)
		    	Synchronize(ProcessCallback);
            else
				ProcessCallback();

            // ????????? ? ??????? ????????? ?????
		    WaveInCheck(waveInAddBuffer(hWaveIn,WaveHdr+curdata,
            		sizeof(WAVEHDR)));
            // ?????? ??????? ?????
            curdata=(curdata+1)%NBUF;
        }
	}
    catch (Exception &e)
    {
    	ErrorMessage(e.Message);
    }
    }
    __finally
    {
    	// ????????? ??????????
        if(hWaveIn) {
			waveInReset(hWaveIn);
			for(int i=0;i<NBUF;++i)
			    waveInUnprepareHeader(hWaveIn,WaveHdr+i,sizeof(WAVEHDR));
		    waveInClose(hWaveIn);
        }
        recording = false;
    	CloseHandle(hEvent);
		for(int i=0;i<NBUF;++i)
		    delete (WaveHdr[i].lpData);
    }
}

void __fastcall TWaveRecorder::ProcessCallback(void)
{
	if(Proc)
 		Proc(pinstance,(DWORD)(WaveHdr[curdata].lpData));
}

// ???????? ????????? ? ????????????? ????????? ??????
void TWaveRecorder::Stop()
{
    Terminate();
}


/*--------------------------------------------------------------
????? TWavePlayer ????????????? ???????? ?????? ?? ??????
---------------------------------------------------------------*/
/*
TWavePlayer::TWavePlayer(long v_blockSize,int SampleRate,HWND hw)
{
	blockSize = v_blockSize;
    hWnd = hw;
    Format.wFormatTag=WAVE_FORMAT_PCM;
    Format.nChannels=1;
    Format.nSamplesPerSec=SampleRate;
    Format.wBitsPerSample=16;
    Format.nBlockAlign=Format.wBitsPerSample*Format.nChannels/8;
    Format.nAvgBytesPerSec=Format.nSamplesPerSec*Format.wBitsPerSample*Format.nChannels/8;
    Format.cbSize=0;

    	dwDataSize=blockSize*Format.wBitsPerSample/8*Format.nChannels;
	    // Allocate and lock memory for the waveform data.
        for(int i=0;i<NBUF;++i) {
		    hData[i] = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, dwDataSize );
		    lpData[i] = (HPSTR)GlobalLock(hData[i]);
		    // Allocate and lock memory for the header.
		    hWaveHdr[i] = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE,
		        (DWORD) sizeof(WAVEHDR));
		    lpWaveHdr[i] = (LPWAVEHDR) GlobalLock(hWaveHdr[i]);
		    // After allocation, set up and prepare header.
		    lpWaveHdr[i]->lpData = lpData[i];
		    lpWaveHdr[i]->dwBufferLength = dwDataSize;
		    lpWaveHdr[i]->dwFlags = 0L;
		    lpWaveHdr[i]->dwLoops = 0L;
        }
    playing = false;
}

void TWavePlayer::Start()
{
	MMRESULT  wResult;
    Stop();
    // Open a waveform device for output using window callback.
    if(waveOutOpen((LPHWAVEOUT)&hWaveOut, WAVE_MAPPER,
                    (LPWAVEFORMATEX)&Format,
                    (DWORD)hWnd, 0, CALLBACK_WINDOW))
	    throw Exception("?? ??????? ??????? ?????????? ?????? ?????");
    playing = true;
    // After allocation, set up and prepare header.
    for(int i=0;i<NBUF;++i) {
	    waveOutPrepareHeader(hWaveOut, lpWaveHdr[i], sizeof(WAVEHDR));
	    wResult = waveOutWrite(hWaveOut, lpWaveHdr[i], sizeof(WAVEHDR));
	    if (wResult != 0)
	    {
        	for(int j=0;j<=i;++j)
            {
		        waveOutUnprepareHeader(hWaveOut, lpWaveHdr[j],sizeof(WAVEHDR));
            }
	    	throw Exception("?? ??????? ????????????? ????");
	    }
    }
    curdata=0;
}

void TWavePlayer::Stop()
{
    waveOutReset(hWaveOut);
    for(int i=0;i<NBUF;++i)
	    waveOutUnprepareHeader(hWaveOut, lpWaveHdr[i],sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    playing = false;
    curdata=0;
}

TWavePlayer::~TWavePlayer()
{
	playing = false;
    waveOutReset(hWaveOut);
    for(int i=0;i<NBUF;++i) {
	    waveOutUnprepareHeader(hWaveOut, lpWaveHdr[i],sizeof(WAVEHDR));
	    GlobalUnlock( hWaveHdr[i]);
	    GlobalFree(hWaveHdr[i]);
	    GlobalUnlock( hData[i]);
	    GlobalFree(hData[i]);
    }
    waveOutClose(hWaveOut);
}

void TWavePlayer::AddBuffer()
{
	MMRESULT wResult;

    if(!playing)
    	return;

    wResult = waveOutWrite(hWaveOut, lpWaveHdr[curdata], sizeof(WAVEHDR));
    // ?????? ??????? ?????
    curdata=(curdata+1)%NBUF;
    if (wResult != 0) {
    	Stop();
        throw Exception("?? ??????? ????????????? ????");
    }
}

short* TWavePlayer::GetBufer()
{
    	return (short*)lpData[curdata];
}
*/
