// Created by Alex Balta on 27.03.2024.
//

#pragma once

#include "Repository.hpp"

class Service {
  private:
    Repository &repo; // Reference to a Repository object

  public:
    /**
     * Constructor for the Service class.
     * @param repo Reference to a Repository object
     */
    explicit Service(Repository &repo);

    /**
     * Retrieves all books from the repository.
     * @return vector containing all books in the repository
     */
    Vector<Book> &getAll();

    /**
     * Adds a new book to the repository.
     * @param title Title of the book to be added
     * @param author Author of the book to be added
     * @param genre Genre of the book to be added
     * @param year Year of the book to be added
     * @throws std::runtime_error if the new book created is not valid or if the book is already in the list.
     */
    void addBook(const std::string &title, const std::string &author,
                 const std::string &genre, int year);

    /**
     * Updates information of an existing book in the repository.
     * @param title Title of the book to be updated
     * @param new_author New author of the book
     * @param new_genre New genre of the book
     * @param new_year New year of the book
     * @throws std::runtime_error if the new book created is not valid or if the book in not in the list.
     */
    void updateBook(const std::string &title, const std::string &new_author,
                    const std::string &new_genre, int new_year);

    /**
     * Deletes a book from the repository.
     * @param title Title of the book to be deleted
     * @throws std::runtime_error if the book is not on the list.
     */
    void deleteBook(const std::string &title);

    /**
     * Finds a book from the repository.
     * @param title Title of the book to be deleted
     * @return array of books whose name fits
     */
    Vector<Book> findBooks(const std::string &title);

    /**
     * Filters all books from the repository by year.
     * @param min_year the year you want to filter by
     * @return array of books whose year is greater than min_year
     */
    Vector<Book> filterBooks(int min_year);

    /**
     * Sorts all books from the repository.
     * @param title Title of the book to be deleted
     * @return array of books whose name fits
     */
    Vector<Book> sortBooks(const std::function<bool(const Book &, const Book &)> &compare);
};
