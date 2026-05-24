#include "ChatGPTClient.h"
#include "MainWindow.h"
#include <QSharedMemory>

#include <QApplication>
#include <QLockFile>
#include <QDir>
#include <QFileSystemWatcher>
#include "MainWindow.h"
#include "DatabaseManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Файл блокировки — любая стабильная директория
    QString lockFilePath = QDir::temp().absoluteFilePath("myapp.lock");
    QLockFile lockFile(lockFilePath);

    // Пытаемся заблокировать
    if (!lockFile.tryLock(100)) {
        // Уже запущено → отправляем сигнал существующему окну
        QFile msgFile(QDir::temp().absoluteFilePath("myapp.msg"));
        msgFile.open(QIODevice::WriteOnly);
        msgFile.write("raise");
        msgFile.close();
        return 0; // <-- не запускаем вторую копию
    }

    DatabaseManager::instance().init("chat_database.db");

    // Запускаем основное окно
    MainWindow window;
    window.setAttribute(Qt::WA_TranslucentBackground);
    window.setWindowFlags(Qt::FramelessWindowHint);
    window.resize(400, 300);
    window.show();

    // Следим за сообщениями (поднять главное окно)
    QFileSystemWatcher watcher;
    watcher.addPath(QDir::temp().absoluteFilePath("myapp.msg"));
    QObject::connect(&watcher, &QFileSystemWatcher::fileChanged, [&]() {
        window.show();
        window.raise();
        window.activateWindow();
    });

    return app.exec();
}
