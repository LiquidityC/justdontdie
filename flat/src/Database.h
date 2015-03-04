#ifndef _DATABASE_H
#define _DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>

/**
 * @brief Handle SQLite databases
 * @author Linus Probert <linus.probert@gmail.com>
 */
class Database {

	public:
		typedef std::vector<std::string> Row;
		typedef std::vector<Row> Result;

	public:

		/**
		 * @brief Constructor
		 * @param filename The database file
		 */
		Database(const std::string& filename);

		/**
		 * @brief destructor
		 */
		~Database();

		/**
		 * @brief Open provided database file
		 * @param filename the path to the file
		 */
		bool open(const std::string& filename);

		/**
		 * @brief Run a query against the database
		 * @return the result of the query
		 */
		Result query(const std::string& query);

		/**
		 * @brief Close the database
		 */
		void close();

		/**
		 * @brief Get table count for given table
		 * @patam table_name The name of the table
		 */
		unsigned int get_table_count(const std::string& table_name);

	private:
		sqlite3* database;
};

#endif
