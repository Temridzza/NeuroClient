#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"
#include "ChatGPTConfig.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>
#include <QGuiApplication>
#include <QClipboard>

SettingsWindow* SettingsWindow::self = nullptr;

static Result getConfig(std::function<Result()> getter, QWidget *parent)
{
    auto result = getter();
    if (!result.success) {
        QMessageBox::warning(parent, "Ошибка", result.error);
        return {0,"error"};
    }
    return result;
}

SettingsWindow* SettingsWindow::instance(QWidget *parent)
{
    if (!self) {
        self = new SettingsWindow(parent);
    }

    // Если окно скрыто — показываем
    if (!self->isVisible()) {
        self->show();
    }

    // Активируем
    self->raise();
    self->activateWindow();

    return self;
}

SettingsWindow::SettingsWindow(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::SettingsWindow)
         {

    ui->setupUi(this);

    ui->apiKeyPathEdit->setText(
        getConfig([](){ return ChatGPTConfig::instance().getApiKeyPath(); }, this).data.toString()
    );

    ui->currentModelEdit->setText(
        getConfig([](){ return ChatGPTConfig::instance().getCurrentModel(); }, this).data.toString()
    );

    if (getConfig([](){ return ChatGPTConfig::instance().getTorEnable(); }, this).data.toBool() == false)
    {
        ui->torEnabledCheck->setChecked(false);
    }else {
        ui->torEnabledCheck->setChecked(true);
    }


    QVariant data = getConfig([]{return ChatGPTConfig::instance().getSelectedModels();}, this).data;;

    if (data.canConvert<QJsonArray>()) {
        QJsonArray arr = data.toJsonArray();
        ui->modelsList->clear();
        for (const auto &v : arr) {
            ui->modelsList->addItem(v.toString());
        }
    }

    ui->modelsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->modelsList, &QListWidget::customContextMenuRequested,
            this, &SettingsWindow::onModelsListContextMenu);

    connect(ui->modelsList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item){
        if (!item) return;

        if (item->text() == "➕ Добавить новую модель") {
            on_addModelBtn_clicked();
        }
    });

    connect(ui->apiKeyPathEdit, &QLineEdit::editingFinished, this, [this]() {
        QString path = ui->apiKeyPathEdit->text();
        auto result = ChatGPTConfig::instance().setApiKeyPath(path);
        if (!result.success) {
            QMessageBox::warning(this, "Ошибка", result.error);
        }
    });


    // --- Автосохранение Current Model ---
    connect(ui->currentModelEdit, &QLineEdit::editingFinished, this, [this]() {
        QString model = ui->currentModelEdit->text();
        auto result = ChatGPTConfig::instance().setCurrentModel(model);
        if (!result.success) {
            QMessageBox::warning(this, "Ошибка", result.error);
        }
    });

    // --- Автосохранение Tor Enabled ---
    connect(ui->torEnabledCheck, &QCheckBox::stateChanged, this, [this](int state){
        bool enabled = (state == Qt::Checked);
        auto result = ChatGPTConfig::instance().setTorEnable(enabled);
        if (!result.success) {
            QMessageBox::warning(this, "Ошибка", result.error);
            // Восстанавливаем предыдущий статус
            ui->torEnabledCheck->setChecked(!enabled);
        }
    });

    reloadModelsList();

    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(700, 350);
}

SettingsWindow::~SettingsWindow() {
    self = nullptr;
    delete ui;
}

void SettingsWindow::reloadModelsList() {
    auto res = getConfig([]{ return ChatGPTConfig::instance().getSelectedModels(); }, this);

    ui->modelsList->clear();

    if (res.success && res.data.canConvert<QJsonArray>()) {
        QJsonArray arr = res.data.toJsonArray();
        for (const auto &v : arr)
            ui->modelsList->addItem(v.toString());
    }

    // Добавляем специальный пункт для добавления новой модели
    QListWidgetItem *addItem = new QListWidgetItem("➕ Добавить новую модель");
    addItem->setForeground(Qt::gray);          // серый цвет
    addItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable); // нельзя редактировать
    ui->modelsList->addItem(addItem);
}


