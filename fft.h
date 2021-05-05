// fft.h "������� �������������� �����"

#ifndef fftH
#define fftH

// ��� ���� �����������
enum TSmoothType {sNone=0,stTriangle,stHamming};

// ������� � ������, ����������� ��� ���������� FFT
class TFFT {
private:
    float *e_re,*e_im;  // ������������ (����������� ���������)
    float *xReC,*xImC;  // ��������������� �������
    float *tReC,*tImC;
    float *triWin;      // ����������� ���� �����������
    float *hamWin;      // ���� ����������� ��������
    int *revert;        // ������� ��� ������������� ����. � ������ �������
    int nn;             // ��������� FFT

public:
    int N;                      // ������ FFT
    short *ReData,*ImData;      // ������� ������
    // �������� ������
    short *E;                   // ������� ������
    short *ReC;                 // ������ ���. �����

    TFFT(int nn);
    ~TFFT();

    void Clear();               // ������� ������
    void Get(float *ReC,float *ImC);    // ��������� FFT � �����. ������
    void GetRe();               // FFT ��� ������. ����� � ������
    void GetE();    // ��������� ������� ������
    void ApplySmoothWindow(TSmoothType type);   // ��������� ���� �����������
};
#endif
