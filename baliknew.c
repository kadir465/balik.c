#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define deste_kart 24
#define oyuncu_kart 6

// fonksiyonları çağırdık
void karistir(int dizi[], int boyut);
void kartDagit(int deste[], int oyuncudizi[], int bilgisayarDizi[]);
bool balikKontrol(int dizi[], int boyut, int *balikSayisi);
void masadanKartCek(int deste[], int hedefDizi[], int *hedefKartSayisi);
void oyuncuKartSec(int deste[], int oyuncudizi[], int bilgisayarDizi[],
                   int *oyuncuKartSayisi, int *bilgisayarKartSayisi,
                   int *oyuncuBalikSayisi);
void bilgisayarKartSec(int deste[], int oyuncudizi[], int bilgisayarDizi[],
                       int *oyuncuKartSayisi, int *bilgisayarKartSayisi,
                       int *bilgisayarBalikSayisi);

int main() {
  srand(time(NULL));

  int deste[24] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
                   4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
  int bilgisayarDizi[6];
  int oyuncudizi[6];
  int oyuncuBalikSayisi = 0, bilgisayarBalikSayisi = 0;
  int bilgisayarKartSayisi = 0, oyuncuKartSayisi = 0;

  printf("balık oyunu başlıyor");
  kartDagit(deste, oyuncudizi, bilgisayarDizi);
  oyuncuKartSayisi = oyuncu_kart;
  bilgisayarKartSayisi = oyuncu_kart;

  while (oyuncuBalikSayisi < 2 &&
         bilgisayarBalikSayisi <
             2) { // balık sayıları 2 den küçükse oyuna devamedilecek
    oyuncuKartSec(deste, oyuncudizi, bilgisayarDizi, &oyuncuKartSayisi,
                  &bilgisayarKartSayisi, &oyuncuBalikSayisi);
    if (oyuncuBalikSayisi >= 2) {
      break;
    }
    bilgisayarKartSec(deste, oyuncudizi, bilgisayarDizi, &oyuncuKartSayisi,
                      &bilgisayarKartSayisi, &bilgisayarBalikSayisi);
  }
  if (oyuncuBalikSayisi >= 2) {
    printf("oyuncu kazandı");
  } else {
    printf("bilgisayar kazandı");
  }
  return 0;
}

void karistir(
    int dizi[],
    int boyut) { // destenin doğru dağılması için önce karıştırmak gerekli
  for (int i = boyut - 1; i > 0;
       i--) { // destede rastgale j değeri oluşturaraak i ye atar böyle
              // karıştırır
    int j = rand() % (i + 1);
    int temp = dizi[i];
    dizi[i] = dizi[j];
    dizi[j] = temp;
  }
}

void kartDagit(int deste[], int oyuncudizi[], int bilgisayarDizi[]) {
  karistir(deste, deste_kart);
  for (int i = 0; i < oyuncu_kart;
       i++) { // sırayla bir kart oyuncuya bir kart bilgisayara verilir
    oyuncudizi[i] = deste[i * 2];
    bilgisayarDizi[i] = deste[i * 2 + 1];
    deste[i * 2] = deste[i * 2 + 1] =
        0; // dağıtılan kaartlardan kurtulamk için 0 atadık
  }
  printf("oyuncunun kartları:\n ");
  for (int i = 0; i < oyuncu_kart; i++) {
    printf("%d ", oyuncudizi[i]);
  }
  printf("bilgisayarın kartları: [Gizli]\n  ");
  printf("masadaki kartlar:\n");
  printf("\n");
}

