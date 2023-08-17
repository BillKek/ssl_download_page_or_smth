
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
// #include <openssl/bio.h>

#include <locale.h>



const char* str_ip4 = 0;
const char* str_url = 0;
const char* str_webser = 0;
const char* str_afterwebser = 0;
int port = 443;
#include "dns_call2.h"

static SOCKET sockfd_new(char* ipv4, int port)
;

#define logg(XXX) printf("var "#XXX" = %i \n", XXX);

int main()
{

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    SSL_CTX *ctx;
    SSL *ssl;
    str_url = "en.wikipedia.org/w/index.php?title=Fdisk&oldid=1952624";//"vc.ru/job"; // //"vc.ru";// "mozilla.org";
    port = 443;

    str_ip4 = dns_call (str_url);
    SOCKET sockfd = sockfd_new(str_ip4,port);
    logg(sockfd);



    // Инициализация библиотеки OpenSSL
    SSL_library_init();

    // Создание контекста SSL
    ctx = SSL_CTX_new(SSLv23_client_method());

    // Создание объекта SSL
    ssl = SSL_new(ctx);

    // Установка соединения
    SSL_set_fd(ssl, sockfd /* сокет для соединения */);

SSL_set_tlsext_host_name(ssl, str_webser);

    // Установка соединения SSL
    int ret = SSL_connect(ssl);
    if (ret != 1) {
        // Ошибка установки соединения
        printf("Error connecting to server: code = %li \n", ret);
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return -1;
    }


    // Установка HTTPS-соединения OK.
    printf("Установка HTTPS-соединения OK.\n");
    // Можно выполнять передачу данных.

    {
        char* buftx; //[] = "Hello, World"; // Буфер с данными для отправки
        buftx = malloc(64*1024);
        memset(buftx,0,64*1024);

        snprintf(buftx, 64*1024-2, "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", str_afterwebser, str_webser);

        int len = strlen(buftx); // Длина данных

        int restx = SSL_write(ssl, buftx, len); // Отправка данных через защищенное соединение

        logg(restx);

        if (restx <= 0) {
            // Произошла ошибка при отправке данных
            int errortx = SSL_get_error(ssl, restx);
            logg(errortx);
            // Обработка ошибки...
        } else {
            // Данные успешно отправлены
        }
    }


    {
        int nbytes;
        char* buffer;
        buffer = malloc(4*1024);
// ...

        FILE*   fileHandle;
        if ((fileHandle = _wfopen( L"_html.html",L"wb+")) == NULL)
        {
            wprintf(L"_wfopen failed!\n");
            return(1);
        }


        while (1)
        {
            memset(buffer,0,4*1024);
            nbytes = SSL_read(ssl, buffer, 4*1024-1);
            logg(nbytes);
            if (nbytes > 0) {
                buffer[nbytes] = '\0';
                // Выводим полученные данные
                //printf("Received: %s\n", buffer);
                if (fwrite(buffer, 1, nbytes, fileHandle) != nbytes)
                {
                    wprintf(L"fwrite failed!\n");
                }

            } else {
                // Ошибка чтения данных
                int error = SSL_get_error(ssl, nbytes);
                logg(error);
                // Обрабатываем ошибку
                // ...
                break;
            }
        }
        fclose(fileHandle);


    }


    // Закрытие соединения
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);


    WSACleanup(); // освободить ресурсы сокетов и закрыть библиотеку WSA

    return 0;
}


static SOCKET sockfd_new(char* ipv4, int port)
{

    WSADATA wsaData;
    SOCKET sock;

    if (WSAStartup (MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf (stderr, "Failed to initialize winsock\n");
        return 1;
    }



// Чтобы открыть сокет для процедуры SSL_set_fd(), нужно выполнить следующие шаги:

// 1. Создать TCP сокет с помощью функции socket(). Например:
// ```c++
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET)
    {
        fprintf (stderr, "Failed to create socket\n");
        return 1;
    }


// ```
// 2. Установить соединение на созданном сокете с помощью функции connect(). Например:
// ```c++

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port); // или любой другой нужный порт
    server_address.sin_addr.s_addr = inet_addr(ipv4); // или любой другой IP-адрес
    int connection_status = connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));

// ```
// 3. Если соединение установлено успешно (connection_status равно 0), то можно использовать SSL_set_fd() для привязки сокета к структуре SSL. Примерно так:
// ```c++

// SSL *ssl = SSL_new(ctx); // ctx - контекст SSL (должен быть предварительно создан и настроен)
// SSL_set_fd(ssl, sockfd);

// ```
// Теперь ваш SSL сокет готов к использованию.
    return sockfd;
}


