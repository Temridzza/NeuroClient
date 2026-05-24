#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MessageWidget.h"
#include "SettingsController.h"
#include <QScrollBar>
#include <QMenu>
#include <csignal>
#include <QMessageBox>
#include <QTimer>

#include "MessageRepository.h"
#include "database/ChatRepository.h"

void sigusr1_handler(int) {
    auto w = MainWindow::instance();
    w->show();
    w->raise();
    w->activateWindow();
}

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet();

    client = new ChatGPTClient();
    client = new ChatGPTClient();
    if (!client->isValid()) {
        QMessageBox::critical(this, "Ошибка", client->lastError());
    }

    ui->chatLayout->setAlignment(Qt::AlignTop);

    //### Sidepanel ###
    sidePanel = new SidePanel(this);

    ChatRepository repo;
    auto chats = repo.listChats();
    sidePanel->loadChats(chats);

    sidePanel->setGeometry(width()+width()+width(), 0, 220, height());
    sidePanel->show();

    // реагируем на выбор чата
    connect(sidePanel, &SidePanel::chatSelected, this, [this](const QString &idChat){
        MainWindow::clearMessages();
        qDebug() << "Чат выбран:" << idChat;
        auto repo = MessageRepository();

        auto message = repo.getMessages(idChat);
        for (auto m : message) {
            if (m["role"] == "user") {
                MainWindow::addMessage(m["content"].toString(), true);
            } else {
                MainWindow::addMessage(m["content"].toString(), false);
            }
        }
    });

}

MainWindow* MainWindow::instance(QWidget *parent)
{
    static MainWindow* _instance = nullptr;

    if (!_instance) {
        _instance = new MainWindow(parent);
        signal(SIGUSR1, sigusr1_handler);
    }

    if (!_instance->isVisible()) {
        _instance->show();
    }

    return _instance;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setStyleSheet() const{
    ui->scrollArea->setStyleSheet("background-color: rgba(0,0,0,100); border: none;");
    ui->lineEditPrompt->setStyleSheet("background-color: rgba(0,0,0,100); color: white; border: none; padding:5px;");
    ui->sendBtn->setStyleSheet("background-color: rgba(0,0,0,100); color: white; border: none;");
}

void MainWindow::addMessage(const QString &text, bool isUser)
{
    auto *msg = new MessageWidget(text, isUser, this);

    ui->chatLayout->addWidget(msg);

    QTimer::singleShot(0, [this]() {
        ui->scrollArea->verticalScrollBar()->setValue(
            ui->scrollArea->verticalScrollBar()->maximum());
    });
}

void MainWindow::clearMessages() {
    QLayoutItem *item;
    while ((item = ui->chatLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}


void MainWindow::on_sendBtn_clicked()
{
    QString userText = ui->lineEditPrompt->text().trimmed();
    if (userText.isEmpty())
        return;

    addMessage(userText, true);   // Сообщение от пользователя
    ui->lineEditPrompt->clear();

    // Получаем ответ от ChatGPT
    QString botReply = client->ask(userText);
    addMessage(botReply, false);

    // //тестовое
    // QString user = "hello";
    // addMessage(user, true);
    // QString bot = "Qt - это кроссплатформенный фреймворк для разработки программного обеспечения, который позволяет создавать приложения для различных операционных систем, включая Windows, macOS, Linux, Android и iOS. Qt предоставляет широкий спектр инструментов и библиотек для разработки GUI-приложений, а также поддерживает разработку консольных и серверных приложений.**Плюсы разработки на Qt:**1. **Кроссплатформенность**: Qt позволяет создавать приложения, которые могут работать на нескольких операционных системах без необходимости переписывать код. 2. **Большая библиотека классов**: Qt предоставляет обширную библиотеку классов и функций, которые упрощают разработку приложений, включая GUI-компоненты, сеть, базы данных и многое другое. 3. **Высокая производительность**: Qt-приложения могут работать быстро и эффективно, благодаря оптимизированному коду и использованию современных технологий. 4. **Открытый исходный код**: Qt имеет открытый исходный код, что позволяет разработчикам изучать и модифицировать код фреймворка. 5. **Большое сообщество**: Qt имеет большое и активное сообщество разработчиков, что обеспечивает быструю поддержку и обмен опытом. 6. **Поддержка современных технологий**: Qt поддерживает современные технологии, такие как Qt Quick, QML, и 3D-графика. 7. **Легкость разработки**: Qt предоставляет удобный и интуитивно понятный API, что упрощает разработку приложений. **Минусы разработки на Qt:**1. **Крутая кривая обучения**: Qt имеет большой и сложный API, что может затруднить изучение для начинающих разработчиков. 2. **Большой размер**: Qt-приложения могут иметь большой размер, из-за необходимости включения библиотек и ресурсов фреймворка. 3. **Зависимость от фреймворка**: Qt-приложения сильно зависят от фреймворка, что может затруднить перенос приложения на другую платформу. 4. **Ограниченная поддержка некоторых платформ**: Хотя Qt поддерживает многие платформы, некоторые из них могут иметь ограниченную поддержку или требовать дополнительных настроек. 5. **Лицензионные ограничения**: Qt имеет несколько лицензий, включая GPL и коммерческую лицензию, что может ограничить использование фреймворка в некоторых проектах. 6. **Не всегда совместим с последними версиями компиляторов**: Qt может не всегда быть совместим с последними версиями компиляторов, что может привести к проблемам при сборке и запуске приложений. 7. **Не всегда поддерживает последние технологии**: Хотя Qt поддерживает многие современные технологии, он может не всегда поддерживать последние и самые новые технологии.В целом, Qt - это мощный и гибкий фреймворк, который может быть использован для разработки широкого спектра приложений. Однако, он требует определенного уровня навыков и опыта, и может иметь некоторые ограничения и недостатки.";
    // addMessage(bot, false);
}

void MainWindow::on_settingsBtn_clicked()
{
    if (!settingsController) {
        settingsController = new SettingsController(this);
        settingsController->setMainWindow(this);
    }

    if (!settingsMenu) {
        settingsMenu = new QMenu(this);
        settingsController->setupSettingsMenu(settingsMenu);

        connect(settingsMenu, &QMenu::aboutToHide, this, [this]() {
            if (settingsMenu) {
                settingsMenu->deleteLater();
                settingsMenu = nullptr;
            }
        });

        ui->settingsBtn->setMenu(settingsMenu);
    }

    settingsMenu->popup(ui->settingsBtn->mapToGlobal(QPoint(0, ui->settingsBtn->height())));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();

    const int triggerZone = 80;
    if (x > width() - triggerZone) {
        sidePanel->showPanel();
    }
}
