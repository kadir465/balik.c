#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_KART 24
#define OYUNCU_KART 6

void karistir(int dizi[], int boyut);
void kartDagit(int kartlar[], int bilgisayar_kartlar[], int oyuncu_kartlar[]);
bool balikKontrol(int kartlar[], int boyut, int *balikSayisi);
void oyuncuHamlesi(int kartlar[], int oyuncu_kartlar[], int bilgisayar_kartlar[], int *oyuncuKartSayisi, int *bilgisayarKartSayisi, int *oyuncuBalikSayisi);
void bilgisayarHamlesi(int kartlar[], int oyuncu_kartlar[], int bilgisayar_kartlar[], int *oyuncuKartSayisi, int *bilgisayarKartSayisi, int *bilgisayarBalikSayisi);
void masadanKartCek(int kartlar[], int hedef_kartlar[], int *hedefKartSayisi);

int main() {
    srand(time(NULL));

    int kartlar[MAX_KART] = {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6};
    int bilgisayar_kartlar[OYUNCU_KART*2] = {0};
    int oyuncu_kartlar[OYUNCU_KART*2] = {0};
    int bilgisayarKartSayisi = 0, oyuncuKartSayisi = 0;
    int bilgisayarBalikSayisi = 0, oyuncuBalikSayisi = 0;

    printf("Balik oyunu basliyor...\n");
    kartDagit(kartlar, bilgisayar_kartlar, oyuncu_kartlar);
    oyuncuKartSayisi = OYUNCU_KART;
    bilgisayarKartSayisi = OYUNCU_KART;

    while(oyuncuBalikSayisi < 2 && bilgisayarBalikSayisi < 2) {
        // Oyuncu hamlesi
        oyuncuHamlesi(kartlar, oyuncu_kartlar, bilgisayar_kartlar, 
                     &oyuncuKartSayisi, &bilgisayarKartSayisi, &oyuncuBalikSayisi);
        if(oyuncuBalikSayisi >= 2) break;
        
        // Bilgisayar hamlesi
        bilgisayarHamlesi(kartlar, oyuncu_kartlar, bilgisayar_kartlar,
                         &oyuncuKartSayisi, &bilgisayarKartSayisi, &bilgisayarBalikSayisi);
    }

    if(oyuncuBalikSayisi >= 2) {
        printf("\nTEBRIKLER! OYUNCU KAZANDI!\n");
    } else {
        printf("\nUZGUNUM! BILGISAYAR KAZANDI!\n");
    }

    return 0;
}

void karistir(int dizi[], int boyut) {
    for(int i = boyut-1; i > 0; i--) {
        int j = rand() % (i+1);
        int temp = dizi[i];
        dizi[i] = dizi[j];
        dizi[j] = temp;
    }
}

void kartDagit(int kartlar[], int bilgisayar_kartlar[], int oyuncu_kartlar[]) {
    karistir(kartlar, MAX_KART);

    for(int i = 0; i < OYUNCU_KART; i++) {
        oyuncu_kartlar[i] = kartlar[i*2];
        bilgisayar_kartlar[i] = kartlar[i*2+1];
        kartlar[i*2] = kartlar[i*2+1] = 0;
    }

    printf("Oyuncu kartlari: ");
    for(int i = 0; i < OYUNCU_KART; i++) {
        printf("%d ", oyuncu_kartlar[i]);
    }
    printf("\n");

    printf("Bilgisayar kartlari: ");
    for(int i = 0; i < OYUNCU_KART; i++) {
        printf("%d ", bilgisayar_kartlar[i]);
    }
    printf("\n");

    printf("Masada kalan kartlar: ");
    for(int i = OYUNCU_KART*2; i < MAX_KART; i++) {
        printf("%d ", kartlar[i]);
    }
    printf("\n\n");
}

bool balikKontrol(int kartlar[], int boyut, int *balikSayisi) {
    int sayac[7] = {0}; // Kart numaraları 1-6 arası
    
    for(int i = 0; i < boyut; i++) {
        if(kartlar[i] != 0) {
            sayac[kartlar[i]]++;
            if(sayac[kartlar[i]] == 4) {
                // Balık yapıldı, bu kartları temizle
                int kartNo = kartlar[i];
                int yeniIndex = 0;
                for(int j = 0; j < boyut; j++) {
                    if(kartlar[j] != kartNo) {
                        kartlar[yeniIndex++] = kartlar[j];
                    }
                }
                // Kalan pozisyonları sıfırla
                for(int j = yeniIndex; j < boyut; j++) {
                    kartlar[j] = 0;
                }
                
                (*balikSayisi)++;
                return true;
            }
        }
    }
    return false;
}

void masadanKartCek(int kartlar[], int hedef_kartlar[], int *hedefKartSayisi) {
    for(int i = 0; i < MAX_KART; i++) {
        if(kartlar[i] != 0) {
            hedef_kartlar[(*hedefKartSayisi)++] = kartlar[i];
            kartlar[i] = 0;
            return;
        }
    }
    printf("Masada kart kalmadi!\n");
}