bool balikKontrol(int dizi[], int boyut, int *balikSayisi) {
  int sayac[7] = {0};
  for (int i = 0; i < boyut; i++) {
    if (dizi[i] !=
        0) { // 0 dan farklı sayıları tutar  eğer farklıysa sayacı arttırır
      sayac[dizi[i]]++;
      if (sayac[dizi[i]] == 4) { // sayaç 4 e eşitse balık yapar
        int kartno = dizi[i];
        int newİndex = 0;
        for (int k = 0; k < boyut;
             k++) { // balık yapan kartı kartno ya attık ve 0 atadık ki desteden
                    // çıkarma yapalım
          if (dizi[k] != kartno) {
            dizi[newİndex++] = dizi[k];
          }
        }
        for (int k = newİndex; k < boyut; k++) {
          dizi[k] = 0;
        }
        (*balikSayisi)++;
        return true;
      }
    }
  }
  return false;
}

void masadanKartCek(int deste[], int hedefDizi[], int *hedefKartSayisi) {
  for (int i = 0; i < deste_kart;
       i++) { // masadaki kart 0 değilse verilir ve seçilen kartı 0 yapar
    if (deste[i] != 0) {
      hedefDizi[(*hedefKartSayisi)++] = deste[i];
      deste[i] = 0;
      return;
    }
  }
  printf("masada kart kalmadı\n");
}

