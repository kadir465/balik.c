#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define max_kart 24
#define oyunucu_kart 6


void kartSecmeOyuncu(int kartlar[], int oyuncu_kartlar[],
                     int bilgisayar_kartlar[], int *oyuncuKartSayisi,
                     int *bilgisayarKartSayisi, int *oyuncuBalikSayisi);

void kartSecmeBilgisayar(int kartlar[], int oyuncu_kartlar[],
                         int bilgisayar_kartlar[], int *oyuncuKartSayisi,
                         int *bilgisayarKartSayisi, int *bilgisayarBalikSayisi);

void karistir(int dizi[], int boyut);
void kartDagit(int kartlar[], int bilgisayar_kartlar[], int oyuncu_kartlar[],
               int *bilgisayarKartSayisi, int *oyuncuKartSayisi);

void kartDagit(int kartlar[], int bilgisayar_kartlar[], int oyuncu_kartlar[],
               int *bilgisayarKartSayisi, int *oyuncuKartSayisi) {
  karistir(kartlar, max_kart);

  for (int i = 0; i < oyunucu_kart; i++) {
      oyuncu_kartlar[i] = kartlar[i*2]; // Çift indeksler oyuncuya
      bilgisayar_kartlar[i] = kartlar[i*2+1]; // Tek indeksler bilgisayara
  }

  printf("Oyuncu kartlari: ");
  for (int i = 0; i < oyunucu_kart; i++) {
    printf("%d ", oyuncu_kartlar[i]);
  }
  printf("\n");

  printf("Bilgisayar kartlari: ");
  for (int i = 0; i < oyunucu_kart; i++) {
    printf("%d ", bilgisayar_kartlar[i]);
  }
  printf("\n");

  printf("Masada kalan kartlar: ");
  for (int i = oyunucu_kart * 2; i < max_kart; i++) {
    printf("%d ", kartlar[i]);
  }
  printf("\n");
}

