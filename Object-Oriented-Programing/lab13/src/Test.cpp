//
// Created by Alex Balta on 26.03.2024.
//

#include "Test.hpp"

#include "Book.hpp"
#include "Validator.hpp"
#include "Repository.hpp"
#include "Service.hpp"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdio>

Test::Test() {
    const int YEAR = 1890;
    const int OTHER_YEAR = 1910;

    title = "War and Peace";
    author = "Lev Tolstoy";
    genre = "roman";
    year = YEAR;

    other_title = "The brothers Karamazov";
    other_author = "Fyodor Dostoevsky";
    other_genre = "literature";
    other_year = OTHER_YEAR;
}

void Test::testDomain() {
    Book new_book(title, author, genre, year);

    assert(new_book.getTitle() == title);
    assert(new_book.getAuthor() == author);
    assert(new_book.getGenre() == genre);
    assert(new_book.getYear() == year);

    assert(new_book.intoString() == "War and Peace, Lev Tolstoy, roman, 1890");

    new_book.setTitle(other_title);
    new_book.setAuthor(other_author);
    new_book.setGenre(other_genre);
    new_book.setYear(other_year);

    assert(new_book.getTitle() == other_title);
    assert(new_book.getAuthor() == other_author);
    assert(new_book.getGenre() == other_genre);
    assert(new_book.getYear() == other_year);

    std::cout << "Domain tests ran successfully.\n";
}

void Test::testRepository() {
    /*
     * TEST LIBRARY
     */

    LibraryVector repo;
    Book new_book(title, author, genre, year);
    Book other_book(other_title, other_author, other_genre, other_year);

    // TEST ADD

    repo.addBook(new_book);
    assert(repo.getLen() == 1);
    repo.addBook(other_book);
    assert(repo.getLen() == 2);

    // TEST DELETE

    repo.deleteBook(title);
    assert(repo.getLen() == 1);

    // TEST UPDATE

    repo.updateBook(other_title, new_book);
    assert(repo.getLen() == 1);

    auto all = repo.getBooks();
    assert(all[0].getTitle() == title);
    assert(all[0].getAuthor() == author);
    assert(all[0].getGenre() == genre);
    assert(all[0].getYear() == year);

    /*
     * TEST SHOPPING CART
     */

    ShoppingCart cart;

    // TEST DELETE ALL

    cart.addBook(new_book);
    cart.addBook(other_book);
    assert(cart.getLen() == 2);

    cart.deleteAllBooks();
    assert(cart.getLen() == 0);

    // TEST FILE REPO

    std::ofstream out("../database/test.txt");
    out.close();

    LibraryFile library_file{"../database/test.txt"};

    try {
        LibraryFile bad_lib{"random_name"};
        assert(false);
    } catch (const std::exception &e) {}

    // TEST ADD

    library_file.addBook(new_book);
    assert(library_file.getLen() == 1);
    library_file.addBook(other_book);
    assert(library_file.getLen() == 2);

    // TEST DELETE

    library_file.deleteBook(title);
    assert(library_file.getLen() == 1);

    // TEST UPDATE

    library_file.updateBook(other_title, new_book);
    assert(library_file.getLen() == 1);

    LibraryFile new_repo("../database/test.txt");
    std::remove("../database/test.txt");

    // TEST RANDOM EXCEPTION

    LibraryDict library_dict{1};

    try {
        library_dict.addBook(new_book);
        assert(false);
    } catch (const std::exception &e) {
        assert(library_dict.getLen() == 0);
    }

    std::cout << "Repository tests ran successfully.\n";
}

void Test::testValidator() {
    Book good_book(title, author, genre, year);
    Validator::validateBook(good_book);

    const int BAD_YEAR = 2050;
    std::string bad_title = "war and peace";
    std::string bad_author = "levTolstoy";
    std::string bad_genre = "randomWORDS";
    int bad_year = BAD_YEAR;

    Book bad_book(bad_title, bad_author, bad_genre, bad_year);

    try {
        Validator::validateBook(bad_book);
        assert(false);
    }
    catch (const std::exception &e) {
        std::string correct_error =
            "\nThe title is not valid.\nThe author is not valid.\nThe genre is not valid.\nThe year is not valid.";
        assert(e.what() == correct_error);
    }

    std::cout << "Validator tests ran successfully.\n";
}

