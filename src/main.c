
/**
 * BERKE PİTE
 * ENES SOYLU
 * AHMET PEKDEMİR
 * ÜZEYİR EMRE TÜRKMEN
 * HASAN YASİR ARSLAN
 */

#include "borulama.h"
#include "kabuk.h"
#include "komut.h"

int main() {
    // SIGCHLD handler'ını ayarla
    struct sigaction sa;
    sa.sa_handler = cocukProses_bitis_handle;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    char input[MAX_INPUT_SIZE];

    while (1) {
        // Kullanıcıdan komut al
        printf("> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            continue;
        }

        // Girdinin sonundaki '\n' karakteri temizlenir
        input[strcspn(input, "\n")] = '\0';

        // Boş girdiyi kontrol et
        if (strlen(input) == 0) {
            continue;
        }

        // Quit komutunu kontrol et (noktalı virgülden önce)
        if (strcmp(input, "quit") == 0) {
            // Tüm arkaplan proseslerinin bitmesini bekle
            for (int i = 0; i < bg_process_count; i++) {
                if (bg_processes[i].running) {
                    waitpid(bg_processes[i].pid, NULL, 0);
                }
            }
            break;
        }

        // Komut dizisini çalıştır
        komut_dizisi_calistir(input);
    }

    return 0;
}