void oyuncuKartSec(int deste[], int oyuncudizi[], int bilgisayarDizi[],
                   int *oyuncuKartSayisi, int *bilgisayarKartSayisi,
                   int *oyuncuBalikSayisi) {
  printf("\n oyuncu hamlesi \n");
  printf("oyuncunu kartları");
  for (int i = 0; i < *oyuncuKartSayisi; i++) {
    printf("%d ", oyuncudizi[i]);
  }
  printf("\n");

  int secim;
  printf("oyunucudan hangi kartı istiyorsunuz\n");
  scanf("%d", &secim);

  bool kartVar = false;
  bool kartvar1 = false;
  int alinanKartSayisi = 0;
  for (int i = 0; i < *bilgisayarKartSayisi;
       i++) { // oyuncunu istediği kart bilgisayara var mı ona bakıılır
    if (bilgisayarDizi[i] == secim) {
      kartVar = true;
      oyuncudizi[(*oyuncuKartSayisi)++] =
          secim; // oyunuc kartı arttırılır bilgisayarın ki de azaltılır
      bilgisayarDizi[i] = 0;
      alinanKartSayisi++;
    }
  }
  if (kartVar) {
    kartvar1 = true;
    printf("%d adet %d kartı alındı.\n", alinanKartSayisi, secim);

    int yeniİindex = 0;
    for (int i = 0; i < *bilgisayarKartSayisi;
         i++) { // bilgisayardan alına kartları 0 yapmıştık onları almaz ve yeni
                // dizi yaptık
      if (bilgisayarDizi[i] != 0) {
        bilgisayarDizi[yeniİindex++] = bilgisayarDizi[i];
      }
    }
    *bilgisayarKartSayisi = yeniİindex;

    if (balikKontrol(oyuncudizi, *oyuncuKartSayisi, oyuncuBalikSayisi)) {
      printf("oyuncu balık yaptı %d\n", *oyuncuBalikSayisi);
      *oyuncuKartSayisi -= 4;
      if (kartvar1) {
        printf("oyuncu tekrar hamle yapıyor");// eğer kart varsa tekrar hamle yapomasını sağlıyoruz
        oyuncuKartSec(deste, oyuncudizi, bilgisayarDizi, oyuncuKartSayisi,
                      bilgisayarKartSayisi, oyuncuBalikSayisi);
      }
    }
    if (kartvar1) {
      printf("oyuncu tekrar hamle yapıyor");
      oyuncuKartSec(deste, oyuncudizi, bilgisayarDizi, oyuncuKartSayisi,
                    bilgisayarKartSayisi, oyuncuBalikSayisi);
    }

  } else {
    printf("Bilgisayarda %d kartı yok. Masadan kart çekiyorsunuz.\n", secim);
    masadanKartCek(deste, oyuncudizi, oyuncuKartSayisi);

    if (balikKontrol(oyuncudizi, *oyuncuKartSayisi, oyuncuBalikSayisi)) {
      printf("BALIK YAPTINIZ! Balık sayınız: %d\n", *oyuncuBalikSayisi);
      *oyuncuKartSayisi -= 4;
    }
  }
}
void bilgisayarKartSec(int deste[], int oyuncudizi[], int bilgisayarDizi[],
                       int *oyuncuKartSayisi, int *bilgisayarKartSayisi,
                       int *bilgisayarBalikSayisi) {
  int sayac[7] = {0};
  for (int i = 0; i < *bilgisayarKartSayisi; i++) {
    sayac[bilgisayarDizi[i]]++;
  }
  int secim = 0;
  int maxSayi = 0;
  for (int i = 1; i <= 6; i++) { // bilgisayarınkartlarına bakar en çok hangi
                                 // karttan varesa onu seçer
    if (sayac[i] > maxSayi) {
      maxSayi = sayac[i];
      secim = i;
    }
  }
  if (maxSayi <= 1) {
    secim = bilgisayarDizi
        [rand() % (*bilgisayarKartSayisi)]; // eğer kart sayıları 1 ve daha
                                            // küçükse rastgele bir kart seçer
  }
  printf("bilgisayar %d kartını sordu\n", secim);
  bool kartVar = false;
  bool kartvar1 = false;
  int alinanKartSayisi = 0;

  for (int i = 0; i < *oyuncuKartSayisi;
       i++) { // bilgisayarın istediği kart oyuncuda var mı ona bakılır
    if (oyuncudizi[i] == secim) {
      kartVar = true;
      bilgisayarDizi[(*bilgisayarKartSayisi)++] =
          secim; // kart varsa bilgisayarın kart sayısı arttırılır oyuncunun
                 // azaltılır 0 yapılır
      oyuncudizi[i] = 0;
      alinanKartSayisi++;
    }
  }
  if (kartVar) {
    kartvar1 = true;
    printf("%d adet %d kartı alındı.\n", alinanKartSayisi, secim);
    int yeniİndex = 0;
    for (int i = 0; i < *oyuncuKartSayisi;
         i++) { // oyuncudan alınan kartları 0 yapmıştık onları almaz ve yeni
                // dizi yaptık
      if (oyuncudizi[i] != 0) {
        oyuncudizi[yeniİndex++] = oyuncudizi[i];
      }
    }
    *oyuncuKartSayisi = yeniİndex;
    if (balikKontrol(
            bilgisayarDizi,
            *bilgisayarKartSayisi, // balık kontrolü yapılır balık varsa
                                   // bilgisayarın kart sayısı azaltılır
            bilgisayarBalikSayisi)) {
      printf("bilgisayar balık yaptı %d\n", *bilgisayarBalikSayisi);
      *bilgisayarKartSayisi -= 4;
      if (kartvar1) {// eğer kart varsa tekrar hamle yapomasını sağlıyoruz
        printf("bilgisayar tekrar hamle yapıyor");
        bilgisayarKartSec(deste, oyuncudizi, bilgisayarDizi, oyuncuKartSayisi,
                          bilgisayarBalikSayisi, bilgisayarKartSayisi);
      }
    }
    if (kartvar1) {
      printf("bilgisayar tekrar hamle yapıyor");
      bilgisayarKartSec(deste, oyuncudizi, bilgisayarDizi, oyuncuKartSayisi,
                        bilgisayarBalikSayisi, bilgisayarKartSayisi);
    }
  } else {
    printf("Sizde %d kartı yok. Bilgisayar masadan kart çekiyor.\n", secim);
    masadanKartCek(deste, bilgisayarDizi, bilgisayarKartSayisi);

    if (balikKontrol(bilgisayarDizi, *bilgisayarKartSayisi,
                     bilgisayarBalikSayisi)) {
      printf("BILGISAYAR BALIK YAPTI! Bilgisayarın balık sayısı: %d\n",
             *bilgisayarBalikSayisi);
      *bilgisayarKartSayisi -= 4;
    }
  }
}
