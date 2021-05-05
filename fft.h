// fft.h "Быстрое преобразование Фурье"

#ifndef fftH
#define fftH

// Тип окна сглаживания
enum TSmoothType {sNone=0,stTriangle,stHamming};

// Функции и данные, необходимые для выполнения FFT
class TFFT {
private:
    float *e_re,*e_im;  // коэффициенты (комплексная экпонента)
    float *xReC,*xImC;  // вспомогательные массивы
    float *tReC,*tImC;
    float *triWin;      // треугольное окно сглаживания
    float *hamWin;      // окно сглаживания Хэмминга
    int *revert;        // индексы для переставления коэф. в нужный порядок
    int nn;             // основание FFT

public:
    int N;                      // размер FFT
    short *ReData,*ImData;      // входные данные
    // Выходные данные
    short *E;                   // энергии частот
    short *ReC;                 // только вещ. часть

    TFFT(int nn);
    ~TFFT();

    void Clear();               // очистка памяти
    void Get(float *ReC,float *ImC);    // получение FFT в компл. числах
    void GetRe();               // FFT для действ. входа и выхода
    void GetE();    // получение энергии частот
    void ApplySmoothWindow(TSmoothType type);   // наложение окна сглаживания
};
#endif
