#!/bin/bash
# Файл: /usr/local/bin/start-tor-if-needed.sh

if ! systemctl is-active --quiet tor.service; then
    echo "Tor не запущен. Запускаем..."
    systemctl start tor.service
else
    echo "Tor уже запущен."
fi