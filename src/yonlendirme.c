/**
 * BERKE PİTE
 * ENES SOYLU
 * AHMET PEKDEMİR
 * ÜZEYİR EMRE TÜRKMEN
 * HASAN YASİR ARSLAN
 */

#include "yonlendirme.h"  // Yönlendirme işlemleri için gerekli tanımlamaları içeren başlık dosyası

// Yönlendirme işlemlerini gerçekleştiren fonksiyon
void yonlendirme_handle(char **args, int *arg_count) {
    char *input_file = NULL;  // Giriş dosyasını tutacak değişken
    char *output_file = NULL;  // Çıkış dosyasını tutacak değişken
    int new_arg_count = 0;  // Yönlendirme sembolleri çıkarıldıktan sonraki argüman sayısı

    // Argümanlarda yönlendirme sembollerini ara
    for (int i = 0; i < *arg_count; i++) {
        if (strcmp(args[i], "<") == 0 && i + 1 < *arg_count) {  
            // '<' sembolü bulunduğunda giriş dosyasını belirle
            input_file = args[i + 1];  
            i++;  // Dosya ismini atla
        } else if (strcmp(args[i], ">") == 0 && i + 1 < *arg_count) {  
            // '>' sembolü bulunduğunda çıkış dosyasını belirle
            output_file = args[i + 1];
            i++;  // Dosya ismini atla
        } else {
            // Yönlendirme sembolü olmayan argümanları yeni argüman listesine ekle
            args[new_arg_count++] = args[i];
        }
    }
    args[new_arg_count] = NULL;  // Yeni argüman dizisini sonlandır
    *arg_count = new_arg_count;  // Argüman sayısını güncelle

    // Giriş yönlendirmesini işle
    if (input_file) {
        int fd = open(input_file, O_RDONLY);  // Giriş dosyasını yalnızca okuma modunda aç
        if (fd == -1) {  // Dosya açılamazsa hata mesajı yazdır ve çıkış yap
            fprintf(stderr, "Giriş dosyası bulunamadı: %s\n", input_file);
            exit(1);
        }
        dup2(fd, STDIN_FILENO);  // Dosyayı standart giriş (STDIN) olarak ayarla
        close(fd);  // Dosya tanımlayıcısını kapat
    }

    // Çıkış yönlendirmesini işle
    if (output_file) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);  
        // Çıkış dosyasını yazma modunda aç, yoksa oluştur ve içeriğini sıfırla
        if (fd == -1) {  // Dosya açılamazsa hata mesajı yazdır ve çıkış yap
            perror("Çıkış dosyası açılamadı");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);  // Dosyayı standart çıkış (STDOUT) olarak ayarla
        close(fd);  // Dosya tanımlayıcısını kapat
    }
}
