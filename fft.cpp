// fft.cpp "������� �������������� �����" 
//---------------------------------------------------------------
#include <math.h>
#include <stdlib.h>
#include <fstream.h>
#include <mem.h>
#pragma hdrstop

#include "fft.h"
//---------------------------------------------------------------


// �������� ������ � ������������ ������
// ����������� ������ ������  (2^nn)*(3*sizeof(float)+sizeof(int))
TFFT::TFFT(int nn_value)
{
    int i,k;

    nn = nn_value;
    N = 1<<nn;  // ����������� ������� ������

    e_re = new float[N/2];
    e_im = new float[N/2];
    xReC = new float[N];
    xImC = new float[N];
    tReC = new float[N];
    tImC = new float[N];
    revert = new int[N];
    ReData = new short[N];
    ImData = new short[N];
    E = new short[N/2];
    ReC = new short[N];
    triWin = new float[N];
    hamWin = new float[N];

    // ���� �����������
    for(i=0;i<N;++i)
    {
        // ����������� ����
        triWin[i] = (i<N/2) ? ((float)i*2.00/N) : (2.00-(float)i*2.00/N);
        // ���� ��������
        hamWin[i] = 0.54-0.46*cos(2.00*M_PI*i/(N-1));
    }

    // ����������� ��� FFT ������������
    for(i=0;i<N/2;++i) {
        e_re[i] = (float)cos(2.00*M_PI*i/(1<<nn));
        e_im[i] = (float)sin(2.00*M_PI*i/(1<<nn));
    }

    // revert[i] "��������������" nn ��� � ���������� i
    // ��������, 11010->01011
    for(i=0; i<N; ++i) {
        revert[i]=0;
        for(k=0;k<nn;++k)
            revert[i]|=(((i>>k)&1)<<(nn-k-1));
    }

    Clear();
}

// ������������ ������
TFFT::~TFFT()
{
    delete e_re;
    delete e_im;
    delete revert;
    delete tReC;
    delete tImC;
    delete xReC;
    delete xImC;

    delete triWin;
    delete hamWin;

    delete ReData;
    delete ImData;
    delete E;
    delete ReC;

}

// ��������� ������
void TFFT::Clear()
{
    memset(ReData,0,N*sizeof(short));
    memset(ImData,0,N*sizeof(short));
}

/* fft() ��������� ��������������� ������������ �����
    ��� ������� ������ ReData,ImData ������������ 2^nn
    (�������������� ������������ �������),
    � ���������� �� � ������� ReC, ImC.
*/
void TFFT::Get(float *ReC,float *ImC)
{
   int i,i1,i2,j,di,ddi,n,k;
    float t_re,t_im;
    float *src_re,*src_im,*dst_re,*dst_im,*tmp_re,*tmp_im;


    // ������ ������: ������� ������ ���������� � x, ������ ���������
        di = N/2;
        for(i = 0; i < di; ++i)
        {
            i2 = i + di;
            ReC[i] = (float)(ReData[i]+ReData[i2]);
            ImC[i] = (float)(ImData[i]+ImData[i2]);

            t_re = (float)(ReData[i]-ReData[i2]);
            t_im = (float)(ImData[i]-ImData[i2]);

            ReC[i2] = t_re*e_re[i] - t_im*e_im[i];
            ImC[i2] = t_re*e_im[i] + t_im*e_re[i];
         }

    // ��������������� ��������� source � destination.
    // ��� ���������� ��� ����������
    src_re = ReC;
    src_im = ImC;
    dst_re = xReC;
    dst_im = xImC;
    for(n = nn-1; n > 0; --n)
    {
      ddi=1<<n; di=ddi>>1;
      for(j = 0;j < N;j+=ddi)
      {
        for(i = 0; i < di; ++i)
        {
            i1 = j + i;
            i2 = i1 + di;
            dst_re[i1] = src_re[i1]+src_re[i2];
            dst_im[i1] = src_im[i1]+src_im[i2];

            t_re = src_re[i1]-src_re[i2];
            t_im = src_im[i1]-src_im[i2];

            k = i*(1<<(nn-n));
            dst_re[i2] = t_re*e_re[k] - t_im*e_im[k];
            dst_im[i2] = t_re*e_im[k] + t_im*e_re[k];

         }
       }
       // source � destination �������� �������
       tmp_re = src_re; tmp_im = src_im;
       src_re = dst_re; src_im = dst_im;
       dst_re = tmp_re; dst_im = tmp_im;
    }

    // ������������ ������������� � ���������� ������� � ������������

    /* ���� ������������ ��������� �� ������� ������,
         ��� �������������� � ���������������. ��� ��������
         nn ����� ����� ��������    */
    if(src_re != xReC)
      for(i=0;i<N;++i) {
        xReC[i] = src_re[i];
        xImC[i] = src_im[i];
      }
    for(i=0;i<N;++i) {
      ReC[i] = xReC[revert[i]];
      ImC[i] = xImC[revert[i]];
    }
}
//----------------------------------------------------------

/* GetRe() ��������� ��������������� �����. �����
    ��� ������� ������ ReData,ImData ������������ 2^nn
    (�������������� ������������ �������),
    � �������� �� �� ���������� ������ ReC (short)
*/
void TFFT::GetRe()
{
    Get(tReC,tImC);
    for(int i=0;i<N;++i)
        ReC[i] = min((float)32766.0,max(tReC[i],-(float)32766.0));
}
//----------------------------------------------------------

/* GetE() ��������� ������� ������
    ��� ������� ������ ReData ������������ 2^nn
    (�������������� ������������ �������),
    � ���������� �� � ������� E �������� 2^nn/2 (short).
*/
void TFFT::GetE()
{
    Get(tReC,tImC);

    // �������� ������� ������
    for(int i=0;i<N/2;++i) {
        E[i] = min(32000.00,sqrt(tReC[i]*tReC[i] +
                tImC[i]*tImC[i])/N);
    }
}
//----------------------------------------------------------

/* ��������� ���� ����������� �� ������ ReData,ImData */
void TFFT::ApplySmoothWindow(TSmoothType type)
{
    int i;

    switch(type) {
    case sNone:
        return;
    case stTriangle:    // �����������
        for(i=0;i<N;++i)
        {
            ReData[i] = ReData[i]*triWin[i];
            ImData[i] = ImData[i]*triWin[i];
        }
        break;
    case stHamming:     // �������
        for(i=0;i<N;++i)
        {
            ReData[i] = ReData[i]*hamWin[i];
            ImData[i] = ImData[i]*hamWin[i];
        }
        break;
    }
}



