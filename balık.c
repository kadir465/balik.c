 #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <stdbool.h> // For bool type

    #define MAX_KART_DESTE 24 // Total cards in the deck
    #define KART_DEGER_SAYISI 6 // Card values are 1 to 6
    #define BASLANGIC_EL_KARTI_SAYISI 6 // Initial number of cards in hand
    #define HEDEF_BALIK_SAYISI 2 // Number of sets (fish) to win

    // Helper function to print a player's hand
    void printHand(const char *playerName, int hand[], int handSize) {
        printf("%s'in elindeki kartlar (%d adet): ", playerName, handSize);
        for (int i = 0; i < handSize; i++) {
            printf("%d ", hand[i]);
        }
        printf("\n");
    }

    // Helper function to remove a card at a specific index from hand
    // Shifts elements to the left to fill the gap
    void removeCardAtIndex(int hand[], int *handSize, int index) {
        if (index < 0 || index >= *handSize) return;
        for (int i = index; i < *handSize - 1; i++) {
            hand[i] = hand[i + 1];
        }
        (*handSize)--;
    }

    // Helper function to remove all cards of a specific value from hand
    // Returns the number of cards removed
    int removeCardsByValue(int hand[], int *handSize, int value) {
        int removedCount = 0;
        int i = 0;
        while (i < *handSize) {
            if (hand[i] == value) {
                removeCardAtIndex(hand, handSize, i);
                removedCount++;
                // Do not increment i, as the next element has shifted to current i
            } else {
                i++;
            }
        }
        return removedCount;
    }


    // Karistir (Shuffle) function - unchanged
    void karistir(int dizi[], int boyut) {
        for (int i = boyut - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = dizi[i];
            dizi[i] = dizi[j];
            dizi[j] = temp;
        }
    }

    // Kart Dagit (Deal Cards) function
    void kartDagit(int insanDizi[], int *insanKartSayisi,
                   int bilgisayarDizi[], int *bilgisayarKartSayisi,
                   int deste[], int *desteBaslangicIndex) {
        karistir(deste, MAX_KART_DESTE);

        *insanKartSayisi = 0;
        *bilgisayarKartSayisi = 0;
        *desteBaslangicIndex = 0; // Start of the actual deck in 'deste' array

        printf("Kartlar dagitiliyor...\n");
        for (int i = 0; i < BASLANGIC_EL_KARTI_SAYISI; i++) {
            if (*desteBaslangicIndex < MAX_KART_DESTE) {
                insanDizi[(*insanKartSayisi)++] = deste[(*desteBaslangicIndex)++];
            }
            if (*desteBaslangicIndex < MAX_KART_DESTE) {
                bilgisayarDizi[(*bilgisayarKartSayisi)++] = deste[(*desteBaslangicIndex)++];
            }
        }

        printHand("Oyuncu", insanDizi, *insanKartSayisi);
        printHand("Bilgisayar", bilgisayarDizi, *bilgisayarKartSayisi);

        printf("Masada kalan kartlar (Deste): ");
        for (int i = *desteBaslangicIndex; i < MAX_KART_DESTE; i++) {
            printf("%d ", deste[i]);
        }
        printf("\n\n");
    }

    // Kart Cek (Draw Card) from deck
    // Returns the drawn card, or -1 if deck is empty
    int kartCek(int deste[], int *desteBaslangicIndex, int *oyuncuKartSayisi, int oyuncuDizi[]) {
        if (*desteBaslangicIndex >= MAX_KART_DESTE) {
            printf("Deste bos!\n");
            return -1; // No cards left
        }
        int cekilenKart = deste[(*desteBaslangicIndex)++];
        if (*oyuncuKartSayisi < MAX_KART_DESTE) { // Ensure hand doesn't overflow (though unlikely with 24 cards total)
             oyuncuDizi[(*oyuncuKartSayisi)++] = cekilenKart;
        } else {
            printf("HATA: Oyuncunun eli maksimum kapasiteye ulasti, kart cekilemedi.\n");
            // This case should ideally not happen if MAX_KART_DESTE is the true limit for hands too
            return -1; // Or handle differently
        }
        return cekilenKart;
    }

    // Balik Kontrol (Check for Fish/Set)
    // Modifies hand and handSize by removing the set, increments balikSayisi
    // Returns true if a fish was made, false otherwise
    bool balikKontrol(int elKartlari[], int *elKartSayisi, int *balikSayisi, const char* oyuncuAdi) {
        if (*elKartSayisi < 4) return false;

        int sayac[KART_DEGER_SAYISI + 1] = {0}; // For cards 1 to 6

        for (int i = 0; i < *elKartSayisi; i++) {
            if (elKartlari[i] >= 1 && elKartlari[i] <= KART_DEGER_SAYISI) {
                sayac[elKartlari[i]]++;
            }
        }

        for (int kartDegeri = 1; kartDegeri <= KART_DEGER_SAYISI; kartDegeri++) {
            if (sayac[kartDegeri] >= 4) {
                printf(">>>> %s %d degerindeki kartlardan 4 tanesini tamamlayarak bir 'balik' yapti! <<<<\n", oyuncuAdi, kartDegeri);
                (*balikSayisi)++;
                printf("%s'in balik sayisi: %d\n", oyuncuAdi, *balikSayisi);
                removeCardsByValue(elKartlari, elKartSayisi, kartDegeri); // Remove the set from hand
                printHand(oyuncuAdi, elKartlari, *elKartSayisi);
                return true; // A fish was made
            }
        }
        return false; // No fish made
    }


    // Oyuncu Turu (Player's Turn)
    // Returns true if player's turn should continue (got a card they asked for)
    // Returns false if player "Go Fish"ed and didn't get what they asked for, or deck empty.
    bool oyuncuTuru(int insanDizi[], int *insanKartSayisi, int *oyuncu_balikSayisi,
                     int bilgisayarDizi[], int *bilgisayarKartSayisi,
                     int deste[], int *desteBaslangicIndex) {

        printf("\n--- Oyuncu Sirasi ---\n");
        printHand("Oyuncu", insanDizi, *insanKartSayisi);
        if (*insanKartSayisi == 0) {
            printf("Oyuncunun karti yok. Kart cekiyor.\n");
            int cekilen = kartCek(deste, desteBaslangicIndex, insanKartSayisi, insanDizi);
            if (cekilen != -1) {
                 printf("Oyuncu %d kartini cekti.\n", cekilen);
                 printHand("Oyuncu", insanDizi, *insanKartSayisi);
                 while(balikKontrol(insanDizi, insanKartSayisi, oyuncu_balikSayisi, "Oyuncu")); // Check for fish after drawing
            } else {
                printf("Deste bos, oyuncu kart cekemedi.\n");
            }
            return false; // Turn ends after drawing if hand was empty
        }

        int secim;
        printf("Bilgisayardan hangi karti istiyorsunuz (1-%d)? ", KART_DEGER_SAYISI);
        scanf("%d", &secim);
        while (secim < 1 || secim > KART_DEGER_SAYISI) {
            printf("Gecersiz kart degeri. Lutfen 1-%d arasi bir deger girin: ", KART_DEGER_SAYISI);
            scanf("%d", &secim);
        }

        printf("Oyuncu bilgisayardan %d istiyor.\n", secim);

        int bulunanKartSayisi = 0;
        for (int i = 0; i < *bilgisayarKartSayisi; i++) {
            if (bilgisayarDizi[i] == secim) {
                bulunanKartSayisi++;
            }
        }

        if (bulunanKartSayisi > 0) {
            printf("Bilgisayarda istediginiz karttan %d adet var! Kart(lar) aliniyor.\n", bulunanKartSayisi);
            for (int i = 0; i < bulunanKartSayisi; i++) { // Add all matching cards
                if (*insanKartSayisi < MAX_KART_DESTE) {
                     insanDizi[(*insanKartSayisi)++] = secim;
                }
            }
            removeCardsByValue(bilgisayarDizi, bilgisayarKartSayisi, secim);
            printHand("Oyuncu", insanDizi, *insanKartSayisi);
            printHand("Bilgisayar", bilgisayarDizi, *bilgisayarKartSayisi);
            while(balikKontrol(insanDizi, insanKartSayisi, oyuncu_balikSayisi, "Oyuncu"));
            return true; // Player got cards, turn continues
        } else {
            printf("Bilgisayarda istediginiz kart yok. 'Baliga Git!' (Kart cekin).\n");
            int cekilenKart = kartCek(deste, desteBaslangicIndex, insanKartSayisi, insanDizi);
            if (cekilenKart != -1) {
                printf("Oyuncu %d kartini cekti.\n", cekilenKart);
                printHand("Oyuncu", insanDizi, *insanKartSayisi);
                while(balikKontrol(insanDizi, insanKartSayisi, oyuncu_balikSayisi, "Oyuncu"));
                if (cekilenKart == secim) {
                    printf("Cekilen kart istenen kartti (%d)! Oyuncu sirasi devam ediyor.\n", secim);
                    return true; // Drawn card was the asked card, turn continues
                }
            } else {
                 printf("Deste bos, kart cekilemedi.\n");
            }
            return false; // Turn ends
        }
    }

    // Bilgisayar Turu (Computer's Turn)
    // Returns true if computer's turn should continue
    // Returns false if computer "Go Fish"ed and didn't get what they asked for, or deck empty.
    bool bilgisayarTuru(int bilgisayarDizi[], int *bilgisayarKartSayisi, int *bilgisayar_balikSayisi,
                        int insanDizi[], int *insanKartSayisi,
                        int deste[], int *desteBaslangicIndex) {

        printf("\n--- Bilgisayar Sirasi ---\n");
        // printHand("Bilgisayar", bilgisayarDizi, *bilgisayarKartSayisi); // Optional: for debugging computer's hand
        if (*bilgisayarKartSayisi == 0) {
            printf("Bilgisayarin karti yok. Kart cekiyor.\n");
            int cekilen = kartCek(deste, desteBaslangicIndex, bilgisayarKartSayisi, bilgisayarDizi);
             if (cekilen != -1) {
                 printf("Bilgisayar bir kart cekti.\n"); // Don't reveal to player
                 // printHand("Bilgisayar", bilgisayarDizi, *bilgisayarKartSayisi);
                 while(balikKontrol(bilgisayarDizi, bilgisayarKartSayisi, bilgisayar_balikSayisi, "Bilgisayar"));
            } else {
                printf("Deste bos, bilgisayar kart cekemedi.\n");
            }
            return false; // Turn ends
        }

        // Simple AI: Computer asks for a card it already has.
        int sorulanKart = bilgisayarDizi[rand() % *bilgisayarKartSayisi];
        printf("Bilgisayar oyuncudan %d istiyor.\n", sorulanKart);

        int bulunanKartSayisi = 0;
        for (int i = 0; i < *insanKartSayisi; i++) {
            if (insanDizi[i] == sorulanKart) {
                bulunanKartSayisi++;
            }
        }

        if (bulunanKartSayisi > 0) {
            printf("Oyuncuda istenen karttan %d adet var! Kart(lar) aliniyor.\n", bulunanKartSayisi);
             for (int i = 0; i < bulunanKartSayisi; i++) {
                if (*bilgisayarKartSayisi < MAX_KART_DESTE) {
                    bilgisayarDizi[(*bilgisayarKartSayisi)++] = sorulanKart;
                }
            }
            removeCardsByValue(insanDizi, insanKartSayisi, sorulanKart);
            // printHand("Bilgisayar", bilgisayarDizi, *bilgisayarKartSayisi);
            printHand("Oyuncu", insanDizi, *insanKartSayisi);
            while(balikKontrol(bilgisayarDizi, bilgisayarKartSayisi, bilgisayar_balikSayisi, "Bilgisayar"));
            return true; // Computer got cards, turn continues
        } else {
            printf("Oyuncuda istenen kart yok. Bilgisayar 'Baliga Gidiyor' (Kart cekiyor).\n");
            int cekilenKart = kartCek(deste, desteBaslangicIndex, bilgisayarKartSayisi, bilgisayarDizi);
            if (cekilenKart != -1) {
                printf("Bilgisayar bir kart cekti.\n"); // Don't reveal
                // printHand("Bilgisayar", bilgisayarDizi, *bilgisayarKartSayisi);
                while(balikKontrol(bilgisayarDizi, bilgisayarKartSayisi, bilgisayar_balikSayisi, "Bilgisayar"));
                if (cekilenKart == sorulanKart) {
                    printf("Bilgisayarin cektigi kart istedigi kartti! Bilgisayar sirasi devam ediyor.\n");
                    return true; // Drawn card was the asked card, turn continues
                }
            } else {
                printf("Deste bos, kart cekilemedi.\n");
            }
            return false; // Turn ends
        }
    }


    int main() {
        srand(time(NULL));

        int tumKartlar[MAX_KART_DESTE] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
                                         4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};

        int bilgisayarDizi[MAX_KART_DESTE]; // Max possible cards computer can hold
        int insanDizi[MAX_KART_DESTE];     // Max possible cards player can hold
        int bilgisayarKartSayisi = 0;
        int insanKartSayisi = 0;
        int oyuncu_balikSayisi = 0, bilgisayar_balikSayisi = 0;
        int desteBaslangicIndex = 0; // Index of the next card to be drawn from tumKartlar

        printf("Balik Oyunu Basliyor...\n");

        kartDagit(insanDizi, &insanKartSayisi,
                  bilgisayarDizi, &bilgisayarKartSayisi,
                  tumKartlar, &desteBaslangicIndex);

        bool oyuncuSiraDevam = false;
        bool bilgisayarSiraDevam = false;
        int turSayisi = 1;

        while (oyuncu_balikSayisi < HEDEF_BALIK_SAYISI && bilgisayar_balikSayisi < HEDEF_BALIK_SAYISI) {
            printf("\n------------------ TUR %d ------------------\n", turSayisi++);

            // Player's turn loop (can have multiple plays if successful)
            oyuncuSiraDevam = true; // Assume turn starts
            while(oyuncuSiraDevam && oyuncu_balikSayisi < HEDEF_BALIK_SAYISI && (insanKartSayisi > 0 || (desteBaslangicIndex < MAX_KART_DESTE))) {
                if (insanKartSayisi == 0 && desteBaslangicIndex >= MAX_KART_DESTE) { // No cards and deck empty
                    printf("Oyuncunun karti yok ve deste bos. Sira bilgisayara gecer.\n");
                    oyuncuSiraDevam = false;
                    break;
                }
                oyuncuSiraDevam = oyuncuTuru(insanDizi, &insanKartSayisi, &oyuncu_balikSayisi,
                                            bilgisayarDizi, &bilgisayarKartSayisi,
                                            tumKartlar, &desteBaslangicIndex);
                 // Check for fish again in case taking cards resulted in a new set implicitly (less common in Go Fish)
                 // or if a drawn card completed a set without being the asked-for card.
                while(balikKontrol(insanDizi, &insanKartSayisi, &oyuncu_balikSayisi, "Oyuncu"));
            }

            if (oyuncu_balikSayisi >= HEDEF_BALIK_SAYISI) break;
            if (insanKartSayisi == 0 && bilgisayarKartSayisi == 0 && desteBaslangicIndex >= MAX_KART_DESTE) break; // Stalemate

            // Computer's turn loop
            bilgisayarSiraDevam = true;
            while(bilgisayarSiraDevam && bilgisayar_balikSayisi < HEDEF_BALIK_SAYISI && (bilgisayarKartSayisi > 0 || (desteBaslangicIndex < MAX_KART_DESTE))) {
                 if (bilgisayarKartSayisi == 0 && desteBaslangicIndex >= MAX_KART_DESTE) { // No cards and deck empty
                    printf("Bilgisayarin karti yok ve deste bos. Sira oyuncuya gecer.\n");
                    bilgisayarSiraDevam = false;
                    break;
                }
                bilgisayarSiraDevam = bilgisayarTuru(bilgisayarDizi, &bilgisayarKartSayisi, &bilgisayar_balikSayisi,
                                                   insanDizi, &insanKartSayisi,
                                                   tumKartlar, &desteBaslangicIndex);
                while(balikKontrol(bilgisayarDizi, &bilgisayarKartSayisi, &bilgisayar_balikSayisi, "Bilgisayar"));
            }


            if (bilgisayar_balikSayisi >= HEDEF_BALIK_SAYISI) break;

            // End game condition: If deck is empty and both players run out of cards or cannot make a move
            if (desteBaslangicIndex >= MAX_KART_DESTE && insanKartSayisi == 0 && bilgisayarKartSayisi == 0) {
                printf("Deste bos ve oyuncularin kartlari bitti. Oyun berabere veya en cok balik yapan kazanir.\n");
                break;
            }
             if (desteBaslangicIndex >= MAX_KART_DESTE && insanKartSayisi == 0 && bilgisayarKartSayisi > 0 && bilgisayar_balikSayisi < HEDEF_BALIK_SAYISI) {
                printf("Deste bos ve oyuncunun karti bitti. Bilgisayar kalan kartlariyla balik yapmaya calisir.\n");
                while(balikKontrol(bilgisayarDizi, &bilgisayarKartSayisi, &bilgisayar_balikSayisi, "Bilgisayar"));
                if (bilgisayar_balikSayisi >= HEDEF_BALIK_SAYISI) break;
            }
            if (desteBaslangicIndex >= MAX_KART_DESTE && bilgisayarKartSayisi == 0 && insanKartSayisi > 0 && oyuncu_balikSayisi < HEDEF_BALIK_SAYISI) {
                printf("Deste bos ve bilgisayarin karti bitti. Oyuncu kalan kartlariyla balik yapmaya calisir.\n");
                while(balikKontrol(insanDizi, &insanKartSayisi, &oyuncu_balikSayisi, "Oyuncu"));
                if (oyuncu_balikSayisi >= HEDEF_BALIK_SAYISI) break;
            }
            if (desteBaslangicIndex >= MAX_KART_DESTE && insanKartSayisi > 0 && bilgisayarKartSayisi > 0) {
                // Both have cards, but deck is empty. Game continues until someone makes fish or runs out.
            }


        }

        printf("\n---------- OYUN SONU ----------\n");
        printf("Oyuncu Balik Sayisi: %d\n", oyuncu_balikSayisi);
        printf("Bilgisayar Balik Sayisi: %d\n", bilgisayar_balikSayisi);

        if (oyuncu_balikSayisi >= HEDEF_BALIK_SAYISI && oyuncu_balikSayisi > bilgisayar_balikSayisi) {
            printf("OYUNCU KAZANDI!\n");
        } else if (bilgisayar_balikSayisi >= HEDEF_BALIK_SAYISI && bilgisayar_balikSayisi > oyuncu_balikSayisi) {
            printf("BILGISAYAR KAZANDI!\n");
        } else if (oyuncu_balikSayisi > bilgisayar_balikSayisi) {
            printf("OYUNCU KAZANDI (Daha fazla balik)!\n");
        } else if (bilgisayar_balikSayisi > oyuncu_balikSayisi) {
             printf("BILGISAYAR KAZANDI (Daha fazla balik)!\n");
        }
        else {
            printf("OYUN BERABERE!\n");
        }

        return 0;
    }
