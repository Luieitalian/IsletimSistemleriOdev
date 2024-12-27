/**
 * BERKE PİTE
 * ENES SOYLU
 * AHMET PEKDEMİR
 * ÜZEYİR EMRE TÜRKMEN
 * HASAN YASİR ARSLAN
 */

#include "komut.h" // Komutla ilgili tanımlamaları içeren başlık dosyası

// Kullanıcıdan alınan komut dizisini çalıştıran fonksiyon
void komut_dizisi_calistir(char *input) {
    char *command;  // Tek bir komut
    char *saveptr1; // strtok_r için geçici gösterici

    // Komutları ';' ile ayır ve sırayla işle
    command = strtok_r(input, ";", &saveptr1);
    while (command != NULL) {
        // Komut başındaki boşlukları atla
        while (*command == ' ')
            command++;

        char *args[MAX_ARG_COUNT]; // Komutun argümanlarını tutacak dizi
        int arg_count = 0;         // Argüman sayısı
        char *token;               // Tek bir argüman
        char *saveptr2;            // strtok_r için geçici gösterici

        // Komut içindeki argümanları ' ' (boşluk) ile ayır
        token = strtok_r(command, " ", &saveptr2);
        while (token != NULL && arg_count < MAX_ARG_COUNT - 1) {
            args[arg_count++] = token;              // Argümanı diziye ekle
            token = strtok_r(NULL, " ", &saveptr2); // Bir sonraki argümanı al
        }
        args[arg_count] = NULL; // Argüman dizisini sonlandır

        // Eğer geçerli bir komut varsa, pipeline_calistir fonksiyonunu çağır
        if (arg_count > 0) {
            pipeline_calistir(args, arg_count); // Komutu çalıştır
        }

        // Bir sonraki komutu al
        command = strtok_r(NULL, ";", &saveptr1);
    }
}
