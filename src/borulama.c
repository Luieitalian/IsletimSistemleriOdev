/**
 * BERKE PİTE
 * ENES SOYLU
 * AHMET PEKDEMİR
 * ÜZEYİR EMRE TÜRKMEN
 * HASAN YASİR ARSLAN
 */

#include "borulama.h" // Borulama ile ilgili tanımlamaları içeren başlık dosyası
#include "yonlendirme.h" // Yönlendirme işlemleri için kullanılan başlık dosyası

// Maksimum arkaplan işlemi sayısı
BackgroundProcess bg_processes[MAX_BG_PROCESSES];
int bg_process_count = 0; // Arkaplan işlemlerinin sayısını takip eden sayaç

// Pipeline (borulama) işlemlerini çalıştıran fonksiyon
void pipeline_calistir(char **args, int arg_count) {
    int background = 0; // Arkaplan işlemi olup olmadığını kontrol eden değişken

    // Eğer argümanların sonu '&' ile bitiyorsa, arkaplan işlemi olarak işaretle
    if (arg_count > 0 && strcmp(args[arg_count - 1], "&") == 0) {
        background = 1;
        args[--arg_count] = NULL; // '&' karakterini argümanlardan çıkar
    }

    int pipe_positions[MAX_ARG_COUNT]; // Pipe sembollerinin pozisyonlarını
                                       // tutar
    int pipe_count = 0;                // Bulunan pipe sayısı

    // Pipe pozisyonlarını bul ve kaydet
    for (int i = 0; i < arg_count; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_positions[pipe_count++] = i;
            args[i] = NULL; // Pipe sembolünü null yaparak komutları ayır
        }
    }

    // Eğer pipe yoksa, sadece tek bir komut çalıştır
    if (pipe_count == 0) {
        pid_t pid = fork();                       // Yeni bir işlem oluştur
        if (pid == 0) {                           // Çocuk işlem
            yonlendirme_handle(args, &arg_count); // Yönlendirme işlemini yap
            execvp(args[0], args);                // Komutu çalıştır
            perror("Komut çalıştırılamadı");      // Hata mesajı yazdır
            exit(1);                              // Çıkış yap
        } else if (pid > 0) {                     // Ana işlem
            if (background) {
                // Arkaplan işlemini kaydet
                bg_processes[bg_process_count].pid = pid;
                bg_processes[bg_process_count].running = 1;
                bg_process_count++;
            } else {
                waitpid(pid, NULL, 0); // Çocuk işlemi bekle
            }
        }
        return;
    }

    // Pipe ile çalıştırılan komutları işle
    int pipes[MAX_ARG_COUNT - 1][2]; // Pipe dosya tanımlayıcılarını tutar
    for (int i = 0; i < pipe_count; i++) {
        if (pipe(pipes[i]) == -1) { // Pipe oluştur
            perror("Pipe oluşturulamadı");
            exit(1);
        }
    }

    int start = 0;  // Başlangıç pozisyonu
    pid_t last_pid; // Son işlem ID'sini tutar

    // Her bir komutu sırayla çalıştır
    for (int i = 0; i <= pipe_count; i++) {
        int end = (i < pipe_count) ? pipe_positions[i]
                                   : arg_count; // Komut bitiş pozisyonu
        char **cmd = &args[start];              // Komutun başlangıcı
        cmd[end - start] = NULL;                // Argümanları sonlandır

        pid_t pid = fork(); // Yeni bir işlem oluştur
        if (pid == 0) {     // Çocuk işlem
            if (i > 0) { // Eğer önceki bir pipe varsa, giriş olarak bağla
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i <
                pipe_count) { // Eğer bir sonraki pipe varsa, çıkış olarak bağla
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Tüm pipe'ları kapat
            for (int j = 0; j < pipe_count; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            int cmd_arg_count = end - start; // Komutun argüman sayısını hesapla
            yonlendirme_handle(cmd, &cmd_arg_count); // Yönlendirme işlemini yap
            execvp(cmd[0], cmd);                     // Komutu çalıştır
            perror("Komut çalıştırılamadı");         // Hata mesajı yazdır
            exit(1);                                 // Çıkış yap
        }

        last_pid = pid; // Son işlemin PID'sini kaydet
        start =
            pipe_positions[i] + 1; // Bir sonraki komutun başlangıcını ayarla
    }

    // Ana işlem için tüm pipe'ları kapat
    for (int i = 0; i < pipe_count; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Arkaplan veya ön planda çalışma durumunu işle
    if (background) {
        // Arkaplan işlemini kaydet
        bg_processes[bg_process_count].pid = last_pid;
        bg_processes[bg_process_count].running = 1;
        bg_process_count++;
    } else {
        // Tüm çocuk işlemleri bekle
        while (wait(NULL) > 0)
            ;
    }
}

// Çocuk işlemin bitişini yakalayan sinyal işleyici
void cocukProses_bitis_handle(int sig) {
    pid_t pid;
    int status;

    // Biten tüm çocuk işlemleri kontrol et
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < bg_process_count; i++) {
            if (bg_processes[i].pid == pid && bg_processes[i].running) {
                // Biten arkaplan işlemini bildir
                printf("\n[%d] Proses bitti:  %d\n", pid, WEXITSTATUS(status));
                fflush(stdout);
                printf("> "); // Komut istemini tekrar göster
                fflush(stdout);
                bg_processes[i].running =
                    0; // İşlemi tamamlanmış olarak işaretle
                break;
            }
        }
    }
}