void oyuncuHamlesi(int kartlar[], int oyuncu_kartlar[], int bilgisayar_kartlar[], 
                  int *oyuncuKartSayisi, int *bilgisayarKartSayisi, int *oyuncuBalikSayisi) {
    printf("\n--- OYUNCU HAMLESI ---\n");
    printf("Elinizdeki kartlar: ");
    for(int i = 0; i < *oyuncuKartSayisi; i++) {
        printf("%d ", oyuncu_kartlar[i]);
    }
    printf("\n");

    int secim;
    printf("Bilgisayardan hangi kartı istiyorsunuz? ");
    scanf("%d", &secim);

    bool kartBulundu = false;
    int alinanKartSayisi = 0;
    
    // Bilgisayarın elinde istenen kart var mı kontrol et
    for(int i = 0; i < *bilgisayarKartSayisi; i++) {
        if(bilgisayar_kartlar[i] == secim) {
            kartBulundu = true;
            // Kartı oyuncuya ekle
            oyuncu_kartlar[(*oyuncuKartSayisi)++] = secim;
            // Kartı bilgisayardan sil
            bilgisayar_kartlar[i] = 0;
            alinanKartSayisi++;
        }
    }

    if(kartBulundu) {
        printf("%d adet %d kartı alındı.\n", alinanKartSayisi, secim);
        
        // Bilgisayarın kartlarını sıkıştır (boşlukları kaldır)
        int yeniIndex = 0;
        for(int i = 0; i < *bilgisayarKartSayisi; i++) {
            if(bilgisayar_kartlar[i] != 0) {
                bilgisayar_kartlar[yeniIndex++] = bilgisayar_kartlar[i];
            }
        }
        *bilgisayarKartSayisi = yeniIndex;

        // Balık kontrolü
        if(balikKontrol(oyuncu_kartlar, *oyuncuKartSayisi, oyuncuBalikSayisi)) {
            printf("BALIK YAPTINIZ! Balık sayınız: %d\n", *oyuncuBalikSayisi);
            *oyuncuKartSayisi -= 4; // Balık yapılan kartlar çıkarıldı
        }
    } else {
        printf("Bilgisayarda %d kartı yok. Masadan kart çekiyorsunuz.\n", secim);
        masadanKartCek(kartlar, oyuncu_kartlar, oyuncuKartSayisi);
        
        // Balık kontrolü
        if(balikKontrol(oyuncu_kartlar, *oyuncuKartSayisi, oyuncuBalikSayisi)) {
            printf("BALIK YAPTINIZ! Balık sayınız: %d\n", *oyuncuBalikSayisi);
            *oyuncuKartSayisi -= 4;
        }
    }
}

void bilgisayarHamlesi(int kartlar[], int oyuncu_kartlar[], int bilgisayar_kartlar[], 
                      int *oyuncuKartSayisi, int *bilgisayarKartSayisi, int *bilgisayarBalikSayisi) {
    printf("\n--- BILGISAYAR HAMLESI ---\n");
    
    // Bilgisayar stratejisi: Elindeki en çok olan kartı seçer
    int sayac[7] = {0};
    for(int i = 0; i < *bilgisayarKartSayisi; i++) {
        sayac[bilgisayar_kartlar[i]]++;
    }
    
    int secim = 0;
    int maxSayi = 0;
    for(int i = 1; i <= 6; i++) {
        if(sayac[i] > maxSayi) {
            maxSayi = sayac[i];
            secim = i;
        }
    }
    
    // Eğer tüm kartlar tekilse rastgele seçim yap
    if(maxSayi <= 1) {
        secim = bilgisayar_kartlar[rand() % (*bilgisayarKartSayisi)];
    }
    
    printf("Bilgisayar sizden %d kartını istiyor.\n", secim);

    bool kartBulundu = false;
    int alinanKartSayisi = 0;
    
    // Oyuncunun elinde istenen kart var mı kontrol et
    for(int i = 0; i < *oyuncuKartSayisi; i++) {
        if(oyuncu_kartlar[i] == secim) {
            kartBulundu = true;
            // Kartı bilgisayara ekle
            bilgisayar_kartlar[(*bilgisayarKartSayisi)++] = secim;
            // Kartı oyuncudan sil
            oyuncu_kartlar[i] = 0;
            alinanKartSayisi++;
        }
    }

    if(kartBulundu) {
        printf("%d adet %d kartı alındı.\n", alinanKartSayisi, secim);
        
        // Oyuncunun kartlarını sıkıştır (boşlukları kaldır)
        int yeniIndex = 0;
        for(int i = 0; i < *oyuncuKartSayisi; i++) {
            if(oyuncu_kartlar[i] != 0) {
                oyuncu_kartlar[yeniIndex++] = oyuncu_kartlar[i];
            }
        }
        *oyuncuKartSayisi = yeniIndex;

        // Balık kontrolü
        if(balikKontrol(bilgisayar_kartlar, *bilgisayarKartSayisi, bilgisayarBalikSayisi)) {
            printf("BILGISAYAR BALIK YAPTI! Bilgisayarın balık sayısı: %d\n", *bilgisayarBalikSayisi);
            *bilgisayarKartSayisi -= 4;
        }
    } else {
        printf("Sizde %d kartı yok. Bilgisayar masadan kart çekiyor.\n", secim);
        masadanKartCek(kartlar, bilgisayar_kartlar, bilgisayarKartSayisi);
        
        // Balık kontrolü
        if(balikKontrol(bilgisayar_kartlar, *bilgisayarKartSayisi, bilgisayarBalikSayisi)) {
            printf("BILGISAYAR BALIK YAPTI! Bilgisayarın balık sayısı: %d\n", *bilgisayarBalikSayisi);
            *bilgisayarKartSayisi -= 4;
        }
    }
}