void SettingsWindow::onModelsListContextMenu(const QPoint &pos) {
    QListWidgetItem *item = ui->modelsList->itemAt(pos);

    QMenu menu(this);

    if (item && item->text() != "➕ Добавить новую модель") {
        // Клик по существующей модели — показываем Edit / Remove / Set / Copy
        QAction *editAction = menu.addAction("Редактировать");
        QAction *removeAction = menu.addAction("Удалить");
        QAction *setCurrentAction = menu.addAction("Сделать текущей");
        QAction *copyNameAction = menu.addAction("Скопировать имя");

        QAction *selectedAction = menu.exec(ui->modelsList->mapToGlobal(pos));
        if (!selectedAction) return;

        ui->modelsList->setCurrentItem(item);

        if (selectedAction == editAction) on_editModelBtn_clicked();
        else if (selectedAction == removeAction) on_removeModelBtn_clicked();
        else if (selectedAction == setCurrentAction) {
            auto result = ChatGPTConfig::instance().setCurrentModel(item->text());
            if (!result.success) QMessageBox::warning(this, "Ошибка", result.error);
            else ui->currentModelEdit->setText(item->text());
        }
        else if (selectedAction == copyNameAction) {
            QGuiApplication::clipboard()->setText(item->text());
        }
    } else {
        // Клик по пустому месту или по "Добавить новую модель"
        QAction *addAction = menu.addAction("Добавить новую модель");
        QAction *selectedAction = menu.exec(ui->modelsList->mapToGlobal(pos));
        if (selectedAction == addAction) on_addModelBtn_clicked();
    }
}



void SettingsWindow::on_saveBtn_clicked() {

}

void SettingsWindow::on_cancelBtn_clicked() {
    this->close();
}

void SettingsWindow::on_addModelBtn_clicked() {
    bool ok;
    QString modelName = QInputDialog::getText(this,
                                              "Добавить модель",
                                              "Название модели:",
                                              QLineEdit::Normal,
                                              "",
                                              &ok);
    if (!ok || modelName.isEmpty()) return;  // пользователь отменил

    // Добавляем в конфиг
    auto result = ChatGPTConfig::instance().addSelectedModels(modelName);
    if (!result.success) {
        QMessageBox::warning(this, "Ошибка", result.error);
        return;
    }

    reloadModelsList();
}

void SettingsWindow::on_editModelBtn_clicked() {
    // Получаем выбранный элемент
    QListWidgetItem *item = ui->modelsList->currentItem();
    if (!item) return;  // ничего не выбрано

    QString oldName = item->text();

    // Запрашиваем новое имя модели
    bool ok;
    QString newName = QInputDialog::getText(this,
                                            "Редактировать модель",
                                            "Новое название модели:",
                                            QLineEdit::Normal,
                                            oldName,
                                            &ok);
    if (!ok || newName.isEmpty() || newName == oldName) return;

    // Удаляем старое название из конфига
    auto delResult = ChatGPTConfig::instance().delSelectedModels(oldName);
    if (!delResult.success) {
        QMessageBox::warning(this, "Ошибка", delResult.error);
        return;
    }

    // Добавляем новое название в конфиг
    auto addResult = ChatGPTConfig::instance().addSelectedModels(newName);
    if (!addResult.success) {
        // Если не удалось добавить, возвращаем старое название обратно
        ChatGPTConfig::instance().addSelectedModels(oldName);
        QMessageBox::warning(this, "Ошибка", addResult.error);
        return;
    }

    reloadModelsList();
}

void SettingsWindow::on_removeModelBtn_clicked() {
    // Получаем выбранный элемент
    QListWidgetItem *item = ui->modelsList->currentItem();
    if (!item) return;

    QString modelName = item->text();

    // Удаляем из конфига
    auto result = ChatGPTConfig::instance().delSelectedModels(modelName);
    if (!result.success) {
        QMessageBox::warning(this, "Ошибка", result.error);
        return;
    }

    reloadModelsList();
}

QJsonObject SettingsWindow::getUpdatedConfig() const {
    //return config;
}


