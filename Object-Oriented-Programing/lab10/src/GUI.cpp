//
// Created by Alex Balta on 07.05.2024.
//

#include "GUI.hpp"

GUI::GUI(Service &service) : service{service} {
    componentsInit();
    signalsInit();
    refreshItemList();
}

void GUI::componentsInit() {
    auto *forms_widget = new QWidget;
    form_layout = new QFormLayout;

    title_text = new QLabel("The title is:");
    title_input_box = new QLineEdit;

    author_text = new QLabel("The author is:");
    author_input_box = new QLineEdit;

    genre_text = new QLabel("The genre is:");
    genre_input_box = new QLineEdit;

    year_text = new QLabel("The year is:");
    year_input_box = new QLineEdit;

    form_layout->addRow(title_text, title_input_box);
    form_layout->addRow(author_text, author_input_box);
    form_layout->addRow(genre_text, genre_input_box);
    form_layout->addRow(year_text, year_input_box);

    add_btn = new QPushButton{"Add"};
    delete_btn = new QPushButton{"Delete"};
    update_btn = new QPushButton{"Update"};

    search_btn = new QPushButton{"Search"};
    filter_year_btn = new QPushButton{"Filter by year"};

    sort_title_btn = new QPushButton{"Sort by title"};
    sort_year_btn = new QPushButton{"Sort by year"};

    undo_btn = new QPushButton{"Undo"};
    refresh_btn = new QPushButton{"Refresh"};
    close_btn = new QPushButton{"Close"};

    auto *button_widget_crud = new QHBoxLayout;
    button_widget_crud->addWidget(add_btn);
    button_widget_crud->addWidget(delete_btn);
    button_widget_crud->addWidget(update_btn);

    auto *button_widget_filter = new QHBoxLayout;
    button_widget_filter->addWidget(search_btn);
    button_widget_filter->addWidget(filter_year_btn);

    auto *button_widget_sort = new QHBoxLayout;
    button_widget_sort->addWidget(sort_title_btn);
    button_widget_sort->addWidget(sort_year_btn);

    auto *button_widget_options = new QHBoxLayout;
    button_widget_options->addWidget(close_btn);
    button_widget_options->addWidget(refresh_btn);
    button_widget_options->addWidget(undo_btn);

    form_layout->addRow(button_widget_crud);
    form_layout->addRow(button_widget_filter);
    form_layout->addRow(button_widget_sort);

    forms_widget->setLayout(form_layout);

    auto *widget_list = new QWidget;
    item_list = new QListWidget;

    auto *layoutList = new QVBoxLayout;
    layoutList->addWidget(item_list);
    layoutList->addItem(button_widget_options);
    widget_list->setLayout(layoutList);

    auto *main_layout = new QHBoxLayout;
    main_layout->addWidget(forms_widget);
    main_layout->addWidget(widget_list);

    this->setLayout(main_layout);
}

void GUI::signalsInit() {
    QObject::connect(add_btn, &QPushButton::clicked, [&]() {
        try {
            service.addBookLib(title_input_box->text().toStdString(), author_input_box->text().toStdString(),
                               genre_input_box->text().toStdString(), year_input_box->text().toInt());
            refreshItemList();
            QMessageBox::information(this, "Succes", "The element was added");
        } catch (const std::exception &error) {
            QMessageBox::warning(this, "Error", error.what());
        }
    });

    QObject::connect(delete_btn, &QPushButton::clicked, [&]() {
        try {
            service.deleteBookLib(title_input_box->text().toStdString());
            refreshItemList();
            QMessageBox::information(this, "Succes", "The element was deleted");
        } catch (const std::exception &error) {
            QMessageBox::warning(this, "Error", error.what());
        }
    });

    QObject::connect(update_btn, &QPushButton::clicked, [&]() {
        try {
            service.updateBookLib(title_input_box->text().toStdString(), author_input_box->text().toStdString(),
                                  genre_input_box->text().toStdString(), year_input_box->text().toInt());
            refreshItemList();
            QMessageBox::information(this, "Succes", "The element was updated");
        } catch (const std::exception &error) {
            QMessageBox::warning(this, "Error", error.what());
        }
    });

    QObject::connect(close_btn, &QPushButton::clicked, [&]() {
        QApplication::quit();
    });

    QObject::connect(search_btn, &QPushButton::clicked, [&]() {
        try {
            auto books = service.findBooksLib(title_input_box->text().toStdString());
            item_list->clear();

            for (const auto &book : books) {
                QString bookQString = toQString(book);
                item_list->addItem(bookQString);
            }
        } catch (const std::exception &error) {
            QMessageBox::warning(this, "Error", error.what());
        }
    });

    QObject::connect(refresh_btn, &QPushButton::clicked, [&]() {
        refreshItemList();
    });

    QObject::connect(filter_year_btn, &QPushButton::clicked, [&]() {
        try {
            auto filter_year = service.filterBooksLib(year_input_box->text().toInt());
            item_list->clear();
            for (const auto &book : filter_year) {
                QString bookQString = toQString(book);
                item_list->addItem(bookQString);
            }
        } catch (const std::exception &error) {
            QMessageBox::warning(this, "Error", error.what());
        }
    });

    QObject::connect(sort_title_btn, &QPushButton::clicked, [&]() {
        auto sorted_title = service.sortBooksLib(
            [&](const Book &a, const Book &b) {
                return a.getTitle() > b.getTitle();
            });

        item_list->clear();
        for (const auto &book : sorted_title) {
            QString bookQString = toQString(book);
            item_list->addItem(bookQString);
        }
    });

    QObject::connect(sort_year_btn, &QPushButton::clicked, [&]() {
        auto sorted_title = service.sortBooksLib(
            [&](const Book &a, const Book &b) {
                return a.getYear() > b.getYear();
            });

        item_list->clear();
        for (const auto &book : sorted_title) {
            QString bookQString = toQString(book);
            item_list->addItem(bookQString);
        }
    });

    QObject::connect(undo_btn, &QPushButton::clicked, [&]() {
        try {
            service.undo();
            refreshItemList();
            QMessageBox::information(this, "Succes", "The last operation was undone");
        } catch (const std::exception &error) {
            QMessageBox::warning(this, "Error", error.what());
        }
    });
}

void GUI::refreshItemList() {
    item_list->clear();
    for (const Book &book : service.getAllLib()) {
        QString bookQString = toQString(book);
        item_list->addItem(bookQString);
    }
}

QString GUI::toQString(const Book &book) {
    auto title = QString::fromStdString(book.getTitle());
    auto author = QString::fromStdString(book.getAuthor());
    auto genre = QString::fromStdString(book.getGenre());
    auto year = QString::number(book.getYear());

    QString bookQString = title + " " + author + " " + genre + " " + year;
    return bookQString;
}