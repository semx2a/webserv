#ifndef UTL_CPP
# define UTL_CPP

# include <algorithm>
# include <cstring>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <string>
# include <sstream>
# include <vector>
# include <map>
# include <cstdlib>
# include <ctime>

extern "C" {
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/stat.h>
}

# define RESET		"\033[m"
# define BOLD 		"\033[1m"

# define BLACK 		"\033[0;30m"
# define RED 		"\033[0;31m"
# define GREEN 		"\033[0;32m"
# define ORANGE 	"\033[0;33m"
# define BLUE 		"\033[0;34m"
# define PURPLE 	"\033[0;35m"
# define CYAN 		"\033[0;36m"
# define WHITE 		"\033[0;37m"

// High Intensty
# define HIGREEN 	"\033[0;92m"
# define HIORANGE 	"\033[0;93m"
# define HIPURPLE 	"\033[0;95m"

// Bold
# define BORANGE 	"\033[1;33m"
# define BBLUE 		"\033[1;34m"
# define BPURPLE 	"\033[1;35m"
# define BCYAN 		"\033[1;36m"

// Bold High Intensity
# define BHIBLACK 	"\033[1;30m"
# define BHIRED 	"\033[1;91m"
# define BHIGREEN 	"\033[1;92m"
# define BHIORANGE 	"\033[1;33m"
# define BHIYELLOW 	"\033[1;93m"
# define BHIBLUE 	"\033[1;94m"
# define BHIPURPLE 	"\033[1;35m"
# define BHIPINK 	"\033[1;95m"
# define BHICYAN 	"\033[1;96m"
# define BHIWHITE 	"\033[1;37m"

// Italic
# define ICYAN 		"\033[3;36m"

class utl {

	public:
		utl(void);
		utl(utl const &);
		~utl(void);

		utl & operator=(utl const &);

		static std::string				trim(std::string const &);
		static int						find_first_occurrence(const std::vector<char> &, const std::string&);
		static int						find_last_occurrence(const std::vector<char> &, const std::string&);
		static std::vector<std::string> tokenize(const std::string str, char sep);
		static std::string 				trimAndReplaceWs(std::string& input);
		static std::string				fileToStr(std::ifstream &file);
		static std::string				printCharArray(char** charArray, int size);
		static void						deleteCharArray(char **);
		static std::string				getDate(void);

		static void						log(int client_fd, std::string str);
		static std::string				str_of(size_t size, const std::string & c);
		static std::string				custom_width(int width, char c, std::string const& content);

		static std::string 				vectorOfCharToStr(std::vector <char> const& buffer);
		static std::string 				print_str(std::string buffer);

		static std::string	   			print_client_added(void);
		static std::string	   			print_end_connexion(void);
		static std::string	   			print_wait(void);

		static bool						isDirectory(std::string path);
		static bool						createDirectory(std::string path);
		static bool						createFile(std::string path, std::vector<char> content, std::string filename);

		template <typename T>
		static std::string				print_vector(std::vector<T> const& vec);

		template <typename T>
		static std::string	 			print_classVector(std::vector<T> const& vec);

		template <typename KeyType, typename ValueType>
		static std::string 				print_map(const std::map<KeyType, ValueType>& myMap);

		template <typename KeyType, typename T>
		static std::string 				print_map_of_vectors(const std::map<KeyType, T>& myMap);

		template<typename T>
		static std::string				numberToString(T number);

		static int						searchVectorChar(std::vector<char> tab, const char *to_find, size_t index);
		static int						searchVectorCharUntil(std::vector<char> tab, const char *to_find, size_t index);
		static std::vector<char>&		replaceVectorChar(std::vector<char> &tab, size_t position, size_t length, std::string str);

};

#include "utl.tpp"

#endif