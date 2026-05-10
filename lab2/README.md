# Лабораторная работа 2

Тема: REST API для каталога настольных игр.

## Цель работы

Развернуть серверное ПО из лабораторной работы 1 в контейнерной инфраструктуре Docker, добавить обратный HTTP/HTTPS-proxy Nginx и продемонстрировать работу API через порты 80 и 443.

## Архитектура

Проект развёрнут через Docker Compose.

Сервисы:

- postgres-service — база данных PostgreSQL.
- migration-service — применение миграций Django, сбор статических файлов, создание суперпользователя.
- backend-service — Django backend, запущенный через Gunicorn.
- nginx-service — обратный proxy Nginx для HTTP и HTTPS.

## Структура

- backend/ — Django-проект.
- nginx/ — Dockerfile и шаблон конфигурации Nginx.
- nginx/templates/default.conf.template — конфигурация reverse proxy.
- docker-compose.yaml — описание контейнерной инфраструктуры.
- .env.example — пример переменных окружения.

## Запуск

Перед запуском нужно создать файл .env на основе .env.example.

Сгенерировать самоподписанный сертификат:

    mkdir nginx\certs
    docker run --rm -v "${PWD}\nginx\certs:/certs" alpine/openssl req -x509 -nodes -newkey rsa:2048 -keyout /certs/lab2.key -out /certs/lab2.crt -days 3650 -subj "/CN=localhost" -addext "subjectAltName=DNS:localhost,IP:127.0.0.1"

Запустить контейнеры:

    docker compose up --build -d

Проверить контейнеры:

    docker compose ps

## Проверка API

HTTP:

    curl.exe -I http://localhost/api/boardgame
    curl.exe -i http://localhost/api/boardgame/1

HTTPS:

    curl.exe -k -I https://localhost/api/boardgame
    curl.exe -k -i https://localhost/api/boardgame/1

## Админка Django

HTTP:

    http://localhost/admin/

HTTPS:

    https://localhost/admin/

Данные суперпользователя задаются через переменные окружения в .env.

## Проверенные возможности

- PostgreSQL работает в отдельном контейнере.
- Миграции и сбор статических файлов выполняются отдельным сервисом.
- Backend работает через Gunicorn.
- Nginx проксирует запросы к backend.
- API доступен на портах 80 и 443.
- Самоподписанный TLS-сертификат подключён.
- Django Admin открывается со стилями.
- Реализованы и проверены CRUD+List-запросы.