void Test::testService() {
    LibraryDict repo{0};
    ShoppingCart cart;
    Service service(repo, cart);

    // TEST ADD

    service.addBookLib(title, author, genre, year);
    assert(service.getAllLib().size() == 1);
    assert(repo.getLen() == 1);
    try {
        service.addBookLib(title, other_author, other_genre, other_year);
        assert(false);
    } catch (const std::exception &e) {
        assert(service.getAllLib().size() == 1);
        assert(repo.getLen() == 1);
    }

    // TEST UPDATE

    service.updateBookLib(title, other_author, other_genre, other_year);
    assert(service.getAllLib().size() == 1);

    auto all_repo = repo.getBooks();
    assert(all_repo[0].getTitle() == title);
    assert(all_repo[0].getAuthor() == other_author);
    assert(all_repo[0].getGenre() == other_genre);
    assert(all_repo[0].getYear() == other_year);

    try {
        service.updateBookLib(other_title, author, genre, year);
        assert(false);
    } catch (const std::exception &e) {
        assert(service.getAllLib().size() == 1);

        auto all = service.getAllLib();
        assert(all[0].getTitle() == title);
        assert(all[0].getAuthor() == other_author);
        assert(all[0].getGenre() == other_genre);
        assert(all[0].getYear() == other_year);
    }

    // TEST DELETE

    service.deleteBookLib(title);
    assert(service.getAllLib().empty());

    try {
        service.addBookLib(title, other_author, other_genre, other_year);
        service.deleteBookLib(other_title);
        assert(false);
    } catch (const std::exception &e) {
        assert(service.getAllLib().size() == 1);
    }

    // TEST FIND

    auto list = service.findBooksLib(other_title);
    assert(list.empty());

    list = service.findBooksLib(title);
    assert(list.size() == 1);

    // TEST FILTER

    const int GOOD_MIN_YEAR = 1900;
    const int BAD_MIN_YEAR = 2000;

    auto filter = service.filterBooksLib(GOOD_MIN_YEAR);
    assert(filter.size() == 1);

    filter = service.filterBooksLib(BAD_MIN_YEAR);
    assert(filter.empty());

    // TEST SORT

    service.deleteBookLib(title);
    service.addBookLib(other_title, other_author, other_genre, other_year);
    service.addBookLib(title, author, genre, year);

    auto sorted = service.sortBooksLib([](const Book &b1, const Book &b2) {
        return b1.getYear() < b2.getYear();
    });

    assert(sorted.size() == 2);
    assert(sorted[0].getTitle() == title);
    assert(sorted[0].getAuthor() == author);
    assert(sorted[0].getGenre() == genre);
    assert(sorted[0].getYear() == year);

    // TEST ADD CART

    service.addBookCart(title);
    assert(service.getShoppingCart().size() == 1);

    service.addBookCart(other_title);
    assert(service.getShoppingCart().size() == 2);

    std::string nonexistent_book = "Metamorphosis";
    try {
        service.addBookCart(nonexistent_book);
        assert(false);
    } catch (const std::exception &e) {
        assert(cart.getLen() == 2);
        assert(service.getShoppingCart().size() == 2);
    }

    // TEST DELETE CART

    service.deleteCart();
    assert(service.getShoppingCart().empty());

    // TEST POPULATE RANDOM

    const int number = 12;
    service.populateRandomCart(number);
    assert(service.getShoppingCart().size() == number);

    service.exportHTML("../database/test.html");
    std::remove("../database/test.html");
    service.exportCSV("../database/test.csv");
    std::remove("../database/test.csv");

    service.populateRandomCart(number);
    assert(service.getShoppingCart().size() == number * 2);

    service.deleteCart();
    service.deleteBookLib(title);
    service.deleteBookLib(other_title);

    try {
        service.populateRandomCart(number);
        assert(false);
    } catch (const std::exception &e) {
        assert(service.getShoppingCart().empty());
    }

    try {
        service.deleteCart();
        assert(false);
    } catch (const std::exception &e) {
        assert(service.getShoppingCart().empty());
    }

    // TEST RAPORT

    const std::string new_title = "Hannibal";
    service.addBookLib(other_title, other_author, other_genre, other_year);
    service.addBookLib(title, author, genre, year);
    service.addBookLib(new_title, author, genre, other_year);

    auto map = service.getRaport();

    assert(map[other_genre] == 1);
    assert(map[genre] == 2);


    // TEST UNDO

    LibraryVector other_repo;
    ShoppingCart other_cart;
    Service other_service(other_repo, other_cart);

    other_service.addBookLib(title, author, genre, year);
    other_service.addBookLib(other_title, other_author, other_genre, other_year);
    assert(other_service.getAllLib().size() == 2);
    other_service.undo();
    assert(other_service.getAllLib().size() == 1);

    other_service.deleteBookLib(title);
    assert(other_service.getAllLib().empty());
    other_service.undo();
    assert(other_service.getAllLib().size() == 1);

    other_service.updateBookLib(title, other_author, other_genre, other_year);
    other_service.undo();
    assert(other_service.getAllLib().size() == 1);
    assert(other_service.getAllLib()[0].getTitle() == title);
    assert(other_service.getAllLib()[0].getAuthor() == author);
    assert(other_service.getAllLib()[0].getGenre() == genre);
    assert(other_service.getAllLib()[0].getYear() == year);

    other_service.undo();
    assert(other_service.getAllLib().empty());

    try {
        other_service.undo();
        assert(false);
    } catch (const std::exception &e) {
        assert(other_service.getAllLib().empty());
    }

    std::cout << "Service tests ran successfully.\n";
}

void Test::runAll() {
    testDomain();
    testValidator();
    testRepository();
    testService();
    std::cout << "All tests ran successfully.\n";
}