void karistir(int dizi[], int boyut) {
  for (int i = boyut - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = dizi[i];
    dizi[i] = dizi[j];
    dizi[j] = temp;
  }
}
////BAKKK BURAYAA
void kartSecmeOyuncu(int kartlar[], int oyuncu_kartlar[],
                     int bilgisayar_kartlar[], int *oyuncuKartSayisi,
                     int *bilgisayarKartSayisi, int *oyuncuBalikSayisi) {
  printf("Oyuncu kart secimi yapiyor...\n");
  int secim;

  scanf("%d", &secim);
  for (int i = 0; i < *bilgisayarKartSayisi; i++) {//seçilen karta birden fazla varsa onlkarın hepsi verilmeli bunu kontrolu yap
    if (bilgisayar_kartlar[i] ==
        secim) { // oyuncu kartı artacak bilgisayar kart sayısı düşecek sıra
                 // oyuncuda devam edecek yine balık konttrolu yapacak
      printf("Bilgisayar da istediginiz kart var.\n");
      printf("Kart oyuncuya geciyor.\n");
      oyuncu_kartlar[*oyuncuKartSayisi]  = secim;
      (*oyuncuKartSayisi)++;
      bilgisayar_kartlar[i] = 0;

      int yeni_index = 0;
      for (int i = 0; i < *bilgisayarKartSayisi; i++) {
        if (bilgisayar_kartlar[i] != 0) {
          bilgisayar_kartlar[yeni_index++] = bilgisayar_kartlar[i];
        }
      }
      *bilgisayarKartSayisi = yeni_index;

      
      int balıkOycuadet1 = 0;
      for (int j = 0; j < *oyuncuKartSayisi; j++) {
        if (oyuncu_kartlar[j] == secim) {
          balıkOycuadet1++;
        }
        }
        if (balıkOycuadet1 == 4) {
          printf("Oyuncu balık yaptı.\n"); // oyuncu balık sayoısı artacak
                                           // fonksiyonunu çağracak ve tekrar
                                           // sorulacak  veeee kartlar çıkarılmalı balık olablar
          int yeni_index = 0;
          for (int i = 0; i < *oyuncuKartSayisi; i++) {
              if (oyuncu_kartlar[i] != secim) {
                  oyuncu_kartlar[yeni_index++] = oyuncu_kartlar[i];
              }
          }
          *oyuncuKartSayisi = yeni_index;
          kartSecmeOyuncu(kartlar, oyuncu_kartlar, bilgisayar_kartlar,
                          oyuncuKartSayisi, bilgisayarKartSayisi,
                          oyuncuBalikSayisi);
          (*oyuncuBalikSayisi)++;
          printf("Oyuncunun balık sayısı: %d\n",(*oyuncuBalikSayisi));
          if ((*oyuncuBalikSayisi) == 2) {
            printf("Oyuncu kazandı.\n");
            return;
          } else
            kartSecmeOyuncu(kartlar, oyuncu_kartlar, bilgisayar_kartlar,
                            oyuncuKartSayisi, bilgisayarKartSayisi,
                            oyuncuBalikSayisi);
        } else {
          kartSecmeOyuncu(kartlar, oyuncu_kartlar, bilgisayar_kartlar,
                        oyuncuKartSayisi, bilgisayarKartSayisi,
                          oyuncuBalikSayisi);
        }
      }
     else {
      printf("Balik avı.\n");
      printf(
          "Oyuncu masadan kart cekiyor.\n"); // masadadn kart azalacak oyuncu
                                             // kartı artacak sıra bilgisayara
                                             // geçecek bilgisayar kart seçme
                                             // fonksiyonu olacak
       int masa_indeks = -1;
       for (int i = 0; i < max_kart; i++) {
         if (kartlar[i] != 0) {
           masa_indeks = i;
           break;
         }
       }
       if (masa_indeks != -1) {
         oyuncu_kartlar[(*oyuncuKartSayisi)++] = kartlar[masa_indeks];
         kartlar[masa_indeks] = 0;
       } else {
         printf("Masada kart kalmadi!\n");
       }

      // sıra bilgisayarda olacak
    }
  }
}
void kartSecmeBilgisayar(int kartlar[], int oyuncu_kartlar[],
                         int bilgisayar_kartlar[], int *oyuncuKartSayisi,
                         int *bilgisayarKartSayisi, int *bilgisayarBalikSayisi) {
  printf("Bilgisayar kart secimi yapiyor...\n");
  int secimBilgisayar;

  int rastgeleIndeks = rand() % (*bilgisayarKartSayisi);
  secimBilgisayar = bilgisayar_kartlar[rastgeleIndeks];

  
  for (int i = 0; i < (*oyuncuKartSayisi); i++) {
    if (oyuncu_kartlar[i] == secimBilgisayar) {
      printf("bilgisayarın istediği kart oyuncuda var.\n");
      printf("kart bilgisayara geciyor.\n");
      int yeni_index = 0;
      for (int i = 0; i < *oyuncuKartSayisi; i++) {
        if (oyuncu_kartlar[i] == secimBilgisayar) {
          bilgisayar_kartlar[*bilgisayarKartSayisi] = secimBilgisayar;
          (*bilgisayarKartSayisi)++;
        } else {
          oyuncu_kartlar[yeni_index++] = oyuncu_kartlar[i];
        }
      }
      *oyuncuKartSayisi = yeni_index;
     
      for (int j = i; j < (*oyuncuKartSayisi) - 1; j++) {
        oyuncu_kartlar[j] = oyuncu_kartlar[j + 1];
      }
      (*oyuncuKartSayisi)--;
      int balıkBilgisayaradet1 = 0;
      for (int j = 0; j < (*bilgisayarKartSayisi); j++) {
        if (bilgisayar_kartlar[j] == secimBilgisayar) {
          balıkBilgisayaradet1++;
          if (balıkBilgisayaradet1 ==
              4) { // bilgisayar balık sayoısı artacak fonksiyonunu çağracak ve
                   // tekrar sorulacak
            printf("Bilgisayar balık yaptı.\n"); // balık sayısı fln görülecek
           (*bilgisayarBalikSayisi)++;
            printf("Bilgisayarın balık sayısı: %d\n", *bilgisayarBalikSayisi);
            if ((*bilgisayarBalikSayisi) == 2) {
              printf("Bilgisayar kazandı.\n");
              break;
            } else
              kartSecmeBilgisayar(kartlar, oyuncu_kartlar, bilgisayar_kartlar,
                                  oyuncuKartSayisi, bilgisayarKartSayisi,
                                  bilgisayarBalikSayisi);
          } else {
            printf("Bilgisayar balık yapmadı.\n"); // bilgisayar tekrardan kart
                                                   // soracak
            kartSecmeBilgisayar(kartlar, oyuncu_kartlar, bilgisayar_kartlar,
            oyuncuKartSayisi, bilgisayarKartSayisi,
            bilgisayarBalikSayisi);
          }
        }
      }
    } else {
      printf("Balik avı.\n");
      printf("Bilgisayar masadan kart cekiyor.\n");
      int masa_indeks = -1;
      for (int i = 0; i < max_kart; i++) {
        if (kartlar[i] != 0) {
          masa_indeks = i;
          break;
        }
      }
      if (masa_indeks != -1) {
        oyuncu_kartlar[(*oyuncuKartSayisi)++] = kartlar[masa_indeks];
        kartlar[masa_indeks] = 0;
      } else {
        printf("Masada kart kalmadi!\n");
      }

      // sıra oyuncuda olacak
    }
  }
}

int main() {
  srand(time(NULL));

  int kartlar[max_kart] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
                           4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
  int bilgisayar_kartlar[oyunucu_kart];
  int oyuncu_kartlar[oyunucu_kart];
  int bilgisayarKartSayisi = 0, oyuncuKartSayisi = 0;
  int bilgisayarBalikSayisi = 0, oyuncuBalikSayisi = 0;

  printf("Balik oyunu basliyor...\n");
kartDagit(kartlar, bilgisayar_kartlar, oyuncu_kartlar,
          &bilgisayarKartSayisi, &oyuncuKartSayisi);




  while(oyuncuBalikSayisi < 2 && bilgisayarBalikSayisi < 2) {
    kartSecmeOyuncu(kartlar, oyuncu_kartlar, bilgisayar_kartlar,
    &oyuncuKartSayisi, &bilgisayarKartSayisi,
    &oyuncuBalikSayisi);
      if(oyuncuBalikSayisi >= 2) break;
    kartSecmeBilgisayar(kartlar, oyuncu_kartlar, bilgisayar_kartlar,
    &oyuncuKartSayisi, &bilgisayarKartSayisi,
    &bilgisayarBalikSayisi);
  }
   
  return 0;
}
